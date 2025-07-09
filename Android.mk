LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Fuby

# Source files
LOCAL_SRC_FILES := \
    main.cpp \
    utils/init.cpp \
    hook/hooks.cpp \
    hook/input.cpp \
    imguisetup.cpp \
    menu.cpp \
    imgui/imgui.cpp \
    imgui/imgui_demo.cpp \
    imgui/imgui_draw.cpp \
    imgui/imgui_tables.cpp \
    imgui/imgui_widgets.cpp \
    imgui/backends/imgui_impl_android.cpp \
    imgui/backends/imgui_impl_opengl3.cpp \
    libs/KittyMemory/KittyArm64.cpp \
    libs/KittyMemory/KittyMemory.cpp \
    libs/KittyMemory/KittyPtrValidator.cpp \
    libs/KittyMemory/KittyScanner.cpp \
    libs/KittyMemory/KittyUtils.cpp \
    libs/KittyMemory/MemoryBackup.cpp \
    libs/KittyMemory/MemoryPatch.cpp \
    libs/xDL/xdl.c \
    libs/xDL/xdl_iterate.c \
    libs/xDL/xdl_linker.c \
    libs/xDL/xdl_lzma.c \
    libs/xDL/xdl_util.c

# Include directories
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/imgui \
    $(LOCAL_PATH)/imgui/backends \
    $(LOCAL_PATH)/json \
    $(LOCAL_PATH)/libs/xDL/include \
    $(LOCAL_PATH)/libs/KittyMemory \
    $(LOCAL_PATH)/libs/KittyMemory/Deps/Keystone/includes \
    $(LOCAL_PATH)/libs/arm64-v8a/dobby/include \
    $(LOCAL_PATH)/libs/arm64-v8a/openssl/include \
    $(LOCAL_PATH)/libs/arm64-v8a/curl/include

# Flags
LOCAL_CFLAGS := -Wall -Wextra -O3 -DNDEBUG -fPIC
LOCAL_CPPFLAGS := -std=c++20 -fno-rtti -fno-exceptions

# Libraries to link
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lGLESv3 -lGLESv1_CM -lz -ldl

# Prebuilt libraries
LOCAL_STATIC_LIBRARIES := dobby keystone

# Build as shared library
LOCAL_SHARED_LIBRARIES :=
include $(BUILD_SHARED_LIBRARY)

# Include prebuilt libraries
include $(CLEAR_VARS)
LOCAL_MODULE := dobby
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/dobby/lib/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := keystone
LOCAL_SRC_FILES := libs/KittyMemory/Deps/Keystone/libs-android/$(TARGET_ARCH_ABI)/libkeystone.a
include $(PREBUILT_STATIC_LIBRARY) 


# Uncomment these if you need them
# include $(CLEAR_VARS)
# LOCAL_MODULE := curl
# LOCAL_SRC_FILES := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/curl/lib/libcurl.a
# include $(PREBUILT_STATIC_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := ssl
# LOCAL_SRC_FILES := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/openssl/lib/libssl.a
# include $(PREBUILT_STATIC_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := crypto
# LOCAL_SRC_FILES := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/openssl/lib/libcrypto.a
# include $(PREBUILT_STATIC_LIBRARY)
