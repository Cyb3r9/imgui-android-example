#include "menu.h"
#include "utils/themes.h"
#include "hook/input.h"
#include "imgui/imgui.h"
#include "utils/init.h"
#include "utils/log.h"
#include <algorithm>
#include <cmath>

// Global UI state (can be moved to struct/settings later)

    /*

        ImGui::Text("Hello from ImGui on Android!");

        static bool myCheckbox = false;
        ImGui::Checkbox("checkbox", &myCheckbox);

        static float mySlider = 0.5f;
        ImGui::SliderFloat("slider", &mySlider, 0.1f, 5.0f);
    */
    
void DrawMenu()
{
    if (!menuOpen.load())
        return;

    if (ImGui::Begin("Fuby"), ImGuiWindowFlags_NoSavedSettings) {

        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_FittingPolicyScroll)) {

            if (ImGui::BeginTabItem("Demo")) {
                AtomicCheckbox("Show Demo Window", showDemoWindow);
                AtomicCheckbox("Show Metrics Window", showMetricsWindow);

                if (showDemoWindow.load())
                    ImGui::ShowDemoWindow(nullptr);
                if (showMetricsWindow.load())
                    ImGui::ShowMetricsWindow(nullptr);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dummy 1")) {
                // dummy
                ImGui::Text("Dummy 1");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dummy 2")) {
                // dummy
                ImGui::Text("Dummy 2");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Theme")) {
                const char* themes[] = { "Dark", "Light", "Classic" };
                if (ImGui::Combo("Theme", &themeSelection, themes, IM_ARRAYSIZE(themes))) {
                    ApplyTheme(themeSelection);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("About")) {
                ImGui::Text("Input Source: %s", hasReceivedTouchFromQueue ? "AInputQueue" : "InputConsumer");
                ImGui::Text("Fuby");
                ImGui::Text("Test");
                
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End(); // End IMGUI Window
    }
}
