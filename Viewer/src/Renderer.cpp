#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))
#define Black glm::vec3(0,0,0)
#define White glm::vec3(1,1,1)

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	//InitOpenGLRendering();
	//CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	//delete[] color_buffer_;
	//delete[] z_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}



void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, glm::vec3& color)
{

	double a, e, DeltaX, DeltaY, Temp;
	int x = p1.x, y = p1.y, Loop = p2.x;
	DeltaY = (double)p2.y - p1.y;
	DeltaX = (double)p2.x - p1.x;
	a = DeltaY / DeltaX;
	if (DeltaX < 0 && DeltaY < 0) //Points are Flipped - a>0 
	{

		DrawLine(p2, p1, color);

	}
	if (DeltaY == 0)
	{
		if (p2.x > p1.x)
		{
			x = p1.x;
			while (x < p2.x)
			{

				PutPixel(x, p1.y, color);
				x += 1;
			}
		}
		else
		{
			x = p1.x;
			while (x > p2.x)
			{
				PutPixel(x, p2.y, color);
				x -= 1;
			}
		}

	}

	else if (a > 0)

	{

		if (a > 1) //Switch a>1
		{

			x = p1.y;
			y = p1.x;
			Temp = DeltaX;
			DeltaX = DeltaY;
			DeltaY = Temp;
			e = -DeltaX;
			Loop = p2.y;
			while (x <= Loop)
			{
				if (e > 0)
				{
					y += 1;
					e -= (2 * DeltaX);
				}
				PutPixel(y, x, color);
				x += 1;
				e += (2 * DeltaY);
			}


		}

		else   //0<a<1
		{
			e = -DeltaX;

			while (x <= Loop)
			{
				if (e > 0)
				{
					y += 1;
					e -= (2 * DeltaX);
				}
				PutPixel(x, y, color);
				x += 1;
				e += (2 * DeltaY);
			}
		}
	}
	else if (a < 0)
	{

		if (a < -1) //Switch&Reflect
		{
			x = p1.y;
			y = p1.x;
			e = -DeltaX;
			Temp = DeltaX;
			DeltaX = DeltaY;
			DeltaY = Temp;
			Loop = p2.y;
			if (DeltaY < 0)
			{
				DeltaX *= (-1);
				while (x <= Loop)
				{
					if (e < 0)
					{
						y -= 1;
						e -= (2 * DeltaX);
					}
					PutPixel(y, x, color);
					x += 1;
					e += (2 * DeltaY);
				}
			}
			else
			{
				DrawLine(p2, p1, color);
			}


		}
		else
		{
			if (a == -1)
			{
				Loop = p2.x;
				if (DeltaY < 0)
				{

					while (x < Loop)
					{
						PutPixel(x, y, color);
						x += 1;
						y -= 1;

					}
				}
				else
				{
					while (x > Loop)
					{
						PutPixel(x, y, color);
						x -= 1;
						y += 1;
					}

				}
			}
			else {
				if (DeltaY < 0)
				{
					DeltaY *= (-1);
					e = -DeltaX;
					while (x <= Loop)
					{
						if (e > 0)
						{
							y -= 1;
							e -= (2 * DeltaX);
						}
						PutPixel(x, y, color);
						x += 1;
						e += (2 * DeltaY);
					}
				}
				else
				{
					DeltaX *= (-1);
					e = -DeltaX;
					while (x >= Loop)
					{
						if (e > 0)
						{
							y += 1;
							e -= (2 * DeltaX);
						}
						PutPixel(x, y, color);
						x -= 1;
						e += (2 * DeltaY);
					}

				}
			}

		}


	}


	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
}

void Renderer::DrawTriangles(const std::vector<glm::vec3> vertexPositions, glm::vec3& color)
{
	glm::ivec2 P_1_Final(vertexPositions[0].x, vertexPositions[0].y);
	glm::ivec2 P_2_Final(vertexPositions[1].x, vertexPositions[1].y);
	glm::ivec2 P_3_Final(vertexPositions[2].x, vertexPositions[2].y);

	DrawLine(P_1_Final, P_2_Final, color);

	DrawLine(P_2_Final, P_3_Final, color);

	DrawLine(P_1_Final, P_3_Final, color);
}

