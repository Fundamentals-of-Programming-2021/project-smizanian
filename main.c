#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

//////////////////////////////////////////////// Start Project //////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <string.h>




//constant variables
const int FPS = 60;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_NUM = 8;
const int HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
const int HEXAGON_SIDE = (HEXAGON_h/(2*0.866025))/1 + 1;
int HEXAGON_COUNT = 85;
//const int HEXAGON_COUNT = ((SCREEN_WIDTH/(3*HEXAGON_SIDE))*(CELL_NUM + CELL_NUM - 1) + CELL_NUM);




// structs
typedef struct my_map_hexagonal{
    int color_index;
    int soldier;
} hexagonal;




// functions
int mod(int a, int b) {
    return a % b;
}

void drawCircle(SDL_Renderer *sdlRenderer, int x_int, int y_int, Uint32 color) {
    Sint16 X = (Sint16)x_int;
    Sint16 Y = (Sint16)y_int;
    Sint16 radius = HEXAGON_SIDE/3;
    filledCircleColor(sdlRenderer, X, Y, radius, color);
}




// important function prototypes

// map
void read_file(FILE* file_ptr, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT], Uint32 map_colors[10]);
void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color);
void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT]);

// images and menu
void imageLoader(SDL_Renderer *sdlRenderer, char image_name[100], int X, int Y, int W, int H);


int main() {

    // SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // welcoming the new player
    /*
    char username[1000]  = {0};
    char trash = 0; // for \n
    printf ("Please enter your username:");
    scanf (" %[^\n]s%c", &username);
    printf ("%s, If you want to play in a random map, write -1. Else, type number of your chosen map (between 1 to 10):", username);
    int map_number = 0;
    scanf ("%d", &map_number);
    while (map_number != -1 && (map_number<1 || map_number>10)) {
        printf ("Please enter a valid map number:");
        scanf ("%d", &map_number);
    }
    printf ("Your selected map number is : %d\n", map_number);
    */



    // reading a map
    FILE* file_ptr;
    file_ptr = fopen("map.txt", "r");
    if (file_ptr == NULL) {
        printf ("Error opening file. \n");
    }
    fscanf(file_ptr, "%d\n", &HEXAGON_COUNT);

    hexagonal map[HEXAGON_COUNT];


    // colors
    Uint32 map_colors[10] = {0xffff00ff, 0xff800000, 0xff0000ff, 0xff800080, 0xff808000, 0xff008080, 0xffff00ff, 0x80ff80ff, 0xffc0c0c0, 0xffffff00};
    Uint32 hexagonal_colors[HEXAGON_COUNT];

    // read file function
    read_file(file_ptr, map, hexagonal_colors,map_colors);

    // mouse place variables
    int mouse_X = 0;
    int mouse_Y = 0;

    // image variables
    char image_name[100] = {"image.bmp"};

    // window
    SDL_Window *sdlWindow = SDL_CreateWindow("myState.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // renderer
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {

        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlRenderer);

        // drawing the map
        drawHexagonalMap(sdlRenderer, map, hexagonal_colors);


        // rendering
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);

        // event handling
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                        SDL_GetMouseState(&mouse_X, &mouse_Y);
                        printf("Left click on X: %d and Y:%d\n", mouse_X / (SCREEN_WIDTH / CELL_NUM) + 1 , mouse_Y / (SCREEN_HEIGHT / CELL_NUM) + 1);
                    }
                    break;
            }
        }
    }

    fclose(file_ptr);

    SDL_Delay(100);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    printf("\nHello World\n");

    SDL_Quit();

    return 0;
}












// functions

// map
void read_file(FILE* file_ptr, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT], Uint32 map_colors[10]) {

    int color_index;
    int soldier;
    Uint32 color;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        fscanf(file_ptr, "%d %d\n", &color_index, &soldier);

        map[i].color_index = color_index;
        map[i].soldier = soldier;
        hexagonal_colors[i] = map_colors[map[i].color_index];
    }

    return;

}

void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color) {
    Sint16 X[6] = {(Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter+HEXAGON_SIDE), (Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE), (Sint16)(xcounter-HEXAGON_SIDE/2)};
    Sint16 Y[6] = {(Sint16)(ycounter+HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter+HEXAGON_SIDE*0.866025)};
    if (hexagon_color != 0xffffff00) {
        filledPolygonColor(sdlRenderer, X, Y, 6, hexagon_color - 0x4f000000);
        drawCircle(sdlRenderer, xcounter, ycounter, hexagon_color);
    }
    else {
        filledPolygonColor(sdlRenderer, X, Y, 6, hexagon_color);
    }

    //X[0]+=1; X[1]+=1; X[2]+=1; X[3]-=1; X[4]-=1; X[5]-=1;
    //Y[0]+=1; Y[2]-=1; Y[3]-=1; Y[4]+=1;
    //polygonColor(sdlRenderer, X, Y, 6, hexagon_color);
}

void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT]) {
    int xcounter = 0;
    int ycounter = 0;
    int hexagon_counter = 0;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            drawHexagon(sdlRenderer, xcounter, ycounter, hexagonal_colors[hexagon_counter]);
            //printf ("X is: %d , Y is: %d\n", xcounter, ycounter);
            hexagon_counter++;
            ycounter = ycounter + HEXAGON_h;
        }
        ycounter = 0;
        xcounter = xcounter + 3*HEXAGON_SIDE;
    }

    xcounter = HEXAGON_SIDE + HEXAGON_SIDE/2;
    ycounter = HEXAGON_h/2;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            drawHexagon(sdlRenderer, xcounter, ycounter, hexagonal_colors[hexagon_counter]);
            //printf("X is: %d , Y is: %d\n", xcounter, ycounter);
            hexagon_counter++;
            ycounter = ycounter + HEXAGON_h;
        }

        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }

    //printf ("Hexagons = %d\n", hexagon_counter);
}

// images and menu
void imageLoader(SDL_Renderer *sdlRenderer, char image_name[100], int X, int Y, int W, int H) {
    SDL_Surface * image = SDL_LoadBMP(image_name);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(sdlRenderer, image);
    SDL_Rect dstrect = { X, Y, W, H };
    SDL_RenderCopy(sdlRenderer, texture, NULL, &dstrect);

    /*
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
     */

    return;
}



