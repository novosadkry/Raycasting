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
    static Unique<DERIVED> Deserialize(std::istream& stream);               \
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
    static Unique<BASE> Deserialize(std::istream& stream);                  \

#define SERIALIZE_HIERARCHY_REGISTER(DERIVED)                               \
    DERIVED::__SerializeConstructor DERIVED::__m_SerializeConstructor = {}; \

// ----------------------------------------------------------------------

template<typename T>
class Serializable
{
private:
    using DeserializeFunc = std::function<Unique<T>(std::istream&)>;
    using HierarchyMap = std::unordered_map<std::string, DeserializeFunc>;

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

    static Unique<T> Deserialize(std::istream& stream)
    {
        return T::Deserialize(stream);
    }

    static Unique<T> Deserialize(const std::string& name, std::istream& stream)
    {
        return GetHierarchy()[name](stream);
    }
};

// ----------------------------------------------------------------------

template<typename T, typename TBase = T>
inline void Serialize(const std::enable_if_t<std::is_base_of_v<Serializable<TBase>, T>, T>& value, std::ostream& stream)
{
    value.Serialize(stream);
}

template<typename T, typename TBase = T>
inline Unique<std::enable_if_t<std::is_base_of_v<Serializable<TBase>, T>, T>> Deserialize(std::istream& stream)
{
    return T::Deserialize(stream);
}

template<typename T>
inline Unique<std::enable_if_t<std::is_base_of_v<Serializable<T>, T>, T>> Deserialize(const std::string& name, std::istream& stream)
{
    return Serializable<T>::Deserialize(name, stream);
}

// ----------------------------------------------------------------------

template<typename T>
inline void Serialize(const std::enable_if_t<std::is_trivially_copyable_v<T>, T>& value, std::ostream& stream)
{
    stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<typename T, typename = std::enable_if_t<!std::is_array_v<T>>>
inline std::enable_if_t<std::is_trivially_copyable_v<T>, T> Deserialize(std::istream& stream)
{
    T value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

// ----------------------------------------------------------------------

template<typename Array, typename Size = size_t, typename T = std::remove_reference_t<decltype(std::declval<Array&>()[0])>>
inline void Serialize(const std::enable_if_t<std::is_trivially_copyable_v<T>, T>* value, Size n, std::ostream& stream)
{
    ::Serialize<Size>(n, stream);
    stream.write(reinterpret_cast<const char*>(value), sizeof(T) * n);
}

template<typename Array, typename Size = size_t, typename T = std::remove_reference_t<decltype(std::declval<Array&>()[0])>>
inline Unique<std::enable_if_t<std::is_trivially_copyable_v<T>, Array>> Deserialize(std::istream& stream, Size& n)
{
    n = ::Deserialize<Size>(stream);
    auto value = MakeUnique<Array>(n);

    stream.read(reinterpret_cast<char*>(value.get()), sizeof(T) * n);
    return value;
}

template<typename Array, typename Size = size_t, typename T = std::remove_reference_t<decltype(std::declval<Array&>()[0])>>
inline Unique<std::enable_if_t<std::is_trivially_copyable_v<T>, Array>> Deserialize(std::istream& stream)
{
    Size n;
    return ::Deserialize<Array>(stream, n);
}
