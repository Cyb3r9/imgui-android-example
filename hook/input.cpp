#include "input.h"
#include "imgui/backends/imgui_impl_android.h"
#include "utils/log.h"

#include <android/input.h>
#include <android/native_activity.h>
#include <dlfcn.h>
#include <dobby.h>

enum class ABIType {
    UNKNOWN,
    ABI_64,
    ABI_32
};

static ABIType detectedABI = ABIType::UNKNOWN;
static bool inputQueueHooked = false;
static bool hasReceivedTouchFromQueue = false;
static bool isImGuiReady = false;

void MarkImGuiReady() {
    isImGuiReady = true;
    LOGI("ImGui is now marked as ready to receive input");
}

enum class GameInputBackend {
    UNKNOWN,
    UNITY,
    UNREAL,
    NATIVE
};

static GameInputBackend backend = GameInputBackend::UNKNOWN;

static void DetectABI() {
#if defined(__aarch64__)
    detectedABI = ABIType::ABI_64;
#elif defined(__arm__)
    detectedABI = ABIType::ABI_32;
#else
    detectedABI = ABIType::UNKNOWN;
#endif

    LOGI("Detected ABI: %s",
         detectedABI == ABIType::ABI_64 ? "64-bit" :
         detectedABI == ABIType::ABI_32 ? "32-bit" : "Unknown");
}

const char* GetConsumeSymbolForABI() {
    switch (detectedABI) {
        case ABIType::ABI_64:
            return "_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEblPjPPNS_10InputEventE";
        case ABIType::ABI_32:
            return "_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEbxPjPPNS_10InputEventE";
        default:
            return nullptr;
    }
}

// External symbol resolver
extern void *ResolveSymbol(const char *lib, const char *sym);

using AInputQueue_getEvent_t = int (*)(AInputQueue *, AInputEvent **);
static AInputQueue_getEvent_t orig_AInputQueue_getEvent = nullptr;

using ConsumeFn = int32_t (*)(void *thiz, void *factory, bool can_block, long timeoutMillis, uint32_t *out_seq, AInputEvent **out_event);
static ConsumeFn origConsume = nullptr;

using InitMotionEventFn = void (*)(void *motionEvent, const void *inputMessage);
static InitMotionEventFn origInitializeMotion = nullptr;

static void DetectGameBackend() {
    if (dlopen("libunity.so", RTLD_NOW | RTLD_NOLOAD)) {
        backend = GameInputBackend::UNITY;
        LOGI("Game backend: Unity");
    } else if (dlopen("libUE4.so", RTLD_NOW | RTLD_NOLOAD) || dlopen("libUnreal.so", RTLD_NOW | RTLD_NOLOAD)) {
        backend = GameInputBackend::UNREAL;
        LOGI("Game backend: Unreal Engine");
    } else {
        backend = GameInputBackend::NATIVE;
        LOGI("Game backend: Native / Unknown");
    }
}

// === Hooked AInputQueue ===
int my_AInputQueue_getEvent(AInputQueue *queue, AInputEvent **outEvent) {
    int result = orig_AInputQueue_getEvent(queue, outEvent);
    if (result >= 0 && outEvent && *outEvent) {
        hasReceivedTouchFromQueue = true;
        if (isImGuiReady) {
            ImGui_ImplAndroid_HandleInputEvent(*outEvent);
        } else {
            LOGV("[Input] Ignored input event (ImGui not ready)");
        }
    }
    return result;
}

// === Hooked consume ===
int32_t myConsume(void *thiz, void *factory, bool can_block, long timeoutMillis, uint32_t *out_seq, AInputEvent **out_event) {
    int32_t result = origConsume(thiz, factory, can_block, timeoutMillis, out_seq, out_event);
    if ((!hasReceivedTouchFromQueue || !inputQueueHooked) && result == 0 && out_event && *out_event) {
        if (isImGuiReady) {
            ImGui_ImplAndroid_HandleInputEvent(*out_event);
            LOGI("[Input] Touch handled via InputConsumer::consume");
        } else {
            LOGV("[Input] Ignored input from consume (ImGui not ready)");
        }
    }
    return result;
}

// === Hooked initializeMotionEvent ===
void myInitializeMotionEvent(void *motionEvent, const void *inputMessage) {
    LOGV("[Input] initializeMotionEvent called");
    origInitializeMotion(motionEvent, inputMessage);
}

// === Init All Hooks ===
void InitInputHooks() {
    DetectABI();
    DetectGameBackend();

    // --- Hook AInputQueue_getEvent ---
    void *sym_queue = ResolveSymbol("libandroid.so", "AInputQueue_getEvent");
    if (sym_queue) {
        if (DobbyHook(sym_queue, (void *)my_AInputQueue_getEvent, (void **)&orig_AInputQueue_getEvent) == 0) {
            LOGI("Successfully hooked AInputQueue_getEvent");
            inputQueueHooked = true;
        } else {
            LOGE("Failed to hook AInputQueue_getEvent");
        }
    } else {
        LOGW("Symbol AInputQueue_getEvent not found");
    }

    // --- Hook InputConsumer::initializeMotionEvent ---
    void *sym_init = ResolveSymbol("libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    if (sym_init) {
        if (DobbyHook(sym_init, (void *)myInitializeMotionEvent, (void **)&origInitializeMotion) == 0) {
            LOGI("Successfully hooked InputConsumer::initializeMotionEvent");
        } else {
            LOGE("Failed to hook InputConsumer::initializeMotionEvent");
        }
    } else {
        LOGW("Symbol InputConsumer::initializeMotionEvent not found");
    }

    // --- Hook InputConsumer::consume (ABI-dependent) ---
    const char *consumeSym = GetConsumeSymbolForABI();
    if (consumeSym) {
        void *sym_consume = ResolveSymbol("libinput.so", consumeSym);
        if (sym_consume) {
            if (DobbyHook(sym_consume, (void *)myConsume, (void **)&origConsume) == 0) {
                LOGI("Successfully hooked InputConsumer::consume");
            } else {
                LOGE("Failed to hook InputConsumer::consume");
            }
        } else {
            LOGW("Symbol InputConsumer::consume not found");
        }
    } else {
        LOGW("Unsupported ABI, skipping consume hook");
    }

    LOGI("Active input source: %s", hasReceivedTouchFromQueue ? "AInputQueue" : "InputConsumer");
}
