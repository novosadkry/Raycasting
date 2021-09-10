#pragma once
#include <Core.hpp>

class Layer
{
public:
    Layer() = default;
    virtual ~Layer() = default;

    virtual void Init() { }
    virtual void Render(float dt) { }
};

class LayerStack
{
public:
    template<typename T>
    void Push(T&& value)
    {
        m_Layers.push_back(MakeShared<T>(std::forward<T>(value)));
    }

    template<typename T, typename... Args>
    void Emplace(Args&&... args)
    {
        m_Layers.push_back(MakeShared<T>(std::forward<Args>(args)...));
    }

    Shared<Layer> Drop()
    {
        return m_Layers.back();
        m_Layers.pop_back();
    }

    Shared<Layer>& Top()
    {
        return m_Layers.back();
    }

    void Clear()
    {
        m_Layers.clear();
    }

    std::deque<Shared<Layer>>::iterator begin()
    {
        return m_Layers.begin();
    }

    std::deque<Shared<Layer>>::iterator end()
    {
        return m_Layers.end();
    }

private:
    std::deque<Shared<Layer>> m_Layers;
};
