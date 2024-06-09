
#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace GL {
	class Window {
		GLFWwindow *window_ptr = nullptr;
		float lastTime = 0.0f;
		float deltaTime = 0.0f;
		float currentTime = 0.0f;
		int frameCount = 0;
		glm::vec2 lastMousePosition;
	public:
		~Window();
		void setTitle(const char *title);
		void setTitleFPS(const char *title = "");
		void create(uint32_t width, uint32_t height, const char *title = "", int msaaSamples = 4);
		void swapBuffer();
		bool shouldExit();
		glm::uvec2 getSize();
		float getRatio();
		void bind();
		float getTimeSeconds();
		void setViewport();
		glm::vec2 getMouseDelta();
		bool isKeyPressed(int key);
		float getTimeDelta();


		void clear(glm::vec3 color);
	};
}

#endif //GL_WINDOW_HPP
