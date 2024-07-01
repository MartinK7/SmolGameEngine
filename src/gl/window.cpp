
#include "glad.h"
#include <cstdio>

#include "sge/gl/window.hpp"

namespace GL {
	void Window::create(uint32_t width, uint32_t height, const char *title, int msaaSamples) {
		width = width;
		height = height;

		if (!glfwInit())
			exit(1);

#ifndef CONFIG_OPENGL_ES2
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif // CONFIG_OPENGL_ES2
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		if(msaaSamples) {
			glfwWindowHint(GLFW_SAMPLES, msaaSamples);
		}

		window_ptr = glfwCreateWindow(width, height, title, NULL, NULL);

		if (window_ptr == NULL) {
			glfwTerminate();
			exit(1);
		}

		glfwMakeContextCurrent(window_ptr);
		glfwSetWindowUserPointer(window_ptr, this);

		// Init OpenGL
#ifndef CONFIG_OPENGL_ES2
		gladLoadGL();
#else
		gladLoadGLES1Loader((GLADloadproc)glfwGetProcAddress);
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
#endif // CONFIG_OPENGL_ES2

		lastTime = (float)glfwGetTime();

		glfwSetCursorPos(this->window_ptr, width / 2, height / 2);

		double curX, curY;
		glfwGetCursorPos(this->window_ptr, &curX, &curY);
		lastMousePosition = glm::vec2((float)curX, (float)curY);

		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::swapBuffer() {
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapInterval(1);
		glfwSwapBuffers(window_ptr);

		// Time Calculator
		currentTime = (float)glfwGetTime();
		frameCount++;
		deltaTime = currentTime - lastTime;
	}

	bool Window::shouldExit() {
		glfwPollEvents();
		return glfwWindowShouldClose(window_ptr);
	}

	glm::uvec2 Window::getSize() {
		int width, height;
		glfwGetWindowSize(window_ptr, &width, &height);
		return glm::uvec2(width, height);
	}

	void Window::setTitle(const char *title) {
		glfwSetWindowTitle(window_ptr, title);
	}

	void Window::setTitleFPS(const char *title) {

		// FPS Calculator
		if(deltaTime >= 0.1f) {
			char txt[64];
			snprintf(txt, 64, "%s [%.2f FPS]", title, (float)frameCount / (currentTime-lastTime));
			setTitle(txt);
			frameCount = 0;
			lastTime = currentTime;
		}
	}

	Window::~Window() {
		if(window_ptr) {
			glfwDestroyWindow(window_ptr);
		}
	}

	void Window::bind() {
		glfwMakeContextCurrent(window_ptr);
	}

	float Window::getTimeSeconds() {
		bind();
		return (float)glfwGetTime();
	}

	void Window::clear(glm::vec3 color) {
		bind();
		glClearColor(color.x, color.y, color.z, 1.0f);
	}

	float Window::getRatio() {
		glm::vec2 size = getSize();
		return (float)size.x / (float)size.y;
	}

	void Window::setViewport() {
		glm::uvec2 size = getSize();
		//
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//
		glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
		//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	glm::vec2 Window::getMouseDelta() {
		double curX, curY;
		glfwGetCursorPos(this->window_ptr, &curX, &curY);
		glm::vec2 now = glm::vec2((float)curX, (float)curY);
		glm::vec2 delta = lastMousePosition - now;
		lastMousePosition = now;

		glm::vec2 size = getSize();
		glfwSetCursorPos(this->window_ptr, size.x / 2, size.y / 2);

		return delta;
	}

	bool Window::isKeyPressed(int key) {
		return glfwGetKey(window_ptr, key) == GLFW_PRESS;
	}

	float Window::getTimeDelta() {
		return deltaTime;
	}
}
