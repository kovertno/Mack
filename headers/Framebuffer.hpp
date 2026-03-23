#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer {
private:
	unsigned int fbo;
	unsigned int rbo;
	unsigned int textureAttachment;
	void CreateFramebuffer();
public:
	Framebuffer() {
		CreateFramebuffer();
	}
	~Framebuffer() {
		glDeleteFramebuffers(1, &fbo);
		glDeleteRenderbuffers(1, &rbo);
		glDeleteTextures(1, &textureAttachment);
	}

	void Bind() const;
	void Unbind() const;
	unsigned int GetTexture() const;
};

#endif