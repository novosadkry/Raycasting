#pragma once
#include <Rpch.hpp>

#ifndef NDEBUG
    #define LOG(x) std::cout    \
        << "[" << __FILE__      \
        << ":" << __func__      \
        << ":" << __LINE__      \
        << "] " << (x)          \
        << std::endl;
    #define ASSERT(x) assert((x));
#else
    #define LOG(x)
    #define ASSERT(x)
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
