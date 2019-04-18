#include "Random.h"

using namespace Atlas;

std::default_random_engine Random::rng(std::chrono::system_clock::now().time_since_epoch().count());

void Random::Reseed()
{
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

int Random::GetInt(int max)
{
	return GetInt(0, max);
}

int Random::GetInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

float Random::GetFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}