//void Renderer::CreateBuffers(int w, int h)
//{
//	CreateOpenGLBuffer(); //Do not remove this line.
//	color_buffer_ = new float[3 * w * h];
//	z_buffer = new float[w * h];
//
//	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
//	ClearZBuffer();
//}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
//void Renderer::InitOpenGLRendering()
//{
//	// Creates a unique identifier for an opengl texture.
//	glGenTextures(1, &gl_screen_tex_);
//
//	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
//	glGenVertexArrays(1, &gl_screen_vtc_);
//
//	GLuint buffer;
//
//	// Makes this VAO the current one.
//	glBindVertexArray(gl_screen_vtc_);
//
//	// Creates a unique identifier for a buffer.
//	glGenBuffers(1, &buffer);
//
//	// (-1, 1)____(1, 1)
//	//	     |\  |
//	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
//	//	     |__\|
//	// (-1,-1)    (1,-1)
//	const GLfloat vtc[] = {
//		-1, -1,
//		 1, -1,
//		-1,  1,
//		-1,  1,
//		 1, -1,
//		 1,  1
//	};
//
//	const GLfloat tex[] = {
//		0,0,
//		1,0,
//		0,1,
//		0,1,
//		1,0,
//		1,1 };
//
//	// Makes this buffer the current one.
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//
//	// This is the opengl way for doing malloc on the gpu. 
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);
//
//	// memcopy vtc to buffer[0,sizeof(vtc)-1]
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
//
//	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
//
//	// Loads and compiles a sheder.
//	GLuint program = InitShader("vshader_color.glsl", "fshader_color.glsl");
//
//	// Make this program the current one.
//	glUseProgram(program);
//
//	// Tells the shader where to look for the vertex position data, and the data dimensions.
//	GLint  vPosition = glGetAttribLocation(program, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//	// Same for texture coordinates data.
//	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
//	glEnableVertexAttribArray(vTexCoord);
//	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));
//
//	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
//
//	// Tells the shader to use GL_TEXTURE0 as the texture id.
//	glUniform1i(glGetUniformLocation(program, "texture"), 0);
//}
/*
void Renderer::fillZbuffer(std::vector<glm::vec3> vertexPositions)
{
	glm::vec3 P1 = vertexPositions[0];
	glm::vec3 P2 = vertexPositions[1];
	glm::vec3 P3 = vertexPositions[2];



	float x_Min = std::min(P1.x, P2.x);
	x_Min = std::min(x_Min, P3.x);
	float x_Max = std::max(P1.x, P2.x);
	x_Max = std::max(x_Max, P3.x);
	bool x_Start_flag = false;
	int x_Start = 0, x_End = 0;
	int yStart = std::min(std::min(P1.y,P2.y),P3.y);
	int yEnd = std::max(std::max(P1.y, P2.y), P3.y);
	for (int j = yStart; j <= yEnd; j++)
	{
		for (int i = x_Min; i <= x_Max; i++)
		{
			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
			{
				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
				{
					float z = Calculate_z(i, j, P1, P2, P3);
					if (z <=  get_Z(i, j))
					{
						Put_z(i, j, z);



					}
				}
			}
		}
	}
}
void Renderer::ScanConversion(std::vector<glm::vec3> vertexPositions, bool grayscale, glm::vec3 facecolor)
{//sorted by y  - descending 



	//Renderer::DrawTriangles(vertexPositions, color);
	glm::vec3 P1 = vertexPositions[0];
	glm::vec3 P2 = vertexPositions[1];
	glm::vec3 P3 = vertexPositions[2];



	float x_Min = std::min(P1.x, P2.x);
	x_Min = std::min(x_Min, P3.x);
	float x_Max = std::max(P1.x, P2.x);
	x_Max = std::max(x_Max, P3.x);
	bool x_Start_flag = false;
	int x_Start = 0, x_End = 0;
	int yStart = std::min(P1.y, std::min(P2.y, P3.y));
	int yEnd = std::max(P1.y, std::max(P2.y, P3.y));
	float max_color = 1;
	float max_element = 1;

	for (int j = yStart; j <= yEnd; j++)
	{
		for (int i = x_Min; i <= x_Max; i++)
		{
			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
			{
				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
				{
					float z = Calculate_z(i, j, P1, P2, P3);
					if (z == get_Z(i, j))
					{
					
						if (grayscale)
							facecolor=ZBufferGrayscale(z);
					
						PutPixel(i, j, facecolor);
					
					}
					//PutPixel(i, j, facecolor);
				}
			}
		}
	}

	
}

void Renderer::ScanConversionFlat(std::vector<glm::vec3> vertexPositions, bool grayscale, glm::vec3 color,const Scene& scene)
	{//sorted by y  - descending 



		//Renderer::DrawTriangles(vertexPositions, color);
		glm::vec3 P1 = vertexPositions[0];
		glm::vec3 P2 = vertexPositions[1];
		glm::vec3 P3 = vertexPositions[2];



		float x_Min = std::min(P1.x, P2.x);
		x_Min = std::min(x_Min, P3.x);
		float x_Max = std::max(P1.x, P2.x);
		x_Max = std::max(x_Max, P3.x);
		bool x_Start_flag = false;
		int x_Start = 0, x_End = 0;
		int yStart = std::min(P1.y, std::min(P2.y, P3.y));
		int yEnd = std::max(P1.y, std::max(P2.y, P3.y));
		int divisionfactor = scene.GetActiveCamera().getColordivisionfactor();
		glm::vec3 max_color;
		float max_element = 1;

		for (int j = yStart; j <= yEnd; j++)
		{
			for (int i = x_Min; i <= x_Max; i++)
			{
				if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
				{
					if (inTriangle(glm::vec2(i, j), P1, P2, P3))
					{
						float z = Calculate_z(i, j, P1, P2, P3);
						if (z == get_Z(i, j))
						{

							color_buffer_[INDEX(viewport_width_, i, j, 0)] = (color_buffer_[INDEX(viewport_width_, i, j, 0)] + color.x/divisionfactor);
							color_buffer_[INDEX(viewport_width_, i, j, 1)] = (color_buffer_[INDEX(viewport_width_, i, j, 1)] + color.y/divisionfactor);
							color_buffer_[INDEX(viewport_width_, i, j, 2)] = (color_buffer_[INDEX(viewport_width_, i, j, 2)] + color.z/divisionfactor);
							//max_color.x = color_buffer_[INDEX(viewport_width_, i, j, 0)] + color.x;
							//max_color.y = color_buffer_[INDEX(viewport_width_, i, j, 1)] + color.y;
							//max_color.z = color_buffer_[INDEX(viewport_width_, i, j, 2)] + color.z;
							//if (max_color.x > 1)
							//	max_element = std::max(max_color.x, max_element);
							//if (max_color.y > 1)
							//	max_element = std::max(max_color.y, max_element);
							//if (max_color.z > 1)
							//	max_element = std::max(max_color.z, max_element);
						}

					}
				}
			}
		}

	//		for (int j = yStart; j <= yEnd; j++)
	//		{
	//			for (int i = x_Min; i <= x_Max; i++)
	//			{
	//				if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
	//				{
	//					if (inTriangle(glm::vec2(i, j), P1, P2, P3))
	//					{
	//						float z = Calculate_z(i, j, P1, P2, P3);
	//						if (z == get_Z(i, j))
	//						{
	//						
	//							
	//								color_buffer_[INDEX(viewport_width_, i, j, 0)] = (color_buffer_[INDEX(viewport_width_, i, j, 0)] + color.x) / max_element;
	//								color_buffer_[INDEX(viewport_width_, i, j, 1)] = (color_buffer_[INDEX(viewport_width_, i, j, 1)] + color.y) / max_element;
	//								color_buffer_[INDEX(viewport_width_, i, j, 2)] = (color_buffer_[INDEX(viewport_width_, i, j, 2)] + color.z) / max_element;
	//							
	//	
	//						}
	//	
	//					}
	//				}
	//			}
	//		}
	//		

}
void Renderer::ScanConversionGouraud(std::vector<glm::vec3> vertexPositions,glm::vec3 V1_Color, glm::vec3 V2_Color, glm::vec3 V3_Color, const Scene& scene)
{//sorted by y  - descending 



	glm::vec3 P1 = vertexPositions[0];
	glm::vec3 P2 = vertexPositions[1];
	glm::vec3 P3 = vertexPositions[2];



	float x_Min = std::min(P1.x, P2.x);
	x_Min = std::min(x_Min, P3.x);
	float x_Max = std::max(P1.x, P2.x);
	x_Max = std::max(x_Max, P3.x);
	int x_Start = 0, x_End = 0;
	int yStart = std::min(P1.y,std::min(P2.y,P3.y));
	int yEnd = std::max(P1.y, std::max(P2.y, P3.y));
	float max_element = 1;
	int divisionfactor = scene.GetActiveCamera().getColordivisionfactor();
	glm::vec3 max_color;


	for (int j = yStart; j <= yEnd; j++)
	{
		for (int i = x_Min; i <= x_Max; i++)
		{
			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
			{
				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
				{
					float z = Calculate_z(i, j, P1, P2, P3);
					if (z == get_Z(i, j))
					{
						glm::vec3 Color = Calculate_Gouraud_Color(i, j, V1_Color, V2_Color, V3_Color, P1, P2, P3);
						//max_color.x = color_buffer_[INDEX(viewport_width_, i, j, 0)] + Color.x;
						//max_color.y = color_buffer_[INDEX(viewport_width_, i, j, 1)] + Color.y;
						//max_color.z = color_buffer_[INDEX(viewport_width_, i, j, 2)] + Color.z;
						//if (max_color.x > 1)
						//	max_element = std::max(max_color.x, max_element);
						//if (max_color.y > 1)
						//	max_element = std::max(max_color.y, max_element);
						//if (max_color.z > 1)
						//	max_element = std::max(max_color.z, max_element);
						color_buffer_[INDEX(viewport_width_, i, j, 0)] += Color.x/divisionfactor;
						color_buffer_[INDEX(viewport_width_, i, j, 1)] += Color.y/divisionfactor;
						color_buffer_[INDEX(viewport_width_, i, j, 2)] += Color.z/divisionfactor;
						//PutPixel(i, j, Color);
					}
				}
			}
		}
	}
	*/

//	for (int j = yStart; j <= yEnd; j++)
//	{
//		for (int i = x_Min; i <= x_Max; i++)
//		{
//			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
//			{
//				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
//				{
//					float z = Calculate_z(i, j, P1, P2, P3);
//					if (z == get_Z(i, j))
//					{
//						glm::vec3 Color = Calculate_Gouraud_Color(i, j, V1_Color, V2_Color, V3_Color, P1, P2, P3);
//						//Color.x /= max_element;
//						//Color.y /= max_element;
//						//Color.z /= max_element;
//						//color_buffer_[INDEX(viewport_width_, i, j, 0)] += Color.x;
//						//color_buffer_[INDEX(viewport_width_, i, j, 1)] += Color.y;
//							//color_buffer_[INDEX(viewport_width_, i, j, 2)] += Color.z
//						
//	
//						color_buffer_[INDEX(viewport_width_, i, j, 0)] = (color_buffer_[INDEX(viewport_width_, i, j, 0)] + Color.x) / max_element;
//						color_buffer_[INDEX(viewport_width_, i, j, 1)] = (color_buffer_[INDEX(viewport_width_, i, j, 1)] + Color.y) / max_element;
//						color_buffer_[INDEX(viewport_width_, i, j, 2)] = (color_buffer_[INDEX(viewport_width_, i, j, 2)] + Color.z) / max_element;
//					}
//				}
//			}
//		}
//	}




//}

