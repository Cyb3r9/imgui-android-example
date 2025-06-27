#pragma once
#include "imgui/imgui.h"
#include <atomic>

// Thread-safe toggle
inline std::atomic<bool> menuOpen = true;

// Thread-safe ImGui init flag
inline std::atomic<bool> g_ImGuiInitialized = false;

// Thread-safe showDemoWindow
extern std::atomic<bool> showDemoWindow;
// Thread-safe showMetricsWindow
extern std::atomic<bool> showMetricsWindow;
inline void AtomicCheckbox(const char *label, std::atomic<bool> &atomicFlag)
{
    bool tmp = atomicFlag.load();
    if (ImGui::Checkbox(label, &tmp))
        atomicFlag.store(tmp);
}