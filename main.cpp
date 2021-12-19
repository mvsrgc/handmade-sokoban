#include <iostream>
#include <SDL2/SDL.h>

static bool Running;

void HandleEvent(SDL_Event *Event)
{
    switch(Event->type)
    {
        case SDL_QUIT:
        {
            Running = false;
        } break;
        case SDL_WINDOWEVENT:
        {
            switch(Event->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", Event->window.data1, Event->window.data2);
                } break;
                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                    static bool IsWhite = true;

                    if (IsWhite == true)
                    {
                        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
                        IsWhite = false;
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
                        IsWhite = true;
                    }

                    SDL_RenderClear(Renderer);
                    SDL_RenderPresent(Renderer);
                } break;
            }
        } break;
    }
}

int main() {
    // Initialize SDL with only the video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: SDL didn't work!
    }

    SDL_Window *Window;

    Window = SDL_CreateWindow("Handmade Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE);

    SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

    Running = true;
    while(Running)
    {
        SDL_Event Event;
        SDL_WaitEvent(&Event);
        HandleEvent(&Event);
    }

    SDL_Quit();

    return 0;
}
