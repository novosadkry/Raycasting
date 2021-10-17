#include "DebugMenu.hpp"

#include <Game/Game.hpp>

#define MAX_LAST_OPEN_FILES 5
namespace fs = std::filesystem;

void DebugMenu::InsertLastOpenFile(std::string path)
{
    auto& deque = m_LastOpenFiles;
    RemoveLastOpenFile(path);
    deque.push_back(path);
}

void DebugMenu::RemoveLastOpenFile(std::string path)
{
    auto& deque = m_LastOpenFiles;
    for (auto it = deque.begin(); it != deque.end(); )
    {
        if (it->compare(path) == 0)
            it = deque.erase(it);
        else
            it++;
    }
}

void DebugMenu::HandleOpenFile(std::string& path)
{
    auto ext = fs::path(path).extension();

    if (ext.compare(L".lvl") == 0)
        Game::Get().LoadLevel(Level::From(path.c_str()));
}

void DebugMenu::HandleOpenMenu()
{
    for (auto& path : m_LastOpenFiles)
    {
        if (ImGui::MenuItem(path.c_str()))
        {
            HandleOpenFile(path);
            InsertLastOpenFile(path);
        }

        if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[1])
            RemoveLastOpenFile(path);
    }

    if (m_LastOpenFiles.size() > 0)
        ImGui::Separator();

    if (ImGui::BeginMenu("Open..."))
    {
        ImGui::PushItemWidth(100.0f);
        ImGui::InputText("Name", m_OpenNameBuffer, 255);

        if (ImGui::Button("Open", ImVec2(130, 20)))
        {
            auto path = std::string(m_OpenNameBuffer);

            try
            {
                HandleOpenFile(path);
                InsertLastOpenFile(path);
            }
            catch (std::exception& e)
            {
                LOG(e.what());
                m_ShowOpenFailTime = 3;
            }
        }

        if (m_ShowOpenFailTime > 0)
            ImGui::TextColored(ImColor(255, 0, 0), "Invalid path or file extension!");

        ImGui::EndMenu();
    }
}

void DebugMenu::HandleNewPopup()
{
    if (ImGui::BeginPopupModal("New Level"))
    {
        static char name[255] = {0};
        static sf::Vector2i levelSize{500, 500};
        static sf::Vector2i gridSize{10, 10};

        auto avail = ImGui::GetContentRegionAvail();

        ImGui::InputText("Name", name, sizeof(name));
        ImGui::InputInt2("Level Size", &levelSize.x);
        ImGui::InputInt2("Grid Size", &gridSize.x);

        ImVec2 pos;

        pos = ImVec2(10, avail.y);
        ImGui::SetCursorPos(pos);
        if (ImGui::Button("OK", ImVec2(avail.x / 2 - 10, 20)))
        {
            Game::Get().LoadLevel(MakeUnique<Level>(levelSize, Grid(gridSize)));
            ImGui::CloseCurrentPopup();
        }

        pos = ImVec2(avail.x / 2 + 10, avail.y);
        ImGui::SetCursorPos(pos);
        if (ImGui::Button("Close", ImVec2(avail.x / 2, 20)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void DebugMenu::Render(float dt)
{
    if (!m_Active)
        return;

    bool openNewPopup = false;

    if (m_ShowDebugWindow)
    {
        if (ImGui::Begin("Debug Menu", &m_ShowDebugWindow))
        {

            ImGui::End();
        }
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                openNewPopup = true;
            }

            if (ImGui::BeginMenu("Open", "Ctrl+O"))
            {
                HandleOpenMenu();
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                Level::Save(Game::Get().GetCurrentLevel(), "SAVE.lvl");
            }

            if (ImGui::MenuItem("Save as..."))
            {

            }

            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                Game::Get().Exit();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (!m_ShowDebugWindow && ImGui::MenuItem("Debug"))
        m_ShowDebugWindow = true;

    if (m_ShowOpenFailTime > 0)
        m_ShowOpenFailTime -= dt;

    while (m_LastOpenFiles.size() > MAX_LAST_OPEN_FILES)
        m_LastOpenFiles.pop_front();

    if (openNewPopup)
        ImGui::OpenPopup("New Level");

    HandleNewPopup();
}
