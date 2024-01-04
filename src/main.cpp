#include <SDL2/SDL.h>
#include "windowmgr.h"
#include <SDL2/SDL_video.h>
#include "renderer.h"

int main(int argc, char* argv[]) {

  SDL_Init(SDL_INIT_EVERYTHING);

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
