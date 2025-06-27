// UI.cpp
#include "UI.h"
#include "Input.h"
#include "imgui/imgui.h"
#include "init.h"
#include "log.h"
#include <algorithm>
#include <cmath>

// Global UI state (can be moved to struct/settings later)

void DrawMenu()
{
    if (!menuOpen.load())
        return;

    ImGui::Begin("Debug");
    ImGui::Text("Input Source: %s", hasReceivedTouchFromQueue ? "AInputQueue" : "InputConsumer");
    /*

        ImGui::Text("Hello from ImGui on Android!");

        static bool myCheckbox = false;
        ImGui::Checkbox("checkbox", &myCheckbox);

        static float mySlider = 0.5f;
        ImGui::SliderFloat("slider", &mySlider, 0.1f, 5.0f);
    */

    AtomicCheckbox("Show Demo Window", showDemoWindow);
    AtomicCheckbox("Show Metrics Window", showMetricsWindow);

    if (showDemoWindow.load())
        ImGui::ShowDemoWindow(nullptr);
    if (showMetricsWindow.load())
        ImGui::ShowMetricsWindow(nullptr);

    if (ImGui::Button("Close Menu"))
    {
        menuOpen.store(false);
    }

    ImGui::End();
}
