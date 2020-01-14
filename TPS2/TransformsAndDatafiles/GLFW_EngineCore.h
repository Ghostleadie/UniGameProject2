#pragma once
#include "IEngineCore.h"
#include <vector>
#include "CameraComponent.h"

//#include "Model.h"

class Game;

class Model;

class GLFW_EngineCore : public IEngineCore
{
public:
	~GLFW_EngineCore() override;

	bool initWindow(int width, int height, std::string windowName) override;
	bool runEngine(Game& game) override;
	void renderColouredBackground(float r, float g, float b) override;

	void setCamera(const CameraComponent* cam) override;
	void drawCube(const glm::mat4& modelMatrix) override;

	void drawModel(Model* model, glm::mat4& modelMatrix) override;

	void setDefaultShaders() override;
	//void setParticleShader(const CameraComponent* cam) override;
private:
	GLuint TextureID;
	GLuint ViewProjMatrixID;
	GLuint CameraUp_worldspace_ID;
	GLuint CameraRight_worldspace_ID;

	GLFWwindow* m_window;

	GLuint loadParticleShaders(const char* vertexShader, const char* fragmentShader);
	static int m_screenWidth;
	static int m_screenHeight;
	static std::vector<bool> m_keyBuffer;
	static const int m_keyBufferSize = 400;

	static void mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos);
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);

	void initCubeModel();


};

