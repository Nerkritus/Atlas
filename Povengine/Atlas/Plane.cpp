#include "Plane.h"
#include "glew.h"

using namespace Atlas;

///
Plane::Plane(float size, float x, float y, float z, unsigned int shaderProgramID)
	:PhysicsEntity(x, y, z, shaderProgramID)
{
	_entityType = EntityTypeEnum::ET_Plane;

	_size = size;

	Initialise(DataFormatEnum::DataColourTex);
}

/// <summary>
///	Initialise the vertex and index data for a plane.
/// </summary>
void Plane::InitData()
{
	_numIndices = 6;
	_indices = new unsigned short[_numIndices]
	{
			0, 1, 2,
			2, 3, 0,
	};

	_numVertices = 4;
	// An array of 3 vectors which represents 3 vertices
	_data = new float[_numVertices * _dataFormat] {
		-_size, 0, -_size,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		_size, 0, -_size,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		_size, 0, _size,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		-_size, 0, _size,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f
	};
}