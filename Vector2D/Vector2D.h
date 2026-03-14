#pragma once


#include <type_traits>
#include <numbers>
#include <cmath>
#include <concepts>
#include <string>
#include <algorithm>
#include <iostream>
#include <optional>
#include <functional>
#include <utility>
#include <chrono>
#include <thread>
#include <vector>
#include <array>
#include <initializer_list>

#define INIT_LIST_MOVE_TO std::advance

#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() std::abort()
#endif

using namespace std::chrono_literals;

template<typename T>
concept NumericalValue = std::integral<T> || std::floating_point<T>;

template<std::invocable Func>
using StaticSwitchArray = const std::array<std::pair<std::function<bool()>, Func>, 3>;

template<std::invocable Func>
using StaticSwitchArray_Elem = std::pair<std::function<bool()>, Func>;

namespace Utility {

    template<std::invocable Func>
    std::invoke_result_t<Func> DO_ONCE(Func func, bool& reset)
    {
        static bool called{ false };

        if (called == false) {
            called = true;

            if (reset == true) called = false;

            return std::forward<Func>(func)();
        }
    }

    template<std::invocable Func>
    std::invoke_result_t<Func> DO_ONCE(Func func)
    {
        static bool called{ false };

        if (called == false) {
            called = true;

            return std::forward<Func>(func)();
        }
    }

    template<std::invocable Func>
    std::invoke_result_t<Func> DO_N_TIMES(Func func, bool& reset, uint16_t times)
    {
        static uint16_t callTimes{};

        if (callTimes < times) {
            callTimes += 1;

            if (reset == true && callTimes >= (times - 1)) {
                callTimes = 0;
            }

            return std::forward<Func>(func)();
        }

    }

    template<std::invocable Func>
    std::invoke_result_t<Func> DO_N_TIMES(Func func, uint16_t times)
    {
        static uint16_t callTimes{};

        if (callTimes < times) {
            callTimes += 1;

            return std::forward<Func>(func)();
        }

    }

    template<std::invocable<int> Func>
    void FOR_LOOP_WITH_DELAY(int loopStart, int loopEnd,const int step, const std::chrono::duration<float> delay, Func&& loop_body) {

        static int index = loopStart;

        loop_body(index);

        index+= step;

        std::this_thread::sleep_for(delay);

        if (index < loopEnd )
            FOR_LOOP_WITH_DELAY(index, loopEnd, step, delay, loop_body);
        
    }

    template<std::invocable Func , std::invocable Cond>
    void WHILE_LOOP_WITH_DELAY(Cond&& condition, const std::chrono::duration<float> delay, Func&& loop_body) {

        loop_body();
        std::this_thread::sleep_for(delay);

        if (condition())
            WHILE_LOOP_WITH_DELAY(condition, delay, loop_body);
        
    }

    template<std::invocable FuncA, std::invocable FuncB>
    void FLIP_FLOP(FuncA&& funcA, FuncB&& funcB) {
        bool bSwitch = false;

        if (bSwitch == false) {
            bSwitch = true;

            funcA();
        }

        if (bSwitch == true) {
            bSwitch = false;

            funcB();
        }

    }

    template<std::invocable Func>
    void MULTICALL(std::vector<Func> functionsToCall) {
        static int index = 0;

        functionsToCall[index]();

        index = index < (functionsToCall.size() - 1) ? index + 1 : 0;
    }

    template<std::invocable Func>
    void MULTICALL(std::initializer_list<Func> functionsToCall) {
        static int index = 0;

        auto list_start = functionsToCall.begin();

        INIT_LIST_MOVE_TO(list_start, index);

        (*list_start)();

        index = index < (functionsToCall.size() - 1) ? index + 1 : 0;
    }

    //Work in progress:
    template<std::invocable Func>
    void STATIC_SWITCH(StaticSwitchArray<Func> func)
    {
        for (StaticSwitchArray_Elem casePair : func)
        {
            if (casePair.first()) {
                casePair.second();
            }
        }
    }
    //==============================================

};

template<NumericalValue numerics>
class Vector2D {

public:

	numerics X, Y;

public:

	Vector2D(numerics x, numerics y) : X(x), Y(y) {};

	constexpr Vector2D() noexcept : X(0.f), Y(0.f) {};

    Vector2D(numerics value) : X(value), Y(value) {};

	constexpr const numerics& getX() const { return X; }

	constexpr const numerics& getY() const { return Y; }

	void setX(numerics X) {
		this->X = X;
	}

	void setY(numerics Y) {
		this->Y = Y;
	}

	explicit operator bool() const {
		return X != 0 || Y != 0;
	}
	
	constexpr Vector2D operator+(const Vector2D& other) const{
		return Vector2D(X + other.X, Y + other.Y);
	}

	constexpr Vector2D operator-(const Vector2D& other) const{
		return Vector2D(X - other.X, Y - other.Y);
	}

    constexpr Vector2D operator*(const Vector2D& other) const {
        return Vector2D(X * other.X, Y * other.Y);
    }

