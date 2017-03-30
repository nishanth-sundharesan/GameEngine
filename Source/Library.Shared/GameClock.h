#pragma once
#include <exception>
#include <chrono>

namespace GameEngineLibrary
{
    class GameTime;

    class GameClock
    {
    public:
        GameClock();
		GameClock(const GameClock& rhs) = delete;
		GameClock& operator=(const GameClock& rhs) = delete;

		const std::chrono::high_resolution_clock::time_point& StartTime() const;
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		const std::chrono::high_resolution_clock::time_point& LastTime() const;

        void Reset();
        void UpdateGameTime(GameTime& gameTime);

		virtual ~GameClock() = default;
    private:
        std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::high_resolution_clock::time_point mLastTime;
    };
}
