#pragma once
#include "MeshModel.h"
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Camera
{
public:
	Camera();
	Camera(std::string ActiveModel, float& width_, float& height_);

	
	virtual ~Camera();
	
	void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	glm::mat4x4 getLookat();
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	
	glm::mat4x4& Camera::getOrthographic();
	const void Camera::UpdateOrthographic();
	const void setOrthographic(const float width);

	//const void setOrthographic(const float left, const float right, const float bottom, const float top, const float near_, const float far_);

	glm::mat4x4& getPrespective();
	const void setPrespective(const float fovy);
	const void updatePrespective();
	const void UpdateFovy(const float fovy);
	
	const float getFovy() const;
	void setFovy(const float fovy);

	const float getZFar() const;
	void setZFar(const float zFar);

	const float getZNear() const;
	void setZNear(const float zNear);

	const float getAspect() const;
	void setAspect(const float aspect);

	glm::vec3 getEye() const;
	void setEye(glm::vec3 eye);

	glm::vec3 getAt() const;
	void setAt(glm::vec3 at);

	glm::vec3 getUp() const;
	void setUp(glm::vec3 up);
	float getLeft() const;
	void setLeft(float left);

	float getRight() const;
	void setRight(float right);

	float getBottom() const;
	void setBottom(float bottom);

	float getTop() const;
	void setTop(float top);

	float getZoom() const;
	void setZoom(float zoom);



	bool getOrthographicflag() const;
	void setOrthographicflag(bool Orthographic);

	void SetProjectionTransformation();

	const void UpdateValues(const float width);
	const void setGrayscaleflag(bool flag);
	const bool getGrayscaleflag();

	const float getHeight();
	const float getWidth();

	const void setCameraTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation);

	const void setCameraLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation);

	const void ResetCameraTransformationMatrix();

	const void ResetCameraLocalTransformationMatrix();

	const void setCameraTransformation();

	const void setLookatflag(bool flag);
	void ResetMatrices();
	const glm::mat4x4 getCameraTransformation() const;
	int getTranslatefactor() const;
	void setTranslatefactor(int translatefactor);

	int getzTranslatefactor() const;
	void setzTranslatefactor(int ztranslatefactor);


	bool getGaussian() const;
	void setGaussian(bool gaussian);
	bool getBloom() const;
	void setBloom(bool bloom);

private:
	float left;
	float right;
	float bottom;
	float top;
	float near_;
	float far_;
	float zoom;
	glm::mat4x4 view_transformation_= glm::mat4(1.0f);
	glm::mat4x4 projection_transformation_= glm::mat4(1.0f);
	glm::mat4x4 Orthographic_transformation= glm::mat4(1.0f);
	glm::mat4x4 Prespective_transformation= glm::mat4(1.0f);
	glm::mat4x4 LookAtMatrix;
	glm::mat4x4 Cinverse = glm::mat4(1.0f);;
	float fovy, aspect, zNear, zFar;
	glm::vec3 eye,at, up;
	bool Orthographic;
	float width;
	float height;
	bool isLookat;
	bool grayscale=true;
	glm::mat4x4 Camera_Transformation_Matrix = glm::mat4(1.0f);
	glm::mat4x4 Camera_TranslateMatrix = glm::mat4(1.0f);
	glm::mat4x4 Camera_TranslateMatrix_Local = glm::mat4(1.0f);
	glm::mat4x4 Camera_RotateMatrix = glm::mat4(1.0f);
	glm::mat4x4 Camera_RotateMatrix_Local = glm::mat4(1.0f);
	int translate_factor = 0;
	int z_translate_factor = 0;
	bool gaussian = false;
	bool bloom = false;







 


    





  


};
