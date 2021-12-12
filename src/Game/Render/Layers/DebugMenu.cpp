#include "DebugMenu.hpp"

#include <Game/Game.hpp>
#include <Game/ECS/ECS.hpp>

#define MAX_LAST_OPEN_FILES 5

static const ImColor ColorRed   = ImColor(255,   0,   0);
static const ImColor ColorWhite = ImColor(255, 255, 255);

static uint32_t FlipRGBA(uint32_t value)
{
    return (
        ((value >> 24) & 0x000000FF) |
        ((value >>  8) & 0x0000FF00) |
        ((value <<  8) & 0x00FF0000) |
        ((value << 24) & 0xFF000000)
    );
}

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
    auto ext = std::fs::path(path).extension();

    if (ext.compare(L".lvl") == 0)
        Game::Get().LoadLevel(Level::From(path));
}

void DebugMenu::HandleOpenMenu(float dt)
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

void DebugMenu::HandleNewPopup(float dt)
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
            auto level = Level::Empty(levelSize, gridSize);
            level->SetName(std::string(name));

            Game::Get().LoadLevel(std::move(level));
            ImGui::CloseCurrentPopup();
        }

        pos = ImVec2(avail.x / 2 + 10, avail.y);
        ImGui::SetCursorPos(pos);
        if (ImGui::Button("Close", ImVec2(avail.x / 2, 20)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void DebugMenu::HandleDebugMenu(float dt)
{
    using namespace Debug;

    if (ImGui::Begin("Debug Menu", &m_ShowDebugWindow))
    {
        if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_Reorderable))
        {
            if (ImGui::BeginTabItem("Memory"))
            {
                static float historyTick = 0.25f;
                static float setTick = historyTick;

                static size_t lowest;
                static size_t highest;

                static int historyMaxSize = 50;
                static std::deque<size_t> history;

            #ifndef TRACK_MEM_ALLOC
                ImGui::TextColored(ImColor(255, 0, 0), "MEMORY ALLOCATION TRACKING DISABLED");
                ImGui::BeginDisabled();
            #endif

                auto& memory = MemoryAlloc::Get();
                ImGui::SliderInt("History Max Size", &historyMaxSize, 0, 100);

            #ifdef TRACK_MEM_ALLOC
                if ((historyTick -= dt) < 0)
                {
                    size_t current = memory.Allocated();

                    if (current < lowest)
                        lowest = current;
                    else if (current > highest)
                        highest = current;

                    history.push_back(current);
                    historyTick = setTick;
                }
            #endif

                while (history.size() > historyMaxSize)
                    history.pop_front();

                auto getter = [](void* data, int i) {
                    auto* h = (std::deque<size_t>*)data;
                    return (i < h->size())
                        ? (float)(h->at(i))
                        : 0.0f;
                };

                if (ImGui::VSliderFloat("##History Tick", ImVec2(20, 100), &historyTick, 0, 5, "%.2f"))
                    setTick = historyTick;

                ImGui::SameLine();
                ImGui::PlotHistogram(
                    "##Memory Allocation",
                    getter, &history,
                    historyMaxSize, 0,
                    "Memory Allocation", lowest, highest * 1.2f,
                    ImVec2(ImGui::GetContentRegionAvailWidth(), 100)
                );

                ImGui::Spacing();
                ImGui::Text("Metrics"); ImGui::Separator();
                ImGui::Text("    Total: %zu bytes", memory.Total());
                ImGui::Text("    Freed: %zu bytes", memory.Freed());
                ImGui::Text("Allocated: %zu bytes", memory.Allocated());

            #ifndef TRACK_MEM_ALLOC
                ImGui::EndDisabled();
            #endif

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

void DebugMenu::RenderAnyValue(auto& meta, auto& any)
{
    using namespace entt::literals;

    entt::meta_any value = meta.get(any);
    const char* name = meta.prop("name"_hs)
        .value()
        .template cast<const char*>();

    if (auto* v = value.try_cast<std::string>())
        ImGui::InputText(name, v);

    else if (auto* v = value.try_cast<int>())
        ImGui::DragInt(name, v);

    else if (auto* v = value.try_cast<float>())
        ImGui::DragFloat(name, v, .15f);

    else if (auto* v = value.try_cast<sf::Vector2u>())
        ImGui::DragScalarN(name, ImGuiDataType_U32, &v->x, 2, .15f);

    else if (auto* v = value.try_cast<sf::Vector2f>())
        ImGui::DragScalarN(name, ImGuiDataType_Float, &v->x, 2, .15f);

    else if (auto* v = value.try_cast<sf::Color>())
    {
        ImU32 hex = v->toInteger();
        hex = FlipRGBA(hex);

        ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(hex);
        ImGui::ColorEdit4(name, &rgba.x);

        hex = ImGui::ColorConvertFloat4ToU32(rgba);
        hex = FlipRGBA(hex);

        *v = sf::Color(hex);
    }

    else
    {
        bool treeOpen = ImGui::TreeNodeEx(
            name,
            ImGuiTreeNodeFlags_DefaultOpen
        );

        if (treeOpen)
        {
            if (auto type = entt::resolve(meta.type().id()))
            {
                for (auto data : type.data())
                    RenderAnyValue(data, value);
            }

            else
                ImGui::TextColored(ColorRed, "Unsupported value");

            ImGui::TreePop();
        }
    }
}

void DebugMenu::RenderEntityTree(float dt)
{
    auto& level     = Game::Get().GetCurrentLevel();
    auto& hierarchy = level.GetHierarchy();
    auto& registry  = hierarchy.GetRegistry();

    bool rTreeOpen = ImGui::TreeNodeEx(
        level.GetName().c_str(),
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_AllowItemOverlap
    );

    ImGui::SameLine();
    if (ImGui::SmallButton("+"))
        hierarchy.CreateEntity();

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("New Entity");

    if (rTreeOpen)
    {
        registry.each([&](auto e) {
            using namespace ECS::Components;
            using namespace entt::literals;

            ECS::Entity entity(e, &registry);

            Tag tag{"Untitled (No Tag)"};
            if (entity.Has<Tag>())
                tag = entity.Get<Tag>();

            static auto eTreeOpenForce
                = entt::entity(entt::null);

            if (eTreeOpenForce == e)
            {
                ImGui::SetNextItemOpen(true);
                eTreeOpenForce = entt::null;
            }

            bool eTreeOpen = ImGui::TreeNodeEx(
                (void*)e,
                ImGuiTreeNodeFlags_AllowItemOverlap,
                "%s", tag.name.c_str()
            );

            if (!eTreeOpen) // ID gets pushed only when opened
                ImGui::PushID((void*)e);

            if (ImGui::IsItemHovered())
            {
                if (Input::GetKey(sf::Keyboard::Delete))
                    hierarchy.DestroyEntity(entity);
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                ImGui::OpenPopup("Delete");

            if (ImGui::BeginPopup("Delete"))
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImU32)ColorRed);

                if (ImGui::MenuItem("Delete?"))
                    hierarchy.DestroyEntity(entity);

                ImGui::PopStyleColor();
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            if (ImGui::SmallButton("+"))
                ImGui::OpenPopup("Components");

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("New Component");

            if (ImGui::BeginPopup("Components"))
            {
                ECS::Components::Types([&](entt::meta_type type) {
                    const char* name = type.prop("name"_hs)
                        .value()
                        .template cast<const char*>();

                    if (ImGui::MenuItem(name))
                    {
                        type.func("emplace"_hs)
                            .invoke({}, entt::forward_as_meta(registry), e);

                        eTreeOpenForce = e;
                    }
                });

                ImGui::EndPopup();
            }

            if (!eTreeOpen)
                ImGui::PopID();

            if (eTreeOpen)
            {
                entity.Each([&](auto type, auto& any)
                {
                    const char* name = type.prop("name"_hs)
                        .value()
                        .template cast<const char*>();

                    bool treeOpen = ImGui::TreeNodeEx(
                        name,
                        ImGuiTreeNodeFlags_DefaultOpen
                    );

                    if (!treeOpen)
                        ImGui::PushID(name);

                    if (ImGui::IsItemHovered())
                    {
                        if (Input::GetKey(sf::Keyboard::Delete))
                        {
                            type.func("remove"_hs)
                                .invoke({}, entt::forward_as_meta(registry), e);
                        }
                    }

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        ImGui::OpenPopup("Delete");

                    if (ImGui::BeginPopup("Delete"))
                    {
                        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImU32)ColorRed);

                        if (ImGui::MenuItem("Delete?"))
                        {
                            type.func("remove"_hs)
                                .invoke({}, entt::forward_as_meta(registry), e);
                        }

                        ImGui::PopStyleColor();
                        ImGui::EndPopup();
                    }

                    if (!treeOpen)
                        ImGui::PopID();

                    if (treeOpen)
                    {
                        for (auto data : type.data())
                            RenderAnyValue(data, any);

                        ImGui::TreePop();
                    }
                });

                ImGui::TreePop();
            }
        });

        ImGui::TreePop();
    }
}

void DebugMenu::HandleEditMenu(float dt)
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
                ImGui::Text("Name: %s", level.GetName().c_str());
                ImGui::Text("Size: %dx%d", levelSize.x, levelSize.y);

                RenderEntityTree(dt);

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

                            if (grid.Get(x, y).type == Cell::Wall)
                                drawList->AddRectFilled(min, max, ColorWhite);
                            else
                                drawList->AddRect(min, max, ColorWhite);

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
                                            ColorRed, 0, 0, s
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
        HandleEditMenu(dt);

    if (m_ShowDebugWindow)
        HandleDebugMenu(dt);

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
                HandleOpenMenu(dt);
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

    HandleNewPopup(dt);
}
