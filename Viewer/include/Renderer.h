#pragma once
#include "Scene.h"
#include "Utils.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector> 
#include "Texture2D.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void ClearZBuffer();
	int GetViewportWidth() const;
	int GetViewportHeight() const;

	glm::vec3 GetDiffuseReflection(glm::vec3 Normal, glm::vec3 I, const Scene& scene, int activelight);


	//glm::vec3 GetDiffuseReflection(glm::vec3& p, glm::vec3& n, Scene& scene, glm::mat4x4& TransMat, glm::mat4x4& ViewPortMatrix);

	glm::vec3 GetAmbientReflection(glm::vec3 MaterialColor, glm::vec3 LightColor);

	glm::vec3 GetSpecularColor(glm::vec3 I, glm::vec3 Normal, const Scene& scene, int activelight);

	void Fog(Scene& scene);
	void LoadShaders();
	void LoadTextures(const std::string& path, const Scene& scene);

	void LoadNormalTextures(const std::string& path, const Scene& scene);

	void LoadSpecularTextures(const std::string& path, const Scene& scene);






	
private:

	ShaderProgram lightShader;
	ShaderProgram colorShader;
	unsigned int normalMap;
	unsigned int diffuseMap;
	unsigned int specularMap;
	Texture2D texture1;
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2,  glm::vec3& color);
	void Renderer::DrawTriangles(const std::vector<glm::vec3> vertexPositions, glm::vec3& color);
	void CreateBuffers(int w, int h);
	void Put_z(int i, int j, float z);
	float get_Z(int i, int j) const;
	void CreateOpenGLBuffer();
	const glm::vec3 Renderer::ZBufferGrayscale(float z)const;
	void InitOpenGLRendering();
	void Renderer::fillZbuffer(std::vector<glm::vec3> vertexPositions);
	void clipping_pixels();
	float sign(const glm::ivec2& p, const glm::ivec3& p1, const glm::ivec3& p2);
	bool inTriangle(const glm::vec2& p, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	float Calculate_z(int point_x, int point_y, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3);
	void Renderer::ScanConversion(std::vector<glm::vec3> vertexPositions, bool grayscale,glm::vec3 facecolor);
	void ScanConversionFlat(std::vector<glm::vec3> vertexPositions, bool grayscale, glm::vec3 color, const Scene& scene);
	void ScanConversionGouraud(std::vector<glm::vec3> vertexPositions, glm::vec3 V1_Color, glm::vec3 V2_Color, glm::vec3 V3_Color, const Scene& scene);
	void ScanConversionPhong(std::vector<glm::vec3> vertexPositions, glm::vec3 V1_Normal, glm::vec3 V2_Normal, glm::vec3 V3_Normal, const Scene& scene, int activelight);
	glm::vec3 Calculate_Gouraud_Color(int i, int j, glm::vec3 V1_color, glm::vec3 V2_color, glm::vec3 V3_color, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 Calculate_Phong_Normal(int i, int j, glm::vec3 V1_Normal, glm::vec3 V2_Normal, glm::vec3 V3_Normal, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	float Renderer::area(const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);
	void FilterCreation();
	glm::vec3 BackgroundColor;
	glm::vec3 ModelColor;
	float* color_buffer_;
	float* z_buffer;
	
	int viewport_width_;
	int viewport_height_;
	float max_z= FLT_MIN , min_z= FLT_MAX;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	void createGaussian();
	//void creatBloom();
	double GKernel[3][3];
	double GaussianMatrixSum = 0;
	
};
