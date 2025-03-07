#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "render_functions.h"
#include "illumination.h"

#include <stdlib.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Surface* surface = NULL;
static Uint32* pixels = NULL;
static const SDL_PixelFormatDetails *formatDetails;

static Camera camera;
static Scene scene;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Raytracing in SDL3", "0.1", "");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("RayTracing", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    surface = SDL_CreateSurface(WINDOW_WIDTH, WINDOW_HEIGHT,SDL_PIXELFORMAT_RGBA32);
    SDL_LockSurface(surface);
    pixels = (Uint32*)surface->pixels;
    formatDetails = SDL_GetPixelFormatDetails(surface->format);
    SDL_UnlockSurface(surface);

    initialize_scene(WINDOW_WIDTH, WINDOW_HEIGHT, &camera, &scene);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* End the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) { 
            case SDLK_W:
                /* Move forward */
                moveCameraForward(&camera);
                break;
            case SDLK_S:
                /* Move backward */
                moveCameraBackward(&camera);
                break;
            case SDLK_A:
                /* Move left */
                moveCameraLeft(&camera);
                break;
            case SDLK_D:
                /* Move right */
                moveCameraRight(&camera);
                break;
            case SDLK_R:
                moveCameraUp(&camera);
                break;
            case SDLK_F:
                moveCameraDown(&camera);
                break;
            case SDLK_Q:
                rotateCameraRight(&camera);
                break;
            case SDLK_E:
                rotateCameraLeft(&camera);
                break;
            case SDLK_O:
                rotateCameraUp(&camera);
                break;
            case SDLK_P:
                rotateCameraDown(&camera);
                break;
            default:
                break;
        }
    }

    return SDL_APP_CONTINUE;  /* Carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    SDL_LockSurface(surface);

    int x, y;
    SDL_Color pixelColor;
    for(x = 0; x< WINDOW_WIDTH;x++)
    {
        for(y = 0; y< WINDOW_HEIGHT;y++)
        {
            pixelColor = computePixelColor(WINDOW_WIDTH, WINDOW_HEIGHT, x, y, &camera, &scene);
            pixels[y * WINDOW_WIDTH + x] = SDL_MapRGBA(formatDetails, NULL, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a); 
        }
    }

    SDL_UnlockSurface(surface);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderTexture(renderer, texture, NULL, NULL);

    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    SDL_DestroySurface(surface);
}

