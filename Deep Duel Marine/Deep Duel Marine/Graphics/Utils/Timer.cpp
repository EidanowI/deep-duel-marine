#include "Timer.h"



std::chrono::time_point<std::chrono::high_resolution_clock> Timer::s_gameStart_timePoint = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::high_resolution_clock> Timer::s_lastFrame_timePoint = std::chrono::high_resolution_clock::now();



void Timer::Update() noexcept {
	s_lastFrame_timePoint = std::chrono::high_resolution_clock::now();
}
float Timer::GetMsFromLastFrame() noexcept {
	std::chrono::duration time_for_computing = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch() - s_lastFrame_timePoint.time_since_epoch());

	return (float)time_for_computing.count() / 1000.0f;
}