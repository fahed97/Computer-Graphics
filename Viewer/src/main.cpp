#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
bool show_transformation_window = false;
bool colors = true;
bool show_bounding_box = false;
bool show_face_normals = false;
bool show_vertex_normals = false;
bool show_camera_transformations = false;
bool clear_model = false;
bool z_buffer_grayscale = false;
bool show_light = false;
bool gaussian = false;
bool bloom = false;
bool fog = false;
bool loadtexture = false;
glm::vec4 clear_color = glm::vec4(0.f, 0.f, 0.f, 1.00f);
glm::vec3 BoundingBoxColor = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 FaceNormalsColor = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 VertexNormalsColor = glm::vec3(0.f, 1.0f, 0.f);
float width_ = 0.f;
float height_ = 0.f;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1366, windowHeight = 768;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;
	
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	renderer.LoadShaders();
	Scene scene = Scene();
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene,renderer);
		RenderFrame(window, scene, renderer, io);
/*
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus();
		ImGui::Render();
		

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);*/
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetWindowAspectRatio(window, renderer.GetViewportWidth(), renderer.GetViewportHeight());
	width_ = renderer.GetViewportWidth();
	height_ = renderer.GetViewportHeight();


	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		Renderer renderer = Renderer(width_, height_);

		// TODO: Set new aspect ratio

	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//added

	//renderer.ClearColorBuffer(clear_color);
	//renderer.ClearZBuffer();
	renderer.Render(scene);
	//renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer)
{
	/**
	 * MeshViewer menu
	 */
	 //ImGui::Begin("MeshViewer Menu");
	std::string choosenModel = "";
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					//width_ = 1920.0f;
					//height_ =1080.0f;
					scene.AddModel(Utils::LoadMeshModel(outPath));
					scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
					//scene.GetActiveCamera().faceColor();
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		else if (ImGui::BeginMenu("Choose Model"))
		{
			if (ImGui::MenuItem("Banana"))
			{
				choosenModel = "banana.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Beethoven"))
			{
				choosenModel = "beethoven.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();

			}
			if (ImGui::MenuItem("Bishop"))
			{
				choosenModel = "bishop.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Blob"))
			{
				choosenModel = "blob.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Bunny"))
			{
				choosenModel = "bunny.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Camera"))
			{
				choosenModel = "camera.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Chain"))
			{
				choosenModel = "chain.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Cow"))
			{
				choosenModel = "cow.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Demo"))
			{
				choosenModel = "demo.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Dolphin"))
			{
				choosenModel = "dolphin.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Feline"))
			{
				choosenModel = "feline.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Pawn"))
			{
				choosenModel = "pawn.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if (scene.GetCameraCount() == 0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();
			}
			if (ImGui::MenuItem("Teapot"))
			{
				choosenModel = "teapot.obj";
				std::string outPath = "C:\\computergraphics2021-fahed-nasser\\Data\\" + choosenModel;
				scene.AddModel(Utils::LoadMeshModel(outPath));
				if(scene.GetCameraCount()==0)
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetModelName(), width_, height_));
				//scene.GetActiveCamera().faceColor();

			}



			//nfdchar_t* outPath = NULL;

			//free(outPath);
			/*if (result == NFD_OKAY)
			{

				scene.AddModel(Utils::LoadMeshModel(outPath));
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel(), width_, height_));
				free(outPath);
			}
			else if (result == NFD_CANCEL)
			{
			}
			else
			{
			}*/


			ImGui::EndMenu();
		}

		else if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Demo Window"))
			{
				if (show_demo_window)
				{
					show_demo_window = false;
				}
				else
					show_demo_window = true;

			}
			ImGui::Text("Fahed Nasser - 2020/2021");
			ImGui::EndMenu();

		}




		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}


	// Controls
	//ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed

	//ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	if (colors)
	{
		ImGui::Begin("Controls", &colors, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::ColorEdit4("Background Color", (float*)&clear_color); // Edit 4 floats representing a color
		//if (ImGui::Button("Gaussian Blur"))
		//{
		//	if (gaussian)
		//	{
		//		scene.GetActiveCamera().setGaussian(false);
		//		gaussian = false;
		//	}
		//	else {
		//		gaussian = true;
		//		scene.GetActiveCamera().setGaussian(true);
		//		
		//	}
		//
		//}
		//if (ImGui::Button("Bloom"))
		//{
		//	if (bloom)
		//	{
		//		scene.GetActiveCamera().setBloom(false);
		//		bloom = false;
		//	}
		//	else {
		//
		//		bloom = true;
		//		scene.GetActiveCamera().setBloom(true);
		//
		//	}
		//}
		
		/*if (ImGui::Button("Z Buffer Grayscale"))
		{
			if (z_buffer_grayscale)
			{
				scene.GetActiveCamera().setGrayscaleflag(false);
				z_buffer_grayscale = false;
			}
			else {
				z_buffer_grayscale = true;
				scene.GetActiveCamera().setGrayscaleflag(true);
			}

		}*/
		//ImGui::SameLine();
		if (scene.GetModelCount())
		{
			if (ImGui::Button("Generate Random Colors"))
			{
				float randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				glm::vec3 color(randomNumber_x, randomNumber_y, randomNumber_z);
				scene.GetActiveModel().setAmbient(color);
				randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
				scene.GetActiveModel().setDiffuse(color);
				randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				color = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
				scene.GetActiveModel().setSpecular(color);

				//if (scene.GetCameraCount())
				//{
				//	scene.GetActiveCamera().faceColor();
				//}
			}
		}
		if (scene.GetModelCount() > 0)
		{
			glm::vec3 diffuse = scene.GetActiveModel().getDiffuse();
			glm::vec3 ambient = scene.GetActiveModel().getAmbient();
			glm::vec3 specular = scene.GetActiveModel().getSpecular();
			ImGui::ColorEdit3("Ambient", (float*)&ambient); // Edit 3 floats representing a color
			ImGui::ColorEdit3("Diffuse", (float*)&diffuse); // Edit 3 floats representing a color
			ImGui::ColorEdit3("Specular", (float*)&specular); // Edit 3 floats representing a color
			int specular_alpha = scene.GetActiveModel().getSpecularalpha();
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0.f, 255.f, 125.f));
			if (ImGui::SliderInt("Specular Alpha", &specular_alpha, 1, 10))
				scene.GetActiveModel().setSpecularalpha(specular_alpha);
			ImGui::PopStyleColor(1);
			scene.GetActiveModel().setAmbient(ambient);
			scene.GetActiveModel().setDiffuse(diffuse);
			scene.GetActiveModel().setSpecular(specular);
		}
		if (scene.GetModelCount())
		{
			if (ImGui::Button("Load Texture"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("jpg,png;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					renderer.LoadTextures(outPath,scene);
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}


			}
			if (ImGui::Button("Texture Coordinates"))
			{
				if (scene.GetActiveModel().getistexturescoords())
				{
					scene.GetActiveModel().setistextures(true);
					scene.GetActiveModel().setPlanar(false);
					scene.GetActiveModel().setSpherical(false);
				}
				else
				{

				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Planar Projection"))
			{
				scene.GetActiveModel().setistextures(false);
				scene.GetActiveModel().setPlanar(true);
				scene.GetActiveModel().setSpherical(false);
			}
			ImGui::SameLine();
			if (ImGui::Button("Spherical Projection"))
			{
				scene.GetActiveModel().setistextures(false);
				scene.GetActiveModel().setPlanar(false);
				scene.GetActiveModel().setSpherical(true);
			}
			if (ImGui::Button("Normal Mapping"))
			{
				if (scene.GetActiveModel().getNormalMapping())
				{
					scene.GetActiveModel().setNormalMapping(false);
				}
				else {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("jpg,png;", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						renderer.LoadNormalTextures(outPath, scene);
						free(outPath);
					}
					else if (result == NFD_CANCEL)
					{
					}
					else
					{
					}
					scene.GetActiveModel().setNormalMapping(true);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Specular Map"))
			{
				if (scene.GetActiveModel().getSpecularMapping())
				{
					scene.GetActiveModel().setSpecularMapping(false);
				}
				else {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("jpg,png;", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						renderer.LoadSpecularTextures(outPath, scene);
						free(outPath);
					}
					else if (result == NFD_CANCEL)
					{
					}
					else
					{
					}
					scene.GetActiveModel().setSpecularMapping(true);
				}
			}
		}

		//if (ImGui::Button("Fog"))
		//{
		//	if (fog)
		//	{
		//		scene.setFog(false);
		//		fog = false;
		//	}
		//	else
		//	{
		//		fog = true;
		//		scene.setFog(true);
		//		
		//	}
		//}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


		ImGui::End();
	}
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		int i;
		static float f = 0.0f;
		static int counter = 0;
		std::string windowname = "Model";


		bool ModelWindow = true;
		ImGui::Begin(windowname.c_str(), &ModelWindow, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::Button("Object Transformations"))
			show_transformation_window = true;
		ImGui::SameLine();
		i = 10;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 255.f, 255.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Camera"))
			show_camera_transformations = true;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		i = 20;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(25.f, 25.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));


		if (ImGui::Button("Lights"))
			show_light = true;

		ImGui::PopStyleColor(3);



		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			//counter++;

		//ImGui::Text("counter = %d", counter);
		i = 5;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(255.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Clear Model and Camera") && scene.GetCameraCount())
		{
			scene.PopActiveCamera();
			scene.clearModels();
			scene.clearLights();
			show_transformation_window = false;
			show_camera_transformations = false;
			show_bounding_box = false;
			show_face_normals = false;
			show_vertex_normals = false;
			z_buffer_grayscale = false;




		}
		ImGui::PopStyleColor(3);



		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	//Transformations Window
	if (show_transformation_window && scene.GetCameraCount() > 0)
	{

		//int activemodel = scene.GetActiveModelIndex() + 1;
		//int numberofmodels = scene.GetModelCount();
		//ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(20.f, 100.f, 255.f));
		//ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0.f, 0.f, 255.f));
		//
		//if (ImGui::SliderInt("Model Number", &activemodel, 1, numberofmodels))
		//	scene.SetActiveModelIndex(activemodel - 1);
		//
		//ImGui::PopStyleColor(2);

			
		
		static float Xtranslate = 0.f;
		static float Ytranslate = 0.f;
		static float Ztranslate = 0.f;
		static float XScale = 1.f;
		static float YScale = 1.f;
		static float ZScale = 1.f;
		static int xAngle = 0;
		static int yAngle = 0;
		static int zAngle = 0;
		static float Xtranslate_Local = 0.f;
		static float Ytranslate_Local = 0.f;
		static float Ztranslate_Local = 0.f;
		static float XScale_Local = 1.f;
		static float YScale_Local = 1.f;
		static float ZScale_Local = 1.f;
		static int xAngle_Local = 0;
		static int yAngle_Local = 0;
		static int zAngle_Local = 0;
		glm::vec3 Current_Rotate_Values(xAngle, yAngle, zAngle);
		glm::vec3 Current_Local_Rotate_Values(xAngle_Local, yAngle_Local, zAngle_Local);
		static int Transformation = 0;
		glm::mat4x4 TranslateMatrix = glm::mat4(1.0f);
		glm::mat4x4 TranslateMatrix_Local = glm::mat4(1.0f);
		glm::mat4x4 ScaleMatrix = glm::mat4(1.0f);
		glm::mat4x4 ScaleMatrix_Local = glm::mat4(1.0f);
		glm::mat4x4 RotateMatrix = glm::mat4(1.0f);
		glm::mat4x4 RotateMatrix_Local = glm::mat4(1.0f);
		const char* Transformations[] = { "Translate", "Scale", "Rotate" };
		ImGui::Begin("Transformation Window", &show_transformation_window, ImGuiWindowFlags_AlwaysAutoResize);
		int activemodel = scene.GetActiveModelIndex() + 1;

		ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(20.f, 100.f, 255.f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0.f, 0.f, 255.f));

		if (ImGui::SliderInt("Model Number", &activemodel, 1, scene.GetModelCount()))
			scene.SetActiveModelIndex(activemodel - 1);

		ImGui::PopStyleColor(2);
		const char* name = scene.GetActiveModel().GetModelName().c_str();
		ImGui::Text("Active Model: ");
		ImGui::SameLine();
		ImGui::Text(name);
		ImGui::Text("Welcome to Object Transformation Window!\nPlease Select a Frame:");

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::BeginTabBar("MyTabBar", tab_bar_flags);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::BeginTabItem("World Transformation"))
		{
			ImGui::ListBox("Select Transformation", &Transformation, Transformations, IM_ARRAYSIZE(Transformations), 3);

			switch (Transformation)
			{
			case 0:
				ImGui::SliderFloat("Translate Factor X", &Xtranslate, -1.f, 1.f);
				ImGui::SliderFloat("Translate Factor Y", &Ytranslate, -1.f, 1.f);
				ImGui::SliderFloat("Translate Factor Z", &Ztranslate, -1.f, 1.f);
				glm::vec3 TranslateVec = glm::vec3(Xtranslate, Ytranslate, Ztranslate);
				TranslateMatrix = glm::translate(glm::mat4(1.0f), TranslateVec);
				scene.GetActiveModel().setTransformationMatrix(TranslateMatrix, 0);
				break;

			case 1:
				ImGui::SliderFloat("Scale Factor X", &XScale, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Y", &YScale, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Z", &ZScale, 0.f, 3.f);
				glm::vec3 ScaleVec = glm::vec3(XScale, YScale, ZScale);
				ScaleMatrix = glm::scale(ScaleVec);
				scene.GetActiveModel().setTransformationMatrix(ScaleMatrix, 1);
				break;

			case 2:

				ImGui::SliderInt("Rotation Angle X", &xAngle, -360, 360);
				ImGui::SliderInt("Rotation Angle Y", &yAngle, -360, 360);
				ImGui::SliderInt("Rotation Angle Z", &zAngle, -360, 360);


				if (xAngle != Current_Rotate_Values.x)
				{
					Current_Rotate_Values.x = xAngle;
					glm::vec3 myRotationAxis = glm::vec3(1, 0, 0);
					RotateMatrix = glm::rotate(glm::radians(float(xAngle)), myRotationAxis);
					scene.GetActiveModel().setTransformationMatrix(RotateMatrix, 2);
				}


				else if (yAngle != Current_Rotate_Values.y)
				{
					Current_Rotate_Values.y = yAngle;

					glm::vec3 myRotationAxis = glm::vec3(0, 1, 0);
					RotateMatrix = glm::rotate(glm::radians(float(yAngle)), myRotationAxis);
					scene.GetActiveModel().setTransformationMatrix(RotateMatrix, 2);

				}


				else if (zAngle != Current_Rotate_Values.z)
				{
					Current_Rotate_Values.z = zAngle;

					glm::vec3 myRotationAxis = glm::vec3(0, 0, 1);
					RotateMatrix = glm::rotate(glm::radians(float(zAngle)), myRotationAxis);
					scene.GetActiveModel().setTransformationMatrix(RotateMatrix, 2);


				}


				//scene.GetActiveModel().setTransformationMatrix(RotateMatrix, 2);

				break;



			}


			if (ImGui::Button("Reset Parameters"))
			{
				Xtranslate = 0;
				Ytranslate = 0;
				Ztranslate = 0;
				TranslateMatrix = glm::mat4(1.0f);
				XScale = 1;
				YScale = 1;
				ZScale = 1;
				ScaleMatrix = glm::mat4(1.0f);
				xAngle = 0;
				yAngle = 0;
				zAngle = 0;
				RotateMatrix = glm::mat4(1.0f);

				scene.GetActiveModel().ResetTransformationMatrix();

			}



			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Local Transformation"))
		{
			ImGui::ListBox("Select Transformation", &Transformation, Transformations, IM_ARRAYSIZE(Transformations), 3);

			switch (Transformation)
			{
			case 0:
				ImGui::SliderFloat("Translate Factor X", &Xtranslate_Local, -1.f, 1.f);
				ImGui::SliderFloat("Translate Factor Y", &Ytranslate_Local, -1.f, 1.f);
				ImGui::SliderFloat("Translate Factor Z", &Ztranslate_Local, -1.f, 1.f);
				glm::vec3 TranslateVec_Local = glm::vec3(Xtranslate_Local, Ytranslate_Local, Ztranslate_Local);
				TranslateMatrix_Local = glm::translate(glm::mat4(1.0f), TranslateVec_Local);
				scene.GetActiveModel().setLocalTransformationMatrix(TranslateMatrix_Local, 0);


				break;

			case 1:
				ImGui::SliderFloat("Scale Factor X", &XScale_Local, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Y", &YScale_Local, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Z", &ZScale_Local, 0.f, 3.f);
				glm::vec3 ScaleVec_Local = glm::vec3(XScale_Local, YScale_Local, ZScale_Local);
				ScaleMatrix_Local = glm::scale(ScaleVec_Local);
				scene.GetActiveModel().setLocalTransformationMatrix(ScaleMatrix_Local, 1);

				break;

			case 2:

				ImGui::SliderInt("Rotation Angle X", &xAngle_Local, -360, 360);
				ImGui::SliderInt("Rotation Angle Y", &yAngle_Local, -360, 360);
				ImGui::SliderInt("Rotation Angle Z", &zAngle_Local, -360, 360);


				if (xAngle_Local != Current_Local_Rotate_Values.x)
				{
					Current_Local_Rotate_Values.x = xAngle_Local;
					glm::vec3 myRotationAxis_Local = glm::vec3(1, 0, 0);
					RotateMatrix_Local = glm::rotate(glm::radians(float(xAngle_Local)), myRotationAxis_Local);
					scene.GetActiveModel().setLocalTransformationMatrix(RotateMatrix_Local, 2);
				}


				else if (yAngle_Local != Current_Local_Rotate_Values.y)
				{
					Current_Local_Rotate_Values.y = yAngle_Local;

					glm::vec3 myRotationAxis_Local = glm::vec3(0, 1, 0);
					RotateMatrix_Local = glm::rotate(glm::radians(float(yAngle_Local)), myRotationAxis_Local);
					scene.GetActiveModel().setLocalTransformationMatrix(RotateMatrix_Local, 2);

				}


				else if (zAngle_Local != Current_Local_Rotate_Values.z)
				{
					Current_Local_Rotate_Values.z = zAngle_Local;

					glm::vec3 myRotationAxis_Local = glm::vec3(0, 0, 1);
					RotateMatrix_Local = glm::rotate(glm::radians(float(zAngle_Local)), myRotationAxis_Local);
					scene.GetActiveModel().setLocalTransformationMatrix(RotateMatrix_Local, 2);

				}


				//	scene.GetActiveModel().setLocalTransformationMatrix(RotateMatrix_Local, 2);

				break;


			}



			if (ImGui::Button("Reset Local Parameters"))
			{
				Xtranslate_Local = 0;
				Ytranslate_Local = 0;
				Ztranslate_Local = 0;
				TranslateMatrix_Local = glm::mat4(1.0f);
				XScale_Local = 1;
				YScale_Local = 1;
				ZScale_Local = 1;
				ScaleMatrix_Local = glm::mat4(1.0f);
				xAngle_Local = 0;
				yAngle_Local = 0;
				zAngle_Local = 0;
				RotateMatrix_Local = glm::mat4(1.0f);
				scene.GetActiveModel().ResetLocalTransformationMatrix();

			}

			ImGui::EndTabItem();
		}
		scene.GetActiveModel().setTransformation();
		ImGui::EndTabBar();

		ImGui::AlignTextToFramePadding();

		//if (ImGui::Button("Show Bounding Box"))
		//{
		//	if (show_bounding_box)
		//	{
		//		scene.GetActiveModel().BoundingBox(false, BoundingBoxColor);
		//		show_bounding_box = false;
		//	}
		//	else
		//		show_bounding_box = true;
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("Show Face Normals"))
		//{
		//	if (show_face_normals)
		//	{
		//		scene.GetActiveModel().FaceNormals(false, FaceNormalsColor);
		//		show_face_normals = false;
		//	}
		//	else
		//		show_face_normals = true;
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("Show Vertex Normals"))
		//{
		//	if (show_vertex_normals)
		//	{
		//		scene.GetActiveModel().VertexNormals(false, VertexNormalsColor);
		//		show_vertex_normals = false;
		//	}
		//	else
		//		show_vertex_normals = true;
		//}
		//
		if (ImGui::Button("Close Me"))
		{
			show_bounding_box = false;
			show_transformation_window = false;
			show_face_normals = false;
			show_vertex_normals = false;


		}
		ImGui::SameLine();
		ImGui::End();



	}
	//if (show_transformation_window && scene.GetCameraCount() > 0)
	//{

	//	//int activemodel = scene.GetActiveModelIndex() + 1;
	//	//int numberofmodels = scene.GetModelCount();
	//	//ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(20.f, 100.f, 255.f));
	//	//ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0.f, 0.f, 255.f));
	//	//
	//	//if (ImGui::SliderInt("Model Number", &activemodel, 1, numberofmodels))
	//	//	scene.SetActiveModelIndex(activemodel - 1);
	//	//
	//	//ImGui::PopStyleColor(2);
	//	static float Xtranslate = 0.f;
	//	static float Ytranslate = 0.f;
	//	static float Ztranslate = 0.f;
	//	static float XScale = 1.f;
	//	static float YScale = 1.f;
	//	static float ZScale = 1.f;
	//	static int xAngle = 0;
	//	static int yAngle = 0;
	//	static int zAngle = 0;
	//	static float Xtranslate_Local = 0.f;
	//	static float Ytranslate_Local = 0.f;
	//	static float Ztranslate_Local = 0.f;
	//	static float XScale_Local = 1.f;
	//	static float YScale_Local = 1.f;
	//	static float ZScale_Local = 1.f;
	//	static int xAngle_Local = 0;
	//	static int yAngle_Local = 0;
	//	static int zAngle_Local = 0;
	//	glm::vec3 Current_Rotate_Values(xAngle, yAngle, zAngle);
	//	glm::vec3 Current_Local_Rotate_Values(xAngle_Local, yAngle_Local, zAngle_Local);
	//	static int Transformation = 0;
	//	glm::mat4x4 TranslateMatrix = glm::mat4(1.0f);
	//	glm::mat4x4 TranslateMatrix_Local = glm::mat4(1.0f);
	//	glm::mat4x4 ScaleMatrix = glm::mat4(1.0f);
	//	glm::mat4x4 ScaleMatrix_Local = glm::mat4(1.0f);
	//	glm::mat4x4 RotateMatrix = glm::mat4(1.0f);
	//	glm::mat4x4 RotateMatrix_Local = glm::mat4(1.0f);
	//	const char* Transformations[] = { "Translate", "Scale", "Rotate" };
	//	ImGui::Begin("Transformation Window", &show_transformation_window, ImGuiWindowFlags_AlwaysAutoResize);
	//	ImGui::Text("Welcome to Object Transformation Window!\nPlease Select a Frame:");

	//	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	//	ImGui::BeginTabBar("MyTabBar", tab_bar_flags);
	//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
	//	if (ImGui::BeginTabItem("World Transformation"))
	//	{
	//		ImGui::ListBox("Select Transformation", &Transformation, Transformations, IM_ARRAYSIZE(Transformations), 3);

	//		switch (Transformation)
	//		{
	//		case 0:
	//			ImGui::SliderFloat("Translate Factor X", &Xtranslate, -1.f, 1.f);
	//			ImGui::SliderFloat("Translate Factor Y", &Ytranslate, -1.f, 1.f);
	//			ImGui::SliderFloat("Translate Factor Z", &Ztranslate, -1.f, 1.f);
	//			glm::vec3 TranslateVec = glm::vec3(Xtranslate, Ytranslate, Ztranslate);
	//			TranslateMatrix = glm::translate(glm::mat4(1.0f), TranslateVec);
	//			scene.GetActiveCamera().setTransformationMatrix(TranslateMatrix, 0);
	//			break;

	//		case 1:
	//			ImGui::SliderFloat("Scale Factor X", &XScale, 0.f, 3.f);
	//			ImGui::SliderFloat("Scale Factor Y", &YScale, 0.f, 3.f);
	//			ImGui::SliderFloat("Scale Factor Z", &ZScale, 0.f, 3.f);
	//			glm::vec3 ScaleVec = glm::vec3(XScale, YScale, ZScale);
	//			ScaleMatrix = glm::scale(ScaleVec);
	//			scene.GetActiveCamera().setTransformationMatrix(ScaleMatrix, 1);
	//			break;

	//		case 2:

	//			ImGui::SliderInt("Rotation Angle X", &xAngle, -360, 360);
	//			ImGui::SliderInt("Rotation Angle Y", &yAngle, -360, 360);
	//			ImGui::SliderInt("Rotation Angle Z", &zAngle, -360, 360);


	//			if (xAngle != Current_Rotate_Values.x)
	//			{
	//				Current_Rotate_Values.x = xAngle;
	//				glm::vec3 myRotationAxis = glm::vec3(1, 0, 0);
	//				RotateMatrix = glm::rotate(glm::radians(float(xAngle)), myRotationAxis);
	//				scene.GetActiveCamera().setTransformationMatrix(RotateMatrix, 2);
	//			}


	//			else if (yAngle != Current_Rotate_Values.y)
	//			{
	//				Current_Rotate_Values.y = yAngle;

	//				glm::vec3 myRotationAxis = glm::vec3(0, 1, 0);
	//				RotateMatrix = glm::rotate(glm::radians(float(yAngle)), myRotationAxis);
	//				scene.GetActiveCamera().setTransformationMatrix(RotateMatrix, 2);

	//			}


	//			else if (zAngle != Current_Rotate_Values.z)
	//			{
	//				Current_Rotate_Values.z = zAngle;

	//				glm::vec3 myRotationAxis = glm::vec3(0, 0, 1);
	//				RotateMatrix = glm::rotate(glm::radians(float(zAngle)), myRotationAxis);
	//				scene.GetActiveCamera().setTransformationMatrix(RotateMatrix, 2);


	//			}


	//			//scene.GetActiveCamera().setTransformationMatrix(RotateMatrix, 2);

	//			break;



	//		}


	//		if (ImGui::Button("Reset Parameters"))
	//		{
	//			Xtranslate = 0;
	//			Ytranslate = 0;
	//			Ztranslate = 0;
	//			TranslateMatrix = glm::mat4(1.0f);
	//			XScale = 1;
	//			YScale = 1;
	//			ZScale = 1;
	//			ScaleMatrix = glm::mat4(1.0f);
	//			xAngle = 0;
	//			yAngle = 0;
	//			zAngle = 0;
	//			RotateMatrix = glm::mat4(1.0f);

	//			scene.GetActiveCamera().ResetTransformationMatrix();

	//		}



	//		ImGui::EndTabItem();
	//	}
	//	if (ImGui::BeginTabItem("Local Transformation"))
	//	{
	//		ImGui::ListBox("Select Transformation", &Transformation, Transformations, IM_ARRAYSIZE(Transformations), 3);

	//		switch (Transformation)
	//		{
	//		case 0:
	//			ImGui::SliderFloat("Translate Factor X", &Xtranslate_Local, -1.f, 1.f);
	//			ImGui::SliderFloat("Translate Factor Y", &Ytranslate_Local, -1.f, 1.f);
	//			ImGui::SliderFloat("Translate Factor Z", &Ztranslate_Local, -1.f, 1.f);
	//			glm::vec3 TranslateVec_Local = glm::vec3(Xtranslate_Local, Ytranslate_Local, Ztranslate_Local);
	//			TranslateMatrix_Local = glm::translate(glm::mat4(1.0f), TranslateVec_Local);
	//			scene.GetActiveCamera().setLocalTransformationMatrix(TranslateMatrix_Local, 0);


	//			break;

	//		case 1:
	//			ImGui::SliderFloat("Scale Factor X", &XScale_Local, 0.f, 3.f);
	//			ImGui::SliderFloat("Scale Factor Y", &YScale_Local, 0.f, 3.f);
	//			ImGui::SliderFloat("Scale Factor Z", &ZScale_Local, 0.f, 3.f);
	//			glm::vec3 ScaleVec_Local = glm::vec3(XScale_Local, YScale_Local, ZScale_Local);
	//			ScaleMatrix_Local = glm::scale(ScaleVec_Local);
	//			scene.GetActiveCamera().setLocalTransformationMatrix(ScaleMatrix_Local, 1);

	//			break;

	//		case 2:

	//			ImGui::SliderInt("Rotation Angle X", &xAngle_Local, -360, 360);
	//			ImGui::SliderInt("Rotation Angle Y", &yAngle_Local, -360, 360);
	//			ImGui::SliderInt("Rotation Angle Z", &zAngle_Local, -360, 360);


	//			if (xAngle_Local != Current_Local_Rotate_Values.x)
	//			{
	//				Current_Local_Rotate_Values.x = xAngle_Local;
	//				glm::vec3 myRotationAxis_Local = glm::vec3(1, 0, 0);
	//				RotateMatrix_Local = glm::rotate(glm::radians(float(xAngle_Local)), myRotationAxis_Local);
	//				scene.GetActiveCamera().setLocalTransformationMatrix(RotateMatrix_Local, 2);
	//			}


	//			else if (yAngle_Local != Current_Local_Rotate_Values.y)
	//			{
	//				Current_Local_Rotate_Values.y = yAngle_Local;

	//				glm::vec3 myRotationAxis_Local = glm::vec3(0, 1, 0);
	//				RotateMatrix_Local = glm::rotate(glm::radians(float(yAngle_Local)), myRotationAxis_Local);
	//				scene.GetActiveCamera().setLocalTransformationMatrix(RotateMatrix_Local, 2);

	//			}


	//			else if (zAngle_Local != Current_Local_Rotate_Values.z)
	//			{
	//				Current_Local_Rotate_Values.z = zAngle_Local;

	//				glm::vec3 myRotationAxis_Local = glm::vec3(0, 0, 1);
	//				RotateMatrix_Local = glm::rotate(glm::radians(float(zAngle_Local)), myRotationAxis_Local);
	//				scene.GetActiveCamera().setLocalTransformationMatrix(RotateMatrix_Local, 2);

	//			}


	//			//	scene.GetActiveCamera().setLocalTransformationMatrix(RotateMatrix_Local, 2);

	//			break;


	//		}



	//		if (ImGui::Button("Reset Local Parameters"))
	//		{
	//			Xtranslate_Local = 0;
	//			Ytranslate_Local = 0;
	//			Ztranslate_Local = 0;
	//			TranslateMatrix_Local = glm::mat4(1.0f);
	//			XScale_Local = 1;
	//			YScale_Local = 1;
	//			ZScale_Local = 1;
	//			ScaleMatrix_Local = glm::mat4(1.0f);
	//			xAngle_Local = 0;
	//			yAngle_Local = 0;
	//			zAngle_Local = 0;
	//			RotateMatrix_Local = glm::mat4(1.0f);
	//			scene.GetActiveCamera().ResetLocalTransformationMatrix();

	//		}

	//		ImGui::EndTabItem();
	//	}
	//	scene.GetActiveCamera().setTransformation();
	//	ImGui::EndTabBar();

	//	ImGui::AlignTextToFramePadding();

	//	if (ImGui::Button("Show Bounding Box"))
	//	{
	//		if (show_bounding_box)
	//		{
	//			scene.GetActiveCamera().BoundingBox(false, BoundingBoxColor);
	//			show_bounding_box = false;
	//		}
	//		else
	//			show_bounding_box = true;
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Show Face Normals"))
	//	{
	//		if (show_face_normals)
	//		{
	//			scene.GetActiveCamera().FaceNormals(false, FaceNormalsColor);
	//			show_face_normals = false;
	//		}
	//		else
	//			show_face_normals = true;
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Show Vertex Normals"))
	//	{
	//		if (show_vertex_normals)
	//		{
	//			scene.GetActiveCamera().VertexNormals(false, VertexNormalsColor);
	//			show_vertex_normals = false;
	//		}
	//		else
	//			show_vertex_normals = true;
	//	}

	//	if (ImGui::Button("Close Me"))
	//	{
	//		show_bounding_box = false;
	//		show_transformation_window = false;
	//		show_face_normals = false;
	//		show_vertex_normals = false;


	//	}
	//	ImGui::SameLine();
	//	ImGui::End();



	//}

	//if (show_bounding_box && scene.GetCameraCount() > 0)
	//{
	//	ImGui::Begin("Bounding Box", &show_bounding_box, ImGuiWindowFlags_AlwaysAutoResize);
	//	ImGui::AlignTextToFramePadding();
	//	ImGui::ColorEdit3("Pick Bounding Box Color", (float*)&BoundingBoxColor);
	//
	//	scene.GetActiveCamera().BoundingBox(true, BoundingBoxColor);
	//
	//	if (ImGui::Button("Hide Bounding Box"))
	//	{
	//		scene.GetActiveCamera().BoundingBox(false, BoundingBoxColor);
	//		show_bounding_box = false;
	//
	//	}
	//	ImGui::End();
	//
	//}
	//if (show_face_normals && scene.GetCameraCount() > 0)
	//{
	//
	//	ImGui::Begin("Face Normals", &show_face_normals, ImGuiWindowFlags_AlwaysAutoResize);
	//	ImGui::AlignTextToFramePadding();
	//	ImGui::ColorEdit3("Pick Face Normals Color", (float*)&FaceNormalsColor);
	//
	//	scene.GetActiveCamera().FaceNormals(true, FaceNormalsColor);
	//
	//	if (ImGui::Button("Hide Face Normals Box"))
	//	{
	//		scene.GetActiveCamera().FaceNormals(false, FaceNormalsColor);
	//		show_face_normals = false;
	//
	//	}
	//	ImGui::End();
	//
	//}
	//if (show_vertex_normals && scene.GetCameraCount() > 0)
	//{
	//
	//	ImGui::Begin("Vertex Normals", &show_vertex_normals, ImGuiWindowFlags_AlwaysAutoResize);
	//	ImGui::AlignTextToFramePadding();
	//	ImGui::ColorEdit3("Pick Vertex Normals Color", (float*)&VertexNormalsColor);
	//
	//	scene.GetActiveCamera().VertexNormals(true, VertexNormalsColor);
	//
	//	if (ImGui::Button("Hide Vertex Normals Box"))
	//	{
	//		scene.GetActiveCamera().FaceNormals(false, VertexNormalsColor);
	//		show_vertex_normals = false;
	//
	//	}
	//
	//	ImGui::End();
	//
	//}
	if (show_camera_transformations && scene.GetCameraCount() > 0)
	{
		static float CameraXtranslate = 0.f;
		static float CameraYtranslate = 0.f;
		static float CameraZtranslate = 0.f;
		static int CameraxAngle = 0;
		static int CamerayAngle = 0;
		static int CamerazAngle = 0;
		static float CameraXtranslate_Local = 0.f;
		static float CameraYtranslate_Local = 0.f;
		static float CameraZtranslate_Local = 0.f;
		static int CameraxAngle_Local = 0;
		static int CamerayAngle_Local = 0;
		static int CamerazAngle_Local = 0;
		glm::vec3 Camera_Current_Rotate_Values(CameraxAngle, CamerayAngle, CamerazAngle);
		glm::vec3 Current_Local_Rotate_Values(CameraxAngle_Local, CamerayAngle_Local, CamerazAngle_Local);
		static int CameraTransformation = 0;
		glm::mat4x4 CameraTranslateMatrix = glm::mat4(1.0f);
		glm::mat4x4 CameraTranslateMatrix_Local = glm::mat4(1.0f);
		glm::mat4x4 ScaleMatrix = glm::mat4(1.0f);
		glm::mat4x4 ScaleMatrix_Local = glm::mat4(1.0f);
		glm::mat4x4 CameraRotateMatrix = glm::mat4(1.0f);
		glm::mat4x4 CameraRotateMatrix_Local = glm::mat4(1.0f);
		static int SelectedProjection = 1;
		static int eye_at_up = 0;
		static float begin = 0.2, end = 60;
		static float orthoWidth = scene.GetActiveCamera().getWidth(), fovy = 45.f, eye_x = 0.f, eye_y = 0.f, eye_z = scene.GetActiveCamera().getEye().z;
		static float at_x = 0.f, at_y = 0.f, at_z = 0.f;
		static float up_x = 0.f, up_y = 1.f, up_z = 0.f;

		ImGui::Begin("Camera", &show_camera_transformations, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Select Projection");
		ImGui::RadioButton("Perspective", &SelectedProjection, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Orthographic", &SelectedProjection, 1);
		if (SelectedProjection)
		{
			scene.GetActiveCamera().ResetMatrices();
			scene.GetActiveCamera().SetCameraLookAt(scene.GetActiveCamera().getEye(), scene.GetActiveCamera().getAt(), scene.GetActiveCamera().getUp());
			ImGui::DragFloatRange2("Orthographic Width Range", &begin, &end, 0.25f, 0.0f, 1000.0f, "Min: %.2f ", "Max: %.2f ");
			ImGui::SliderFloat("Width", &orthoWidth, begin, end);
			scene.GetActiveCamera().setOrthographicflag(true);
			scene.GetActiveCamera().setOrthographic(orthoWidth);
			scene.GetActiveCamera().SetProjectionTransformation();

		}
		else
		{
			scene.GetActiveCamera().ResetMatrices();

			scene.GetActiveCamera().SetCameraLookAt(scene.GetActiveCamera().getEye(), scene.GetActiveCamera().getAt(), scene.GetActiveCamera().getUp());
			ImGui::SliderFloat("FOV", &fovy, 1.f, 180.f);
			scene.GetActiveCamera().setOrthographicflag(false);
			scene.GetActiveCamera().setPrespective(fovy);
			scene.GetActiveCamera().SetProjectionTransformation();

		}
		if (ImGui::TreeNode("Look At"))
		{
			scene.GetActiveCamera().setLookatflag(true);



			ImGui::RadioButton("Eye", &eye_at_up, 0);
			ImGui::SameLine();
			ImGui::RadioButton("At", &eye_at_up, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Up", &eye_at_up, 2);
			switch (eye_at_up)
			{
			case 0:


				ImGui::InputFloat("X", &eye_x, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Y", &eye_y, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Z", &eye_z, .5f, 1000.0f, "%.1f");
				break;

			case 1:


				ImGui::InputFloat("X", &at_x, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Y", &at_y, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Z", &at_z, .5f, 1000.0f, "%.1f");
				break;
			case 2:


				ImGui::InputFloat("X", &up_x, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Y", &up_y, .5f, 1000.0f, "%.1f");
				ImGui::InputFloat("Z", &up_z, .5f, 1000.0f, "%.1f");
				break;
			}
			scene.GetActiveCamera().SetCameraLookAt(glm::vec3(eye_x, eye_y, eye_z), glm::vec3(at_x, at_y, at_z), glm::vec3(up_x, up_y, up_z));
			ImGui::TreePop();

		}



		if (ImGui::TreeNode("Camera Transformations"))
		{
			scene.GetActiveCamera().setLookatflag(false);

			const char* CameraTransformations[] = { "Translate", "Rotate" };
			ImGui::Text("Please Select a Frame");

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			ImGui::BeginTabBar("MyTabBar", tab_bar_flags);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			if (ImGui::BeginTabItem("World"))
			{
				ImGui::ListBox("Select Camera Transformation", &CameraTransformation, CameraTransformations, IM_ARRAYSIZE(CameraTransformations), 2);

				switch (CameraTransformation)
				{
				case 0:
					ImGui::SliderFloat("Translate Factor X", &CameraXtranslate, -1.f, 1.f);
					ImGui::SliderFloat("Translate Factor Y", &CameraYtranslate, -1.f, 1.f);
					ImGui::SliderFloat("Translate Factor Z", &CameraZtranslate, -1.f, 1.f);
					glm::vec3 CameraTranslateVec = glm::vec3(CameraXtranslate, CameraYtranslate, CameraZtranslate);
					CameraTranslateMatrix = glm::translate(glm::mat4(1.0f), CameraTranslateVec);
					scene.GetActiveCamera().setCameraTransformationMatrix(CameraTranslateMatrix, 0);
					break;


				case 1:

					ImGui::SliderInt("Rotation Angle X", &CameraxAngle, -360, 360);
					ImGui::SliderInt("Rotation Angle Y", &CamerayAngle, -360, 360);
					ImGui::SliderInt("Rotation Angle Z", &CamerazAngle, -360, 360);


					if (CameraxAngle != Camera_Current_Rotate_Values.x)
					{
						Camera_Current_Rotate_Values.x = CameraxAngle;
						glm::vec3 Camera_myRotationAxis = glm::vec3(1, 0, 0);
						CameraRotateMatrix = glm::rotate(glm::radians(float(CameraxAngle)), Camera_myRotationAxis);
						scene.GetActiveCamera().setCameraTransformationMatrix(CameraRotateMatrix, 2);
					}


					else if (CamerayAngle != Camera_Current_Rotate_Values.y)
					{
						Camera_Current_Rotate_Values.y = CamerayAngle;

						glm::vec3 Camera_myRotationAxis = glm::vec3(0, 1, 0);
						CameraRotateMatrix = glm::rotate(glm::radians(float(CamerayAngle)), Camera_myRotationAxis);
						scene.GetActiveCamera().setCameraTransformationMatrix(CameraRotateMatrix, 2);

					}


					else if (CamerazAngle != Camera_Current_Rotate_Values.z)
					{
						Camera_Current_Rotate_Values.z = CamerazAngle;

						glm::vec3 Camera_myRotationAxis = glm::vec3(0, 0, 1);
						CameraRotateMatrix = glm::rotate(glm::radians(float(CamerazAngle)), Camera_myRotationAxis);
						scene.GetActiveCamera().setCameraTransformationMatrix(CameraRotateMatrix, 2);


					}


					//scene.GetActiveCamera().setCameraTransformationMatrix(CameraRotateMatrix, 2);

					break;



				}


				if (ImGui::Button("Reset Parameters"))
				{
					CameraXtranslate = 0;
					CameraYtranslate = 0;
					CameraZtranslate = 0;
					CameraTranslateMatrix = glm::mat4(1.0f);
					CameraxAngle = 0;
					CamerayAngle = 0;
					CamerazAngle = 0;
					CameraRotateMatrix = glm::mat4(1.0f);

					scene.GetActiveCamera().ResetCameraTransformationMatrix();

				}



				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Local"))
			{
				ImGui::ListBox("Select CameraTransformation", &CameraTransformation, CameraTransformations, IM_ARRAYSIZE(CameraTransformations), 2);

				switch (CameraTransformation)
				{
				case 0:
					ImGui::SliderFloat("Translate Factor X", &CameraXtranslate_Local, -1.f, 1.f);
					ImGui::SliderFloat("Translate Factor Y", &CameraYtranslate_Local, -1.f, 1.f);
					ImGui::SliderFloat("Translate Factor Z", &CameraZtranslate_Local, -1.f, 1.f);
					glm::vec3 CameraTranslateVec_Local = glm::vec3(CameraXtranslate_Local, CameraYtranslate_Local, CameraZtranslate_Local);
					CameraTranslateMatrix_Local = glm::translate(glm::mat4(1.0f), CameraTranslateVec_Local);
					scene.GetActiveCamera().setCameraLocalTransformationMatrix(CameraTranslateMatrix_Local, 0);


					break;


				case 1:

					ImGui::SliderInt("Rotation Angle X", &CameraxAngle_Local, -360, 360);
					ImGui::SliderInt("Rotation Angle Y", &CamerayAngle_Local, -360, 360);
					ImGui::SliderInt("Rotation Angle Z", &CamerazAngle_Local, -360, 360);


					if (CameraxAngle_Local != Current_Local_Rotate_Values.x)
					{
						Current_Local_Rotate_Values.x = CameraxAngle_Local;
						glm::vec3 Camera_myRotationAxis_Local = glm::vec3(1, 0, 0);
						CameraRotateMatrix_Local = glm::rotate(glm::radians(float(CameraxAngle_Local)), Camera_myRotationAxis_Local);
						scene.GetActiveCamera().setCameraLocalTransformationMatrix(CameraRotateMatrix_Local, 2);
					}


					else if (CamerayAngle_Local != Current_Local_Rotate_Values.y)
					{
						Current_Local_Rotate_Values.y = CamerayAngle_Local;

						glm::vec3 Camera_myRotationAxis_Local = glm::vec3(0, 1, 0);
						CameraRotateMatrix_Local = glm::rotate(glm::radians(float(CamerayAngle_Local)), Camera_myRotationAxis_Local);
						scene.GetActiveCamera().setCameraLocalTransformationMatrix(CameraRotateMatrix_Local, 2);

					}


					else if (CamerazAngle_Local != Current_Local_Rotate_Values.z)
					{
						Current_Local_Rotate_Values.z = CamerazAngle_Local;

						glm::vec3 Camera_myRotationAxis_Local = glm::vec3(0, 0, 1);
						CameraRotateMatrix_Local = glm::rotate(glm::radians(float(CamerazAngle_Local)), Camera_myRotationAxis_Local);
						scene.GetActiveCamera().setCameraLocalTransformationMatrix(CameraRotateMatrix_Local, 2);

					}


					//	scene.GetActiveCamera().setLocalCameraTransformationMatrix(CameraRotateMatrix_Local, 2);

					break;


				}



				if (ImGui::Button("Reset Local Parameters"))
				{
					CameraXtranslate_Local = 0;
					CameraYtranslate_Local = 0;
					CameraZtranslate_Local = 0;
					CameraTranslateMatrix_Local = glm::mat4(1.0f);
					ScaleMatrix_Local = glm::mat4(1.0f);
					CameraxAngle_Local = 0;
					CamerayAngle_Local = 0;
					CamerazAngle_Local = 0;
					CameraRotateMatrix_Local = glm::mat4(1.0f);
					scene.GetActiveCamera().ResetCameraLocalTransformationMatrix();

				}

				ImGui::EndTabItem();
			}
			scene.GetActiveCamera().setCameraTransformation();
			ImGui::EndTabBar();


			ImGui::TreePop();
		}

		if (ImGui::Button("Close Me"))
		{

			show_camera_transformations = false;


		}
		ImGui::End();
	}

	if (show_light && scene.GetCameraCount() > 0)
	{
		bool clicked;
		ImGui::Begin("Lights Window", &show_light, ImGuiWindowFlags_AlwaysAutoResize);
		if (clicked = ImGui::Button("Add Light Source"))
		{
			scene.AddLight(std::make_shared<Light>());

			if (scene.GetLightCount() == 1)
			{
				scene.SetActiveLightIndex(0);
			}
		}
		int numberoflights = scene.GetLightCount();




		ImGui::SameLine();
		int i = 50;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 255.f, 255.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));


		ImGui::PopStyleColor(3);

		if (scene.GetLightCount() > 0)
		{

			int activelight = scene.GetActiveLightIndex() + 1;

			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(20.f, 100.f, 255.f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV(0.f, 0.f, 255.f));

			if (ImGui::SliderInt("Light Number", &activelight, 1, numberoflights))
				scene.SetActiveLightIndex(activelight - 1);

			ImGui::PopStyleColor(2);
			float LightXtranslate;
			float LightYtranslate;
			float LightZtranslate;
			float LightXtranslate_Local;
			float LightYtranslate_Local;
			float LightZtranslate_Local;
			if (scene.GetActiveCamera().getOrthographicflag())
			{
				LightXtranslate = scene.GetActiveLight().getLightXtranslate() / scene.GetActiveModel().getTranslatefactor();
				LightYtranslate = scene.GetActiveLight().getLightYtranslate() / scene.GetActiveModel().getTranslatefactor();
				LightZtranslate = scene.GetActiveLight().getLightZtranslate() / scene.GetActiveModel().getzTranslatefactor();
				LightXtranslate_Local = scene.GetActiveLight().getLightXtranslateLocal() / scene.GetActiveModel().getTranslatefactor();
				LightYtranslate_Local = scene.GetActiveLight().getLightYtranslateLocal() / scene.GetActiveModel().getTranslatefactor();
				LightZtranslate_Local = scene.GetActiveLight().getLightZtranslateLocal() / scene.GetActiveModel().getzTranslatefactor();
			}
			else {
				LightXtranslate = scene.GetActiveLight().getLightXtranslate();
				LightYtranslate = scene.GetActiveLight().getLightYtranslate();
				LightZtranslate = scene.GetActiveLight().getLightZtranslate();
				LightXtranslate_Local = scene.GetActiveLight().getLightXtranslateLocal();
				LightYtranslate_Local = scene.GetActiveLight().getLightYtranslateLocal();
				LightZtranslate_Local = scene.GetActiveLight().getLightZtranslateLocal();
			}
			int LightxAngle = scene.GetActiveLight().getLightxAngle();
			int LightyAngle = scene.GetActiveLight().getLightyAngle();
			int LightzAngle = scene.GetActiveLight().getLightzAngle();

			int LightxAngle_Local = scene.GetActiveLight().getLightxAngleLocal();
			int LightyAngle_Local = scene.GetActiveLight().getLightyAngleLocal();
			int LightzAngle_Local = scene.GetActiveLight().getLightzAngleLocal();
			glm::vec3 Light_Current_Rotate_Values(LightxAngle, LightyAngle, LightzAngle);
			glm::vec3 Current_Local_Rotate_Values(LightxAngle_Local, LightyAngle_Local, LightzAngle_Local);
			glm::mat4x4 LightTranslateMatrix = glm::mat4(1.0f);
			glm::mat4x4 LightTranslateMatrix_Local = glm::mat4(1.0f);
			glm::mat4x4 ScaleMatrix = glm::mat4(1.0f);
			glm::mat4x4 ScaleMatrix_Local = glm::mat4(1.0f);
			glm::mat4x4 LightRotateMatrix = glm::mat4(1.0f);
			glm::mat4x4 LightRotateMatrix_Local = glm::mat4(1.0f);
			static int  LightTransformation = 0;
			float LightXdirection = scene.GetActiveLight().getLightDirection().x;
			float LightYdirection = scene.GetActiveLight().getLightDirection().y;
			float LightZdirection = scene.GetActiveLight().getLightDirection().z;

			int SelectedLightType = scene.GetActiveLight().getLightType();
			static int SelectedShading = 2;
			ImGui::Text("Select Light Source Type");
			ImGui::RadioButton("Point Source", &SelectedLightType, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Parallel Source", &SelectedLightType, 1);
			//ImGui::Text("Please Select Shading:");
			//ImGui::RadioButton("Flat Shading", &SelectedShading, 0);
			//ImGui::SameLine();
			//ImGui::RadioButton("Gouraud Shading", &SelectedShading, 1);
			//ImGui::SameLine();
			ImGui::RadioButton("Phong Shading", &SelectedShading, 2);

			switch (SelectedShading)
			{
			case 0:
				scene.GetActiveLight().setFlat(true);
				break;
			case 1:
				scene.GetActiveLight().setGouraud(true);
				break;
			case 2:
				scene.GetActiveLight().setPhong(true);
				break;
			}
			if (SelectedLightType)
			{

				glm::vec3 parallel_ambient = scene.GetActiveLight().getParallelambient();
				glm::vec3 parallel_diffuse = scene.GetActiveLight().getParalleldiffuse();
				glm::vec3 parallel_specular = scene.GetActiveLight().getParallelspecular();
				scene.GetActiveLight().setLightType(1);
				ImGui::ColorEdit3("Ambient", (float*)&parallel_ambient); // Edit 3 floats representing a color
				ImGui::ColorEdit3("Diffuse", (float*)&parallel_diffuse); // Edit 3 floats representing a color
				ImGui::ColorEdit3("Specular", (float*)&parallel_specular); // Edit 3 floats representing a color
				if (ImGui::Button("Generate Random"))
				{
					float randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					parallel_ambient = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);

					randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					parallel_diffuse = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);

					randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					parallel_specular = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
				}
				scene.GetActiveLight().setParallelambient(parallel_ambient);
				scene.GetActiveLight().setParalleldiffuse(parallel_diffuse);
				scene.GetActiveLight().setParallelspecular(parallel_specular);

			}
			else
			{
				glm::vec3 point_ambient = scene.GetActiveLight().getPointambient();
				glm::vec3 point_diffuse = scene.GetActiveLight().getPointdiffuse();
				glm::vec3 point_specular = scene.GetActiveLight().getPointspecular();
				scene.GetActiveLight().setLightType(0);
				ImGui::ColorEdit3("Ambient", (float*)&point_ambient); // Edit 3 floats representing a color
				ImGui::ColorEdit3("Diffuse", (float*)&point_diffuse); // Edit 3 floats representing a color
				ImGui::ColorEdit3("Specular", (float*)&point_specular);
				if (ImGui::Button("Generate Random"))
				{
					float randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					point_ambient = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);

					randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					point_diffuse = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
					randomNumber_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					randomNumber_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					point_specular = glm::vec3(randomNumber_x, randomNumber_y, randomNumber_z);
				}
				scene.GetActiveLight().setPointambient(point_ambient);
				scene.GetActiveLight().setPointdiffuse(point_diffuse);
				scene.GetActiveLight().setPointspecular(point_specular);

			}
			


			
			if (ImGui::TreeNode("Light Transformations"))
			{
				if (!SelectedLightType)
				{
					const char* LightTransformations[] = { "Translate", "Rotate" };
					ImGui::Text("Please Select a Frame");

					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					ImGui::BeginTabBar("MyTabBar", tab_bar_flags);
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);

					if (ImGui::BeginTabItem("World"))
					{
						ImGui::ListBox("Select Light Transformation", &LightTransformation, LightTransformations, IM_ARRAYSIZE(LightTransformations), 2);

						switch (LightTransformation)
						{
						case 0:
							glm::vec3 LightTranslateVec;
							ImGui::SliderFloat("Translate Factor X", &LightXtranslate, -2.f, 2.f);
							ImGui::SliderFloat("Translate Factor Y", &LightYtranslate, -2.f, 2.f);
							ImGui::SliderFloat("Translate Factor Z", &LightZtranslate, -2.f, 2.f);
							if (!scene.GetActiveCamera().getOrthographicflag())
							{
								scene.GetActiveLight().setLightXtranslate(LightXtranslate);
								scene.GetActiveLight().setLightYtranslate(LightYtranslate);
								scene.GetActiveLight().setLightZtranslate(LightZtranslate);
								LightTranslateVec = glm::vec3(LightXtranslate, LightYtranslate, LightZtranslate);
							}
							else {
								scene.GetActiveLight().setLightXtranslate(LightXtranslate * scene.GetActiveModel().getTranslatefactor());
								scene.GetActiveLight().setLightYtranslate(LightYtranslate * scene.GetActiveModel().getTranslatefactor());
								scene.GetActiveLight().setLightZtranslate(LightZtranslate * scene.GetActiveModel().getzTranslatefactor());
								LightTranslateVec = glm::vec3(LightXtranslate * scene.GetActiveModel().getTranslatefactor(), LightYtranslate * scene.GetActiveModel().getTranslatefactor(), LightZtranslate * scene.GetActiveModel().getzTranslatefactor());
							}
							LightTranslateMatrix = glm::translate(glm::mat4(1.0f), LightTranslateVec);
							scene.GetActiveLight().setLocalTransformationMatrix(LightTranslateMatrix, 0);
							break;


						case 1:

							ImGui::SliderInt("Rotation Angle X", &LightxAngle, -360, 360);
							ImGui::SliderInt("Rotation Angle Y", &LightyAngle, -360, 360);
							ImGui::SliderInt("Rotation Angle Z", &LightzAngle, -360, 360);
							scene.GetActiveLight().setLightxAngle(LightxAngle);
							scene.GetActiveLight().setLightyAngle(LightyAngle);
							scene.GetActiveLight().setLightzAngle(LightzAngle);

							if (LightxAngle != Light_Current_Rotate_Values.x)
							{
								Light_Current_Rotate_Values.x = LightxAngle;
								glm::vec3 Light_myRotationAxis = glm::vec3(1, 0, 0);
								LightRotateMatrix = glm::rotate(glm::radians(float(LightxAngle)), Light_myRotationAxis);
								scene.GetActiveLight().setTransformationMatrix(LightRotateMatrix, 1);
							}


							else if (LightyAngle != Light_Current_Rotate_Values.y)
							{
								Light_Current_Rotate_Values.y = LightyAngle;

								glm::vec3 Light_myRotationAxis = glm::vec3(0, 1, 0);
								LightRotateMatrix = glm::rotate(glm::radians(float(LightyAngle)), Light_myRotationAxis);
								scene.GetActiveLight().setTransformationMatrix(LightRotateMatrix, 1);

							}


							else if (LightzAngle != Light_Current_Rotate_Values.z)
							{
								Light_Current_Rotate_Values.z = LightzAngle;

								glm::vec3 Light_myRotationAxis = glm::vec3(0, 0, 1);
								LightRotateMatrix = glm::rotate(glm::radians(float(LightzAngle)), Light_myRotationAxis);
								scene.GetActiveLight().setTransformationMatrix(LightRotateMatrix, 1);


							}


							//scene.GetActiveLight().setLightTransformationMatrix(LightRotateMatrix, 2);

							break;



						}


						if (ImGui::Button("Reset Parameters"))
						{
							LightXtranslate = 0;
							LightYtranslate = 0;
							LightZtranslate = 0;
							scene.GetActiveLight().setLightXtranslate(LightXtranslate);
							scene.GetActiveLight().setLightYtranslate(LightYtranslate);
							scene.GetActiveLight().setLightZtranslate(LightZtranslate);
							LightTranslateMatrix = glm::mat4(1.0f);
							LightxAngle = 0;
							LightyAngle = 0;
							LightzAngle = 0;
							scene.GetActiveLight().setLightxAngle(LightxAngle);
							scene.GetActiveLight().setLightyAngle(LightyAngle);
							scene.GetActiveLight().setLightzAngle(LightzAngle);
							LightRotateMatrix = glm::mat4(1.0f);
							scene.GetActiveLight().ResetTransformationMatrix();

						}



						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Local"))
					{
						ImGui::ListBox("Select LightTransformation", &LightTransformation, LightTransformations, IM_ARRAYSIZE(LightTransformations), 2);

						switch (LightTransformation)
						{
						case 0:
							glm::vec3 LightTranslateVec_Local;
							ImGui::SliderFloat("Translate Factor X", &LightXtranslate_Local, -2.f, 2.f);
							ImGui::SliderFloat("Translate Factor Y", &LightYtranslate_Local, -2.f, 2.f);
							ImGui::SliderFloat("Translate Factor Z", &LightZtranslate_Local, -2.f, 2.f);
							if (!scene.GetActiveCamera().getOrthographicflag())
							{
								scene.GetActiveLight().setLightXtranslateLocal(LightXtranslate_Local);
								scene.GetActiveLight().setLightYtranslateLocal(LightYtranslate_Local);
								scene.GetActiveLight().setLightZtranslateLocal(LightZtranslate_Local);
								LightTranslateVec_Local = glm::vec3(LightXtranslate_Local, LightYtranslate_Local , LightZtranslate_Local);
							}
							else {
								scene.GetActiveLight().setLightXtranslateLocal(LightXtranslate_Local * scene.GetActiveModel().getTranslatefactor());
								scene.GetActiveLight().setLightYtranslateLocal(LightYtranslate_Local * scene.GetActiveModel().getTranslatefactor());
								scene.GetActiveLight().setLightZtranslateLocal(LightZtranslate_Local * scene.GetActiveModel().getzTranslatefactor());
								LightTranslateVec_Local = glm::vec3(LightXtranslate_Local * scene.GetActiveModel().getTranslatefactor(), LightYtranslate_Local * scene.GetActiveModel().getTranslatefactor(), LightZtranslate_Local * scene.GetActiveModel().getzTranslatefactor());
							}
							LightTranslateMatrix_Local = glm::translate(glm::mat4(1.0f), LightTranslateVec_Local);
							scene.GetActiveLight().setLocalTransformationMatrix(LightTranslateMatrix_Local, 0);


							break;


						case 1:

							ImGui::SliderInt("Rotation Angle X", &LightxAngle_Local, -360, 360);
							ImGui::SliderInt("Rotation Angle Y", &LightyAngle_Local, -360, 360);
							ImGui::SliderInt("Rotation Angle Z", &LightzAngle_Local, -360, 360);
							scene.GetActiveLight().setLightxAngleLocal(LightxAngle_Local);
							scene.GetActiveLight().setLightyAngleLocal(LightyAngle_Local);
							scene.GetActiveLight().setLightzAngleLocal(LightzAngle_Local);

							if (LightxAngle_Local != Current_Local_Rotate_Values.x)
							{
								Current_Local_Rotate_Values.x = LightxAngle_Local;
								glm::vec3 Light_myRotationAxis_Local = glm::vec3(1, 0, 0);
								LightRotateMatrix_Local = glm::rotate(glm::radians(float(LightxAngle_Local)), Light_myRotationAxis_Local);
								scene.GetActiveLight().setLocalTransformationMatrix(LightRotateMatrix_Local, 1);
							}


							else if (LightyAngle_Local != Current_Local_Rotate_Values.y)
							{
								Current_Local_Rotate_Values.y = LightyAngle_Local;

								glm::vec3 Light_myRotationAxis_Local = glm::vec3(0, 1, 0);
								LightRotateMatrix_Local = glm::rotate(glm::radians(float(LightyAngle_Local)), Light_myRotationAxis_Local);
								scene.GetActiveLight().setLocalTransformationMatrix(LightRotateMatrix_Local, 1);

							}


							else if (LightzAngle_Local != Current_Local_Rotate_Values.z)
							{
								Current_Local_Rotate_Values.z = LightzAngle_Local;

								glm::vec3 Light_myRotationAxis_Local = glm::vec3(0, 0, 1);
								LightRotateMatrix_Local = glm::rotate(glm::radians(float(LightzAngle_Local)), Light_myRotationAxis_Local);
								scene.GetActiveLight().setLocalTransformationMatrix(LightRotateMatrix_Local, 1);

							}


							//	scene.GetActiveLight().setLocalLightTransformationMatrix(LightRotateMatrix_Local, 2);

							break;


						}



						if (ImGui::Button("Reset Local Parameters"))
						{
							LightXtranslate_Local = 0;
							LightYtranslate_Local = 0;
							LightZtranslate_Local = 0;
							scene.GetActiveLight().setLightXtranslateLocal(LightXtranslate_Local);
							scene.GetActiveLight().setLightYtranslateLocal(LightYtranslate_Local);
							scene.GetActiveLight().setLightZtranslateLocal(LightZtranslate_Local);
							LightTranslateMatrix_Local = glm::mat4(1.0f);
							LightxAngle_Local = 0;
							LightyAngle_Local = 0;
							LightzAngle_Local = 0;
							scene.GetActiveLight().setLightxAngleLocal(LightxAngle_Local);
							scene.GetActiveLight().setLightyAngleLocal(LightyAngle_Local);
							scene.GetActiveLight().setLightzAngleLocal(LightzAngle_Local);
							LightRotateMatrix_Local = glm::mat4(1.0f);
							scene.GetActiveLight().ResetLocalTransformationMatrix();

						}

						ImGui::EndTabItem();
					}
					scene.GetActiveLight().setTransformation();


					ImGui::EndTabBar();

				}

				ImGui::TreePop();

			}
			if (SelectedLightType) {



				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				ImGui::BeginTabBar("MyTabBar", tab_bar_flags);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);

				if (ImGui::BeginTabItem("Direction"))
				{
					ImGui::SliderFloat("X Direction", &LightXdirection, -20.f, 20.f);
					ImGui::SliderFloat("Y Direction", &LightYdirection, -20.f, 20.f);
					ImGui::SliderFloat("Z Direction", &LightZdirection, -20.f, 20.f);
					scene.GetActiveLight().setLightDirection(glm::vec3(LightXdirection, LightYdirection, LightZdirection));
				}
				ImGui::EndTabItem();
				ImGui::EndTabBar();


			}


			ImGui::Text("Selected Light: %d", activelight);
			if (ImGui::Button("Remove Light Source") && scene.GetLightCount() > 0)
			{
				scene.PopLightbyIndex(activelight);
				if(activelight==1)
					activelight = 1;
				else
					activelight -= 1;
				scene.SetActiveLightIndex(activelight - 1);


			}
		}







		ImGui::Text("Number of Active Lights: %d", scene.GetLightCount());


		if (ImGui::Button("Clear Lights"))
		{
			if (scene.GetLightCount() > 0)
			{
				scene.clearLights();
			}


		}
		ImGui::End();


	}
	if (fog)
	{
		ImGui::Begin("Fog Window", &fog, ImGuiWindowFlags_AlwaysAutoResize);
		static int SelectedFogType = 0;
		static int FogDensity = 0;
		static int FogStart = 1;
		static int FogEnd = 1;
		ImGui::Text("Select Light Source Type");
		if (ImGui::RadioButton("Linear Fog", &SelectedFogType, 0))
		{
			scene.setLinearfog(true);
			scene.setExponentialfog(false);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Exponential Fog", &SelectedFogType, 1))
		{
			scene.setLinearfog(false);
			scene.setExponentialfog(true);
			
		}
		if (SelectedFogType)
		{
			ImGui::SliderInt("Fog Density", &FogDensity, 0, 100);
			scene.setFogdensity(FogDensity);
		}
		else {
			ImGui::SliderInt("Fog Start", &FogStart, 1, 100);
			ImGui::SliderInt("Fog End", &FogEnd, 1, 100);
			scene.setFogstartmultfactor(FogStart);
			scene.setFogendmultfactor(FogEnd);
		}




		ImGui::End();

	}

}