/*
void Renderer::ScanConversionPhong(std::vector<glm::vec3> vertexPositions, glm::vec3 V1_Normal, glm::vec3 V2_Normal, glm::vec3 V3_Normal, const Scene& scene,int activelight)
{//sorted by y  - descending 



	glm::vec3 P1 = vertexPositions[0];
	glm::vec3 P2 = vertexPositions[1];
	glm::vec3 P3 = vertexPositions[2];





	float x_Min = std::min(P1.x, P2.x);
	x_Min = std::min(x_Min, P3.x);
	float x_Max = std::max(P1.x, P2.x);
	x_Max = std::max(x_Max, P3.x);
	int x_Start = 0, x_End = 0;
	int yStart = std::min(P1.y, std::min(P2.y, P3.y));
	int yEnd = std::max(P1.y, std::max(P2.y, P3.y));
	glm::vec3 LightPosition = scene.GetLight(activelight).getLightPosition();
	glm::vec3 LightDirection = glm::normalize(scene.GetLight(activelight).getLightDirection());
	int divisionfactor = scene.GetActiveCamera().getColordivisionfactor();
	glm::vec3 I_a, I_d, I_s;
	glm::vec3 Color;
	float max_element=1;
	glm::vec3 max_color;
	for (int j = yStart; j <= yEnd; j++)
	{
		for (int i = x_Min; i <= x_Max; i++)
		{
			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
			{
				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
				{
					float z = Calculate_z(i, j, P1, P2, P3);
					if (z == get_Z(i, j))
					{
						I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(activelight).getPointambient());
						glm::vec3 Normal = Calculate_Phong_Normal(i, j, V1_Normal, V2_Normal, V3_Normal, P1, P2, P3);
						if(!scene.GetLight(activelight).getLightType())
						LightDirection = glm::normalize(glm::vec3(i,j,z) - glm::vec3(LightPosition));
						I_d = GetDiffuseReflection(Normal, LightDirection, scene,activelight);
						I_s = GetSpecularColor(LightDirection, Normal, scene, activelight);
						Color = I_a + I_d + I_s;
						//max_color.x = color_buffer_[INDEX(viewport_width_, i, j, 0)] + Color.x;
						//max_color.y = color_buffer_[INDEX(viewport_width_, i, j, 1)] + Color.y;
						//max_color.z = color_buffer_[INDEX(viewport_width_, i, j, 2)] + Color.z;
						//if (max_color.x > 1)
						//	max_element = std::max(max_color.x, max_element);
						//if (max_color.y > 1)
						//	max_element = std::max(max_color.y, max_element);
						//if (max_color.z > 1)
						//	max_element = std::max(max_color.z, max_element);
						//max_color = std::max(Color.x, std::max(Color.y, Color.z));
						//if(max_color>1)
						//	max_element = std::max(max_element, max_color);
						color_buffer_[INDEX(viewport_width_, i, j, 0)] += Color.x/divisionfactor;
						color_buffer_[INDEX(viewport_width_, i, j, 1)] += Color.y/divisionfactor;
						color_buffer_[INDEX(viewport_width_, i, j, 2)] += Color.z/divisionfactor;
						//if (max_color = color_buffer_[INDEX(viewport_width_, i, j, 0)] + Color.x > 1)
						//	max_element = std::max(max_color, max_element);
						//if (max_color = color_buffer_[INDEX(viewport_width_, i, j, 1)] + Color.y > 1)
						//	max_element = std::max(max_color, max_element);
						//if (max_color = color_buffer_[INDEX(viewport_width_, i, j, 2)] + Color.z > 1)
						//	max_element = std::max(max_color, max_element);
						//PutPixel(i, j, Color);
					}
				}
			}
		}
	}
	
//	for (int j = yStart; j <= yEnd; j++)
//	{
//		for (int i = x_Min; i <= x_Max; i++)
//		{
//			if (i > 0 && i < viewport_width_ && j>0 && j < viewport_height_)
//			{
//				if (inTriangle(glm::vec2(i, j), P1, P2, P3))
//				{
//					float z = Calculate_z(i, j, P1, P2, P3);
//					if (z == get_Z(i, j))
//					{
//						I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(activelight).getPointambient());
//						glm::vec3 Normal = Calculate_Phong_Normal(i, j, V1_Normal, V2_Normal, V3_Normal, P1, P2, P3);
//	
//						//LightDirection = glm::normalize(glm::vec3(i, j, z) - glm::vec3(LightPosition));
//						I_d = GetDiffuseReflection(Normal, LightDirection, scene, activelight);
//						I_s = GetSpecularColor(LightDirection, Normal, scene, activelight);
//						Color = I_a + I_d + I_s;
//						//Color.x /= max_element;
//						//Color.y /= max_element;
//						//Color.z /= max_element;
//						
//
//						color_buffer_[INDEX(viewport_width_, i, j, 0)] = (color_buffer_[INDEX(viewport_width_, i, j, 0)] + Color.x);//max_element;
//						color_buffer_[INDEX(viewport_width_, i, j, 1)] = (color_buffer_[INDEX(viewport_width_, i, j, 1)] + Color.y);// max_element;
//						color_buffer_[INDEX(viewport_width_, i, j, 2)] = (color_buffer_[INDEX(viewport_width_, i, j, 2)] + Color.z);// max_element;
//						
//	
//						//PutPixel(i, j, Color);
//					}
//				}
//			}
//		}
//	}



}*/


/*
void Renderer::clipping_pixels() 
{
	float max_color = 1;
	float color;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			if (color=color_buffer_[INDEX(viewport_width_, i, j, 0)] > 1)
			{
				max_color =std::max(color,max_color);
			}
			if (color = color_buffer_[INDEX(viewport_width_, i, j, 1)] > 1)
			{
				max_color = std::max(color, max_color);
			}
			if (color=color_buffer_[INDEX(viewport_width_, i, j, 2)] > 1)
			{
				max_color = std::max(color, max_color);
			}

		}
	}
	if (max_color > 1)
	{
		for (int i = 0; i < viewport_width_; i++)
		{
			for (int j = 0; j < viewport_height_; j++)
			{
				color_buffer_[INDEX(viewport_width_, i, j, 0)] /= max_color;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] /= max_color;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] /= max_color;
			}
		}
	}
}
*/
/*
float Renderer::sign(const glm::ivec2& p, const glm::ivec3& p1, const glm::ivec3& p2)
{
	return (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
}*/
/*bool Renderer::inTriangle(const glm::vec2& p, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	float dX = p.x - p3.x;
	float dY = p.y - p3.y;
	float dX21 = p3.x - p2.x;
	float dY12 = p2.y - p3.y;
	float D = dY12 * (p1.x - p3.x) + dX21 * (p1.y - p3.y);
	float s = dY12 * dX + dX21 * dY;
	float t = (p3.y - p1.y) * dX + (p1.x - p3.x) * dY;
	if (D < 0) return s <= 0 && t <= 0 && s + t >= D;
	return s >= 0 && t >= 0 && s + t <= D;
	/*float max_x = std::max(std::max(p1.x, p2.x), p3.x);
	float min_x = std::min(std::min(p1.x, p2.x), p3.x);
	float max_y = std::max(std::max(p1.y, p2.y), p3.y);
	float min_y = std::min(std::min(p1.y, p2.y), p3.y);
	if ( p.x<min_x || p.x>max_x || p.y > max_y || p.y < min_y)
		return false;
	float s1, s2, s3;
	bool negative, positive;

	s1 = sign(p, p1, p2);
	s2 = sign(p, p2, p3);
	s3 = sign(p, p3, p1);

	negative = (s1 < 0) || (s2 < 0) || (s3 < 0);
	positive = (s1 > 0) || (s2 > 0) || (s3 > 0);

	return !(negative && positive);

}*/

/*
float Renderer::Calculate_z(int point_x, int  point_y, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
{
	//float Triangle_Area = area(p1, p2, p3);
	float A1 = area(glm::vec3(point_x, point_y, 1), p2, p3);
	float A2 = area(glm::vec3(point_x, point_y, 1), p1, p3);
	float A3 = area(glm::vec3(point_x, point_y, 1), p1, p2);
	float Triangle_Area = A1 + A2 + A3;

	return ((A1 / Triangle_Area * p1.z) + (A2 / Triangle_Area * p2.z) + (A3 / Triangle_Area * p3.z));




	
}

glm::vec3 Renderer::Calculate_Gouraud_Color(int i, int j, glm::vec3 V1_color, glm::vec3 V2_color, glm::vec3 V3_color, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//float Triangle_Area = area(p1, p2, p3);
	float A1 = area(glm::vec3(i, j, 1), p2, p3);
	float A2 = area(glm::vec3(i, j, 1), p1, p3);
	float A3 = area(glm::vec3(i, j, 1), p1, p2);
	float Triangle_Area = A1 + A2 + A3;

	float R = ((A1 / Triangle_Area * V1_color.x) + (A2 / Triangle_Area * V2_color.x) + (A3 / Triangle_Area * V3_color.x));
	float G = ((A1 / Triangle_Area * V1_color.y) + (A2 / Triangle_Area * V2_color.y) + (A3 / Triangle_Area * V3_color.y));
	float B = ((A1 / Triangle_Area * V1_color.z) + (A2 / Triangle_Area * V2_color.z) + (A3 / Triangle_Area * V3_color.z));
	return glm::vec3(R, G, B);





}
glm::vec3 Renderer::Calculate_Phong_Normal(int i, int j,  glm::vec3 V1_Normal,  glm::vec3 V2_Normal,  glm::vec3 V3_Normal, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//float Triangle_Area = area(p1, p2, p3);
	float A1 = area(glm::vec3(i, j, 1), p2, p3);
	float A2 = area(glm::vec3(i, j, 1), p1, p3);
	float A3 = area(glm::vec3(i, j, 1), p1, p2);
	float Triangle_Area = A1 + A2 + A3;

	float x = ((A1 / Triangle_Area * V1_Normal.x) + (A2 / Triangle_Area * V2_Normal.x) + (A3 / Triangle_Area * V3_Normal.x));
	float y = ((A1 / Triangle_Area * V1_Normal.y) + (A2 / Triangle_Area * V2_Normal.y) + (A3 / Triangle_Area * V3_Normal.y));
	float z = ((A1 / Triangle_Area * V1_Normal.z) + (A2 / Triangle_Area * V2_Normal.z) + (A3 / Triangle_Area * V3_Normal.z));
	return glm::vec3(x, y, z);





}
*/
/*
float Renderer::area(const glm::vec3& P1,const glm::vec3& P2,const glm::vec3& P3)
{
	//return abs(((P1.x * (P2.y - P3.y)) + (P2.x * (P3.y - P1.y)) + (P3.x * (P1.y - P2.y))) / 2);

	return abs((((float)P2.x - (float)P1.x) * ((float)P3.y - (float)P1.y) - ((float)P3.x - (float)P1.x) * ((float)P2.y - (float)P1.y)) / 2.0);
}
*/



