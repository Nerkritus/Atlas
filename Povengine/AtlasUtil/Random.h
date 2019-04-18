#pragma once

#include <chrono>
#include <random>

namespace Atlas {

	class Random
	{
	public:
		static void Reseed();

		static int GetInt(int max);
		static int GetInt(int min, int max);
		static float GetFloat(float min = 0.0f, float max = 1.0f);
		
	private:

		static std::default_random_engine rng;
	};
}