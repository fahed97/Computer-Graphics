#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"

class Scene {
public:
	Scene();
	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	void PopActiveCamera();
	void PopActiveModel();
	void PopActiveLight();
	void PopLightbyIndex(int index);
	int GetModelCount() const;
	int GetActiveLightIndex() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera()const;

	void AddLight(const std::shared_ptr<Light>& light);
	int GetLightCount() const;
	Light& GetLight(int index)const;
	Light& GetActiveLight()const;
	void clearLights();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	void SetActiveLightIndex(int index);
	int GetActiveModelIndex() const;

	float getFogbegining() const;
	void setFogbegining(float fogbegining);

	float getFogending() const;
	void setFogending(float fogending);

	bool getLinearfog() const;
	void setLinearfog(bool linearfog);

	bool getExponentialfog() const;
	void setExponentialfog(bool exponentialfog);

	float getFogdensity() const;
	void setFogdensity(float fogdensity);

	bool getFog() const;
	void setFog(bool fog);

	int getFogstartmultfactor() const;
	void setFogstartmultfactor(int fogstartmultfactor);

	int getFogendmultfactor() const;
	void setFogendmultfactor(int fogendmultfactor);

	void clearModels();
	
private:
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;
	std::vector<std::shared_ptr<Light>> lights_;
	
	int active_camera_index_;
	int active_model_index_;
	int active_light_index_;
	
	float fog_begining=0;
	float fog_ending=0;
	bool linear_fog = true;
	bool exponential_fog = false;
	float fog_density = 0;
	bool fog=false;
	int fogstartmultfactor = 1;
	int fogendmultfactor = 1;

    




};