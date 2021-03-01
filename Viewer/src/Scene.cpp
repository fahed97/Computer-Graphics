#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Utils.h"

Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0),
	active_light_index_(0)
{


}





void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
}
void Scene::PopActiveCamera()
{
	cameras_.pop_back();
	PopActiveModel();
}

void Scene::PopActiveModel()
{
	mesh_models_.pop_back();
	
}

void Scene::PopActiveLight()
{
	lights_.pop_back();
}

void Scene::PopLightbyIndex(int index)
{
	lights_.erase(lights_.begin() + index - 1);
}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

int Scene::GetActiveLightIndex() const
{
	return active_light_index_;
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras_[index];
}

Camera& Scene::GetActiveCamera() const
{
	return *cameras_[active_camera_index_];
}

void Scene::AddLight(const std::shared_ptr<Light>& light)
{
	lights_.push_back(light);
}

int Scene::GetLightCount() const
{
	return lights_.size();
}

Light& Scene::GetLight(int index) const
{
	return *lights_[index];
}

Light& Scene::GetActiveLight() const
{
	return *lights_[active_light_index_];
}

void Scene::clearLights() 
{
	lights_.clear();
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}
void Scene::SetActiveLightIndex(int index)
{
	active_light_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}
float Scene::getFogbegining() const
{
    return fog_begining;
}

void Scene::setFogbegining(float fogbegining)
{
    fog_begining = fogbegining;
}

float Scene::getFogending() const
{
    return fog_ending;
}

void Scene::setFogending(float fogending)
{
    fog_ending = fogending;
}

bool Scene::getLinearfog() const
{
    return linear_fog;
}

void Scene::setLinearfog(bool linearfog)
{
    linear_fog = linearfog;
}

bool Scene::getExponentialfog() const
{
    return exponential_fog;
}

void Scene::setExponentialfog(bool exponentialfog)
{
    exponential_fog = exponentialfog;
}

float Scene::getFogdensity() const
{
    return fog_density;
}

void Scene::setFogdensity(float fogdensity)
{
    fog_density = fogdensity;
}


bool Scene::getFog() const
{
    return fog;
}

void Scene::setFog(bool fog)
{
    this->fog = fog;
}


int Scene::getFogstartmultfactor() const
{
    return fogstartmultfactor;
}

void Scene::setFogstartmultfactor(int fogstartmultfactor)
{
    this->fogstartmultfactor = fogstartmultfactor;
}

int Scene::getFogendmultfactor() const
{
    return fogendmultfactor;
}

void Scene::setFogendmultfactor(int fogendmultfactor)
{
    this->fogendmultfactor = fogendmultfactor;
}

void Scene::clearModels()
{
	mesh_models_.clear();
}