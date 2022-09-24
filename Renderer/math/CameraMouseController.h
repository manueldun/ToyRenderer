#pragma once
#include"GLFW/glfw3.h"
#include<glm/glm.hpp>
#include<functional>
class CameraMouseController {
public:
	CameraMouseController()=default;
	CameraMouseController(GLFWwindow* window);
	glm::mat4 control(const float& deltaTime, const float& xpos, const float& ypos,const bool& screenFocus);
	std::function<void(int button, int action, int mods)> getMouseCallBack();
	glm::vec3 m_cameraPosition;
private:
	glm::vec2 m_currentMousePosition;
	glm::vec2 m_deltaMousePosition;
	glm::vec2 m_dragOrigin;
	GLFWwindow* m_window;
};