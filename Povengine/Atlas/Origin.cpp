#include "Origin.h"
#include "AtlasGL.h"

using namespace Atlas;

///
Origin::Origin(float size, Shader* shader)
	:BaseEntity(0, 0, 0, shader)
{
	_entityType = EntityTypeEnum::ET_Origin;

	SetUniformScale(size);

	Initialise(DataFormatEnum::DataColour);
}

Origin::~Origin()
{
}

void Origin::InitData()
{
	_numVertices = 6;
	// An array of 3 vectors which represents 3 vertices
	_data = new float[_numVertices * 6]{
		// Pos			// Colours
		0, 0, 0,		1.0f, 0.0f, 0.0f,
		1.0f, 0,0,		1.0f, 0.0f, 0.0f,
		0, 0, 0,		0.0f, 1.0f, 0.0f,
		0, 1.0f, 0,		0.0f, 1.0f, 0.0f,
		0, 0, 0,		0.0f, 0.0f, 1.0f,
		0, 0, 1.0f,		0.0f, 0.0f, 1.0f,
	};

	
	_mode = GL_LINES;
}