#pragma once

#include <EGL/egl.h>
#include <android/native_window.h>
#include <chrono>
static std::chrono::steady_clock::time_point g_LastShutdownTime;
static constexpr int REINIT_DELAY_MS = 100;

/// Inisialisasi ImGui dengan ANativeWindow dan konteks OpenGL/EGL.
void InitImGui(ANativeWindow *window, EGLDisplay display, EGLSurface surface);

/// Render frame ImGui (dipanggil setiap kali swapBuffers di-hook).
void RenderImGui();

/// Optional: Shutdown jika ingin membersihkan resource (tidak wajib).
void ShutdownImGui();

/// Mengecek apakah ImGui sudah diinisialisasi.
bool IsImGuiInitialized();
