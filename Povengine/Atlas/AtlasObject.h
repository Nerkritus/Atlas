#pragma once
#include <string>

namespace Atlas
{
	class AtlasObject
	{
	public:

		std::string GetID() { return _id; }
		int GetNumID() { return _numID; }
		void SetID(const std::string& id) { _id = id; }
		void SetID(const int id) { _numID = id; }

	protected:
		std::string _id;
		int _numID;
	};
}