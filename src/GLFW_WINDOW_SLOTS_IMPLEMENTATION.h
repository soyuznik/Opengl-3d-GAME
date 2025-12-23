#pragma once
#include "GLOBALS.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	
	// if X is pressed move 5x faster
	double speed = 1.0;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		speed = 5;

	// if 1 has been pressesd display M4 insdead of deagle
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		display_gun = true;
		display_deagle = false;
	}
	// else if 2 dispaly deagle
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		display_gun = false;
		display_deagle = true;
	}
	// check mouse buttons LMB shoot and RMB -> ADS
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) { ADS = true; }
	else { ADS = false; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) { Shooting = true; }
	else {
		shoot_cooldown = false;
		Shooting = false;
	}

	// if escape has been pressed quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// if space pressed jump.
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		should_jump = true;

	//  CHECK for wasd MOVEMENT
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime * speed);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}