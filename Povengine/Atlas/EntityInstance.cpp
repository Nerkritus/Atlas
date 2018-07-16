#include "EntityInstance.h"

using namespace Atlas;

EntityInstance::EntityInstance()
	:_initialised(false), _internalEntity(nullptr), Transformable(glm::vec3(0,0,0))
{

}

bool EntityInstance::Initialise(std::string& id, BaseEntity* entity)
{
	if (_initialised) {
		return false;
	}

	_id = id;
	_internalEntity = entity;
	_visible = true;
	_initialised = true;
}

void EntityInstance::Update()
{
	if (_internalEntity != nullptr) {
		_internalEntity->Update();
	}
}

/// <summary>
/// Add a text object to the scene
/// </summary>
void EntityInstance::SetMaterial(Material& mat)
{
	_material.diffuseColour = mat.diffuseColour;
	_material.ambientColour = mat.ambientColour;
	_material.specularColour = mat.specularColour;
	_material.shininess = mat.shininess;
}

/// <summary>
/// Prepare the internal entity to be rendered with the appropriate parameters for this instance
/// </summary>
void EntityInstance::PrepareEntity()
{
	_internalEntity->SetScale(GetScaleX(), GetScaleY(), GetScaleZ());
	_internalEntity->SetPosition(GetPosition());
	_internalEntity->SetMaterial(_material);
}

/// <summary>
/// Creates a new entityinstance from the given create info class
/// </summary>
EntityInstance* EntityInstance::FromCreateInfo(EntityCreateInfo& info, BaseEntity* instanceOf)
{
	auto ei = new EntityInstance();
	ei->Initialise(info.id, instanceOf);
	ei->SetPosition(info.pos);
	ei->SetUniformScale(info.uniformScale);
	ei->SetID(info.numID);


	return ei;
}