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
        m_Layers.push_front(MakeUnique<T>(std::forward<T>(value)));
    }

    template<typename T, bool Init = false, typename... Args>
    T& Emplace(Args&&... args)
    {
        auto layer = MakeUnique<T>(std::forward<Args>(args)...);

        T* ref = layer.get();
        if (Init) ref->Init();

        m_Layers.push_front(std::move(layer));

        return *ref;
    }

    void Drop()
    {
        m_Layers.pop_front();
    }

    template<typename... T, typename = std::enable_if<(sizeof...(T) > 1)>::type>
    void Drop()
    {
        return (Drop<T>(), ...);
    }

    template<typename T>
    void Drop()
    {
        for (auto it = m_Layers.begin(); it != m_Layers.end(); it++)
        {
            if (dynamic_cast<T*>(it->get()))
                it = m_Layers.erase(it);
        }
    }

    template<typename T>
    T* Get()
    {
        for (auto& layer : m_Layers)
        {
            if (T* value = dynamic_cast<T*>(layer.get()))
                return value;
        }

        return nullptr;
    }

    Layer& Top()
    {
        return *m_Layers.front();
    }

    void Clear()
    {
        m_Layers.clear();
    }

    std::deque<Unique<Layer>>::reverse_iterator begin()
    {
        return m_Layers.rbegin();
    }

    std::deque<Unique<Layer>>::reverse_iterator end()
    {
        return m_Layers.rend();
    }

private:
    std::deque<Unique<Layer>> m_Layers;
};