/*void Renderer::FilterCreation()
	{
		// intialising standard deviation to 1.0 
		//double sigma = 1.0;
		//double r, s = 2.0 * sigma * sigma;
		//
		//// sum is for normalization 
		//double sum = 0.0;
		//
		//// generating 5x5 kernel 
		//for (int x = -1; x <= 1; x++) {
		//	for (int y = -1; y <= 1; y++) {
		//		r = sqrt(x * x + y * y);
		//		GKernel[x + 1][y + 1] = (exp(-(r * r) / s)) / (M_PI * s);
		//		sum += GKernel[x + 1][y + 1];
		//	}
		//}
		//GaussianMatrixSum = sum;
		//// normalising the Kernel 
		//for (int i = 0; i < 3; ++i)
		//	for (int j = 0; j < 3; ++j)
		//		GKernel[i][j] /= sum;
	for(int i=0;i<3;i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GKernel[i][j] = 0;
		}
	}
	GKernel[1][1] = 1;
	GaussianMatrixSum = 1;
	}*/

/*
void Renderer::createGaussian()
{

	float* currentwindow = new float[3 * 3]; //index 4 is the middle one
	float colorx, colory, colorz;
	float currentsum=0;
	for (int i = 1; i < viewport_width_-1; i++)
	{
		for (int j = 1; j < viewport_height_-1; j++)
		{
			
			colorx = color_buffer_[INDEX(viewport_width_, i-1, j-1, 0)]; //x
			currentwindow[0] = GKernel[0][0] * colorx;
			currentsum += currentwindow[0];
			colorx = color_buffer_[INDEX(viewport_width_, i-1, j, 0)]; //x
			currentwindow[1] = GKernel[0][1] * colorx;
			currentsum += currentwindow[1];
			colorx = color_buffer_[INDEX(viewport_width_, i-1, j+1, 0)]; //x
			currentwindow[2] = GKernel[0][2] * colorx;
			currentsum += currentwindow[2];
			colorx = color_buffer_[INDEX(viewport_width_, i, j-1, 0)]; //x
			currentwindow[3] = GKernel[1][0] * colorx;
			currentsum += currentwindow[3];
			colorx = color_buffer_[INDEX(viewport_width_, i, j, 0)]; //x
			currentwindow[4] = GKernel[1][1] * colorx;
			currentsum += currentwindow[4];
			colorx = color_buffer_[INDEX(viewport_width_, i, j+1, 0)]; //x
			currentwindow[5] = GKernel[1][2] * colorx;
			currentsum += currentwindow[5];
			colorx = color_buffer_[INDEX(viewport_width_, i+1, j-1, 0)]; //x
			currentwindow[6] = GKernel[2][0] * colorx;
			currentsum += currentwindow[6];
			colorx = color_buffer_[INDEX(viewport_width_, i+1, j, 0)]; //x
			currentwindow[7] = GKernel[2][1] * colorx;
			currentsum += currentwindow[7];
			colorx = color_buffer_[INDEX(viewport_width_, i+1, j+1, 0)]; //x
			currentwindow[8] = GKernel[2][2] * colorx;
			currentsum += currentwindow[8];
			
			color_buffer_[INDEX(viewport_width_, i, j, 0)] = currentsum / GaussianMatrixSum;
			currentsum = 0;
			colory = color_buffer_[INDEX(viewport_width_, i - 1, j - 1, 1)]; //y
			currentwindow[0] = GKernel[0][0] * colory;
			currentsum += currentwindow[0];
			colory = color_buffer_[INDEX(viewport_width_, i - 1, j, 1)]; //y
			currentwindow[1] = GKernel[0][1] * colory;
			currentsum += currentwindow[1];
			colory = color_buffer_[INDEX(viewport_width_, i - 1, j + 1, 1)]; //y
			currentwindow[2] = GKernel[0][2] * colory;
			currentsum += currentwindow[2];
			colory = color_buffer_[INDEX(viewport_width_, i, j - 1, 1)]; //y
			currentwindow[3] = GKernel[1][0] * colory;
			currentsum += currentwindow[3];
			colory = color_buffer_[INDEX(viewport_width_, i, j, 1)]; //y
			currentwindow[4] = GKernel[1][1] * colory;
			currentsum += currentwindow[4];
			colory = color_buffer_[INDEX(viewport_width_, i, j + 1, 1)]; //y
			currentwindow[5] = GKernel[1][2] * colory;
			currentsum += currentwindow[5];
			colory = color_buffer_[INDEX(viewport_width_, i + 1, j - 1, 1)]; //y
			currentwindow[6] = GKernel[2][0] * colory;
			currentsum += currentwindow[6];
			colory = color_buffer_[INDEX(viewport_width_, i + 1, j, 1)]; //y
			currentwindow[7] = GKernel[2][1] * colory;
			currentsum += currentwindow[7];
			colory = color_buffer_[INDEX(viewport_width_, i + 1, j + 1, 1)]; //y
			currentwindow[8] = GKernel[2][2] * colory;
			currentsum += currentwindow[8];

			color_buffer_[INDEX(viewport_width_, i, j, 1)] = currentsum / GaussianMatrixSum;
			currentsum = 0;


			colorz = color_buffer_[INDEX(viewport_width_, i - 1, j - 1, 2)]; //z
			currentwindow[0] = GKernel[0][0] * colorz;
			currentsum += currentwindow[0];
			colorz = color_buffer_[INDEX(viewport_width_, i - 1, j, 2)]; //z
			currentwindow[1] = GKernel[0][1] * colorz;
			currentsum += currentwindow[1];
			colorz = color_buffer_[INDEX(viewport_width_, i - 1, j + 1, 2)]; //z
			currentwindow[2] = GKernel[0][2] * colorz;
			currentsum += currentwindow[2];
			colorz = color_buffer_[INDEX(viewport_width_, i, j - 1, 2)]; //z
			currentwindow[3] = GKernel[1][0] * colorz;
			currentsum += currentwindow[3];
			colorz = color_buffer_[INDEX(viewport_width_, i, j, 2)]; //z
			currentwindow[4] = GKernel[1][1] * colorz;
			currentsum += currentwindow[4];
			colorz = color_buffer_[INDEX(viewport_width_, i, j + 1, 2)]; //z
			currentwindow[5] = GKernel[1][2] * colorz;
			currentsum += currentwindow[5];
			colorz = color_buffer_[INDEX(viewport_width_, i + 1, j - 1, 2)]; //z
			currentwindow[6] = GKernel[2][0] * colorz;
			currentsum += currentwindow[6];
			colorz = color_buffer_[INDEX(viewport_width_, i + 1, j, 2)]; //z
			currentwindow[7] = GKernel[2][1] * colorz;
			currentsum += currentwindow[7];
			colorz = color_buffer_[INDEX(viewport_width_, i + 1, j + 1, 2)]; //z
			currentwindow[8] = GKernel[2][2] * colorz;
			currentsum += currentwindow[8];

			color_buffer_[INDEX(viewport_width_, i, j, 2)] = currentsum / GaussianMatrixSum;
		}
	}

}

*/
//void Renderer::creatBloom()
//{
//}



/*
void Renderer::Put_z(int i, int j, float z)
{
	this->max_z = std::max(max_z, z);
	this->min_z = std::min(min_z, z);
	z_buffer[Z_INDEX(viewport_width_, i, j)] = z;
	//z_buffer[(i * viewport_height_) + j] = z;
}
*/



