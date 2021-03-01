#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshModelValues.h"

#include "Utils.h"


glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	float max_x=0, max_y = 0, max_z = 0, min_x=FLT_MIN, min_y=FLT_MIN, min_z = FLT_MIN;
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	auto ModelValues = MeshModelValues();
	std::vector<glm::vec2> textureCoords;
	
	std::ifstream ifile(filePath.c_str());
	
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			auto Vertex = Utils::Vec3fFromStream(issLine);
			if (Vertex.x > max_x)
			{
				max_x = Vertex.x;
			}
			if (Vertex.x < min_x)
			{
				min_x = Vertex.x;

			}
			if (Vertex.y > max_y)
			{
				max_y = Vertex.y;
			}
			if (Vertex.y < min_y)
			{
				min_y = Vertex.y;
			}
			if (Vertex.z > max_z)
			{
				max_z = Vertex.z;
			}
			if (Vertex.z < min_z)
			{
				min_z = Vertex.z;
			}
			vertices.push_back(Vertex);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));

		}
		else if (lineType == "vt")
		{
			textureCoords.push_back(Utils::Vec2fFromStream(issLine));
	
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
			
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	//Scale and Translation -> to fit in 0-1000 window
	glm::vec3 TranslateVec = glm::vec3(-min_x, -min_y, -min_z);
	glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4(1.0f),TranslateVec );
	auto max = std::max(max_x, max_y);
	auto final_max = std::max(max, max_z);
	float x_scale_factor = 300.0f / final_max;
	float y_scale_factor = 300.0f / final_max;
	float z_scale_factor = 300.0f/ final_max;
	glm::mat4x4 ScaleMatrix = glm::scale(glm::vec3(x_scale_factor, y_scale_factor, z_scale_factor));
	glm::mat4 Transformation_Matrix = ScaleMatrix*TranslateMatrix;
	


	ModelValues.MaxX_MaxY_MaxZ = glm::vec4(max_x, max_y, max_z,1);
	ModelValues.MaxX_MaxY_MinZ = glm::vec4(max_x, max_y, min_z, 1);
	ModelValues.MaxX_MinY_MaxZ = glm::vec4(max_x, min_y, max_z, 1);
	ModelValues.MaxX_MinY_MinZ = glm::vec4(max_x, min_y, min_z, 1);
	ModelValues.MinX_MaxY_MaxZ = glm::vec4(min_x, max_y, max_z, 1);
	ModelValues.MinX_MaxY_MinZ = glm::vec4(min_x, max_y, min_z, 1);
	ModelValues.MinX_MinY_MaxZ = glm::vec4(min_x, min_y, max_z, 1);
	ModelValues.MinX_MinY_MinZ = glm::vec4(min_x, min_y, min_z, 1);
	




	float x = (max_x + min_x);
	float y = (max_y+min_y);
	float z = (max_z+min_z);
	glm::vec3 ModelCenter(x / 2, y / 2, z / 2);

	ModelValues.ModelCenter = ModelCenter;










	


	

	return std::make_shared<MeshModel>(faces, vertices, normals,textureCoords, Utils::GetFileName(filePath),Transformation_Matrix, ModelValues);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	std::string Model_Name= filePath.substr(index + 1, len - index);
	return Model_Name;
}