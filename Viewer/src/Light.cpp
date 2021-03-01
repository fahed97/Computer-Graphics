#include "Light.h"


Light::Light()
{
	LightxAngle_Local = 0;
	LightyAngle_Local = 0;
	LightzAngle_Local = 0;
	LightXtranslate = 0.f;
	LightYtranslate = 0.f;
	LightZtranslate = 0.f;
	LightxAngle = 0;
	LightyAngle = 0;
	LightzAngle = 0;
	LightXtranslate_Local = 0.f;
	LightYtranslate_Local = 0.f;
	LightZtranslate_Local = 0.f;
	float randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	parallel_ambient = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	point_ambient = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	point_diffuse = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	point_specular = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	parallel_diffuse = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	parallel_specular= glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
	
	float vertices[] = {
		LightPosition.x, LightPosition.y, LightPosition.z  
  
	};

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Light::setLightType(int type)
{
	this->lighttype = type;

}

const int Light::getLightType() const
{
	return this->lighttype;
}

glm::vec3 Light::getPointambient() const
{
    return point_ambient;
}

void Light::setPointambient(glm::vec3 pointambient)
{
    point_ambient = pointambient;
}

glm::vec3 Light::getPointdiffuse() const
{
    return point_diffuse;
}

void Light::setPointdiffuse(glm::vec3 pointdiffuse)
{
    point_diffuse = pointdiffuse;
}

glm::vec3 Light::getPointspecular() const
{
    return point_specular;
}

void Light::setPointspecular(glm::vec3 pointspecular)
{
    point_specular = pointspecular;
}

glm::vec3 Light::getParallelambient() const
{
    return parallel_ambient;
}

void Light::setParallelambient(glm::vec3 parallelambient)
{
    parallel_ambient = parallelambient;
}

glm::vec3 Light::getParalleldiffuse() const
{
    return parallel_diffuse;
}

void Light::setParalleldiffuse(glm::vec3 paralleldiffuse)
{
    parallel_diffuse = paralleldiffuse;
}

glm::vec3 Light::getParallelspecular() const
{
    return parallel_specular;
}

void Light::setParallelspecular(glm::vec3 parallelspecular)
{
    parallel_specular = parallelspecular;
}


const void Light::setTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation) //0 translate,1 scale, 2 rotate
{

	switch (Transformation)
	{
	case 0:
	{
		this->TranslateMatrix = TransformationMatrix;
		break;
	}

	case 1:
	{
		if (RotateMatrix != glm::mat4(1.0f))
		{
			this->RotateMatrix = TransformationMatrix * RotateMatrix;
			break;
		}
		this->RotateMatrix = TransformationMatrix;
		break;
	}


	}

}

const void Light::setLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation)//0 translate,1 scale, 2 rotate
{
	switch (Transformation)
	{
	case 0:
	{
		this->TranslateMatrix_Local = TransformationMatrix;
		break;
	}
	case 1:

	{
		if (RotateMatrix_Local != glm::mat4(1.0f))
		{
			this->RotateMatrix_Local = TransformationMatrix * RotateMatrix_Local;
			break;
		}
		this->RotateMatrix_Local = TransformationMatrix;
		break;
	}


	}


}

const void Light::ResetTransformationMatrix()
{
	this->TranslateMatrix = glm::mat4(1.0f);
	this->RotateMatrix = glm::mat4(1.0f);
	Light::setTransformation();

}
const void Light::ResetLocalTransformationMatrix()
{
	this->TranslateMatrix_Local = glm::mat4(1.0f);
	this->RotateMatrix_Local = glm::mat4(1.0f);
	Light::setTransformation();

}

const void Light::setTransformation()
{

	glm::mat4 Mw_Mo = RotateMatrix * TranslateMatrix * RotateMatrix_Local * TranslateMatrix_Local;
	this->Transformation_Matrix = Mw_Mo;
}

const glm::mat4 Light::getOriginalTransformationMatrix() const
{
	return this->Original_Transformation_Mat;
}

const glm::mat4x4 Light::getTransformationMatrix() const
{
	return this->Transformation_Matrix;
}


