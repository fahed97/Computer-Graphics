#include "MeshModel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>


MeshModel::MeshModel()
{
}

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name, glm::mat4& TransformationMatrix, MeshModelValues ModelValues) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	textureCoords_(textureCoords),
	model_name_(model_name),
	Transformation_Matrix(glm::mat4(1.0f)),
	Original_Transformation_Mat(TransformationMatrix),
	CurrentModelValues(ModelValues)

	


{
	if (model_name_ == "banana.obj" || model_name_ == "bishop.obj" || model_name_ == "pawn.obj" || model_name_ == "Sphere.obj")
	{
		translate_factor = 1;
		z_translate_factor = 1;
		setColordivisionfactor(1);
	}
	else if (model_name_ == "dolphin.obj")
	{
		translate_factor = 1;
		z_translate_factor = 1;
		setColordivisionfactor(6);
	}
	else if (model_name_ == "beethoven.obj")
	{
		translate_factor = 1;
		z_translate_factor = 1;
		setColordivisionfactor(12);
	}
	else
	{
		translate_factor = 1;
		z_translate_factor = 1;
		setColordivisionfactor(3);
	}
	specular_alpha = 1;
	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int vertexnormalindex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			VertexNormal vertexnormal;
			vertex.position = vertices[vertexIndex];
			
			if(normals.size()>0)
			vertex.normal = normals[vertexnormalindex];
			//vertexnormal.position = vertex.position;
			
			
			if (textureCoords.size() > 0)
			{
				
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords_[textureCoordsIndex];
			}
			



			modelVertices.push_back(vertex);
			

		}

	}
	for ( int i = 0; i < modelVertices.size(); i += 3) {
		
		Vertex& v0 = modelVertices[i + 0];
		Vertex& v1 = modelVertices[i + 1];
		Vertex& v2 = modelVertices[i + 2];


		glm::vec2& uv0 = v0.textureCoords;
		glm::vec2& uv1 = v1.textureCoords;
		glm::vec2& uv2 = v2.textureCoords;

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1.position - v0.position;
		glm::vec3 deltaPos2 = v2.position - v0.position;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		modelVertices[i + 0].tangent = tangent;
		modelVertices[i + 1].tangent = tangent;
		modelVertices[i + 2].tangent = tangent;

		modelVertices[i + 0].bitangent = bitangent;
		modelVertices[i + 1].bitangent = bitangent;
		modelVertices[i + 2].bitangent = bitangent;

		/*Vertex& v0 = modelVertices[i];
		Vertex& v1 = modelVertices[i + 1];
		Vertex& v2 = modelVertices[i + 2];

		glm::vec3 Edge1 = v1.position - v0.position;
		glm::vec3 Edge2 = v2.position - v0.position;

		float DeltaU1 = v1.textureCoords.x - v0.textureCoords.x;
		float DeltaV1 = v1.textureCoords.y - v0.textureCoords.y;
		float DeltaU2 = v2.textureCoords.x - v0.textureCoords.x;
		float DeltaV2 = v2.textureCoords.y - v0.textureCoords.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		glm::vec3 Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

		v0.tangent = glm::normalize(Tangent);
		v1.tangent = glm::normalize(Tangent);
		v2.tangent = glm::normalize(Tangent);
		v0.bitangent = Bitangent;
		v1.bitangent = Bitangent;
		v2.bitangent = Bitangent;*/
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Tangant Normals Coords
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// BiTangant Normals Coords
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);

	//glGenVertexArrays(1, &vertexNormalsVAO);
	//glGenBuffers(1, &vertexNormalsVBO);
	//
	//glBindVertexArray(vertexNormalsVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsVBO);
	//glBufferData(GL_ARRAY_BUFFER, verticesNormals.size() * sizeof(verticesNormals), &verticesNormals[0], GL_STATIC_DRAW);
	//
	//// FaceNormalVertex Positions
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(verticesNormals), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}
void MeshModel::ModelPrint() const
{
	std::string Model_Name = GetModelName();
	std::cout << "Loaded Model: " << Model_Name.substr(0,Model_Name.length()-4) << "\n";
	std::cout << "--------------------";

	for (int FaceNum = 0; FaceNum < this->GetFacesCount(); FaceNum++) {
		std::cout << "\nFace" << FaceNum + 1 << ": \n";
		this->GetFace(FaceNum).FacePrint();
		
	}
	std::cout << "\n--------------------\n\n";

	for (std::vector<int>::size_type vtx = 0; vtx != vertices_.size(); vtx++) {
		std::cout << "Vertex: " << vtx + 1 <<" X:"<< vertices_[vtx].x << " Y:" << vertices_[vtx].y << " Z:" << vertices_[vtx].z << "\n";
	}
	std::cout << "\n--------------------\n\n";

	/*for (std::vector<int>::size_type normal = 0; normal != normals_.size(); normal++) {
		std::cout << "Normal "<<normal+1<<" X:"<<normals_[normal].x << " Y:" << normals_[normal].y << " Z:" << normals_[normal].z << "\n";
	}*/
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

void MeshModel::setmodelname(const std::string model_name)
{
	this->model_name_ = model_name;
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

const glm::vec3 MeshModel::getVertex(int Index) const
{

	return vertices_[Index-1];


}
const glm::vec3 MeshModel::getVertexNormal(int Index) const
{

	return normals_[Index - 1];


}

const glm::mat4x4 MeshModel::getTransformationMatrix() const
{
	return this->Transformation_Matrix;
}

const void MeshModel::setTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation) //0 translate,1 scale, 2 rotate
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
		this->ScaleMatrix = TransformationMatrix;
		break;
	}
	case 2:
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

