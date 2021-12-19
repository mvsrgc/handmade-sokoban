#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <sys/mman.h>

static bool Running;
static SDL_Texture *Texture;
static void *BitmapMemory;
static int BitmapWidth;
static int BitmapHeight;
static int BytesPerPixel = 4;

static void RenderWeirdGradient(int XOffset, int YOffset) {
    int Pitch = BitmapWidth * BytesPerPixel;
    uint8_t *Row = (uint8_t *) BitmapMemory;

    for (int Y = 0; Y < BitmapHeight; ++Y) {
        uint32_t *Pixel = (uint32_t *) Row;

        for (int X = 0; X < BitmapWidth; ++X) {
            *Pixel = (uint8_t) (X + XOffset);
            ++Pixel;

            *Pixel = (uint8_t) (Y + YOffset);
            ++Pixel;

            *Pixel = 0;
            ++Pixel;

            *Pixel = 0;
            ++Pixel;
        }

        Row += Pitch;
    }

}

static void SDLResizeTexture(SDL_Renderer *Renderer, int Width, int Height) {
    if (BitmapMemory) {
        munmap(BitmapMemory, BitmapWidth * BitmapHeight * BytesPerPixel);
    }
    if (Texture) {
        SDL_DestroyTexture(Texture);
    }
    Texture = SDL_CreateTexture(Renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                Width,
                                Height);
    BitmapWidth = Width;
    BitmapHeight = Height;
    BitmapMemory = mmap(0, Width * Height * BytesPerPixel, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

static void SDLUpdateWindow(SDL_Window *Window, SDL_Renderer *Renderer) {
    SDL_UpdateTexture(Texture, 0, BitmapMemory, BitmapWidth * BytesPerPixel);

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
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
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

    SDL_GetWindowSize(Window, &Width, &Height);
    SDLResizeTexture(Renderer, Width, Height);

    Running = true;
    int XOffset = 0;
    int YOffset = 0;
    while (Running) {
        SDL_Event Event;
        SDL_WaitEvent(&Event);
        while (SDL_PollEvent(&Event)) {
            HandleEvent(&Event);
        }
        RenderWeirdGradient(XOffset, YOffset);
        SDLUpdateWindow(Window, Renderer);
        ++XOffset;
        YOffset += 2;
    }

    SDL_Quit();

    return 0;
}
