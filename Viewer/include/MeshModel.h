#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include "Face.h"
#include "Texture2D.h"
#include "MeshModelValues.h"
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};



struct FaceNormal
{
	glm::vec3 position;
};

struct VertexNormal
{
	glm::vec3 position;
};
class MeshModel
{
public:
	MeshModel();
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name,glm::mat4& TransformationMatrix,MeshModelValues ModelValues);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	void ModelPrint() const;
	void setmodelname(const std::string model_name);
	const glm::vec3 MeshModel::getVertex(int Index) const;
	const glm::vec3 getVertexNormal(int Index) const;
	const glm::mat4 MeshModel::getTransformationMatrix() const;
	const void setTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation);

	const void setLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix,int Transformation);

	const void ResetTransformationMatrix();

	const void ResetLocalTransformationMatrix();

	const void setTransformation();

	const glm::mat4 MeshModel::getOriginalTransformationMatrix() const;
	const MeshModelValues MeshModel::getMeshModelValues() const;
	const void MeshModel::BoundingBox(bool BoundingBox, glm::vec3 Color);
	const bool MeshModel::getBoundingBoxgflag();
	const glm::vec3 MeshModel::getBoundingBoxColor();
	const void FaceNormals(bool FaceNormals, glm::vec3 Color);
	const bool getFacesNormalsflag();
	const glm::vec3 getFacesNormalsColor();
	const void VertexNormals(bool VertexNormals, glm::vec3 Color);
	const bool getVertexNormalsflag();
	const glm::vec3 getVertexNormalsColor();
	const glm::mat4 getLocalRotationMatrix() const;
	const glm::mat4 getWorldRotationMatrix() const;
	std::vector<Face> getFaces() const { return faces_; }

	std::vector<glm::vec3> getVertices() const { return vertices_; }

	std::vector<glm::vec3> getNormals() const { return normals_; }

	const std::string getModelname() const { return model_name_; }
	
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	glm::mat4x4 Original_Transformation_Mat = glm::mat4(1.0f);
	MeshModelValues CurrentModelValues;
	const void faceColor();
	glm::vec3 getAmbient() const;
	void setAmbient(glm::vec3 ambient);

	glm::vec3 getDiffuse() const;
	void setDiffuse(glm::vec3 diffuse);

	glm::vec3 getSpecular() const;
	void setSpecular(glm::vec3 specular);

	int getColordivisionfactor() const;
	void setColordivisionfactor(int colordivisionfactor);
	std::vector<glm::vec2> textureCoords_;
	GLuint GetVAO() const;
	const std::vector<Vertex>& GetModelVertices();
	int getTranslatefactor() const;
	void setTranslatefactor(int translatefactor);

	int getzTranslatefactor() const;
	void setzTranslatefactor(int ztranslatefactor);

	int getSpecularalpha() const;

	void setSpecularalpha(int specularalpha);

	GLuint getVertexNormalsVAO() const;
	void setVertexNormalsVAO(GLuint vertexNormalsVAO);

	GLuint getVertexNormalsVBO() const;
	void setVertexNormalsVBO(GLuint vertexNormalsVBO);

	const bool getistextures();
	bool getPlanar() const;
	void setPlanar(bool planar);

	bool getSpherical() const;
	void setSpherical(bool spherical);

	bool getNormalMapping() const;

	void setNormalMapping(bool normalmapping);

	bool getSpecularMapping() const;

	void setSpecularMapping(bool specularmapping);

	const void setistextures(bool flag);

	bool getistexturescoords();
	Texture2D modeltexture;
	Texture2D modelspeculartexture;
	Texture2D normalmapltexture;

private:
	
	glm::mat4x4 Transformation_Matrix=glm::mat4 (1.0f);
	glm::mat4x4 TranslateMatrix = glm::mat4(1.0f);
	glm::mat4x4 TranslateMatrix_Local = glm::mat4(1.0f);
	glm::mat4x4 ScaleMatrix = glm::mat4(1.0f);
	glm::mat4x4 ScaleMatrix_Local = glm::mat4(1.0f);
	glm::mat4x4 RotateMatrix = glm::mat4(1.0f);
	glm::mat4x4 RotateMatrix_Local = glm::mat4(1.0f);
	glm::vec3 face_Color = glm::vec3(1, 1, 1);
	glm::vec3 ambient = glm::vec3(0, 0, 0);
	glm::vec3 diffuse = glm::vec3(0, 0, 0);
	glm::vec3 specular = glm::vec3(0, 0, 0);
	bool ShowBoundingBox = false;
	glm::vec3 BoundingBoxColor;
	bool ShowFaceNormals = false;
	glm::vec3 FaceNormalsColor;
	bool ShowVertexNormals = false;
	glm::vec3 VertexNormalsColor;
	int color_division_factor;
	bool istextures=false;
	

	int z_translate_factor = 0;
	int translate_factor = 0;

	
	

	int specular_alpha;
	
	bool planar=false;
	bool spherical=false;

	
	bool normalmapping = false;
	bool specularmapping = false;


  
protected:

	

	std::vector<Vertex> modelVertices;
	std::vector<VertexNormal> verticesNormals;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;

	glm::vec3 color;

	GLuint vbo;
	GLuint vao;
	GLuint vertexNormalsVAO;
	GLuint vertexNormalsVBO;
 




 




};
