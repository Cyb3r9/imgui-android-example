#pragma once

#include <EGL/egl.h>
#include <android/native_window.h>
#include <chrono>
static std::chrono::steady_clock::time_point g_LastShutdownTime;
static constexpr int REINIT_DELAY_MS = 100;

void InitImGui(ANativeWindow *window, EGLDisplay display, EGLSurface surface);
void RenderImGui();
void ShutdownImGui();

bool IsImGuiInitialized();