const void MeshModel::setLocalTransformationMatrix(const glm::mat4x4& TransformationMatrix, int Transformation)//0 translate,1 scale, 2 rotate
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
		this->ScaleMatrix_Local = TransformationMatrix;
		break;
	}
	case 2:
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

const void MeshModel::ResetTransformationMatrix()
{
	this->TranslateMatrix = glm::mat4(1.0f);
	this->ScaleMatrix = glm::mat4(1.0f);
	this->RotateMatrix= glm::mat4(1.0f);
	MeshModel::setTransformation();

}
const void MeshModel::ResetLocalTransformationMatrix()
{
	this->TranslateMatrix_Local = glm::mat4(1.0f);
	this->ScaleMatrix_Local = glm::mat4(1.0f);
	this->RotateMatrix_Local = glm::mat4(1.0f);
	MeshModel::setTransformation();

}

const void MeshModel::setTransformation()
{
	
	glm::mat4 Mw_Mo = ScaleMatrix * RotateMatrix * TranslateMatrix * ScaleMatrix_Local * RotateMatrix_Local * TranslateMatrix_Local;
	this->Transformation_Matrix = Mw_Mo;
}

const glm::mat4 MeshModel::getOriginalTransformationMatrix() const
{
	return this->Original_Transformation_Mat;
}

const MeshModelValues MeshModel::getMeshModelValues() const
{
	return this->CurrentModelValues;
}

const void MeshModel::BoundingBox(bool BoundingBox,glm::vec3 Color )
{
	this->ShowBoundingBox = BoundingBox;
	this->BoundingBoxColor = Color;
}

const bool MeshModel::getBoundingBoxgflag()
{
	return this->ShowBoundingBox;
}

const glm::vec3 MeshModel::getBoundingBoxColor()
{
	return BoundingBoxColor;
}

const void MeshModel::FaceNormals(bool FaceNormals, glm::vec3 Color)
{
	this->ShowFaceNormals = FaceNormals;
	this->FaceNormalsColor = Color;
}
const bool MeshModel::getFacesNormalsflag()
{
	return ShowFaceNormals;
}

const glm::vec3 MeshModel::getFacesNormalsColor()
{
	return FaceNormalsColor;
}

const void MeshModel::VertexNormals(bool VertexNormals, glm::vec3 Color)
{
	this->ShowVertexNormals = VertexNormals;
	this->VertexNormalsColor = Color;
}
const bool MeshModel::getVertexNormalsflag()
{
	return ShowVertexNormals;
}

const glm::vec3 MeshModel::getVertexNormalsColor()
{
	return VertexNormalsColor;
}

const glm::mat4 MeshModel::getLocalRotationMatrix() const
{
	return RotateMatrix_Local;
}

const glm::mat4 MeshModel::getWorldRotationMatrix() const
{
	return RotateMatrix;
}

const void MeshModel::faceColor()
{

	for (int FaceNum = 0; FaceNum < GetFacesCount(); FaceNum++)
	{
		float randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glm::vec3 color(randomNumber_x, randomNumber_y, randomNumber_z);
		faces_[FaceNum].setFaceColor(color);
	}

}


glm::vec3 MeshModel::getAmbient() const
{
    return ambient;
}

void MeshModel::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

glm::vec3 MeshModel::getDiffuse() const
{
    return diffuse;
}

void MeshModel::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

glm::vec3 MeshModel::getSpecular() const
{
    return specular;
}

void MeshModel::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}



int MeshModel::getColordivisionfactor() const
{
    return color_division_factor;
}

void MeshModel::setColordivisionfactor(int colordivisionfactor)
{
    color_division_factor = colordivisionfactor;
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}
const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

int MeshModel::getTranslatefactor() const
{
	return translate_factor;
}

void MeshModel::setTranslatefactor(int translatefactor)
{
	translate_factor = translatefactor;
}

int MeshModel::getzTranslatefactor() const
{
	return this->z_translate_factor;
}
void MeshModel::setzTranslatefactor(int ztranslatefactor)
{
	this->z_translate_factor = ztranslatefactor;
}

int MeshModel::getSpecularalpha() const
{
	return specular_alpha;
}

void MeshModel::setSpecularalpha(int specularalpha)
{
	specular_alpha = specularalpha;
}
GLuint MeshModel::getVertexNormalsVAO() const
{
    return vertexNormalsVAO;
}

void MeshModel::setVertexNormalsVAO(GLuint vertexNormalsVAO)
{
    this->vertexNormalsVAO = vertexNormalsVAO;
}

GLuint MeshModel::getVertexNormalsVBO() const
{
    return vertexNormalsVBO;
}

void MeshModel::setVertexNormalsVBO(GLuint vertexNormalsVBO)
{
    this->vertexNormalsVBO = vertexNormalsVBO;
}

const bool MeshModel::getistextures()
{
	return this->istextures;
}

const void MeshModel::setistextures(bool flag)
{
	this->istextures = flag;
}

bool MeshModel::getistexturescoords()
{
	return(textureCoords_.size() > 0);
}


bool MeshModel::getPlanar() const
{
    return planar;
}

void MeshModel::setPlanar(bool planar)
{
    this->planar = planar;
}

bool MeshModel::getSpherical() const
{
    return spherical;
}

void MeshModel::setSpherical(bool spherical)
{
    this->spherical = spherical;
}

bool MeshModel::getNormalMapping() const
{
	return normalmapping;
}

void MeshModel::setNormalMapping(bool normalmapping)
{
	this->normalmapping = normalmapping;
}

bool MeshModel::getSpecularMapping() const
{
	return specularmapping;
}

void MeshModel::setSpecularMapping(bool specularmapping)
{
	this->specularmapping = specularmapping;
}