float Light::getLightXtranslate() const
{
    return LightXtranslate;
}

void Light::setLightXtranslate(float LightXtranslate)
{
    this->LightXtranslate = LightXtranslate;
}

float Light::getLightYtranslate() const
{
    return LightYtranslate;
}

void Light::setLightYtranslate(float LightYtranslate)
{
    this->LightYtranslate = LightYtranslate;
}

float Light::getLightZtranslate() const
{
    return LightZtranslate;
}

void Light::setLightZtranslate(float LightZtranslate)
{
    this->LightZtranslate = LightZtranslate;
}

int Light::getLightxAngle() const
{
    return LightxAngle;
}

void Light::setLightxAngle(int LightxAngle)
{
    this->LightxAngle = LightxAngle;
}

int Light::getLightyAngle() const
{
    return LightyAngle;
}

void Light::setLightyAngle(int LightyAngle)
{
    this->LightyAngle = LightyAngle;
}

int Light::getLightzAngle() const
{
    return LightzAngle;
}

void Light::setLightzAngle(int LightzAngle)
{
    this->LightzAngle = LightzAngle;
}

float Light::getLightXtranslateLocal() const
{
    return LightXtranslate_Local;
}

void Light::setLightXtranslateLocal(float LightXtranslateLocal)
{
    LightXtranslate_Local = LightXtranslateLocal;
}

float Light::getLightYtranslateLocal() const
{
    return LightYtranslate_Local;
}

void Light::setLightYtranslateLocal(float LightYtranslateLocal)
{
    LightYtranslate_Local = LightYtranslateLocal;
}

float Light::getLightZtranslateLocal() const
{
    return LightZtranslate_Local;
}

void Light::setLightZtranslateLocal(float LightZtranslateLocal)
{
    LightZtranslate_Local = LightZtranslateLocal;
}

int Light::getLightxAngleLocal() const
{
    return LightxAngle_Local;
}

void Light::setLightxAngleLocal(int LightxAngleLocal)
{
    LightxAngle_Local = LightxAngleLocal;
}

int Light::getLightyAngleLocal() const
{
    return LightyAngle_Local;
}

void Light::setLightyAngleLocal(int LightyAngleLocal)
{
    LightyAngle_Local = LightyAngleLocal;
}

int Light::getLightzAngleLocal() const
{
    return LightzAngle_Local;
}

void Light::setLightzAngleLocal(int LightzAngleLocal)
{
    LightzAngle_Local = LightzAngleLocal;
}


glm::vec4 Light::getLightPosition() const
{
    return LightPosition;
}

void Light::setLightPosition(glm::vec4 LightPosition)
{
    this->LightPosition = LightPosition;
}

glm::vec3 Light::getLightDirection() const
{
    return LightDirection;
}

void Light::setLightDirection(glm::vec3 LightDirection)
{
    this->LightDirection = LightDirection;
}


bool Light::getFlat() const
{
    return flat;
}

void Light::setFlat(bool flat)
{
    this->flat = flat;
	if (flat)
	{
		setGouraud(false);
		setPhong(false);
	}
}

bool Light::getGouraud() const
{
    return gouraud;
}

void Light::setGouraud(bool gouraud)
{
    this->gouraud = gouraud;
	if (gouraud)
	{
		setFlat(false);
		setPhong(false);
	}
}

bool Light::getPhong() const
{
    return phong;
}

void Light::setPhong(bool phong)
{
    this->phong = phong;
	if (phong)
	{
		setFlat(false);
		setGouraud(false);
	}
}

int Light::getShading() const
{//0 flat, 1 gouraud, 2 phong
	if (flat)
		return 0;
	else if (gouraud)
		return 1;
	else return 2;
}


int Light::getSpecularalpha() const
{
    return specular_alpha;
}

void Light::setSpecularalpha(int specularalpha)
{
    specular_alpha = specularalpha;
}


GLuint Light::getVBO() const
{
    return VBO;
}

void Light::setVBO(GLuint VBO)
{
    this->VBO = VBO;
}

GLuint Light::getVAO() const
{
    return VAO;
}

void Light::setVAO(GLuint VAO)
{
    this->VAO = VAO;
}

