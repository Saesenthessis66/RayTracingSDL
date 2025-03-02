#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "camera.h"
#include "scene.h"

#include <stdlib.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint8 color = 0;

static SDL_Surface* surface = NULL;
static Uint32* pixels = NULL;
static const SDL_PixelFormatDetails *formatDetails;

static Camera camera;
static Scene scene;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    surface = SDL_CreateSurface(WINDOW_WIDTH, WINDOW_HEIGHT,SDL_PIXELFORMAT_RGBA32);
    SDL_LockSurface(surface);
    pixels = (Uint32*)surface->pixels;
    formatDetails = SDL_GetPixelFormatDetails(surface->format);
    SDL_UnlockSurface(surface);

    initCamera(&camera, (Vector){0, 0, 1}, (Vector){0, 0, -1}, (Vector){0, 1, 0}, 90.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    initScene(&scene, 5, 5, 5, 5, 5, 5);

    Vector t1, t2, t3;

    t1 = (Vector){-1, -1, -1};
    t2 = (Vector){1, -1, -1};
    t3 = (Vector){0, 1, -1};

    Material tMat = (Material){(SDL_Color){255,0,0,255}, 0.5f, 0.5f};

    addTriangle(&scene,t1, t2, t3, tMat);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        color++;
        color %= 3;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    SDL_LockSurface(surface);

    int x, y;
    float* t = malloc(sizeof(float));
    for(x = 0; x< WINDOW_WIDTH;x++)
    {
        for(y = 0; y< WINDOW_HEIGHT;y++)
        {
            Ray ray = mapPixelToRay(&camera, x, y, WINDOW_WIDTH, WINDOW_HEIGHT);
            if(intersectRayTriangle(ray, scene.objects.triangles[0], t))
            {
                pixels[y * WINDOW_WIDTH + x] = SDL_MapRGBA(formatDetails, NULL, 
                    scene.objects.triangles[0].material.color.r, scene.objects.triangles[0].material.color.g,
                    scene.objects.triangles[0].material.color.b, scene.objects.triangles[0].material.color.a); //set pixels to triangle color
            }
            else
            {
                pixels[y * WINDOW_WIDTH + x] = SDL_MapRGBA(formatDetails, NULL, 255, 255, 255, 255); //set pixels to white
            }
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

