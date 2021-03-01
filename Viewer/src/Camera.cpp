#include "Camera.h"




Camera::Camera()
{}

Camera::Camera(std::string ActiveModel, float& width_, float& height_)
{
    /*  right = 100;
      left = -100;
      top = 100;
      bottom = -100;
      near_ = 0.1;// 100;//100;
      far_ = 200;// -100;//-100;
      fovy = glm::radians(45.0f);*/
    aspect = width_ / height_;
    near_ = 0.1;
    far_ = 200;
    Orthographic = true;
    isLookat = true;
    if (ActiveModel == "banana.obj" || ActiveModel == "bishop.obj" || ActiveModel == "pawn.obj" || ActiveModel == "Sphere.obj")
    {
        UpdateValues(1);
        //translate_factor = 1;
        //z_translate_factor = 10000;
        //setColordivisionfactor(1);
    }
    else if (ActiveModel == "dolphin.obj")
    {
        UpdateValues(500);
        //translate_factor = 100;
        //z_translate_factor = 500;
        //setColordivisionfactor(6);
    }
    else if (ActiveModel == "beethoven.obj")
    {
        UpdateValues(45);
        //translate_factor = 15;
        //z_translate_factor = 4000;
        //setColordivisionfactor(12);
    }
    else
    {
        UpdateValues(30);
        //translate_factor = 10;
        //z_translate_factor = 3000;
        //setColordivisionfactor(3);
    }
    setPrespective(45.f);
    if (ActiveModel == "banana.obj" || ActiveModel == "bishop.obj" || ActiveModel == "pawn.obj" || ActiveModel == "Sphere.obj")
        eye = glm::vec3(0, 0, 1);
    else if (ActiveModel == "dolphin.obj"|| ActiveModel == "beethoven.obj")
        eye = glm::vec3(0, 0, 30);
    else
        eye = glm::vec3(0, 0, 10);
    //eye= glm::vec3(0, 0, 0.5);
    grayscale = true;
	at = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);
    UpdateOrthographic();
    SetCameraLookAt(eye, at, up);
    SetProjectionTransformation();

}

Camera::~Camera()
{
	
}

glm::mat4x4& Camera::getOrthographic()
{
	return this->Orthographic_transformation;
	// TODO: insert return statement here
}
const void Camera::setOrthographic(const float width)
{
    
    UpdateValues(width);
    this->Orthographic_transformation = glm::ortho(left, right, bottom, top, near_, far_);


}

const void Camera::UpdateOrthographic()
{
    this->Orthographic_transformation = glm::ortho(left, right, bottom, top, near_, far_);
     
}


glm::mat4x4& Camera::getPrespective()
{
	return this->Prespective_transformation;
	// TODO: insert return statement here
}

const void Camera::setPrespective(const float fovy_)
{

    UpdateFovy(fovy_);
	this->Prespective_transformation = glm::perspective(this->fovy, this->aspect, this->near_, this->far_);
}

const void Camera::updatePrespective()
{
    this->Prespective_transformation = glm::perspective(fovy, aspect, zNear, zFar);
}

const void Camera::UpdateFovy(const float fovy_)
{
    this->fovy = glm::radians(fovy_);
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
    this->LookAtMatrix = glm::lookAt(eye, at, up);
    this->eye = eye;
    this->at = at;
    this->up = up;

}

glm::mat4x4 Camera::getLookat()
{
    return this->LookAtMatrix;
}



const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}
void Camera::SetProjectionTransformation() 
{
    if (this->Orthographic)
        this->projection_transformation_ = this->Orthographic_transformation;
    else
        this->projection_transformation_ = this->Prespective_transformation;
}

const void Camera::UpdateValues(const float width)
{
    this->width = width;
    left = -width / 2;
    right = width / 2;
    height = width/aspect;
    bottom = -height / 2;
    top = height / 2;
}

const void Camera::setGrayscaleflag(bool flag)
{
    this->grayscale = flag;
}

const bool Camera::getGrayscaleflag()
{
    return this->grayscale;
}


const float Camera::getHeight()
{
    return this->height;
}
const float Camera::getWidth()
{
    return this->width;
}

const void Camera::setCameraTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation)
{
    switch (Transformation)
    {
    case 0:
    {
        this->Camera_TranslateMatrix = TransformationMatrix;
        break;
    }
    case 2:
    {
        if (Camera_RotateMatrix != glm::mat4(1.0f))
        {
            this->Camera_RotateMatrix = TransformationMatrix * Camera_RotateMatrix;
            break;
        }
        this->Camera_RotateMatrix = TransformationMatrix;
        break;
    }


    }

}



