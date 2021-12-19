#include <iostream>
#include <SDL2/SDL.h>

bool HandleEvent(SDL_Event *Event)
{
    bool ShouldQuit = false;

    switch(Event->type)
    {
        case SDL_QUIT:
        {
            printf("SQL_QUIT\n");
            ShouldQuit = true;
        } break;
        case SDL_WINDOWEVENT:
        {
            switch(Event->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", Event->window.data1, Event->window.data2);
                } break;
            }
        } break;
    }

    return(ShouldQuit);
}

int main() {
    // Initialize SDL with only the video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: SDL didn't work!
    }

    SDL_Window *Window;

    Window = SDL_CreateWindow("Handmade Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE);

    for (;;)
    {
        SDL_Event Event;
        SDL_WaitEvent(&Event);
        if (HandleEvent(&Event)) {
            break;
        }
    }

    SDL_Quit();

    return 0;
}
