#define STB_IMAGE_IMPLEMENTATION
#include "MineSweeper.h"
#include <chrono>
[[noreturn]] static void raise_error(const char* error_text)
{
	std::cerr << '\n' << error_text << '\n';
	system("pause");
	exit(1);
}

//MineSweeper global variable
static MineSweeper ms{};

static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	ms.processInput(key, action);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ms.processMouse(xpos, ypos, button, action);
}

static GLFWwindow* InitGL()
{
	if (!glfwInit())
		raise_error("GLFW was not initialized");

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(ms.RESOLUTION.x, ms.RESOLUTION.y, ms.TITLE, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		raise_error("GLFW window initialization error");
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	auto icon = ms.loadIcon();
	glfwSetWindowIcon(window, 1, &icon);
	stbi_image_free(icon.pixels);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		raise_error("Failed to initialize GLAD");

	return window;
}

int main()
{
	auto window = InitGL();

	//Initialize mine sweeper
	ms.init();
	while (!glfwWindowShouldClose(window))
	{
		ms.render();
		ms.update();

		if (GLenum err = glGetError(); err != GL_NO_ERROR)
			std::cerr << "OpenGL error: " << err << std::endl;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 1;
}