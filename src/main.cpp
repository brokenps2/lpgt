#include <SDL2/SDL.h>
#include "windowmgr.h"


int main(int argc, char* argv[]) {

  initWindow();

  while(1) {
    
    SDL_Event quitEvent;
    if(SDL_PollEvent(&quitEvent) && quitEvent.type == SDL_QUIT) break;

    updateWindow();


  }



  return 0;

}
