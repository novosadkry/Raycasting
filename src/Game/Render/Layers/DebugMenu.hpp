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
    void InsertLastOpenFile(std::string path);
    void RemoveLastOpenFile(std::string path);
    void HandleOpenFile(std::string& path);
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
    char m_OpenNameBuffer[255] = {0};
    std::deque<std::string> m_LastOpenFiles;

    enum GridMode { SELECT, EDIT };
    GridMode m_CurrentGridMode;
};
