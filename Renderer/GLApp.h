#pragma once
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"GL/Mesh.h"
#include<string>
#include<vector>
#include"math/CameraMouseController.h"



#include "imGUI/imgui.h"
#include "imGUI/backends/imgui_impl_glfw.h"
#include "imGUI/backends/imgui_impl_opengl3.h"

class GLApp {
public:
	GLApp();
	void run();
private:
	void init();
	void initOpengl();
	void loop();
	void cleanUp();

	const bool renderGUI(
	float* const lightTranslatex,
	float* const lightTranslatey,
	float* const lightTranslatez,
	float* const lightAngle1,
	float* const lightAngle2,
	float* const lightScale,
	float* const indirectSpan,
	unsigned int debugTextureID
	);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static std::vector<std::function<void(int, int, int)>> m_mouseButtonCallbacks;

	CameraMouseController m_cameraMouseController;
	
	const unsigned int m_windowHeight;
	const unsigned int m_windowWidth;
	GLFWwindow* m_window;

};
