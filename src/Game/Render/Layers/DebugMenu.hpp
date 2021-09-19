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
    bool m_Active;
};
