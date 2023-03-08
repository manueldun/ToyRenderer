#include"GLApp.h"
#include<iostream>
#include<future>
#include<glm/glm.hpp>
#include<glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include<glm/ext/matrix_clip_space.hpp> // glm::perspective
#include<glm/gtc/type_ptr.hpp>
#include"math/CameraMouseController.h"
#include"Utils/OBJData.h"
#include"Utils/loadOBJFile.h"
#include"Utils/Utils.h"
#include"GL/ConvertToMeshes.h"
#include"GL/Renderer.h"
#include"VertexArray.h"

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include"tinygltf/tiny_gltf.h" 

#include"imGUI/ImGuiFileDialog.h"

std::vector<std::function<void(int, int, int)>> GLApp::m_mouseButtonCallbacks;

GLApp::GLApp() :m_window(nullptr), m_windowWidth(1280), m_windowHeight(720)
{
}

void GLApp::run()
{
	init();
	initOpengl();
	loop();
	cleanUp();
}

void GLApp::init()
{
	if (!glfwInit()) {
		throw std::runtime_error("Error initializing glfw!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Hello World", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Error creating gltf window!");
	}
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Error creating loading Opengl API!");
	}
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);



	const char* glsl_version = "#version 400";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	glfwSetMouseButtonCallback(m_window, &GLApp::mouse_button_callback);
	m_cameraMouseController = CameraMouseController(m_window);
	m_mouseButtonCallbacks.push_back(m_cameraMouseController.getMouseCallBack());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

}



void GLApp::initOpengl()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glEnable(GL_FRAMEBUFFER_SRGB);

}

void GLApp::loop()
{

	const Renderer renderer(m_windowWidth, m_windowHeight);



	VertexArray::unbind();


	std::future<std::vector<Mesh>> futureLoad = std::async(std::launch::async, []()->std::vector<Mesh> {

		std::vector<Mesh> meshes;
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;


		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "Assets/gltfSponza/sponza.gltf");

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		if (!ret) {
			printf("Failed to parse glTF\n");
			throw std::runtime_error("Failed to `parse gltf");
		}

		meshes = convertToMeshes(model);
		//for (auto& mesh : meshes)
		//{
		//	mesh.loadToGPU();
		//}

		return meshes;
		});




	glm::mat4 projectionMatrix = glm::perspective(glm::pi<float>() / 3.0f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 10000.f);



	double oldTime = glfwGetTime();


	std::vector<Mesh> meshes;
	bool loaded = false;
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		double deltaTime = glfwGetTime() - oldTime;
		oldTime = glfwGetTime();
		renderer.clearAll();



		static float lightTranslatex = 0.0f;
		static float lightTranslatey = 0.0f;
		static float lightTranslatez = 0.0f;
		static float lightAngle1 = 0.0f;
		static float lightAngle2 = 90.0f;
		static float lightScale = 2000.0f;
		static float indirectSpan = 0.2f;



		const bool isWindowFocused = renderGUI(
			&lightTranslatex,
			&lightTranslatey,
			&lightTranslatez,
			&lightAngle1,
			&lightAngle2,
			&lightScale,
			&indirectSpan,
			renderer.GBufferNormal->getID());

		double xpos = 0.0;
		double ypos = 0.0;
		glfwGetCursorPos(m_window, &xpos, &ypos);



		glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(lightTranslatex, lightTranslatey, lightTranslatez));
		glm::mat4 rotatey = glm::rotate(translate, (lightAngle1 * glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotatex = glm::rotate(rotatey, (lightAngle2 * glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 lightMatrix = glm::scale(rotatex, glm::vec3(lightScale));
		glm::vec3 lightDirection = glm::vec3(rotatex * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));

		glm::mat4 shadowMatrix = glm::inverse(lightMatrix);

		glm::mat4 viewMatrix = m_cameraMouseController.control(static_cast<float>(deltaTime), static_cast<float>(xpos), static_cast<float>(ypos), !isWindowFocused);


		if (!loaded && futureLoad.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
		{
			loaded = true;
			meshes = futureLoad.get();
		}
		if(loaded)
		{
			if (!meshes.at(0).isInGPU())
			{
				for (auto& mesh : meshes)
				{
					mesh.loadToGPU();
				}
			}
			for (const auto& mesh : meshes)
			{
				renderer.renderShadowMap(glm::value_ptr(shadowMatrix), mesh);
			}
			for (const auto& mesh : meshes)
			{
				renderer.renderGBuffer(glm::value_ptr(projectionMatrix), glm::value_ptr(viewMatrix), mesh);
			}

			renderer.renderDeferredPass(glm::value_ptr(shadowMatrix), glm::value_ptr(lightDirection), glm::value_ptr(m_cameraMouseController.m_cameraPosition));

		}


		Framebuffer::unbind();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);

	}
}

void GLApp::cleanUp()
{

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

}

const bool GLApp::renderGUI(
	float* const lightTranslatex,
	float* const lightTranslatey,
	float* const lightTranslatez,
	float* const lightAngle1,
	float* const lightAngle2,
	float* const lightScale,
	float* const indirectSpan,
	unsigned int debugTextureID)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");

	ImGui::Text("Debug Window:");
	ImGui::SliderFloat("Light X Position", lightTranslatex, -10.0f, 10.0f);
	ImGui::SliderFloat("Light Y Position", lightTranslatey, -10.0f, 10.0f);
	ImGui::SliderFloat("Light Z Position", lightTranslatez, -10.0f, 10.0f);
	ImGui::SliderFloat("Light Angle1", lightAngle1, 0.0f, 360.0f);
	ImGui::SliderFloat("Light Angle2", lightAngle2, 0.0f, 360.0f);
	ImGui::SliderFloat("Light Scale", lightScale, 0.01f, 5000.0f);
	ImGui::SliderFloat("indirectSpan", indirectSpan, -3.0f, 3.0f);

	ImGui::Text("Cammera position: %.3f,%.3f,%.3f",
		m_cameraMouseController.m_cameraPosition.x,
		m_cameraMouseController.m_cameraPosition.y,
		m_cameraMouseController.m_cameraPosition.z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	bool isWindowFocused = ImGui::IsWindowFocused();
	ImGui::Image((void*)(intptr_t)debugTextureID, ImVec2(640, 360), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();

	return isWindowFocused;
}




void GLApp::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	for (const auto& callback : m_mouseButtonCallbacks)
	{
		callback(button, action, mods);
	}
}

