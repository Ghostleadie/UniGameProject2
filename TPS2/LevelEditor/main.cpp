#define USE_GLFW 1
#ifdef USE_GLFW
	#include "GLFW_Imgui_EngineCore.h"
#endif


#include "LevelEditor.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;



#ifdef USE_GLFW
	engineCore = new GLFW_Imgui_EngineCore;
#endif

	if (!engineCore->initWindow(800, 600, "LevelEditor"))
		return -1;

	LevelEditor levelEditor;

	engineCore->runEngine(levelEditor);

	return 0;
}
