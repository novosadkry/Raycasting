#pragma once
#include <Core.hpp>

#include <Game/Render/Layer.hpp>

class DebugMenu : public Layer
{
public:
    void Render(float dt) override;

    inline void SetActive(bool value) { m_Active = value; };
    inline void Toggle() { m_Active = !m_Active; }

private:
    void InsertLastOpenFile(const std::fs::path& path);
    void RemoveLastOpenFile(const std::fs::path& path);
    void HandleOpenFile(const std::fs::path& path);
    void HandleOpenMenu(float dt);
    void HandleNewPopup(float dt);
    void HandleEditMenu(float dt);
    void HandleDebugMenu(float dt);
    void RenderEntityTree(float dt);
    void RenderAnyValue(auto& meta, auto& any);

    bool m_Active;
    bool m_ShowEditWindow;
    bool m_ShowDebugWindow;

    float m_ShowOpenFailTime;
    std::deque<std::string> m_LastOpenFiles;

    enum GridMode { SELECT, EDIT };
    GridMode m_CurrentGridMode;
};
