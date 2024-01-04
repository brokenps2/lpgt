#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

SDL_Window* window;

void initWindow() {
  window = SDL_CreateWindow("chisel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

  //glewInit();

  glewInit();

  glewExperimental = GL_TRUE;

  SDL_GL_CreateContext(window);

  

const GLenum err = glewInit();

if (GLEW_OK != err)
{
    std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    exit(1);
}

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  glViewport(0, 0, 640, 480);
}

void updateWindow() {
  SDL_UpdateWindowSurface(window);
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0, 0, 0, 0);
}

SDL_Window* getWindow() {
  return window;
} 
