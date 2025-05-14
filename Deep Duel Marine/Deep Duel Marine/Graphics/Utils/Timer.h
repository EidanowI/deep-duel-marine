#pragma once
#include <chrono>



class Timer {
public:
	static void Update() noexcept;
	static float GetMsFromLastFrame() noexcept;


private:
	static std::chrono::time_point<std::chrono::high_resolution_clock> s_gameStart_timePoint;
	static std::chrono::time_point<std::chrono::high_resolution_clock> s_lastFrame_timePoint;
};