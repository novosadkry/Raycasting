/*
    Shitty, yet simple serialization header

    ----------- Base and hierarchy functions -----------

    Serialization:
        To serialize [base class] or [class inside hierarchy]:
            - call Serialize<Type>(value, stream)

    Deserialization:
        To deserialize [base class]:
            - call Deserialize<Type>(stream)

        To deserialize [class inside hierarchy]:
            - call Deserialize<Base>(stream)
            or
            - call Deserialize<Type, Base>(stream)

        To deserialize [class inside hierarchy] with name:
            - call Deserialize<Base>(name, stream)

    ---------------------  Usage -----------------------

    Example [base class]:
        - Foo.hpp
            class Foo : public Serializable<Foo>
            {
            public:
                SERIALIZE_BASE(Foo)
                ...
            }
        - Foo.cpp
            void Foo::Serialize(std::ostream& stream) const
            {
                ...
            }
            Foo Foo::Deserialize(std::istream& stream)
            {
                ...
            }

    Example [class inside hierarchy]:
        - Bar.hpp
            class Bar : public Foo
            {
            public:
                SERIALIZE_HIERARCHY(Bar, Foo)
                ...
            }
        - Bar.cpp
            SERIALIZE_HIERARCHY_REGISTER(Bar)
            ... (same as Foo.cpp example)
*/

#pragma once
#include <Core.hpp>

#define SERIALIZE_HIERARCHY(DERIVED, BASE)                                  \
    void Serialize(std::ostream& stream) const override;                    \
    static DERIVED Deserialize(std::istream& stream);                       \
    struct __SerializeConstructor                                           \
    {                                                                       \
        __SerializeConstructor()                                            \
        {                                                                   \
            Serializable<BASE>::Register(#DERIVED, DERIVED::Deserialize);   \
        }                                                                   \
    };                                                                      \
    static __SerializeConstructor __m_SerializeConstructor;                 \

#define SERIALIZE_BASE(BASE)                                                \
    void Serialize(std::ostream& stream) const override;                    \
    static BASE Deserialize(std::istream& stream);                          \

#define SERIALIZE_HIERARCHY_REGISTER(DERIVED)                               \
    DERIVED::__SerializeConstructor DERIVED::__m_SerializeConstructor = {}; \

// ----------------------------------------------------------------------

template<typename T>
class Serializable
{
private:
    using DeserializeFunc = std::function<T(std::istream&)>;
    using HierarchyMap = std::map<std::string, DeserializeFunc>;

    static HierarchyMap& GetHierarchy()
    {
        static HierarchyMap map = {};
        return map;
    }

public:
    static void Register(const std::string& name, DeserializeFunc func)
    {
        GetHierarchy()[name] = func;
    }

    virtual void Serialize(std::ostream& stream) const
    { };

    static T Deserialize(std::istream& stream)
    {
        return T::Deserialize(stream);
    }

    static T Deserialize(const std::string& name, std::istream& stream)
    {
        return GetHierarchy()[name](stream);
    }
};

// ----------------------------------------------------------------------

template<typename T, typename TBase = T>
inline void Serialize(const std::enable_if_t<std::is_base_of<Serializable<TBase>, T>::value, T>& value, std::ostream& stream)
{
    value.Serialize(stream);
}

template<typename T, typename TBase = T>
inline std::enable_if_t<std::is_base_of<Serializable<TBase>, T>::value, T> Deserialize(std::istream& stream)
{
    return T::Deserialize(stream);
}

template<typename T>
inline std::enable_if_t<std::is_base_of<Serializable<T>, T>::value, T> Deserialize(const std::string& name, std::istream& stream)
{
    return Serializable<T>::Deserialize(name, stream);
}

// ----------------------------------------------------------------------

template<typename T>
inline void Serialize(const std::enable_if_t<std::is_trivially_copyable<T>::value, T>& value, std::ostream& stream)
{
    stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<typename T>
inline std::enable_if_t<std::is_trivially_copyable<T>::value, T> Deserialize(std::istream& stream)
{
    T value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

// ----------------------------------------------------------------------

template<typename T>
inline void Serialize(const std::enable_if_t<std::is_trivially_copyable<T>::value, T>* value, int x, std::ostream& stream)
{
    stream.write(reinterpret_cast<const char*>(value), sizeof(T) * x);
}

template<typename T>
inline std::enable_if_t<std::is_trivially_copyable<T>::value, T>* Deserialize(int x, std::istream& stream)
{
    T* value = new T[x];
    stream.read(reinterpret_cast<char*>(value), sizeof(T) * x);
    return value;
}