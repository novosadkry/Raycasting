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
        m_Layers.push_front(MakeShared<T>(std::forward<T>(value)));
    }

    template<typename T, bool Init = false, typename... Args>
    Shared<T> Emplace(Args&&... args)
    {
        auto layer = MakeShared<T>(std::forward<Args>(args)...);
        m_Layers.push_front(layer);

        if (Init) layer->Init();

        return layer;
    }

    Shared<Layer> Drop()
    {
        return m_Layers.front();
        m_Layers.pop_front();
    }

    template<typename... T, typename = std::enable_if<(sizeof...(T) > 1)>::type>
    std::tuple<Shared<T>...> Drop()
    {
        return std::make_tuple<Shared<T>...>(Drop<T>()...);
    }

    template<typename T>
    Shared<T> Drop()
    {
        Shared<T> value;
        for (auto it = m_Layers.begin(); it != m_Layers.end(); it++)
        {
            if (value = std::dynamic_pointer_cast<T>(*it))
            {
                it = m_Layers.erase(it);
                break;
            }
        }

        return value;
    }

    Shared<Layer>& Top()
    {
        return m_Layers.front();
    }

    void Clear()
    {
        m_Layers.clear();
    }

    std::deque<Shared<Layer>>::reverse_iterator begin()
    {
        return m_Layers.rbegin();
    }

    std::deque<Shared<Layer>>::reverse_iterator end()
    {
        return m_Layers.rend();
    }

private:
    std::deque<Shared<Layer>> m_Layers;
};
