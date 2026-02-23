#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
	unsigned int width;
	unsigned int height;

public:
	Window(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}
	~Window() = default;

	GLFWwindow* InitWindow(); 
};

#endif