float Renderer::get_Z(int i, int j) const
{
	return z_buffer[Z_INDEX(viewport_width_, i, j)];
	//return z_buffer[(i * viewport_height_) + j];
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	this->BackgroundColor = color;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}
/*
const glm::vec3 Renderer::ZBufferGrayscale(float z)const
{

	float a = 1 / (max_z - min_z);
	float b = -1*a * min_z;
	float color_ = (a * (z) + b);
	glm::vec3 color = glm::vec3((1-color_), (1-color_ ), (1-color_ ));
	return color;

}*/

void Renderer::ClearZBuffer()
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{

			z_buffer[Z_INDEX(viewport_width_,i,j)] = FLT_MAX;
		}
	}
	this->max_z = FLT_MIN;
	this->min_z = FLT_MAX;
}


void Renderer::Render(Scene& scene)

{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	
	int cameraCount = scene.GetCameraCount();
	glm::vec3 lightsPosition[5];
	glm::vec3 lightsDirection[5];
	glm::vec3 Ambient[5];
	glm::vec3 Diffuse[5];
	glm::vec3 Specular[5];
	float lightsType[5];

	if (cameraCount > 0)
	{
		Camera& camera = scene.GetActiveCamera();
		colorShader.use();
		colorShader.setUniform("lightCount", scene.GetLightCount());
		MeshModelValues Values = scene.GetActiveModel().getMeshModelValues();
		glm::vec3 ModelCenter = Values.getModelCenter();
		if (scene.GetLightCount())
		{
			
			
			
			for (int lightindex = 0; lightindex < scene.GetLightCount(); lightindex++)
			{
				scene.GetLight(lightindex).setLightPosition(scene.GetLight(lightindex).getTransformationMatrix()*glm::vec4(ModelCenter,1));
				colorShader.use();
				colorShader.setUniform("islight", true);
				colorShader.setUniform("light", scene.GetLight(lightindex).getTransformationMatrix());
				lightsPosition[lightindex] = glm::vec3(scene.GetLight(lightindex).getLightPosition()/scene.GetLight(lightindex).getLightPosition().w);
				lightsDirection[lightindex] = scene.GetLight(lightindex).getLightDirection();
				lightsType[lightindex] = scene.GetLight(lightindex).getLightType();
				if(!scene.GetLight(lightindex).getLightType())//point
					Ambient[lightindex] = scene.GetLight(lightindex).getPointambient();
				else
					Ambient[lightindex] = scene.GetLight(lightindex).getParallelambient();
				if (!scene.GetLight(lightindex).getLightType())//point
					Diffuse[lightindex] = scene.GetLight(lightindex).getPointdiffuse();
				else
					Diffuse[lightindex] = scene.GetLight(lightindex).getParalleldiffuse();
				if (!scene.GetLight(lightindex).getLightType())//point
					Specular[lightindex] = scene.GetLight(lightindex).getPointspecular();
				else
					Specular[lightindex] = scene.GetLight(lightindex).getParallelspecular();
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				GLuint vao = scene.GetLight(lightindex).getVAO();
				glBindVertexArray(scene.GetLight(lightindex).getVAO());
				glDrawArrays(GL_POINTS, 0, 1);
				glEnable(GL_PROGRAM_POINT_SIZE);
				glBindVertexArray(0);

				

				
			}
			
			colorShader.setUniform("lightsPosition", lightsPosition, scene.GetLightCount());
			colorShader.setUniform("lightsType", lightsType, scene.GetLightCount());
			colorShader.setUniform("eye", camera.getEye());
			colorShader.setUniform("lightsDirection", lightsDirection, scene.GetLightCount());
			colorShader.setUniform("Ambient", Ambient, scene.GetLightCount());
			colorShader.setUniform("Diffuse", Diffuse, scene.GetLightCount());
			colorShader.setUniform("Specular", Specular, scene.GetLightCount());
			colorShader.setUniform("isFogExists", scene.getFog());
			colorShader.setUniform("isLinearFog", scene.getLinearfog());
			colorShader.setUniform("FogEnding", scene.getFogending());
			colorShader.setUniform("FogStart", scene.getFogbegining());
			colorShader.setUniform("FogDensity", scene.getFogdensity());
		}
		int modelCount = scene.GetModelCount();
		
		
		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			MeshModel& currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();
			colorShader.setUniform("islight", false);
			// Set the uniform variables
			colorShader.setUniform("model", currentModel.getTransformationMatrix());
			colorShader.setUniform("view", camera.getLookat() * camera.getCameraTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("material.Ambient", currentModel.getAmbient());
			colorShader.setUniform("material.Diffuse", currentModel.getDiffuse());
			colorShader.setUniform("material.Specular", currentModel.getSpecular());
			colorShader.setUniform("material.alpha", currentModel.getSpecularalpha());
			colorShader.setUniform("isOrtho", camera.getOrthographicflag());
			colorShader.setUniform("material.diffuse", 0);
			colorShader.setUniform("material.normalMap", 1);
			colorShader.setUniform("material.specular", 2);
			colorShader.setUniform("istextures", currentModel.getistextures());
			colorShader.setUniform("isplanar", currentModel.getPlanar());
			colorShader.setUniform("isspherical", currentModel.getSpherical());
			colorShader.setUniform("normalmapping", currentModel.getNormalMapping());
			
			// bind diffuse map
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, diffuseMap);
			// bind normal map
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, normalMap);
			// Set 'texture1' as the active texture at slot #0
			//currentModel.modeltexture.bind(0);
			currentModel.normalmapltexture.bind(1);
			//currentModel.modelspeculartexture.bind(2);
			
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, normalMap);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			GLuint vao = currentModel.GetVAO();
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);

			// Unset 'texture1' as the active texture at slot #0
			//currentModel.modeltexture.unbind(0);
			currentModel.normalmapltexture.unbind(1);
			//currentModel.modelspeculartexture.unbind(2);
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D,1);

			//colorShader.setUniform("color", glm::vec3(0, 0, 0));

			// Drag our model's faces (triangles) in line mode (wireframe)
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glBindVertexArray(currentModel.GetVAO());
			//glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			//glBindVertexArray(0);
		}
	}
	/*
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	ClearZBuffer();


	float thickness = 0.5;


		

	if (scene.GetCameraCount() > 0)
	{
	auto currModel_ = scene.GetActiveCamera();
	glm::mat4x4 ScaleMatrix = glm::scale(glm::vec3(half_width, half_height, 1));
	glm::mat4x4 TranslateMatrix;
	if (scene.GetActiveCamera().getOrthographicflag())
	{
		TranslateMatrix = glm::translate(glm::vec3(1, 1, 1));
	}
	else
		TranslateMatrix = glm::translate(glm::vec3(1, 1, 0));
	glm::mat4x4 ViewPortMatrix = ScaleMatrix * TranslateMatrix;

	if (scene.GetLightCount())
	{
		
		MeshModelValues Values = scene.GetActiveCamera().getMeshModelValues();
		glm::vec3 ModelCenter = Values.getModelCenter();
		glm::vec3 LightPoisition (ModelCenter.x, ModelCenter.y, ModelCenter.z);

		
		
		
		for (int light = 0; light < scene.GetLightCount(); light++)
		{
			glm::mat4x4 TransMat = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().getLookat() * scene.GetActiveCamera().getCameraTransformation()* scene.GetLight(light).getTransformationMatrix();
			glm::vec4 V = TransMat * glm::vec4(LightPoisition, 1);
			
			if (!scene.GetActiveCamera().getOrthographicflag())
			{
			
				V.x /= V.w;
				V.y /= V.w;
				V.z /= V.w;
				V.w = 1;
			
			
			
			}



			V = ViewPortMatrix * V;
			scene.GetLight(light).setLightPosition(glm::vec3(V.x, V.y, V.z));



			std::vector<glm::vec3> vertexPositions;
			std::vector<glm::vec3> vertexPositions2;
			vertexPositions.push_back(glm::vec3(V.x, V.y + 15, -V.z));
			vertexPositions.push_back(glm::vec3(V.x - 15, V.y, -V.z));
			vertexPositions.push_back(glm::vec3(V.x + 15, V.y, -V.z));
			
			vertexPositions2.push_back(glm::vec3(V.x - 15, V.y,-V.z));
			vertexPositions2.push_back(glm::vec3(V.x + 15, V.y,-V.z));
			vertexPositions2.push_back(glm::vec3(V.x, V.y - 15,-V.z));
			
			fillZbuffer(vertexPositions);
			fillZbuffer(vertexPositions2);
			ScanConversion(vertexPositions, false, White);
			ScanConversion(vertexPositions2, false, White);
		}
	}






	
		//Draw Model 

		for (int FaceNum = 0; FaceNum < scene.GetActiveCamera().GetFacesCount(); FaceNum++)
		{
		
			bool isOrtho = scene.GetActiveCamera().getOrthographicflag();
			//Face currface_ = scene.GetActiveCamera().GetFace(FaceNum);
			int Vertex1 = scene.GetActiveCamera().GetFace(FaceNum).GetVertexIndex(0);
			glm::vec3 P1 = scene.GetActiveCamera().getVertex(Vertex1);
			int Vertex2 = scene.GetActiveCamera().GetFace(FaceNum).GetVertexIndex(1);
			glm::vec3 P2 = scene.GetActiveCamera().getVertex(Vertex2);
			int Vertex3 = scene.GetActiveCamera().GetFace(FaceNum).GetVertexIndex(2);
			glm::vec3 P3 = scene.GetActiveCamera().getVertex(Vertex3);
			int V_Normal_Index1 = scene.GetActiveCamera().GetFace(FaceNum).GetNormalIndex(0);
			int V_Normal_Index2 = scene.GetActiveCamera().GetFace(FaceNum).GetNormalIndex(1);
			int V_Normal_Index3 = scene.GetActiveCamera().GetFace(FaceNum).GetNormalIndex(2);
			glm::mat4 Rotate_Mat = scene.GetActiveCamera().getWorldRotationMatrix() * scene.GetActiveCamera().getLocalRotationMatrix();
			glm::vec4 Temp_Vertex_Normal = glm::vec4(scene.GetActiveCamera().getVertexNormal(V_Normal_Index1), 1);
			glm::vec4 Vertex_Normal_1 = Rotate_Mat * Temp_Vertex_Normal;
			Temp_Vertex_Normal = glm::vec4(scene.GetActiveCamera().getVertexNormal(V_Normal_Index2), 1);
			glm::vec4 Vertex_Normal_2 = Rotate_Mat * Temp_Vertex_Normal;
			Temp_Vertex_Normal = glm::vec4(scene.GetActiveCamera().getVertexNormal(V_Normal_Index3), 1);
			glm::vec4 Vertex_Normal_3 =  Rotate_Mat * Temp_Vertex_Normal;
			glm::vec3 CurrFaceNormal = normalize(cross(glm::vec3(P1 - P2), glm::vec3(P1 - P3)));
			//glm::vec4 P_Vector = glm::vec4(P1, 1);
			glm::mat4x4 TransMat = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().getLookat() * scene.GetActiveCamera().getCameraTransformation() * scene.GetActiveCamera().getTransformationMatrix();
			//glm::vec4 Transformed_Vec = TransMat * P_Vector;
			glm::vec4 V1 = TransMat*glm::vec4(P1, 1);
			glm::vec4 V2 = TransMat*glm::vec4(P2, 1);
			glm::vec4 V3 = TransMat*glm::vec4(P3, 1);
			


			if (!isOrtho)
			{
				
				V1.x /= V1.w;
				V2.x /= V2.w;
				V3.x /= V3.w;
				V1.y /= V1.w;
				V2.y /= V2.w;
				V3.y /= V3.w;
				V1.z /= V1.w;
				V2.z /= V2.w;
				V3.z /= V3.w;
				V1.w = 1;
				V2.w = 1;
				V3.w = 1;
	
								

			}
		
			V1 = ViewPortMatrix * V1;
			V2 = ViewPortMatrix * V2;
			V3 = ViewPortMatrix * V3;
			glm::vec3 FaceCenter = (V1 + V2 + V3) / 3.f;


			std::vector <glm::vec3> Normals;
			Normals.push_back(glm::vec3(Vertex_Normal_1.x,Vertex_Normal_1.y, Vertex_Normal_1.z));
			Normals.push_back(glm::vec3(Vertex_Normal_2.x, Vertex_Normal_2.y, Vertex_Normal_2.z));
			Normals.push_back(glm::vec3(Vertex_Normal_3.x, Vertex_Normal_3.y, Vertex_Normal_3.z));
			
			std::vector <glm::vec3> vertexPositions;
			vertexPositions.push_back(glm::vec3(V1.x, V1.y, V1.z));
			vertexPositions.push_back(glm::vec3(V2.x, V2.y, V2.z));
			vertexPositions.push_back(glm::vec3(V3.x, V3.y, V3.z));
			

			glm::vec3 VN_1 = glm::vec3(Vertex_Normal_1.x, Vertex_Normal_1.y, Vertex_Normal_1.z);
			glm::vec3 VN_2 = glm::vec3(Vertex_Normal_2.x, Vertex_Normal_2.y, Vertex_Normal_2.z);
			glm::vec3 VN_3 = glm::vec3(Vertex_Normal_3.x, Vertex_Normal_3.y, Vertex_Normal_3.z);
			
			fillZbuffer(vertexPositions);
			
			ScanConversion(vertexPositions, scene.GetActiveCamera().getGrayscaleflag(),Black);
			if(scene.GetLightCount())
			{
				glm::vec3 I_a=Black;
				glm::vec3 I_d=Black;
				glm::vec3 I_s=Black;
				glm::vec3 V1_Color;
				glm::vec3 V2_Color;
				glm::vec3 V3_Color;
				scene.GetActiveCamera().setGrayscaleflag(false);
		
				for (int x = 0; x < scene.GetLightCount(); x++)
				{

				if (!scene.GetLight(x).getLightType())
				{

					
						glm::vec3 LightPosition = scene.GetLight(x).getLightPosition();
						glm::vec3 LightDirection = glm::normalize(glm::vec3(FaceCenter) - glm::vec3(LightPosition));
						scene.GetLight(x).setLightDirection(LightDirection);
						glm::vec4 FaceNormal = Rotate_Mat * glm::vec4(CurrFaceNormal, 1);
						
						if (scene.GetLight(x).getShading() == 0)//Flat
						{	
							I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(x).getPointambient());
							I_d = GetDiffuseReflection(FaceNormal, LightDirection, scene,x);
							I_s = GetSpecularColor(LightDirection, FaceNormal, scene, x);
							ScanConversionFlat(vertexPositions, scene.GetActiveCamera().getGrayscaleflag(), I_a + I_d + I_s,scene);
						}
						else if (scene.GetLight(x).getShading() == 1)//Gouraud
						{

							LightDirection = glm::normalize(glm::vec3(V1.x, V1.y, V1.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
							I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(x).getPointambient());
							I_d = GetDiffuseReflection(Vertex_Normal_1, LightDirection, scene, x);
							I_s = GetSpecularColor(LightDirection, Vertex_Normal_1, scene, x);
							V1_Color = I_a + I_d + I_s;
							//V1_Color.x = V1_Color.x > 1.f ? 1.f : V1_Color.x;
							//V1_Color.y = V1_Color.y > 1.f ? 1.f : V1_Color.y;
							//V1_Color.z = V1_Color.z > 1.f ? 1.f : V1_Color.z;
							LightDirection = glm::normalize(glm::vec3(V2.x, V2.y, V2.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
							I_d = GetDiffuseReflection(Vertex_Normal_2, LightDirection, scene, x);
							I_s = GetSpecularColor(LightDirection, Vertex_Normal_2, scene, x);
							V2_Color = I_a + I_d + I_s;
							//V2_Color.x = V2_Color.x > 1.f ? 1.f : V2_Color.x;
							//V2_Color.y = V2_Color.y > 1.f ? 1.f : V2_Color.y;
							//V2_Color.z = V2_Color.z > 1.f ? 1.f : V2_Color.z;
							LightDirection = glm::normalize(glm::vec3(V3.x, V3.y, V3.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
							I_d = GetDiffuseReflection(Vertex_Normal_3, LightDirection, scene, x);
							I_s = GetSpecularColor(LightDirection, Vertex_Normal_3, scene, x);
							V3_Color = I_a + I_d + I_s;
							//V3_Color.x = V3_Color.x > 1.f ? 1.f : V3_Color.x;
							//V3_Color.y = V3_Color.y > 1.f ? 1.f : V3_Color.y;
							//V3_Color.z = V3_Color.z > 1.f ? 1.f : V3_Color.z;
							ScanConversionGouraud(vertexPositions, V1_Color, V2_Color, V3_Color,scene);

						}
						else//Phong
						{


							ScanConversionPhong(vertexPositions, Vertex_Normal_1, Vertex_Normal_2, Vertex_Normal_3, scene,x);

						}
					}

					//glm::vec3 I_d = GetDiffuseReflection()
					//glm::vec3 I_s = GetSpecularReflection()
				else
				{

					//glm::vec3 LightPosition = scene.GetLight(x).getLightPosition();
					glm::vec3 LightDirection = glm::normalize(scene.GetLight(x).getLightDirection());
					//scene.GetLight(x).setLightDirection(scene.GetLight(x).getLightDirection());
					glm::vec4 FaceNormal = Rotate_Mat * glm::vec4(CurrFaceNormal, 1);

					if (scene.GetLight(x).getShading() == 0)//Flat
					{
						I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(x).getPointambient());
						I_d = GetDiffuseReflection(FaceNormal, LightDirection, scene,x);
						I_s = GetSpecularColor(LightDirection, FaceNormal, scene, x);
						ScanConversionFlat(vertexPositions, scene.GetActiveCamera().getGrayscaleflag(), I_a + I_d + I_s,scene);
					}
					else if (scene.GetLight(x).getShading() == 1)//Gouraud
					{

						//LightDirection = glm::normalize(glm::vec3(V1.x, V1.y, V1.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
						I_a = GetAmbientReflection(scene.GetActiveCamera().getAmbient(), scene.GetLight(x).getPointambient());
						I_d = GetDiffuseReflection(Vertex_Normal_1, LightDirection, scene, x);
						I_s = GetSpecularColor(LightDirection, Vertex_Normal_1, scene, x);
						V1_Color = I_a + I_d + I_s;
						//V1_Color.x = V1_Color.x > 1.f ? 1.f : V1_Color.x;
						//V1_Color.y = V1_Color.y > 1.f ? 1.f : V1_Color.y;
						//V1_Color.z = V1_Color.z > 1.f ? 1.f : V1_Color.z;
						//LightDirection = glm::normalize(glm::vec3(V2.x, V2.y, V2.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
						I_d = GetDiffuseReflection(Vertex_Normal_2, LightDirection, scene, x);
						I_s = GetSpecularColor(LightDirection, Vertex_Normal_2, scene, x);
						V2_Color = I_a + I_d + I_s;
						//V2_Color.x = V2_Color.x > 1.f ? 1.f : V2_Color.x;
						//V2_Color.y = V2_Color.y > 1.f ? 1.f : V2_Color.y;
						//V2_Color.z = V2_Color.z > 1.f ? 1.f : V2_Color.z;
						//LightDirection = glm::normalize(glm::vec3(V3.x, V3.y, V3.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
						I_d = GetDiffuseReflection(Vertex_Normal_3, LightDirection, scene, x);
						I_s = GetSpecularColor(LightDirection, Vertex_Normal_3, scene, x);
						V3_Color = I_a + I_d + I_s;
						//V3_Color.x = V3_Color.x > 1.f ? 1.f : V3_Color.x;
						//V3_Color.y = V3_Color.y > 1.f ? 1.f : V3_Color.y;
						//V3_Color.z = V3_Color.z > 1.f ? 1.f : V3_Color.z;
						ScanConversionGouraud(vertexPositions, V1_Color, V2_Color, V3_Color,scene);

					}
					else//Phong
					{


						ScanConversionPhong(vertexPositions, Vertex_Normal_1, Vertex_Normal_2, Vertex_Normal_3, scene, x);

					}


					//glm::vec3 I_d = GetDiffuseReflection()
					//glm::vec3 I_s = GetSpecularReflection()
				}
				}
				
			}
			//clipping_pixels();

			//Renderer::DrawTriangles(vertexPositions, Random);

			
			
			
			
			




		}
		if (currModel_.getFacesNormalsflag())
		{//Face Normals
			for (int FaceNum = 0; FaceNum < scene.GetActiveCamera().GetFacesCount(); FaceNum++)
			{


				bool isOrtho = scene.GetActiveCamera().getOrthographicflag();
				auto currface_ = scene.GetActiveCamera().GetFace(FaceNum);
				auto Vertex1 = currface_.GetVertexIndex(0);
				auto P1 = scene.GetActiveCamera().getVertex(Vertex1);
				auto Vertex2 = currface_.GetVertexIndex(1);
				auto P2 = scene.GetActiveCamera().getVertex(Vertex2);
				auto Vertex3 = currface_.GetVertexIndex(2);
				auto P3 = scene.GetActiveCamera().getVertex(Vertex3);
				
				glm::vec3 CurrFaceNormal = normalize(cross(glm::vec3(P1 - P2), glm::vec3(P1 - P3)));
				glm::vec4 P_Vector = glm::vec4(P1, 1);
				glm::mat4x4 TransMat = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().getLookat() * scene.GetActiveCamera().getCameraTransformation() * scene.GetActiveCamera().getTransformationMatrix();
				auto Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;
				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P1.x = Transformed_Vec.x;
				P1.y = Transformed_Vec.y;
				P1.z = Transformed_Vec.z;

				P_Vector = glm::vec4(P2, 1);
				Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;

				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P2.x = Transformed_Vec.x;
				P2.y = Transformed_Vec.y;
				P2.z = Transformed_Vec.z;
				P_Vector = glm::vec4(P3, 1);
				Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;

				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P3.x = Transformed_Vec.x;
				P3.y = Transformed_Vec.y;
				P3.z = Transformed_Vec.z;

				glm::vec3 Center = (P1 + P2 + P3) / 3.f;

				glm::vec3 ScaleVec = glm::vec3(30, 30, 30);
				glm::mat4 ScaleMatrix = glm::scale(ScaleVec);
				glm::vec4 FaceNormal = ScaleMatrix * glm::vec4(CurrFaceNormal, 1) + glm::vec4(Center, 0);
				DrawLine(glm::ivec2(Center.x, Center.y), glm::ivec2(FaceNormal.x / FaceNormal.w, FaceNormal.y / FaceNormal.w), glm::vec3(currModel_.getFacesNormalsColor()));




			}
		}

		if (currModel_.getVertexNormalsflag())
		{ //Vertex Normals
			for (int FaceNum = 0; FaceNum < scene.GetActiveCamera().GetFacesCount(); FaceNum++)
			{
				auto vertexnormalscolor = scene.GetActiveCamera().getVertexNormalsColor();
				bool isOrtho = scene.GetActiveCamera().getOrthographicflag();
				auto currface_ = scene.GetActiveCamera().GetFace(FaceNum);
				auto Vertex1 = currface_.GetVertexIndex(0);
				auto P1 = scene.GetActiveCamera().getVertex(Vertex1);
				auto Vertex2 = currface_.GetVertexIndex(1);
				auto P2 = scene.GetActiveCamera().getVertex(Vertex2);
				auto Vertex3 = currface_.GetVertexIndex(2);
				auto P3 = scene.GetActiveCamera().getVertex(Vertex3);
				glm::vec4 P_Vector = glm::vec4(P1, 1);
				glm::mat4x4 TransMat = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().getLookat() * scene.GetActiveCamera().getCameraTransformation() * scene.GetActiveCamera().getTransformationMatrix();
				auto Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;
				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P1.x = Transformed_Vec.x;
				P1.y = Transformed_Vec.y;
				P1.z = Transformed_Vec.z;

				P_Vector = glm::vec4(P2, 1);
				Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;
				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P2.x = Transformed_Vec.x;
				P2.y = Transformed_Vec.y;
				P2.z = Transformed_Vec.z;
				P_Vector = glm::vec4(P3, 1);
				Transformed_Vec = TransMat * P_Vector;

				if (!isOrtho)
				{
					Transformed_Vec /= Transformed_Vec.w;
				}
				Transformed_Vec = ViewPortMatrix * Transformed_Vec;
				P3.x = Transformed_Vec.x;
				P3.y = Transformed_Vec.y;
				P3.z = Transformed_Vec.z;
				auto V_Normal_Index1 = currface_.GetNormalIndex(0);
				auto V_Normal_Index2 = currface_.GetNormalIndex(1);
				auto V_Normal_Index3 = currface_.GetNormalIndex(2);
				glm::vec3 ScaleVec = glm::vec3(40, 40, 40);
				glm::mat4 ScaleMatrix = glm::scale(ScaleVec);
				glm::mat4 Rotate_Mat = currModel_.getWorldRotationMatrix() * currModel_.getLocalRotationMatrix();
				glm::vec4 Temp_Vertex_Normal = glm::vec4(currModel_.getVertexNormal(V_Normal_Index1), 1);
				auto Vertex_Normal_1 = ScaleMatrix * Rotate_Mat * Temp_Vertex_Normal;
				Temp_Vertex_Normal = glm::vec4(currModel_.getVertexNormal(V_Normal_Index2), 1);
				auto Vertex_Normal_2 = ScaleMatrix * Rotate_Mat * Temp_Vertex_Normal;
				Temp_Vertex_Normal = glm::vec4(currModel_.getVertexNormal(V_Normal_Index3), 1);
				auto Vertex_Normal_3 = ScaleMatrix * Rotate_Mat * Temp_Vertex_Normal;
				DrawLine(glm::ivec2(P1.x, P1.y), glm::ivec2((Vertex_Normal_1.x / Vertex_Normal_1.w) + P1.x, (Vertex_Normal_1.y / Vertex_Normal_1.w) + P1.y), vertexnormalscolor);
				DrawLine(glm::ivec2(P2.x, P2.y), glm::ivec2((Vertex_Normal_2.x / Vertex_Normal_2.w) + P2.x, (Vertex_Normal_2.y / Vertex_Normal_2.w) + P2.y), vertexnormalscolor);
				DrawLine(glm::ivec2(P3.x, P3.y), glm::ivec2((Vertex_Normal_3.x / Vertex_Normal_3.w) + P3.x, (Vertex_Normal_3.y / Vertex_Normal_3.w) + P3.y), vertexnormalscolor);

			}
		}
		if (currModel_.getBoundingBoxgflag())
		{ //bounding box
			bool isOrtho = scene.GetActiveCamera().getOrthographicflag();
			glm::vec3 boundingboxcolor = currModel_.getBoundingBoxColor();
			auto P = currModel_.getMeshModelValues();
			glm::mat4x4 TransMat = scene.GetActiveCamera().GetProjectionTransformation() * scene.GetActiveCamera().getLookat() * scene.GetActiveCamera().getCameraTransformation() * scene.GetActiveCamera().getTransformationMatrix();
			auto P1 = TransMat * P.getMaxX_MaxY_MaxZ();
			auto P2 = TransMat * P.getMaxX_MaxY_MinZ();
			auto P3 = TransMat * P.getMaxX_MinY_MaxZ();
			auto P4 = TransMat * P.getMaxX_MinY_MinZ();
			auto P5 = TransMat * P.getMinX_MaxY_MaxZ();
			auto P6 = TransMat * P.getMinX_MaxY_MinZ();
			auto P7 = TransMat * P.getMinX_MinY_MaxZ();
			auto P8 = TransMat * P.getMinX_MinY_MinZ();
			if (!isOrtho)
			{
				P1 /= P1.w;
				P2 /= P2.w;
				P3 /= P3.w;
				P4 /= P4.w;
				P5 /= P5.w;
				P6 /= P6.w;
				P7 /= P7.w;
				P8 /= P8.w;
			}
			P1 = ViewPortMatrix * P1;
			P2 = ViewPortMatrix * P2;
			P3 = ViewPortMatrix * P3;
			P4 = ViewPortMatrix * P4;
			P5 = ViewPortMatrix * P5;
			P6 = ViewPortMatrix * P6;
			P7 = ViewPortMatrix * P7;
			P8 = ViewPortMatrix * P8;



			DrawLine(glm::vec4(P5.x / P5.w, P5.y / P5.w, 0, 0), glm::vec4(P1.x / P1.w, P1.y / P1.w, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P5.x / P5.w, P5.y / P5.w, 0, 0), glm::vec4(P6.x, P6.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P6.x, P6.y, 0, 0), glm::vec4(P2.x, P2.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P2.x, P2.y, 0, 0), glm::vec4(P1.x, P1.y, 0, 0), boundingboxcolor);

			DrawLine(glm::vec4(P5.x / P5.w, P5.y / P5.w, 0, 0), glm::vec4(P7.x, P7.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P6.x, P6.y, 0, 0), glm::vec4(P8.x, P8.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P2.x, P2.y, 0, 0), glm::vec4(P4.x, P4.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P1.x, P1.y, 0, 0), glm::vec4(P3.x, P3.y, 0, 0), boundingboxcolor);


			DrawLine(glm::vec4(P7.x, P7.y, 0, 0), glm::vec4(P3.x, P3.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P7.x, P7.y, 0, 0), glm::vec4(P8.x, P8.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P8.x, P8.y, 0, 0), glm::vec4(P4.x, P4.y, 0, 0), boundingboxcolor);
			DrawLine(glm::vec4(P4.x, P4.y, 0, 0), glm::vec4(P3.x, P3.y, 0, 0), boundingboxcolor);

		}

		if (scene.getFog())
		{
			Fog(scene);
		}
		*/


}









