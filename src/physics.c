/*
    - Author:   Gavin Sereg
    - Date:     01/17/2024
*/


/*|------------> Includes <------------|*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/*|------------> Constants <------------|*/

#define TITLE "ball physics simulator"
#define HEIGHT 500
#define WIDTH 1000

#define TOP 0
#define BOTTOM HEIGHT // Bottom floor is the HEIGHT of the screen

#define RIGHT WIDTH // Right wall is the WIDTH of the screen
#define LEFT 0

#define NUMBEROFCIRCLES 30

const SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };
const SDL_Color RED = { 255, 0, 0, SDL_ALPHA_OPAQUE };
const SDL_Color GREEN = { 0, 255, 0, SDL_ALPHA_OPAQUE };
const SDL_Color BLUE = { 0, 0, 255, SDL_ALPHA_OPAQUE };
const SDL_Color WHITE = { 255, 255, 255, SDL_ALPHA_OPAQUE };


/*|------------> Global Variables <------------|*/

SDL_Window* window;
SDL_Renderer* renderer;

/*
    - Struct to hold a single circle with these values:
    int posX -               Postion on x axis
    int posY -               Postion on y axis
    int velX -               Velocity on x axis
    int velY -               Velocity on y axis
    int radius -             Radius of the cirlce
    SDL_color color -        An SDL_Color of the circle
*/
typedef struct Circle {
    int posX;
    int posY;
    int velX;
    int velY;
    int radius;
    SDL_Color color;
}Circle;

/*|------------> Setup Functions <------------|*/

/*
    - Calls:
        SDL_Init
        SDL_GetError
        SDL_CreateWindow
        SDL_Quit
    - Return:
        true if window and renderer creation success, false if failure
*/
bool Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        printf("SDL_Init failed with error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if (window == NULL) {
        printf("Unable to create the main window. Error: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_PRESENTVSYNC
    );

    return true;
}

/*
    - Calls:
        SDL_SetRenderDrawColor
        SDL_RenderClear
    - Return:
        void
*/
static void ClearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(renderer);
}

/*
    - Calls:
        SDL_DestroyWindow
        SDL_DestroyRenderer
        SDL_Quit
    - Return:
        void
*/
void Shutdown() {
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    SDL_Quit();
}


/*|------------> Custom Functions <------------|*/

/*
    - Custom function to get a random color
    - Calls:
        rand
    - Return:
        an SDL_color, color
*/
SDL_Color setRandomColor()
{
    int randNum = rand() % 3 + 1;
    SDL_Color color;

    if (randNum == 1) {color = RED;}
    if (randNum == 2) {color = GREEN;}
    if (randNum == 3) {color = BLUE;}
    
    return color;
}

/*
    - Custom function to create random circles inside the screens dimensions
    - Calls:
        rand
        setRandomColor
    - Returns:
        An array of circles to update the values of the circles array in main
*/
Circle* fillCircles(Circle* circles)
{
    for (int i = 0; i < NUMBEROFCIRCLES; i++) {
        circles[i].posX = (int) (rand() % 901) + 50;
        circles[i].posY = (int) (rand() % 101) + 50;
        circles[i].velX = (int) (rand() % 10 - 5) + 1;
        circles[i].velY = (int) (rand() % 10 - 5) + 1;
        circles[i].radius = (int) ((rand() % 5) * 10) + 10;
        circles[i].color = setRandomColor();
    }

    return circles;
}

/*
    - Custom function to draw a circle to the SDL window
    - Calls:
        SDL_SetRenderDrawColor
        SDL_RenderDrawPoint
    - Return:
        void
*/
void drawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

/*
    - Custom function to draw mutiple circles from an array of structs
    - Calls:
        drawCircle
    - Return:
        void
*/
void drawCircles(SDL_Renderer* renderer, Circle circles[])
{
    for (int i = 0; i < NUMBEROFCIRCLES; i++) {
        drawCircle(renderer, circles[i].posX, circles[i].posY, circles[i].radius, circles[i].color);
    }
}

/*
    - Custom function to detect wall collisions for each ball in the circles array of structs
    - Calls:
        SDL_SetWindowBordered
    - Return:
        an array of data type Circle, circles
*/
Circle* moveCircles(SDL_Renderer* renderer, Circle* circles)
{
    for (int i = 0; i < NUMBEROFCIRCLES; i++) {
        
        if ( (circles[i].posY >= BOTTOM - circles[i].radius) && (circles[i].velY < 1) ) {
            circles[i].posY = BOTTOM - circles[i].radius;
            SDL_SetWindowBordered(window, true);
        }

        if (circles[i].posY < BOTTOM - circles[i].radius) {
            circles[i].posY += circles[i].velY;
            circles[i].velY += 1;
        }

        if ( (circles[i].posY >= BOTTOM - circles[i].radius) && (circles[i].velY > 2) ) {
            circles[i].posY = BOTTOM - circles[i].radius;
            circles[i].velY *= -1;
            circles[i].velY += 5;
            circles[i].posY += circles[i].velY;
        }

        if (circles[i].posY <= TOP + circles[i].radius) {
            circles[i].velY *= -1;
            circles[i].posY += circles[i].velY;
        }

        

    }

    return circles;
}


/*|------------> Main Function <------------|*/

/*
    - Return:
        EXIT_SUCCESS if successful execution
        EXIT_FAILURE if failed execution
*/
int main(int argc, char const *argv[])
{
    ClearScreen(renderer);
    if (Init() == false) { Shutdown(); } // Initialize the window and renderer

    srand((unsigned int) time(NULL)); // Seed rand

    Circle* circles = (Circle*) malloc(sizeof(Circle) * NUMBEROFCIRCLES); // Allocate memory for circles
    circles = fillCircles(circles); // Fill circles array
    
    SDL_Event event; // SDL_Event to determine events during the loop
    
    bool running = true;

    while(running) {
        ClearScreen(renderer); // Clear the screen for new renders
        if (SDL_PollEvent(&event)) { // Poll any SDL events
            switch (event.type) {
                case SDL_KEYDOWN: { running = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;break; }
                case SDL_QUIT: { running = false; break; }
                default: { break; }
            }
        }

        drawCircles(renderer, circles); // Draw all circles to the window
        circles = moveCircles(renderer, circles); // Update the circles array to the new circles positions

        SDL_RenderPresent(renderer); // Update screen
        SDL_Delay(10); // Delay draw time (FPS)
    }

    Shutdown(); // Shutdown SDL2
    return EXIT_SUCCESS; // Exit gracefully
}