    constexpr Vector2D operator/(const Vector2D& other) const {
        return Vector2D(X / other.X, Y / other.Y);
    }

    constexpr Vector2D operator*(numerics value) const {
        return Vector2D(X * value, Y * value);
    }

    constexpr Vector2D operator/(numerics value) const {
        return Vector2D(X / value, Y / value);
    }

	static numerics dot(const Vector2D& a, const Vector2D& b)
	{
		return a.getX() * b.getX() + a.getY() * b.getY();
	}

	static numerics cross(const Vector2D& a, const Vector2D& b)
	{
		return a.getX() * b.getY() - a.getY() * b.getX();
	}

	static float deg2rad(numerics degrees)
	{
		return degrees * std::numbers::pi_v<float> / 180.0f;
	}

	static float rad2deg(numerics radians)
	{
		return radians * 180.0f / std::numbers::pi_v<float>;
	}

	std::string to_string() const {
		return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
	}

	static double getLength(const Vector2D<numerics>& vec) {
		return (double)(std::sqrt(vec.X * vec.X + vec.Y * vec.Y));
	}

	constexpr static float getAngle_Radians(const Vector2D& vec1, const Vector2D& vec2) {
		return std::atan2(cross(vec1, vec2), dot(vec1, vec2));
	}

	constexpr static float getAngle_Degrees(const Vector2D& vec1, const Vector2D& vec2) {
		return (getAngle_Radians(vec1, vec2) * 180.f / 3.14159265);
	}

	constexpr static float getDistance(const Vector2D& point1, const Vector2D& point2) {
		return std::sqrt(std::pow((point2.X - point1.X), 2.f) + std::pow((point2.Y - point1.Y), 2.f));
	}

	constexpr static double getPerpendicularDistanceToLine(const Vector2D& lineStart, const Vector2D& lineEnd, const Vector2D& point) {
		return std::abs((lineEnd.Y - lineStart.Y) * point.X - (lineEnd.X - lineStart.X) * point.Y + lineEnd.X * lineStart.Y - lineEnd.Y * lineStart.X) / std::sqrt((std::pow((lineEnd.Y - lineStart.Y), 2.f) + std::pow((lineEnd.X - lineStart.X), 2.f)));
	}

    constexpr static numerics mapRangeUnclamped(numerics value, numerics inA, numerics inB, numerics outA, numerics outB) {
        return outA + (value - inA) * (outB - outA) / (inB - inA);
    }

    constexpr static numerics mapRangeClamped(numerics value, numerics inA, numerics inB, numerics outA, numerics outB) {
        return std::clamp((outA + (value - inA) * (outB - outA) / (inB - inA)), outA, outB);
    }

    constexpr static Vector2D<numerics> angleToVector(float angle) {
        return Vector2D<numerics>(std::cos(deg2rad(angle)), std::sin(deg2rad(angle)));
    }

    inline constexpr static Vector2D<float> getRightVector() noexcept {
        return Vector2D<float>(1.f, 0.f);
    }

    inline constexpr static Vector2D<float> getUpVector() noexcept {
        return Vector2D<float>(0.f, 1.f);
    }

    inline constexpr static Vector2D<float> ZeroVector() noexcept {
        return Vector2D<float>(NULL, NULL);
    }

    constexpr static std::optional<Vector2D<numerics>> getIntersectionPoint(Vector2D<numerics> D1, Vector2D<numerics> D2, Vector2D<numerics> P1, Vector2D<numerics> P2) {

        float denom = D1.getX() * D2.getY() - D1.getY() * D2.getX();
        if (denom == 0.f) {
            std::cout << "Lines are parallel or coincident.\n";

            return std::nullopt;
        }
        else {
            float t = ((P2.getX() - P1.getX()) * D2.getY() - (P2.getY() - P1.getY()) * D2.getX()) / denom;
            Vector2D<numerics> intersection = P1 + (D1 * t);
            std::cout << "Intersection at: " << intersection.to_string() << "\n";

            return intersection;
        }
    }

    static numerics Num_InterpTo(numerics current, numerics target, float deltaTime, numerics speed)
    {

        static float startDeltaTime;

        Utility::DO_ONCE([deltaTime]() mutable {startDeltaTime = deltaTime; });

        float localDeltaTime =  deltaTime - startDeltaTime;

        float alpha = std::clamp(localDeltaTime * speed, 0.0f, 1.0f);
        return current + (target - current) * alpha;
    }

    static const Vector2D<numerics>& Vec_InterpTo(const Vector2D<numerics>& current, const Vector2D<numerics>& target, float deltaTime, numerics speed)
    {

        static float startDeltaTime;

        Utility::DO_ONCE([deltaTime]() mutable {startDeltaTime = deltaTime; });

        float localDeltaTime = deltaTime - startDeltaTime;

        float alpha = std::clamp(localDeltaTime * speed, 0.0f, 1.0f);
        return current + (target - current) * alpha;
    }

};

using MappedSpace = std::pair<Vector2D<double>, Vector2D<double>>;

