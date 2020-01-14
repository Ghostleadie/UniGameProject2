#include "GLFW_Imgui_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw_gl3.h"
#include "LevelEditor.h"

std::vector<bool> GLFW_Imgui_EngineCore::m_keyBuffer;
int GLFW_Imgui_EngineCore::m_screenWidth;
int GLFW_Imgui_EngineCore::m_screenHeight;

GLFW_Imgui_EngineCore::~GLFW_Imgui_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_Imgui_EngineCore::initWindow(int width, int height, std::string windowName)
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
	std::cout << "Failed to create GLFW m_window" << std::endl;
	glfwTerminate();
	return false;
	}

	// set up ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_window, true);

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	//initCubeModel();

	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// enable depth test
glEnable(GL_DEPTH_TEST);


return true;
}


bool GLFW_Imgui_EngineCore::runEngine(Game& game)
{
	game.m_engineInterfacePtr = this;
	double xPos, yPos;
	int windowX, windowY;

	//glfwSetCursorPosCallback(m_window, game.m_inputHandler.handleMouseInput);

	float deltaTime = 1.0f / 60.0f;
	double lastTime = glfwGetTime();
	double currentTime;

	const char* GameObjectTypesName[] = { "StaticGameObject" };
	const char* GameObjectMeshesName[] = { "floor", "hanger", "teapot", "grass" };
	static int listboxGameobjectCurrentIndex = -1;
	static const char* currentItem = NULL;
	bool showLoadWindow = false;
	bool showSaveWindow = false;
	bool showNewLevelWindow = false;

	// message loop
	while (!glfwWindowShouldClose(m_window))
	{
		currentTime = glfwGetTime();
		deltaTime = (float)(currentTime - lastTime);
		lastTime = currentTime;

		//std::cout << deltaTime << std::endl;
		glfwGetWindowSize(m_window, &windowX, &windowY);
		glfwGetCursorPos(m_window, &xPos, &yPos);
		game.m_inputHandler.handleMouseInput(xPos, yPos, windowX, windowY);
		//glfwSetCursorPos(m_window,windowX/2, windowY/2);
		game.m_inputHandler.handleInputs(m_keyBuffer);

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::ShowDemoWindow();

		int windowWidth, windowHeight;
		glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);

		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Load Level", NULL, &showLoadWindow);
			ImGui::MenuItem("Save Level", NULL, &showSaveWindow);
			ImGui::MenuItem("New Level", NULL, &showNewLevelWindow);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		/*if (showNewLevelWindow) {
			game.m_sceneObject->getComponent<SceneComponent>()
		}*/
		//	OBJECT SELECT WINDOW //
		ImGui::Begin("Main Window");
		ImGui::SetWindowPos(ImVec2(0, 20));
		ImGui::SetWindowSize(ImVec2(windowWidth*0.4, windowHeight*0.25));

		static float viewportSize = 0.75f;
		ImGui::SliderFloat("viewportSize", &viewportSize, 0.25, 1.0);

		static int currentSelectedGameObjectType = 0;
		static int currentSelectedGameObjectMesh = 0;
		ImGui::Combo("GameObject Types", &currentSelectedGameObjectType, GameObjectTypesName, IM_ARRAYSIZE(GameObjectTypesName));
		ImGui::Combo("GameObject Meshes", &currentSelectedGameObjectMesh, GameObjectMeshesName, IM_ARRAYSIZE(GameObjectMeshesName));

		//game.m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects;

		if (ImGui::Button("Add Object")) {
			//Buttons return true when clicked
			static_cast<LevelEditor*>(&game)->addModels(
				GameObjectMeshesName[currentSelectedGameObjectMesh],
				GameObjectTypesName[currentSelectedGameObjectType], game.m_sceneObject->getComponent<SceneComponent>());
		}

		ImGui::End();

		//	OBJECT LIST WINDOW //
		ImGui::Begin("Scene Objects");
		ImGui::SetWindowPos(ImVec2(0, windowHeight*0.30));
		ImGui::SetWindowSize(ImVec2(windowWidth*0.4, windowHeight*0.65));
		
		// a lambda function for obtaining the names of the gameobjects
		static auto getGameObjectNameFunc = [](void* vec, int idx, const char**
			out_text)
		{
			// need a cast as it is passed in as void* - a bit messy
			std::vector<GameObject*>* vector = static_cast<std::vector<GameObject*>*>(vec);
			// only get the name if it’s a valid index
			if (idx < 0 || idx >= vector->size())
				return false;
			*out_text = vector->at(idx)->getName();
			return true;
		};


		std::cout << listboxGameobjectCurrentIndex << endl;

		ImGui::End();

		//	PROPERTIES WINDOW //
		ImGui::Begin("Object Properties");
		ImGui::SetWindowPos(ImVec2(windowWidth*0.4, windowHeight*0.70));
		ImGui::SetWindowSize(ImVec2(windowWidth*0.6, windowHeight*0.20));

		ImGui::End();

		glViewport(windowWidth - windowWidth*viewportSize, windowHeight - windowHeight*viewportSize, windowWidth*viewportSize, windowHeight*viewportSize);

		game.update(deltaTime); // update game logic
		game.render(); // prepare game to send info to the renderer in engine core

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

	}
	return true;
}

void GLFW_Imgui_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------Private functions------------------------------
void GLFW_Imgui_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void GLFW_Imgui_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

// loading some default shaders to get things up and running
void GLFW_Imgui_EngineCore::setDefaultShaders()
{
	// Load contents of vertex file
	std::ifstream inFile("assets/shaders/defaultShader.vert");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2("assets/shaders/defaultShader.frag");
	if (!inFile2) {
		fprintf(stderr, "Error opening file: shader\n");
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };
	
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);	
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	m_defaultShaderProgram = glCreateProgram();
	glAttachShader(m_defaultShaderProgram, vertexShader);
	glAttachShader(m_defaultShaderProgram, fragmentShader);
	glLinkProgram(m_defaultShaderProgram);
	// check for linking errors
	glGetProgramiv(m_defaultShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_defaultShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(m_defaultShaderProgram);
}

// a simple function to initialise a cube model in memory
void GLFW_Imgui_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
}

void GLFW_Imgui_EngineCore::setCamera(const CameraComponent* cam)
{
	// set the view and projection components of our shader to the camera values
	glm::mat4 projection = glm::perspective(glm::radians(cam->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(cam->position()));
	
}

void GLFW_Imgui_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLFW_Imgui_EngineCore::drawModel(Model* model, glm::mat4& modelMatrix)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	model->render(m_defaultShaderProgram);

}