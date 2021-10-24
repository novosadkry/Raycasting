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
            Game::Get().LoadLevel(Level::Empty(levelSize, gridSize));
            ImGui::CloseCurrentPopup();
        }

        pos = ImVec2(avail.x / 2 + 10, avail.y);
        ImGui::SetCursorPos(pos);
        if (ImGui::Button("Close", ImVec2(avail.x / 2, 20)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void DebugMenu::HandleDebugMenu()
{
    if (ImGui::Begin("Debug Menu", &m_ShowDebugWindow))
    {

    }

    ImGui::End();
}

void DebugMenu::HandleEditMenu()
{
    if (ImGui::Begin("Edit Menu", &m_ShowEditWindow))
    {
        auto& level     = Game::Get().GetCurrentLevel();
        auto  levelSize = level.GetSize();

        auto& grid      = level.GetGrid();
        auto  gridSize  = grid.GetSize();

        if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_Reorderable))
        {
            if (ImGui::BeginTabItem("Level"))
            {
                ImGui::Text("Name: Empty");
                ImGui::Text("Size: %dx%d", levelSize.x, levelSize.y);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Grid"))
            {
                ImGui::Text("Size: %dx%d", gridSize.x, gridSize.y);
                ImGui::Separator(); ImGui::Spacing();

                {
                    const char* modes[] = { "Select", "Edit" };

                    if (ImGui::BeginCombo("Modes", nullptr, ImGuiComboFlags_NoPreview))
                    {
                        for (int i = 0; i < IM_ARRAYSIZE(modes); i++)
                        {
                            bool selected = m_CurrentGridMode == i;
                            selected = ImGui::Selectable(modes[i], selected);

                            if (selected)
                            {
                                m_CurrentGridMode = GridMode(i);
                                ImGui::SetItemDefaultFocus();
                            }
                        }

                        ImGui::EndCombo();
                    }
                }

                ImGui::Spacing();

                {
                    ImGui::BeginChild("Grid Canvas", ImVec2(0, 0), 0, ImGuiWindowFlags_NoMove);

                    const ImVec2 p     = ImGui::GetCursorScreenPos();
                    const ImVec2 avail = ImGui::GetContentRegionAvail();
                    const ImVec2 mouse = ImGui::GetMousePos();

                    const int s = 1;
                    const int w = (avail.x / gridSize.x) - s;
                    const int h = (avail.y / gridSize.y) - s;

                    static std::optional<sf::Vector2i> selectedCell;

                    bool isRowHovered = ImGui::IsWindowHovered(
                        ImGuiHoveredFlags_ChildWindows |
                        ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
                    );

                    for (int y = 0; y < gridSize.y; y++)
                    {
                        char rowName[255];
                        std::sprintf(rowName, "Row%d", y);

                        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + y * (h + s)));
                        ImGui::BeginChild(rowName, ImVec2(avail.x, h + s));
                        ImGui::SetCursorScreenPos(p);

                        ImDrawList* drawList = ImGui::GetWindowDrawList();

                        for (int x = 0; x < gridSize.x; x++)
                        {
                            if ((x % gridSize.x) != 0)
                                ImGui::SameLine();

                            const ImVec2 min = ImVec2(p.x + x * (w + s),     p.y + y * (h + s));
                            const ImVec2 max = ImVec2(p.x + x * (w + s) + w, p.y + y * (h + s) + h);

                            const ImColor red   = ImColor(255,   0,   0);
                            const ImColor white = ImColor(255, 255, 255);

                            if (grid.Get(x, y).type == Cell::Wall)
                                drawList->AddRectFilled(min, max, white);
                            else
                                drawList->AddRect(min, max, white);

                            bool isCellHovered =
                                mouse.x > min.x && mouse.x < max.x &&
                                mouse.y > min.y && mouse.y < max.y;

                            switch (m_CurrentGridMode)
                            {
                                case SELECT:
                                {
                                    if (isCellHovered && isRowHovered)
                                    {
                                        if (ImGui::GetIO().MouseClicked[0])
                                        {
                                            if (selectedCell != sf::Vector2i{x, y})
                                                selectedCell = {x, y};
                                            else
                                                selectedCell = {};
                                        }

                                        // else if (selected && ImGui::GetIO().MouseDown[0])
                                        //     selected = {x, y};
                                    }

                                    if (selectedCell == sf::Vector2i{x, y})
                                    {
                                        drawList->AddRect(
                                            ImVec2(min.x - s / 2, min.y - s / 2),
                                            ImVec2(max.x + s / 2, max.y + s / 2),
                                            red, 0, 0, s
                                        );
                                    }
                                } break;

                                case EDIT:
                                {
                                    if (isCellHovered && isRowHovered)
                                    {
                                        if (ImGui::GetIO().MouseDown[0])
                                            grid.Set(x, y, Cell::Wall);
                                        else if (ImGui::GetIO().MouseDown[1])
                                            grid.Set(x, y, Cell::Empty);
                                    }
                                } break;
                            }
                        }

                        ImGui::EndChild();
                    }

                    ImGui::EndChild();
                    ImGui::SetCursorScreenPos(ImVec2(p.x + avail.x, p.y + avail.y));
                    ImGui::Spacing(); ImGui::Separator();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

void DebugMenu::Render(float dt)
{
    if (!m_Active)
        return;

    bool openNewPopup = false;

    if (m_ShowEditWindow)
        HandleEditMenu();

    if (m_ShowDebugWindow)
        HandleDebugMenu();

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

        if (ImGui::MenuItem("Edit", nullptr, m_ShowEditWindow))
            m_ShowEditWindow = !m_ShowEditWindow;

        if (ImGui::MenuItem("Debug", nullptr, m_ShowDebugWindow))
            m_ShowDebugWindow = !m_ShowDebugWindow;

        ImGui::EndMainMenuBar();
    }

    if (m_ShowOpenFailTime > 0)
        m_ShowOpenFailTime -= dt;

    while (m_LastOpenFiles.size() > MAX_LAST_OPEN_FILES)
        m_LastOpenFiles.pop_front();

    if (openNewPopup)
        ImGui::OpenPopup("New Level");

    HandleNewPopup();
}
