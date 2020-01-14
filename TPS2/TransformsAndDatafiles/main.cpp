#define USE_GLFW 1
#ifdef USE_GLFW
	#include "GLFW_EngineCore.h"
#endif
#include "irrKlang.h"

using namespace irrklang;

#include "MyGame.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

	/* Setup window
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_window, true);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		//start of while loop
		ImGui_ImplGlfwGL3_NewFrame();

		// Show a simple window.
		ImGui::Begin("Main");
		
		ImGui::End();

		//end of while loop
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	*/

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(800, 600, "TransformAndData"))
		return -1;
	
	MyGame game;

	engineCore->runEngine(game);	

	return 0;
}