const void Camera::setCameraLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation)
{
    switch (Transformation)
    {
    case 0:
    {
        this->Camera_TranslateMatrix_Local = TransformationMatrix;
        break;
    }

    case 2:
    {
        if (Camera_RotateMatrix_Local != glm::mat4(1.0f))
        {
            this->Camera_RotateMatrix_Local = TransformationMatrix * Camera_RotateMatrix_Local;
            break;
        }
        this->Camera_RotateMatrix_Local = TransformationMatrix;
        break;
    }
    }
}



const void Camera::ResetCameraTransformationMatrix()
 {
        this->Camera_TranslateMatrix = glm::mat4(1.0f);
        this->Camera_RotateMatrix = glm::mat4(1.0f);
        setCameraTransformation();
 }

const void Camera::ResetCameraLocalTransformationMatrix()
{
    this->Camera_TranslateMatrix_Local = glm::mat4(1.0f);
    this->Camera_RotateMatrix_Local = glm::mat4(1.0f);
    setCameraTransformation();
}

const void Camera::setCameraTransformation()
{
    glm::mat4 Mw_Mo = Camera_RotateMatrix * Camera_TranslateMatrix * Camera_RotateMatrix_Local* Camera_TranslateMatrix_Local;
    
    Cinverse = inverse(Mw_Mo);
   
}
const glm::mat4x4 Camera::getCameraTransformation() const
{
    return this->Cinverse;
}


const void Camera::setLookatflag(bool flag)
{
    this->isLookat = flag;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}



const float Camera::getFovy() const
{
    return fovy;
}

void Camera::setFovy(const float fovy)
{
    this->fovy = fovy;
}

const float Camera::getZFar() const
{
    return zFar;
}

void Camera::setZFar(const float zFar)
{
    this->zFar = zFar;
}

const float Camera::getZNear() const
{
    return zNear;
}

void Camera::setZNear(const float zNear)
{
    this->zNear = zNear;
}

const float Camera::getAspect() const
{
    return aspect;
}

void Camera::setAspect(const float aspect)
{
    this->aspect = aspect;
}


glm::vec3 Camera::getEye() const
{
    return eye;
}

void Camera::setEye(glm::vec3 eye)
{
    this->eye = eye;
}

glm::vec3 Camera::getAt() const
{
    return at;
}

void Camera::setAt(glm::vec3 at)
{
    this->at = at;
}

glm::vec3 Camera::getUp() const
{
    return up;
}

void Camera::setUp(glm::vec3 up)
{
    this->up = up;
}


float Camera::getLeft() const
{
    return left;
}

void Camera::setLeft(float left)
{
    this->left = left;
}

float Camera::getRight() const
{
    return right;
}

void Camera::setRight(float right)
{
    this->right = right;
}

float Camera::getBottom() const
{
    return bottom;
}

void Camera::setBottom(float bottom)
{
    this->bottom = bottom;
}

float Camera::getTop() const
{
    return top;
}

void Camera::setTop(float top)
{
    this->top = top;
}


float Camera::getZoom() const
{
    return zoom;
}

void Camera::setZoom(float zoom)
{
    this->zoom = zoom;
}



bool Camera::getOrthographicflag() const
{
    return Orthographic;
}

void Camera::setOrthographicflag(bool Orthographic)
{
    this->Orthographic = Orthographic;
}

void Camera::ResetMatrices()
{
    this->view_transformation_ = glm::mat4(1.0f);
    this->projection_transformation_ = glm::mat4(1.0f);
    this->Orthographic_transformation = glm::mat4(1.0f);
    this->Prespective_transformation = glm::mat4(1.0f);

}


int Camera::getTranslatefactor() const
{
    return translate_factor;
}

void Camera::setTranslatefactor(int translatefactor)
{
    translate_factor = translatefactor;
}

int Camera::getzTranslatefactor() const
{
    return this->z_translate_factor;
}
void Camera::setzTranslatefactor(int ztranslatefactor)
{
    this->z_translate_factor = ztranslatefactor;
}



bool Camera::getGaussian() const
{
    return gaussian;
}

void Camera::setGaussian(bool gaussian)
{
    this->gaussian = gaussian;
}

bool Camera::getBloom() const
{
    return bloom;
}

void Camera::setBloom(bool bloom)
{
    this->bloom = bloom;
}

