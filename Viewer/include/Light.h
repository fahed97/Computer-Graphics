#pragma once
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
class Light
{
public:
	
	Light();
	void setLightType(int type);
	const int getLightType() const;

	glm::vec3 getPointambient() const;
	void setPointambient(glm::vec3 pointambient);

	glm::vec3 getPointdiffuse() const;
	void setPointdiffuse(glm::vec3 pointdiffuse);

	glm::vec3 getPointspecular() const;
	void setPointspecular(glm::vec3 pointspecular);

	glm::vec3 getParallelambient() const;
	void setParallelambient(glm::vec3 parallelambient);

	glm::vec3 getParalleldiffuse() const;
	void setParalleldiffuse(glm::vec3 paralleldiffuse);

	glm::vec3 getParallelspecular() const;
	void setParallelspecular(glm::vec3 parallelspecular);

	const void setTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation);

	const void setLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation);

	const void ResetTransformationMatrix();

	const void ResetLocalTransformationMatrix();

	const void setTransformation();

	const glm::mat4 getOriginalTransformationMatrix() const;

	const glm::mat4x4 getTransformationMatrix() const;

	
	float getLightXtranslate() const;
	void setLightXtranslate(float LightXtranslate);

	float getLightYtranslate() const;
	void setLightYtranslate(float LightYtranslate);

	float getLightZtranslate() const;
	void setLightZtranslate(float LightZtranslate);

	int getLightxAngle() const;
	void setLightxAngle(int LightxAngle);

	int getLightyAngle() const;
	void setLightyAngle(int LightyAngle);

	int getLightzAngle() const;
	void setLightzAngle(int LightzAngle);

	float getLightXtranslateLocal() const;
	void setLightXtranslateLocal(float LightXtranslateLocal);

	float getLightYtranslateLocal() const;
	void setLightYtranslateLocal(float LightYtranslateLocal);

	float getLightZtranslateLocal() const;
	void setLightZtranslateLocal(float LightZtranslateLocal);

	int getLightxAngleLocal() const;
	void setLightxAngleLocal(int LightxAngleLocal);

	int getLightyAngleLocal() const;
	void setLightyAngleLocal(int LightyAngleLocal);

	int getLightzAngleLocal() const;
	void setLightzAngleLocal(int LightzAngleLocal);

	glm::vec4 getLightPosition() const;
	void setLightPosition(glm::vec4 LightPosition);

	glm::vec3 getLightDirection() const;
	void setLightDirection(glm::vec3 LightDirection);

	
	bool getFlat() const;
	void setFlat(bool flat);

	bool getGouraud() const;
	void setGouraud(bool gouraud);

	bool getPhong() const;
	void setPhong(bool phong);

	int getShading() const;

	int getSpecularalpha() const;
	void setSpecularalpha(int specularalpha);


	GLuint getVBO() const;
	void setVBO(GLuint VBO);

	GLuint getVAO() const;
	void setVAO(GLuint VAO);
private:
	int lighttype = 0; //0= Point Source , 1= Parallel Source
	int specular_alpha = 1;
	glm::vec3 point_ambient=glm::vec3(0, 0, 0);
	glm::vec3 point_diffuse=glm::vec3(0, 0, 0);
	glm::vec3 point_specular=glm::vec3(0, 0, 0);
	glm::vec3 parallel_ambient = glm::vec3(0, 0, 0);
	glm::vec3 parallel_diffuse = glm::vec3(0, 0, 0);
	glm::vec3 parallel_specular = glm::vec3(0, 0, 0);
	glm::mat4x4 Transformation_Matrix = glm::mat4(1.0f);
	glm::mat4x4 TranslateMatrix = glm::mat4(1.0f);
	glm::mat4x4 TranslateMatrix_Local = glm::mat4(1.0f);
	glm::mat4x4 RotateMatrix = glm::mat4(1.0f);
	glm::mat4x4 RotateMatrix_Local = glm::mat4(1.0f);
	glm::mat4x4 Original_Transformation_Mat = glm::mat4(1.0f);
	glm::vec4 LightPosition = glm::vec4(0, 0, 0,1);
	glm::vec3 LightDirection = glm::vec3(0, 0, 0);
	float LightXtranslate = 0.f;
	float LightYtranslate = 0.f;
	float LightZtranslate = 0.f;
	int LightxAngle = 0;
	int LightyAngle = 0;
	int LightzAngle = 0;
	float LightXtranslate_Local = 0.f;
	float LightYtranslate_Local = 0.f;
	float LightZtranslate_Local = 0.f;
	int LightxAngle_Local = 0;
	int LightyAngle_Local = 0;
	int LightzAngle_Local = 0;
	bool flat = false;
	bool gouraud = false;
	bool phong = false;

	GLuint VBO, VAO;


    




 


    


 

};

