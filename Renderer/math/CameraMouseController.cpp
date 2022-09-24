#include"CameraMouseController.h"

#include<glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

CameraMouseController::CameraMouseController(GLFWwindow* window) :
	m_cameraPosition(glm::vec3(0.0f, 0.0f, 0.0f)), m_window(window),
	m_currentMousePosition(glm::vec2(0.0f,0.0f)),
	m_deltaMousePosition(glm::vec2(0.0f,0.0f)),
	m_dragOrigin(glm::vec2(0.0f,0.0f))
{
}

glm::mat4 CameraMouseController::control(const float& deltaTime, const float& xpos, const float& ypos, const bool& screenFocus)
{


	m_currentMousePosition = glm::vec2(xpos, ypos);
	if (screenFocus)
	{

		int state = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			m_deltaMousePosition = m_currentMousePosition - m_dragOrigin;

		}

	}
	glm::vec2 dragMotion = m_deltaMousePosition;

	glm::mat4 rotatey = glm::rotate(glm::mat4(1.0f), -dragMotion.x * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateyx = glm::rotate(rotatey, dragMotion.y * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 cameraForward = rotateyx * (glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraLeft = glm::normalize(glm::cross(up, cameraForward));
	glm::vec3 cameraUp = glm::cross(cameraForward, cameraLeft);

	glm::mat4 cameraOrientationMatrix = glm::mat4(
		glm::vec4(cameraLeft, 0.0f),
		glm::vec4(cameraUp, 0.0f),
		glm::vec4(cameraForward, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	glm::vec3 moveDir;
	int isKeyWPressed = glfwGetKey(m_window, GLFW_KEY_W);
	int isKeySPressed = glfwGetKey(m_window, GLFW_KEY_S);
	if (isKeyWPressed == GLFW_PRESS)
	{
		moveDir.x = 1.0f;
	}
	else if (isKeySPressed == GLFW_PRESS)
	{
		moveDir.x = -1.0f;
	}
	else
	{
		moveDir.x = 0.0f;
	}
	int isKeyAPressed = glfwGetKey(m_window, GLFW_KEY_A);
	int isKeyDPressed = glfwGetKey(m_window, GLFW_KEY_D);
	if (isKeyAPressed == GLFW_PRESS)
	{
		moveDir.y = 1.0f;
	}
	else if (isKeyDPressed == GLFW_PRESS)
	{
		moveDir.y = -1.0f;
	}
	else
	{
		moveDir.y = 0.0f;
	}
	glm::vec3 deltaPosition = ((cameraForward * moveDir.x) + (cameraLeft * moveDir.y)) * deltaTime * 1000.0f;
	m_cameraPosition = m_cameraPosition + deltaPosition;
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_cameraPosition));
	return glm::transpose(cameraOrientationMatrix) * translation;
}

std::function<void(int, int, int)> CameraMouseController::getMouseCallBack()
{

	return [=](int button, int action, int mods) {
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_dragOrigin = m_currentMousePosition - m_deltaMousePosition;

		}
	};
}

