#include "DebugMenu.hpp"

void DebugMenu::Render(float dt)
{
    static bool s_ShowDebugWindow = true;

    if (!m_Active)
        return;

    if (s_ShowDebugWindow)
    {
        if (ImGui::Begin("Debug Menu", &s_ShowDebugWindow))
        {

            ImGui::End();
        }
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                // TODO: Create new file
            }

            if (ImGui::BeginMenu("Open"))
            {
                // TODO: Get last opened files
                // e.g. use std::queue<>
                ImGui::MenuItem("EXAMPLE1.lvl");
                ImGui::MenuItem("EXAMPLE2.lvl");
                ImGui::MenuItem("EXAMPLE3.lvl");

                ImGui::Separator();
                if (ImGui::BeginMenu("Open..."))
                {
                    char buf[255] = {0};
                    ImGui::PushItemWidth(100.0f);
                    ImGui::InputText("Name", buf, sizeof(buf));

                    if (ImGui::Button("Open", ImVec2(130, 20)))
                    {
                        // TODO: Open and handle file
                    }

                    ImGui::EndPopup();
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Save"))
            {
                // TODO: Save current file
            }

            ImGui::EndMenu();
        }

        if (!s_ShowDebugWindow && ImGui::MenuItem("Debug"))
            s_ShowDebugWindow = true;

        ImGui::EndMainMenuBar();
    }
}
