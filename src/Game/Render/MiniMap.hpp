#pragma once
#include "Object.hpp"
#include "Player.hpp"

enum MiniMapFlags
{
    SHOW_ALL        = -1,
    SHOW_NONE       = 0,
    SHOW_PLAYER     = 1 << 0,
    SHOW_LIGHTS     = 1 << 1,
    SHOW_WALL_RAYS  = 1 << 2,
    SHOW_LIGHT_RAYS = 1 << 3,
};

template<typename _Tu = std::underlying_type<MiniMapFlags>::type>
inline MiniMapFlags operator|(const MiniMapFlags& lhs, const MiniMapFlags& rhs)
{
    return static_cast<MiniMapFlags>(static_cast<_Tu>(lhs) | static_cast<_Tu>(rhs));
}

class MiniMap : public Object
{
public:
    MiniMap(std::shared_ptr<Player> player, sf::Vector2i size)
        : m_Spacing(1.0f), m_Size(size), m_Flags(SHOW_PLAYER | SHOW_LIGHTS), m_Origin({0, 0}), m_Player(player) { }

    MiniMap(std::shared_ptr<Player> player, sf::Vector2i size, MiniMapFlags flags)
        : m_Spacing(1.0f), m_Size(size), m_Flags(flags), m_Origin({0, 0}), m_Player(player) { }

    MiniMap(std::shared_ptr<Player> player, sf::Vector2i size, float spacing, sf::Vector2f origin, MiniMapFlags flags)
        : m_Spacing(spacing), m_Size(size), m_Flags(flags), m_Origin(origin), m_Player(player) { }

    inline sf::Vector2i GetSize()
    {
        return m_Size;
    }

    inline MiniMapFlags GetFlags()
    {
        return m_Flags;
    }

    inline void SetFlags(MiniMapFlags flags)
    {
        m_Flags = flags;
    }

    void Render(float dt) override;
    void RenderMiniMap();

private:
    float m_Spacing;
    sf::Vector2i m_Size;
    MiniMapFlags m_Flags;
    sf::Vector2f m_Origin;

    std::shared_ptr<Player> m_Player;
};