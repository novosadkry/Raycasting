#pragma once
#include <Core.hpp>

#ifdef _WIN32
    #define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#elif __linux__
    #define __FILENAME__ strrchr("/" __FILE__, '/') + 1
#else
    #define __FILENAME__ __FILE__
#endif

#ifdef DEBUG
    #define LOG(x) std::cout    \
        << "[" << __FILENAME__  \
        << ":" << __FUNCTION__  \
        << ":" << __LINE__      \
        << "] " << x            \
        << std::endl;

    #define ASSERT(x) assert((x));
#else
    #define LOG(x)
    #define ASSERT(x)
#endif

namespace Debug
{
    class MemoryAlloc
    {
    public:
        inline static MemoryAlloc& Get()
        {
            static MemoryAlloc m;
            return m;
        }

        inline size_t Total() { return m_Total; };
        inline size_t Freed() { return m_Freed; };

        inline size_t Allocated()
        {
            return m_Total - m_Freed;
        };

        friend void* ::operator new(size_t);
        friend void ::operator delete(void*, size_t);

    private:
        size_t m_Total;
        size_t m_Freed;
    };
}

#ifdef DEBUG
    #ifdef TRACK_MEM_ALLOC
        inline void* operator new(size_t size)
        {
            using namespace Debug;

            auto& memory = MemoryAlloc::Get();
            memory.m_Total += size;

            return std::malloc(size);
        }

        inline void operator delete(void* data, size_t size)
        {
            using namespace Debug;

            auto& memory = MemoryAlloc::Get();
            memory.m_Freed += size;

            return std::free(data);
        }
    #endif
#endif

namespace Debug
{
    class Timer
    {
        using Clock = std::chrono::high_resolution_clock;
        using Duration = std::chrono::milliseconds;

    public:
        void Start() { m_Start = Clock::now(); }
        void Stop() { m_End = Clock::now(); }

        Duration Elapsed()
        {
            return std::chrono::duration_cast<Duration>(m_End - m_Start);
        }

        friend std::ostream& operator<<(std::ostream& os, Timer& timer)
        {
            auto elapsed = timer.Elapsed();
            return os << "Time elapsed: " << elapsed.count() << "ms";
        }

    private:
        std::chrono::time_point<Clock> m_Start, m_End;
    };

    class ScopedTimer : public Timer
    {
    public:
        ScopedTimer()
        {
            Start();
        }

        ~ScopedTimer()
        {
            Stop();
            LOG(*this)
        }
    };
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vector)
{
    return os
        << typeid(T).name()
        << "{ " << vector.x
        << ", " << vector.y
        << " }";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector3<T>& vector)
{
    return os
        << typeid(T).name()
        << "{ " << vector.x
        << ", " << vector.y
        << ", " << vector.z
        << " }";
}
