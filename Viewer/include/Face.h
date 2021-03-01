#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;
	void FacePrint() const;
	const glm::vec3 getFaceColor() const;
	const void setFaceColor(glm::vec3& color);
private:
	std::vector<int> vertex_indices_;
	std::vector<int> normal_indices_;
	std::vector<int> texture_indices_;
	glm::vec3 color;
};