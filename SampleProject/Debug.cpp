//#include <cstdio>
//#include <cstdlib>
//
//#define GLM_FORCE_RADIANS 1
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <SDL2/SDL.h>
//
//#include "glad.h"
//
//static const int SCREEN_FULLSCREEN = 1;
//static const int SCREEN_WIDTH = 960;
//static const int SCREEN_HEIGHT = 540;
//static SDL_Window *window = NULL;
//static SDL_GLContext maincontext;
//
//static void sdl_die(const char * message) {
//	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
//	exit(2);
//}
//
//void init_screen(const char * caption) {
//	// Initialize SDL 
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//		sdl_die("Couldn't initialize SDL");
//	atexit(SDL_Quit);
//	SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.
//
//							  // Request an OpenGL 4.3 context (should be core)
//	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
//	// Also request a depth buffer
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//	// Create the window
//	if (SCREEN_FULLSCREEN) {
//		window = SDL_CreateWindow(
//			caption,
//			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//			0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
//		);
//	}
//	else {
//		window = SDL_CreateWindow(
//			caption,
//			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL
//		);
//	}
//	if (window == NULL) sdl_die("Couldn't set video mode");
//
//	maincontext = SDL_GL_CreateContext(window);
//	if (maincontext == NULL)
//		sdl_die("Failed to create OpenGL context");
//
//	// Check OpenGL properties
//	printf("OpenGL loaded\n");
//	gladLoadGLLoader(SDL_GL_GetProcAddress);
//	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
//	printf("Renderer: %s\n", glGetString(GL_RENDERER));
//	printf("Version:  %s\n", glGetString(GL_VERSION));
//
//	// Use v-sync
//	SDL_GL_SetSwapInterval(1);
//
//	// Disable depth test and face culling.
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//
//	int w, h;
//	SDL_GetWindowSize(window, &w, &h);
//	glViewport(0, 0, w, h);
//	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
//}
//
//
//int main(int argc, char** argv) {
//	init_screen("OpenGL 4.3");
//	SDL_Event event;
//	bool quit = false;
//	while (!quit) {
//		SDL_GL_SwapWindow(window);
//		while (SDL_PollEvent(&event)) {
//			if (event.type == SDL_QUIT) {
//				quit = true;
//			}
//		}
//	}
//	return 0;
//}
//
//
