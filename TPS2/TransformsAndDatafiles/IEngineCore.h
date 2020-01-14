#pragma once
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class CameraComponent;
class Game;
class Model;

class IEngineCore
{
public:
	GLuint m_defaultShaderProgram;
	GLuint m_particleShaderProgram;
	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game& game) = 0;

	// some simple drawing tools - will want to move these to a proper renderer class later
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual	void setCamera(const CameraComponent* cam) = 0;
	virtual void drawModel(Model* model, glm::mat4& modelMatrix) = 0;
	virtual void drawCube(const glm::mat4& modelMatrix) = 0;
	virtual void setDefaultShaders() = 0;
//	virtual void setParticleShader(const CameraComponent* cam) = 0;
};
