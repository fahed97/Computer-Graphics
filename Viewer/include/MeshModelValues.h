#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include "Face.h"

class MeshModelValues
{
public:

	glm::vec4 MaxX_MaxY_MaxZ;
	glm::vec4 MaxX_MaxY_MinZ;
	glm::vec4 MaxX_MinY_MaxZ;
	glm::vec4 MaxX_MinY_MinZ;
	glm::vec4 MinX_MaxY_MaxZ;
	glm::vec4 MinX_MinY_MaxZ;
	glm::vec4 MinX_MaxY_MinZ;
	glm::vec4 MinX_MinY_MinZ;
	glm::vec3 ModelCenter;
	std::map<int, std::vector<int>> VertexNormals;
	
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::map<int, glm::vec3> VertexAverageNormal;
	std::string model_name_;

	glm::vec4 getMaxX_MaxY_MaxZ()
	{
		return this->MaxX_MaxY_MaxZ;
	}
	glm::vec4 getMaxX_MaxY_MinZ()
	{
		return this->MaxX_MaxY_MinZ;
	}
	glm::vec4 getMaxX_MinY_MaxZ()
	{
		return this->MaxX_MinY_MaxZ;
	}
	glm::vec4 getMaxX_MinY_MinZ()
	{
		return this->MaxX_MinY_MinZ;
	}
	glm::vec4 getMinX_MaxY_MaxZ()
	{
		return this->MinX_MaxY_MaxZ;
	}
	glm::vec4 getMinX_MinY_MaxZ()
	{
		return this->MinX_MinY_MaxZ;
	}
	glm::vec4 getMinX_MaxY_MinZ()
	{
		return this->MinX_MaxY_MinZ;
	}
	glm::vec4 getMinX_MinY_MinZ()
	{
		return this->MinX_MinY_MinZ;
	}
	glm::vec3 getModelCenter() const
	{
		return ModelCenter;
	}

    std::vector<Face> getFaces() const { return faces_; }
    void setFaces(std::vector<Face> faces) { faces_ = faces; }

    std::vector<glm::vec3> getVertices() const { return vertices_; }
    void setVertices(std::vector<glm::vec3> vertices) { vertices_ = vertices; }

    std::vector<glm::vec3> getNormals() const { return normals_; }
    void setNormals(std::vector<glm::vec3> normals) { normals_ = normals; }

    std::string getModelname() const { return model_name_; }
    void setModelname(std::string modelname) { model_name_ = modelname; }

};
#pragma once