int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}
//glm::vec3 Renderer::GetDiffuseReflection(glm::vec3 Normal, glm::vec3 I, const Scene& scene,int activelight)
//{
//	glm::vec3 MaterialColor = scene.GetActiveCamera().getDiffuse();
//	glm::vec3 LightColor=Black;
//	if (scene.GetLight(activelight).getLightType())
//		LightColor = scene.GetLight(activelight).getParalleldiffuse();
//	else
//		LightColor = scene.GetLight(activelight).getPointdiffuse();
//	glm::vec3 temp(MaterialColor.x * LightColor.x, MaterialColor.y * LightColor.y, MaterialColor.z * LightColor.z);
//	float i_n = glm::dot(/*-normalize*/-(Normal), I);
//	return temp * i_n;
//}

/*
glm::vec3 Renderer::GetAmbientReflection(glm::vec3 MaterialColor, glm::vec3 LightColor)
{
	glm::vec3 I_a (MaterialColor.x * LightColor.x, MaterialColor.y * LightColor.y, MaterialColor.z * LightColor.z);
	return I_a;
}
glm::vec3 Renderer::GetSpecularColor(glm::vec3 I, glm::vec3 Normal, const Scene& scene,int activelight)
{
	glm::vec3 temp_Normal;
	if (!scene.GetActiveCamera().getOrthographicflag())
		temp_Normal = Normal;

	else
		temp_Normal = -Normal;
	Normal = normalize(Normal);
	
	I = normalize(I);
	int alpha = scene.GetLight(activelight).getSpecularalpha();
	glm::vec3 MaterialColor = scene.GetActiveCamera().getSpecular();
	glm::vec3 LightColor;
	if (scene.GetLight(activelight).getLightType())
		LightColor = scene.GetLight(activelight).getParallelspecular();
	else
		LightColor = scene.GetLight(activelight).getPointspecular();
	glm::vec3 temp = glm::vec3(MaterialColor.x * LightColor.x, MaterialColor.y * LightColor.y, MaterialColor.z * LightColor.z);
	glm::vec3 r = (2.f * glm::dot(temp_Normal, I) * Normal - I);
	float Power = (std::pow(std::max(0.0f, glm::dot((r), normalize(scene.GetActiveCamera().getEye()))), alpha));
	glm::vec3 I_s(temp * Power);
	return I_s;
}
*/
void Renderer::Fog(Scene& scene)
{
	scene.setFogbegining(min_z*scene.getFogstartmultfactor());//check
	scene.setFogending(max_z*scene.getFogendmultfactor());
	
	glm::vec3 color;
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = get_Z(i,j);
			if (z != FLT_MAX)
			{
				color = glm::vec3(color_buffer_[INDEX(viewport_width_, i, j, 0)], color_buffer_[INDEX(viewport_width_, i, j, 1)], color_buffer_[INDEX(viewport_width_, i, j, 2)]);
				float vertexViewDistance = z;
				float fogFactor;
				if (scene.getLinearfog())
				{
					if (!scene.GetActiveCamera().getOrthographicflag())
						int x = 3;
					fogFactor = (scene.getFogending() - vertexViewDistance) / (scene.getFogending() - scene.getFogbegining());
				}
				else
				{
					fogFactor = std::exp(-(vertexViewDistance * vertexViewDistance * scene.getFogdensity() * scene.getFogdensity()));
					if (fogFactor < 0 || fogFactor > 1)
						fogFactor = 1;
				}
				color = ((1 - fogFactor) * glm::vec3(0.5, 0.5, 0.5) + fogFactor * color);
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
			}
		}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
}

void Renderer::LoadTextures(const std::string& path,const Scene& scene)
{
	//if (!scene.GetActiveModel().modeltexture.loadTexture(path, true))
	//{
	//	scene.GetActiveModel().modeltexture.loadTexture(path, true);
	//}
	if (!scene.GetActiveModel().modeltexture.loadTexture(path,true))
	{
		diffuseMap=scene.GetActiveModel().modeltexture.loadTexture(path, true);
		
	}
}

void Renderer::LoadNormalTextures(const std::string& path, const Scene& scene)
{
	if (!scene.GetActiveModel().normalmapltexture.loadTexture(path, true))
	{
		diffuseMap = scene.GetActiveModel().normalmapltexture.loadTexture(path, true);

	}

}

void Renderer::LoadSpecularTextures(const std::string& path, const Scene& scene)
{
	if (!scene.GetActiveModel().modelspeculartexture.loadTexture(path, true))
	{
		specularMap = scene.GetActiveModel().modelspeculartexture.loadTexture(path, true);

	}

}