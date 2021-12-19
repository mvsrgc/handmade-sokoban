#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>

static bool Running;
static SDL_Texture *Texture;
static void *Pixels;
static int TextureWidth;

static void SDLResizeTexture(SDL_Renderer *Renderer, int Width, int Height) {
    if (Pixels) {
        free(Pixels);
    }
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
    Texture = SDL_CreateTexture(Renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                Width,
                                Height);
    TextureWidth = Width;
    Pixels = malloc(Width * Height * 4);
}

static void SDLUpdateWindow(SDL_Window *Window, SDL_Renderer *Renderer) {
    SDL_UpdateTexture(Texture, 0, Pixels, TextureWidth * 4);

    SDL_RenderCopy(Renderer, Texture, 0, 0);

    SDL_RenderPresent(Renderer);
}

void HandleEvent(SDL_Event *Event) {
    switch (Event->type) {
        case SDL_QUIT: {
            Running = false;
        }
            break;
        case SDL_WINDOWEVENT: {
            switch (Event->window.event) {
                case SDL_WINDOWEVENT_RESIZED: {
                    SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);
                    printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", Event->window.data1, Event->window.data2);
                    SDLResizeTexture(Renderer, Event->window.data1, Event->window.data2);
                }
                    break;
                case SDL_WINDOWEVENT_EXPOSED: {
                    SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
                    SDL_Renderer *Renderer = SDL_GetRenderer(Window);

                    SDLUpdateWindow(Window, Renderer);
                }
                    break;
            }
        }
            break;
    }
}

int main() {
    // Initialize SDL with only the video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: SDL didn't work!
    }

    SDL_Window *Window;

    int Width = 640;
    int Height = 480;
    Window = SDL_CreateWindow("Handmade Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              Width, Height,
                              SDL_WINDOW_RESIZABLE);

    SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

    SDL_Texture *Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING, Width,
                                             Height);

    Running = true;
    while (Running) {
        SDL_Event Event;
        SDL_WaitEvent(&Event);
        HandleEvent(&Event);
    }

    SDL_Quit();

    return 0;
}
