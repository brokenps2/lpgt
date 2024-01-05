#include <string>
#include <SDL2/SDL.h>
#include "windowmgr.h"
#include <SDL2/SDL_video.h>
#include "renderer.h"
#include "config.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

  SDL_Init(SDL_INIT_EVERYTHING);

  cfgSetPath("master.cfg");

  initWindow();
  initRenderer();

  while(1) {
    
    SDL_Event quitEvent;
    if(SDL_PollEvent(&quitEvent) && quitEvent.type == SDL_QUIT) break;

    updateWindow();
    
    updRenderer();

  }
 
  return 0;

}
