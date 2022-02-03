#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

//////////////////////////////////////////////// Start Project //////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "main_menu.h"




//constant variables
const int CELL_NUM = 8;
const int HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
const int HEXAGON_SIDE = (HEXAGON_h/(2*0.866025))/1 + 1;
int HEXAGON_COUNT = 85;
int NUMBER_OF_PLAYERS = 6;
int MAIN_MATRIX_UNITS_COUNT = 10;
//const int HEXAGON_COUNT = ((SCREEN_WIDTH/(3*HEXAGON_SIDE))*(CELL_NUM + CELL_NUM - 1) + CELL_NUM);




// structs
// attacks
typedef struct hexagonal_attacks{
    bool finished;
    int soldiers;
    int id_dis;
    int X_dis;
    int Y_dis;
    struct hexagonal_attacks* next_attack;
} attacks_struct;

// for map
typedef struct my_map_hexagonal{
    int color_index;
    float soldier;
    int X;
    int Y;
    int potion_id;
    int soldier_speed;
    float rate;
    bool is_s;
    bool is_d;
    attacks_struct* attacks;
} hexagonal;

// for scoreboard
typedef struct users_in_score_board{
    char name[1000];
    char rank_in_string[3];
    int score;
    char score_in_string[9];
    struct users_in_score_board* next_user;
}score_board_struct;

// Potions
typedef struct game_Potions{
    int potion_time;
    bool potion_in_use;
    short potion_id;
}potion;

// Soldier
typedef struct game_soldier{
    float X;
    float Y;
    float V_x;
    float V_y;
    int X_d;
    int Y_d;
    int land_id;
}soldier;

// matrix_units
typedef struct matrix_unit{
    bool Potion;
    potion local_potion;
    soldier local_soldiers[8];
}matrix_unit;





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
void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, float soldiers);
void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT]);
// menu
// main menu
int Which_item_selected(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items);
int main_menu(char username[100], SDL_Renderer* sdlMenuRenderer, int letter_counter);
// scoreboard
int user_count_in_scoreboard();
void read_score_board_file(score_board_struct* users_ptr, int users_counter);
void sort_score_board_file(score_board_struct* users_ptr, int user_counter);
void index_to_rank_in_scoreboard(char temp_username_in_scoreboard_menu_rank[3], int index);
int power(int number);
void score_to_score_in_scoreboard(char temp_username_in_scoreboard_menu_score[9], int score);
void fill_strings_for_ranks_and_scores(score_board_struct* users_ptr, int user_counter);
void writing_on_scoreboard(SDL_Renderer * sdlScoreboardRenderer, score_board_struct* users_head_ptr, int user_counter);
int Which_item_selected_in_scoreboard(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items);
int Scoreboard_manu(SDL_Renderer* sdlScoreboardRenderer);
// logic
int Delta_D_calculator(int X1, int Y1, int X2, int Y2);
int X_Y_to_hexagon(int mouse_X, int mouse_Y);
int potion_generator(SDL_Renderer * sdlRenderer, int* X_of_potion, int* Y_of_potion, hexagonal map[HEXAGON_COUNT]);
int AI(int X_s, int Y_s, hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, int myID);
void filling_main_matrix_for_attacks(matrix_unit main_matrix[60][60], hexagonal map[HEXAGON_COUNT], int X_dis, int Y_dis, int i, int j);
void first_initialization_of_main_matrix(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
void soldier_generator_in_attacks(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i);




int main() {

    // SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    printf ("Hello!\n");

    // random set
    srand(time(0));


    // window
    SDL_Window *sdlWindow = SDL_CreateWindow("myState.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // renderer
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);



    // main menu and scoreboard
    // username
    char username[100] = {0};
    int letter_counter = 0;
    for (int i=0; i<100; i++) {
        username[i] = 0;
    }

    int item_selected_in_main_menu = main_menu(username, sdlRenderer, 0);
    if (username[0] == 0) {
        letter_counter = 0;
    }
    else {
        letter_counter = strlen(username);
    }
    int item_selected_in_scoreboard = 0;


    while (item_selected_in_main_menu != 1 && item_selected_in_main_menu != 2) {
        if (item_selected_in_main_menu == 3) {
            item_selected_in_scoreboard = Scoreboard_manu(sdlRenderer);
            if (item_selected_in_scoreboard == 1) {
                item_selected_in_main_menu = main_menu(username, sdlRenderer, letter_counter);
                if (username[0] == 0) {
                    letter_counter = 0;
                }
                else {
                    letter_counter = strlen(username);
                }
            }
            if (item_selected_in_scoreboard == 2) {
                printf("EXIT\n");
                SDL_DestroyRenderer(sdlRenderer);
                SDL_DestroyWindow(sdlWindow);
                return 0;
            }
        }
        else if (item_selected_in_main_menu == 4) {
            printf ("EXIT\n");
            SDL_DestroyRenderer(sdlRenderer);
            SDL_DestroyWindow(sdlWindow);
            return 0;
        }
    }

    if (username[0] == 0) {
        strcpy(username, "new user 1");
    }

    // saving username
    char scoreboard_name[] = "scoreboard.txt";
    FILE* scoreboard_ptr = fopen(scoreboard_name, "a");
    fprintf (scoreboard_ptr, "%s\n%d\n", username, 0);

    puts(username);





    // reading a map
    FILE* file_ptr;
    file_ptr = fopen("map.txt", "r");
    if (file_ptr == NULL) {
        printf ("Error opening file. \n");
    }
    fscanf(file_ptr, "%d\n", &HEXAGON_COUNT);
    fscanf(file_ptr, "%d\n", &NUMBER_OF_PLAYERS);

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

    // potion variables
    int X_of_potion;
    int Y_of_potion;
    int potion_flag;

    // logic variables and first initializations for main matrix
    matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    matrix_unit main_matrix_temp[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    first_initialization_of_main_matrix(main_matrix);
    first_initialization_of_main_matrix(main_matrix_temp);


    // variables for soldier count in lands
    int soldier_count_in_lands = 0;
    attacks_struct* local_attacks = NULL;

    // logical variables for AI and attack
    int X_dis = 0;
    int Y_dis = 0;
    int id_dis = 0;


    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {

        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlRenderer);

        // drawing the map
        drawHexagonalMap(sdlRenderer, map, hexagonal_colors);


        // logical part of code
        // lands soldier adding and attacks managing in lands
        for (int i=0; i<HEXAGON_COUNT; i++) {
            if (map[i].color_index != 9 && map[i].color_index != 8 && map[i].soldier < 150) {
                // lands soldier adding
                if (map[i].soldier < 150 && (map[i].soldier + map[i].rate) > 150) {
                    map[i].soldier = 150;
                } else {
                    map[i].soldier += map[i].rate;
                }
            }
            // attacks managing in lands
            if (map[i].color_index != 9 && map[i].color_index != 8) {
                soldier_generator_in_attacks(map, main_matrix, i);
            }



        }


        // potions
        potion_flag = potion_generator(sdlRenderer, &X_of_potion, &Y_of_potion, map);
        if (potion_flag == 1) {
            main_matrix[X_of_potion][Y_of_potion].Potion = true;
            main_matrix[X_of_potion][Y_of_potion].local_potion.potion_in_use = false;
            main_matrix[X_of_potion][Y_of_potion].local_potion.potion_id = rand()%4;
            main_matrix[X_of_potion][Y_of_potion].local_potion.potion_time = rand()%100000 + 1000;
        }


        // attacks
        for (int i=0; i<HEXAGON_COUNT; i++) {
            if (map[i].color_index == 9 || map[i].color_index == 8 || map[i].color_index == 0) {
                continue;
            }
            if (rand()%200 == 50) {
                //printf ("Attack\n");

                id_dis = AI(map[i].X, map[i].Y, map, &X_dis, &Y_dis, i);
                //printf ("X_dis = %d, Y_dis = %d ", X_dis, Y_dis);
                //printf ("from X_s = %d, Y_s = %d\n", map[i].X, map[i].Y);
                map[i].is_s = true;
                map[id_dis].is_d = true;

                int count = 0;
                local_attacks = map[i].attacks;
                while (local_attacks->next_attack != NULL) {
                    count++;
                    //printf ("Count is: %d\n", count);
                    local_attacks = local_attacks->next_attack;
                }
                local_attacks->next_attack = (attacks_struct*)malloc(sizeof(attacks_struct));
                local_attacks = local_attacks->next_attack;
                local_attacks->next_attack = NULL;
                local_attacks->soldiers = 10*map[i].soldier;
                map[i].soldier = 0;
                local_attacks->Y_dis = Y_dis;
                local_attacks->X_dis = X_dis;
                local_attacks->id_dis = id_dis;


            }
        }













        // drawing main matrix
        for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
            for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {

                // drawing potions
                if (main_matrix[i][j].Potion == true) {
                    if (main_matrix[i][j].local_potion.potion_time == 0) {
                        main_matrix[i][j].Potion = false;
                    }
                    else {
                        if (main_matrix[i][j].local_potion.potion_in_use == true) {
                            main_matrix[i][j].local_potion.potion_time -= 100;
                        }
                        filledCircleColor(sdlRenderer, 10*i+5, 10*j+5, 5, 0xffffffff);
                    }
                }

                // drawing soldiers and moving for next state
                for (int k=0; k<8; k++) {
                    if (main_matrix[i][j].local_soldiers[k].X != 0) {
                        //filledCircleColor(sdlRenderer, 10*i+5, 10*j+5, 5, map_colors[main_matrix[i][j].local_soldiers[k].land_id]);
                        filledCircleColor(sdlRenderer, main_matrix[i][j].local_soldiers[k].X/10*10+5, main_matrix[i][j].local_soldiers[k].Y/10*10+5, 5, map_colors[main_matrix[i][j].local_soldiers[k].land_id]); // 0xfff00fff Pink
                        if (pow(Delta_D_calculator(main_matrix[i][j].local_soldiers[k].X, main_matrix[i][j].local_soldiers[k].Y, main_matrix[i][j].local_soldiers[k].X_d, main_matrix[i][j].local_soldiers[k].Y_d), 0.5)<= HEXAGON_SIDE/3) {
                            main_matrix[i][j].local_soldiers[k].X = 0;
                        }
                        else {
                            main_matrix[i][j].local_soldiers[k].X += main_matrix[i][j].local_soldiers[k].V_x;
                            main_matrix[i][j].local_soldiers[k].Y += main_matrix[i][j].local_soldiers[k].V_y;
                        }
                    }
                }
            }
        }



        // main matrix conflict checking
        int conflict_checking = -1;
        for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
            for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {
                main_matrix_temp[i][j].Potion = main_matrix[i][j].Potion;
                for (int k=0; k<8; k++) {
                    if (main_matrix[i][j].local_soldiers[k].X != 0) {
                        for (int b=0; b<8; b++) {
                            if (main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[b].X == 0) {
                                conflict_checking = b;
                            }
                            else if (main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[b].land_id != main_matrix[i][j].local_soldiers[k].land_id) {
                                main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[b].X = 0;
                                main_matrix[i][j].local_soldiers[k].X = 0;
                                conflict_checking = -1;
                                break;
                            }
                        }
                        if (conflict_checking != -1) {
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].X = main_matrix[i][j].local_soldiers[k].X;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].Y = main_matrix[i][j].local_soldiers[k].Y;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].V_x = main_matrix[i][j].local_soldiers[k].V_x;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].V_y = main_matrix[i][j].local_soldiers[k].V_y;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].X_d = main_matrix[i][j].local_soldiers[k].X_d;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].Y_d = main_matrix[i][j].local_soldiers[k].Y_d;
                            main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].land_id = main_matrix[i][j].local_soldiers[k].land_id;
                            main_matrix[i][j].local_soldiers[k].X = 0;
                        }
                    }
                    conflict_checking = -1;
                }
            }
        }

        // main matrix temp copy
        for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / MAIN_MATRIX_UNITS_COUNT; j++) {
                main_matrix[i][j].Potion = main_matrix_temp[i][j].Potion;
                for (int k=0; k<8; k++) {
                    main_matrix[i][j].local_soldiers[k].V_y = main_matrix_temp[i][j].local_soldiers[k].V_y;
                    main_matrix[i][j].local_soldiers[k].V_x = main_matrix_temp[i][j].local_soldiers[k].V_x;
                    main_matrix[i][j].local_soldiers[k].Y = main_matrix_temp[i][j].local_soldiers[k].Y;
                    main_matrix[i][j].local_soldiers[k].X = main_matrix_temp[i][j].local_soldiers[k].X;
                    main_matrix[i][j].local_soldiers[k].X_d = main_matrix_temp[i][j].local_soldiers[k].X_d;
                    main_matrix[i][j].local_soldiers[k].Y_d = main_matrix_temp[i][j].local_soldiers[k].Y_d;
                    main_matrix[i][j].local_soldiers[k].land_id = main_matrix_temp[i][j].local_soldiers[k].land_id;
                }
            }
        }
        first_initialization_of_main_matrix(main_matrix_temp);

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
                        printf ("You clicked on X %d, Y %d, id %d\n", mouse_X, mouse_Y, X_Y_to_hexagon(mouse_X, mouse_Y));
                        printf ("Color of your clicked hexagon is in index %d\n", map[X_Y_to_hexagon(mouse_X, mouse_Y)-1].color_index);
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
    int temp_x;
    int temp_y;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        fscanf(file_ptr, "%d %d %d %d\n", &color_index, &soldier, &temp_x, &temp_y);

        map[i].color_index = color_index;
        map[i].soldier = (float)soldier;
        map[i].X = temp_x;
        map[i].Y = temp_y;
        map[i].potion_id = 0;
        map[i].rate = 0.4;
        map[i].soldier_speed = 2;
        map[i].attacks = (attacks_struct*)malloc(sizeof(attacks_struct));
        map[i].attacks->next_attack = NULL;
        map[i].attacks->finished = true;
        map[i].is_s = false;
        map[i].is_d = false;
        hexagonal_colors[i] = map_colors[map[i].color_index];
    }

    return;

}

void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, float soldiers) {

    char number_of_soldiers[9] = {0};
    score_to_score_in_scoreboard(number_of_soldiers, (int)soldiers);

    Sint16 X[6] = {(Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter+HEXAGON_SIDE), (Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE), (Sint16)(xcounter-HEXAGON_SIDE/2)};
    Sint16 Y[6] = {(Sint16)(ycounter+HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter+HEXAGON_SIDE*0.866025)};
    if (hexagon_color != 0xffffff00) {
        filledPolygonColor(sdlRenderer, X, Y, 6, hexagon_color - 0x4f000000);
        drawCircle(sdlRenderer, xcounter, ycounter, hexagon_color);
        stringColor(sdlRenderer, xcounter-8, ycounter+15, number_of_soldiers, 0xff000000);

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
            drawHexagon(sdlRenderer, xcounter, ycounter, hexagonal_colors[hexagon_counter], map[hexagon_counter].soldier);
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
            drawHexagon(sdlRenderer, xcounter, ycounter, hexagonal_colors[hexagon_counter], map[hexagon_counter].soldier);
            //printf("X is: %d , Y is: %d\n", xcounter, ycounter);
            hexagon_counter++;
            ycounter = ycounter + HEXAGON_h;
        }

        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }

    //printf ("Hexagons = %d\n", hexagon_counter);
}



// main menu functions

int Which_item_selected(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items) {

    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items) && Y_of_mouse>=Y_item1 && Y_of_mouse<=(Y_item1+H_items)) {
        return 1;
    }
    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items) && Y_of_mouse>=(Y_item1+H_items+D_items) && Y_of_mouse<=(Y_item1+H_items+D_items+H_items)) {
        return 2;
    }
    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items) && Y_of_mouse>=(Y_item1+(H_items+D_items)*2) && Y_of_mouse<=(Y_item1+(H_items+D_items)*2+H_items)) {
        return 3;
    }
    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items/3) && Y_of_mouse>=(Y_item1+(H_items+D_items)*3) && Y_of_mouse<=(Y_item1+(H_items+D_items)*3+H_items)) {
        return 4;
    }

    return 0;
}

int main_menu(char username[100], SDL_Renderer* sdlMenuRenderer, int letter_counter) {



    // image of menu
    char image_name[100] = {0};
    strcpy(image_name, "Sohrab_and_Rostam_first_battle.bmp");
    SDL_Surface * main_menu_image = SDL_LoadBMP(image_name);
    SDL_Texture * main_menu_texture = SDL_CreateTextureFromSurface(sdlMenuRenderer, main_menu_image);
    SDL_Rect main_menu_image_dstrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // menu texts and variables
    char username_text[] = "Please enter a username:";
    char item1_text[] = "new game";
    char item2_text[] = "last game";
    char item3_text[] = "Scoreboard";
    char item4_text[] = "EXIT";
    int X_of_mouse, Y_of_mouse;
    int item_selected_in_main_menu = 0;
    Uint32 item1_box_color = 0xffAfAf00; // persian green
    Uint32 item2_box_color = 0xffAfAf00; // persian green
    Uint32 item3_box_color = 0xffAfAf00; // persian green
    Uint32 item4_box_color = 0xff0000ff;


    // username variables
    int upper_case = 0;
    int caps_lock_check_flag = 1;




    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        SDL_SetRenderDrawColor(sdlMenuRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlMenuRenderer);


        // background image
        SDL_RenderCopy(sdlMenuRenderer, main_menu_texture, NULL, &main_menu_image_dstrect);

        // texts
        stringColor(sdlMenuRenderer, 275, 200, username_text, 0xff000000);

        // username box
        boxColor(sdlMenuRenderer, 275, 220, 460, 260, 0xffffffff);
        stringColor(sdlMenuRenderer, 280, 235, username, 0xff000000);




        // item boxes
        Uint16 X_item1 = 350;
        Uint16 Y_item1 = 300;
        Uint16 W_items = 150;
        Uint16 H_items = 45;
        Uint16 D_items = 15;

        // drawing item boxes
        boxColor(sdlMenuRenderer, X_item1, Y_item1, X_item1+W_items, Y_item1+H_items, item1_box_color);
        boxColor(sdlMenuRenderer, X_item1, Y_item1+H_items+D_items, X_item1+W_items, Y_item1+H_items*2+D_items, item2_box_color);
        boxColor(sdlMenuRenderer, X_item1, Y_item1+(H_items+D_items)*2, X_item1+W_items, Y_item1+(H_items+D_items)*2+H_items, item3_box_color);
        boxColor(sdlMenuRenderer, X_item1, Y_item1+(H_items+D_items)*3, X_item1+W_items/3, Y_item1+(H_items+D_items)*3+H_items, item4_box_color);


        // text in items
        stringColor(sdlMenuRenderer, X_item1+W_items/3 - 10, Y_item1+H_items/2 - 5, item1_text, 0xff000000);
        stringColor(sdlMenuRenderer, X_item1+W_items/4, Y_item1+H_items+D_items+H_items/2 - 5, item2_text, 0xff000000);
        stringColor(sdlMenuRenderer, X_item1+W_items/5 + 10, Y_item1+(H_items+D_items)*2+H_items/2 - 5, item3_text, 0xff000000);
        stringColor(sdlMenuRenderer, X_item1 + 10, Y_item1+(H_items+D_items)*3+H_items/2, item4_text, 0xff000000);


        // item color change
        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
        item_selected_in_main_menu = Which_item_selected(X_of_mouse, Y_of_mouse, X_item1, Y_item1,H_items, W_items, D_items);
        switch(item_selected_in_main_menu) {
            case 0:
                item1_box_color = 0xffAfAf00;
                item2_box_color = 0xffAfAf00;
                item3_box_color = 0xffAfAf00;
                item4_box_color = 0xff0000ff;
                break;
            case 1:
                item1_box_color = 0xffAfAf00 - 0xA1000000;
                break;
            case 2:
                item2_box_color = 0xffAfAf00 - 0xA1000000;
                break;
            case 3:
                item3_box_color = 0xffAfAf00 - 0xA1000000;
                break;
            case 4:
                item4_box_color = 0xff0000ff - 0xA1000000;
                break;
        }



        // event handling
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    item_selected_in_main_menu = 4;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                        item_selected_in_main_menu = Which_item_selected(X_of_mouse, Y_of_mouse, X_item1, Y_item1, H_items, W_items, D_items);
                        if (item_selected_in_main_menu != 0) {
                            shallExit = SDL_TRUE;
                        }

                    }
                    break;
                case SDL_KEYDOWN:
                    SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                    if (sdlEvent.key.keysym.sym == SDLK_CAPSLOCK) {
                        if (upper_case == ('A' - 'a')) {
                            upper_case = 0;
                        }
                        else {
                            upper_case = 'A' - 'a';
                        }
                    }
                    if (X_of_mouse<= 460 && X_of_mouse>=275 && Y_of_mouse<=260 && Y_of_mouse>=220) {
                        if (caps_lock_check_flag == 1 && (sdlEvent.key.keysym.mod & KMOD_CAPS)) {
                            upper_case = 'A' - 'a';
                            caps_lock_check_flag = 0;

                        }
                        if (caps_lock_check_flag == 1 && !(sdlEvent.key.keysym.mod & KMOD_CAPS))  {
                            upper_case = 0;
                            caps_lock_check_flag = 0;
                        }
                        if (sdlEvent.key.keysym.sym == SDLK_LSHIFT || sdlEvent.key.keysym.sym == SDLK_RSHIFT) {
                            upper_case = 'A' - 'a' - upper_case;
                        }
                        if (sdlEvent.key.keysym.sym >= 'a' && sdlEvent.key.keysym.sym <= 'z' && letter_counter<=19 && letter_counter>=0) {
                            //printf ("Upper case is: %d\n", upper_case);
                            username[letter_counter] = sdlEvent.key.keysym.sym + upper_case;
                            if (letter_counter != 19) {
                                letter_counter++;
                            }
                        }
                        else if (sdlEvent.key.keysym.sym >= '0' && sdlEvent.key.keysym.sym <= '9' && letter_counter<=19 && letter_counter>=0) {
                            //printf ("Upper case is: %d\n", upper_case);
                            username[letter_counter] = sdlEvent.key.keysym.sym;
                            if (letter_counter != 19) {
                                letter_counter++;
                            }
                        }
                        if (sdlEvent.key.keysym.sym == 32 && letter_counter<=19 && letter_counter>=0) {         // 32 = space
                            username[letter_counter] = 32;
                            if (letter_counter != 19) {
                                letter_counter++;
                            }
                        }
                        if (sdlEvent.key.keysym.sym == 8 && letter_counter<=19 && letter_counter>=0) {          // 8 = Backspace
                            if (letter_counter != 0) {
                                letter_counter--;
                            }
                            username[letter_counter] = 0;
                        }
                        //printf ("%d\n", sdlEvent.key.keysym.sym);
                    }
                    break;
                case SDL_KEYUP:
                    if (sdlEvent.key.keysym.sym == SDLK_LSHIFT || sdlEvent.key.keysym.sym == SDLK_RSHIFT) {
                        upper_case = 'A' - 'a' - upper_case;
                    }
                    break;
            }


        }

        SDL_RenderPresent(sdlMenuRenderer);
        SDL_Delay(1000 / FPS);
    }


    SDL_Delay(100);
    SDL_DestroyTexture(main_menu_texture);
    SDL_FreeSurface(main_menu_image);
    //SDL_DestroyRenderer(sdlMenuRenderer);
    //SDL_DestroyWindow(sdlMenu);
    SDL_RenderClear(sdlMenuRenderer);

    return item_selected_in_main_menu;
}


//score board functions
int user_count_in_scoreboard() {
    char scoreboard_name[] = "scoreboard.txt";
    int user_counter = 0;
    int read_from_file = 1;
    int score;
    char name[1000] = {0};

    FILE* file_ptr = fopen(scoreboard_name, "r");

    while (read_from_file == 1) {
        read_from_file = fscanf(file_ptr, " %[^\n]s", &name);
        read_from_file = fscanf(file_ptr, "%d\n", &score);
        if (read_from_file == 1) {
            user_counter++;
        }
    }

    fclose(file_ptr);

    return user_counter;
}


void read_score_board_file(score_board_struct* users_ptr, int users_counter) {

    if (users_ptr == NULL) {
        return;
    }

    char scoreboard_name[] = "scoreboard.txt";

    FILE* file_ptr = fopen(scoreboard_name, "r");

    for (int i=0; i<users_counter; i++) {
        fscanf(file_ptr, " %[^\n]s", &(users_ptr+i)->name);
        fscanf(file_ptr, "%d\n", &(users_ptr+i)->score);
        if (i != users_counter-1) {
            (users_ptr+i)->next_user = users_ptr+i+1;
        }
        else {
            (users_ptr+i)->next_user = NULL;
        }

    }

    fclose(file_ptr);

    return;

}

void swap_users_in_scoreboard(score_board_struct* temp_i, score_board_struct* temp_j) {

    int temp_score;
    char temp_name[1000] = {0};

    temp_score = temp_i->score;
    strcpy(temp_name, temp_i->name);
    temp_i->score = temp_j->score;
    strcpy(temp_i->name, temp_j->name);
    temp_j->score = temp_score;
    strcpy(temp_j->name, temp_name);

    return;

}

void sort_score_board_file(score_board_struct* users_ptr, int user_counter) {

    if (users_ptr == NULL) {
        return;
    }

    score_board_struct* temp_i, *temp_j;
    temp_i = users_ptr;

    for (int i=0; i<user_counter; i++) {

        temp_j = temp_i->next_user;
        for (int j=i+1; j<user_counter; j++) {

            if (temp_i->score < temp_j->score) {

                // swapping users
                swap_users_in_scoreboard(temp_i, temp_j);
            }

            temp_j = temp_j->next_user;

        }

        temp_i = temp_i->next_user;

    }

    return;
}


void index_to_rank_in_scoreboard(char temp_username_in_scoreboard_menu_rank[3], int index) {
    if (index < 10) {
        temp_username_in_scoreboard_menu_rank[0] = '0' + index;
        temp_username_in_scoreboard_menu_rank[1] = 0;
        temp_username_in_scoreboard_menu_rank[2] = 0;
        return;
    }

    temp_username_in_scoreboard_menu_rank[0] = (index)/10 + '0';
    temp_username_in_scoreboard_menu_rank[1] = (index)%10 + '0';
    temp_username_in_scoreboard_menu_rank[2] = 0;
    return;

}

int power(int number) {

    int result = 1;
    for (int i=0; i<number; i++) {
        result = result * 10;
    }

    return result;
}

void score_to_score_in_scoreboard(char temp_username_in_scoreboard_menu_score[9], int score) {

    for (int i=0; i<9; i++) {
        temp_username_in_scoreboard_menu_score[i] = 0;
    }

    if (score == 0) {
        temp_username_in_scoreboard_menu_score[0] = '0';
        return;
    }

    if (score < 0) {
        score = -score;
    }

    int temp_score = score;
    int digit_counter = 0;

    while (temp_score != 0) {
        temp_score = temp_score/10;
        digit_counter++;
    }

    int counter = 0;
    int temp;

    for (int i=0; i<digit_counter; i++) {
        temp = score/(power(digit_counter-i-1));
        temp_username_in_scoreboard_menu_score[i] = temp + '0';
        score = score%(power(digit_counter-i-1));
    }


    return;
}

void fill_strings_for_ranks_and_scores(score_board_struct* users_ptr, int user_counter) {

    char temp_username_in_scoreboard_menu_rank[3] = {0};
    char temp_username_in_scoreboard_menu_score[9] = {0};

    for (int i=0; i<user_counter; i++) {
        index_to_rank_in_scoreboard(temp_username_in_scoreboard_menu_rank, i+1);
        score_to_score_in_scoreboard(temp_username_in_scoreboard_menu_score, (users_ptr+i)->score);
        strcpy((users_ptr+i)->rank_in_string, temp_username_in_scoreboard_menu_rank);
        strcpy((users_ptr+i)->score_in_string, temp_username_in_scoreboard_menu_score);
    }

    return;
}

void writing_on_scoreboard(SDL_Renderer * sdlScoreboardRenderer, score_board_struct* users_head_ptr, int user_counter) {

    score_board_struct* users_ptr = users_head_ptr;

    Uint16 D_users = 10;
    Uint16 X_first = 350;
    Uint16 Y_first = 120;
    char temp_username_in_scoreboard_menu[1000] = {0};
    char temp_username_in_scoreboard_menu_score_under0[] = "-";
    char label1[] = "Rank";
    char label2[] = "Username";
    char label3[] = "Score";

    // writing labels
    stringColor(sdlScoreboardRenderer, 300, 100,  label1, 0xff000000);
    stringColor(sdlScoreboardRenderer, 350, 100,  label2, 0xff000000);
    stringColor(sdlScoreboardRenderer, 500, 100,  label3, 0xff000000);

    // writing ranks
    for (int j=0; j<user_counter; j++) {
        //index_to_rank_in_scoreboard(temp_username_in_scoreboard_menu_rank, j+1);
        stringColor(sdlScoreboardRenderer, 300, Y_first,  users_ptr->rank_in_string, 0xff000000);

        switch (j) {
            case 0:
                filledCircleColor(sdlScoreboardRenderer, 290, Y_first+3, 5, 0xff37afd4);
                break;
            case 1:
                filledCircleColor(sdlScoreboardRenderer, 290, Y_first+3, 5, 0xffc0c0c0);
                break;
            case 2:
                filledCircleColor(sdlScoreboardRenderer, 290, Y_first+3, 5, 0xff327fcd);
                break;
        }

        if ( j != user_counter-1) {
            users_ptr = users_ptr->next_user;
        }
        if ( j == 24) {
            break;
        }
        Y_first = Y_first + D_users;
    }

    users_ptr = users_head_ptr;
    Y_first = 120;

    // writing names
    for (int j=0; j<user_counter; j++) {
        //strcpy(temp_username_in_scoreboard_menu, users_ptr->name);
        stringColor(sdlScoreboardRenderer, X_first, Y_first,  users_ptr->name, 0xff000000);
        if ( j != user_counter-1) {
            users_ptr = users_ptr->next_user;
        }
        if ( j == 24) {
            break;
        }
        Y_first = Y_first + D_users;
    }

    users_ptr = users_head_ptr;
    Y_first = 120;

    // writing scores
    for (int j=0; j<user_counter; j++) {
        //score_to_score_in_scoreboard(users_ptr->, users_ptr->score);
        stringColor(sdlScoreboardRenderer, 500, Y_first,  users_ptr->score_in_string, 0xff000000);


        if ( users_ptr->score < 0) {
            stringColor(sdlScoreboardRenderer, 490, Y_first,  temp_username_in_scoreboard_menu_score_under0, 0xff000000);
        }
        if ( j != user_counter-1) {
            users_ptr = users_ptr->next_user;
        }
        if ( j == 24) {
            break;
        }
        Y_first = Y_first + D_users;
    }

    return;
}

int Which_item_selected_in_scoreboard(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items) {

    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items) && Y_of_mouse>=Y_item1 && Y_of_mouse<=(Y_item1+H_items)) {
        return 1;
    }
    if (X_of_mouse>=X_item1 && X_of_mouse<=(X_item1+W_items/3) && Y_of_mouse>=(Y_item1+(H_items+D_items)) && Y_of_mouse<=(Y_item1+(H_items+D_items)+H_items)) {
        return 2;
    }

    return 0;
}

int Scoreboard_manu(SDL_Renderer* sdlScoreboardRenderer) {

    // reading and sorting users in scoreboard
    int user_counter = user_count_in_scoreboard();


    score_board_struct* users_head_ptr = NULL;
    score_board_struct* users_ptr = NULL;
    users_head_ptr = (score_board_struct*)malloc(user_counter*sizeof(score_board_struct));
    read_score_board_file(users_head_ptr, user_counter);
    sort_score_board_file(users_head_ptr, user_counter);
    fill_strings_for_ranks_and_scores(users_head_ptr, user_counter);


    // image of menu
    char image_name[100] = {0};
    strcpy(image_name, "Sohrab_and_Rostam_first_battle2.bmp");
    SDL_Surface * score_board_image = SDL_LoadBMP(image_name);
    SDL_Texture * score_board_texture = SDL_CreateTextureFromSurface(sdlScoreboardRenderer, score_board_image);
    SDL_Rect score_board_image_dstrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // variables of items on scoreboard
    Uint16 X_item1 = 350;
    Uint16 Y_item1 = 420;
    Uint16 W_items = 150;
    Uint16 H_items = 45;
    Uint16 D_items = 15;
    Uint32 item1_box_color = 0xffAfAf00; // persian green
    Uint32 item2_box_color = 0xff0000ff;
    int X_of_mouse, Y_of_mouse;
    int item_selected_in_scoreboard = 0;
    char item1_text[] = "back to main menu";
    char item2_text[] = "EXIT";


    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {


        SDL_SetRenderDrawColor(sdlScoreboardRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlScoreboardRenderer);


        // background image
        SDL_RenderCopy(sdlScoreboardRenderer, score_board_texture, NULL, &score_board_image_dstrect);



        // writing on screen
        writing_on_scoreboard(sdlScoreboardRenderer, users_head_ptr, user_counter);

        // drawing items on scoreboard
        boxColor(sdlScoreboardRenderer, X_item1, Y_item1, X_item1+W_items, Y_item1+H_items, item1_box_color);
        boxColor(sdlScoreboardRenderer, X_item1, Y_item1+(H_items+D_items), X_item1+W_items/3, Y_item1+(H_items+D_items)+H_items, item2_box_color);

        // item color change
        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
        item_selected_in_scoreboard = Which_item_selected_in_scoreboard(X_of_mouse, Y_of_mouse, X_item1, Y_item1,H_items, W_items, D_items);
        switch(item_selected_in_scoreboard) {
            case 0:
                item1_box_color = 0xffAfAf00;
                item2_box_color = 0xff0000ff;
                break;
            case 1:
                item1_box_color = 0xffAfAf00 - 0xA1000000;
                break;
            case 2:
                item2_box_color = 0xff0000ff - 0xA1000000;
                break;
        }

        // text in items
        stringColor(sdlScoreboardRenderer, X_item1 + 10, Y_item1+H_items/2 - 5, item1_text, 0xff000000);
        stringColor(sdlScoreboardRenderer, X_item1 + 10, Y_item1+(H_items+D_items)+H_items/2, item2_text, 0xff000000);



        // event handling
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    item_selected_in_scoreboard = 2;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                        item_selected_in_scoreboard = Which_item_selected_in_scoreboard(X_of_mouse, Y_of_mouse, X_item1, Y_item1, H_items, W_items, D_items);
                        if (item_selected_in_scoreboard != 0) {
                            shallExit = SDL_TRUE;
                        }

                    }
                    break;
            }
        }

        SDL_RenderPresent(sdlScoreboardRenderer);
        SDL_Delay(1000 / FPS);
    }


    SDL_Delay(100);
    SDL_DestroyTexture(score_board_texture);
    SDL_FreeSurface(score_board_image);
    SDL_RenderClear(sdlScoreboardRenderer);

    return item_selected_in_scoreboard;
}


// logic
int Delta_D_calculator(int X1, int Y1, int X2, int Y2) {
    return ((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2));
}
int X_Y_to_hexagon(int mouse_X, int mouse_Y) {

    int id_check[3][2];
    int check_counter = 0;
    int xcounter = 0;
    int ycounter = 0;
    int hexagon_counter = 0;
    int Delta_D = 0;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {
        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            hexagon_counter++;

            Delta_D = Delta_D_calculator(xcounter, ycounter, mouse_X, mouse_Y);

            if (Delta_D <= (HEXAGON_h/2)*(HEXAGON_h/2)) {
                return hexagon_counter++;
            }
            if (Delta_D <= (HEXAGON_SIDE)*(HEXAGON_SIDE)) {
                id_check[check_counter][0] = Delta_D;
                id_check[check_counter][1] = hexagon_counter;
                check_counter++;
            }

            ycounter = ycounter + HEXAGON_h;

        }
        ycounter = 0;
        xcounter = xcounter + 3*HEXAGON_SIDE;
    }

    xcounter = HEXAGON_SIDE + HEXAGON_SIDE/2;
    ycounter = HEXAGON_h/2;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            hexagon_counter++;

            Delta_D = Delta_D_calculator(xcounter, ycounter, mouse_X, mouse_Y);

            if (Delta_D <= (HEXAGON_h/2)*(HEXAGON_h/2)) {
                return hexagon_counter++;
            }
            if (Delta_D <= (HEXAGON_SIDE)*(HEXAGON_SIDE)) {
                id_check[check_counter][0] = Delta_D;
                id_check[check_counter][1] = hexagon_counter;
                check_counter++;
            }

            ycounter = ycounter + HEXAGON_h;

        }

        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }


    int smallest_D = 2000000;
    int result = 0;
    for (int i=0; i<check_counter; i++) {
        if (id_check[i][0] < smallest_D) {
            smallest_D = id_check[i][0];
            result = id_check[i][1];
        }
    }

    return result;

}

int potion_generator(SDL_Renderer * sdlRenderer, int* X_of_potion, int* Y_of_potion, hexagonal map[HEXAGON_COUNT]) {

    int random_number = (rand()%1000);
    if (random_number%100 != 0) {
        return 0;
    }

    *(X_of_potion) = (rand()%(SCREEN_WIDTH-200) + 100)/10;
    *(Y_of_potion) = (rand()%(SCREEN_HEIGHT-200) + 100)/10;

    /*
    while (map[X_Y_to_hexagon(*(X_of_potion), *(Y_of_potion))].color_index == 9) {
        *(X_of_potion) = (rand()%(SCREEN_WIDTH-200) + 100)/10;
        *(Y_of_potion) = (rand()%(SCREEN_HEIGHT-200) + 100)/10;
    }
    */

    //printf("Potion is in X = %d Y = %d\n", *(X_of_potion), *(Y_of_potion));

    return 1;

}

int AI(int X_s, int Y_s, hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, int myID) {

    int smallest_Delta_D = 300*300;
    int Delta_D;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 9 || (map[i].X == X_s && map[i].Y == Y_s)) {
            continue;
        }
        if (map[i].is_d == false && map[i].is_s == true) {
            Delta_D = Delta_D_calculator(X_s, Y_s, map[i].X, map[i].Y);
            if (Delta_D < smallest_Delta_D) {
                *(X_dis) = map[i].X;
                *(Y_dis) = map[i].Y;
                smallest_Delta_D = Delta_D;
            }
        }

    }

    if (smallest_Delta_D != 300*300) {
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        //printf ("%d - %d\n", map[X_Y_to_hexagon(*(X_dis), *(Y_dis))-1].color_index, X_Y_to_hexagon(*(X_dis), *(Y_dis))-1);
        return X_Y_to_hexagon(*(X_dis), *(Y_dis));
    }

    smallest_Delta_D = 300*300;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 9 || (map[i].X == X_s && map[i].Y == Y_s)) {
            continue;
        }
        if (map[i].is_d == false && map[i].color_index == 8) {
            Delta_D = Delta_D_calculator(X_s, Y_s, map[i].X, map[i].Y);
            if (Delta_D < smallest_Delta_D) {
                *(X_dis) = map[i].X;
                *(Y_dis) = map[i].Y;
                smallest_Delta_D = Delta_D;
            }
        }

    }

    if (smallest_Delta_D != 300*300) {
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        //printf ("%d - %d\n", map[X_Y_to_hexagon(*(X_dis), *(Y_dis))-1].color_index, X_Y_to_hexagon(*(X_dis), *(Y_dis))-1);
        return X_Y_to_hexagon(*(X_dis), *(Y_dis));
    }

    if (map[X_Y_to_hexagon(X_s, Y_s - HEXAGON_h)-1].color_index != 9) {
        //printf ("Hi\n");
        *(X_dis) = X_s;
        *(Y_dis) = Y_s - HEXAGON_h;
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        return myID - 1;
    }
}

void filling_main_matrix_for_attacks(matrix_unit main_matrix[60][60], hexagonal map[HEXAGON_COUNT], int X_dis, int Y_dis, int i, int j) {
    //printf ("%d - %d\n", map[X_Y_to_hexagon((X_dis), (Y_dis))-1].color_index, X_Y_to_hexagon((X_dis), (Y_dis))-1);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].X = (float)map[i].X;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].Y = (float)map[i].Y;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].land_id = map[i].color_index;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].Y_d = Y_dis;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].X_d = X_dis;
    float Delta_D = (float)pow(Delta_D_calculator(map[i].X, map[i].Y, X_dis, Y_dis), 0.5);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_x = (float)map[i].soldier_speed * ((float)(X_dis - map[i].X)/Delta_D);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_y = (float)map[i].soldier_speed * ((float)(Y_dis - map[i].Y)/Delta_D);
    //printf ("Speed is Vx = %f, Yy = %f\n", main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_x, main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_y);
}

void first_initialization_of_main_matrix(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {
    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {
            main_matrix[i][j].Potion = false;
            for (int k=0; k<8; k++) {
                main_matrix[i][j].local_soldiers[k].V_y = 0;
                main_matrix[i][j].local_soldiers[k].V_x = 0;
                main_matrix[i][j].local_soldiers[k].Y = 0;
                main_matrix[i][j].local_soldiers[k].X = 0;
                main_matrix[i][j].local_soldiers[k].X_d = 0;
                main_matrix[i][j].local_soldiers[k].Y_d = 0;
                main_matrix[i][j].local_soldiers[k].land_id = 9;
            }
        }
    }
    return;
}

void soldier_generator_in_attacks(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i) {
    attacks_struct* local_attacks = map[i].attacks;
    //int count = 0;
    while (local_attacks != NULL) {
        //printf ("%d is not NULL\n", map[i].color_index);
        if (local_attacks->finished == false) {
            local_attacks->soldiers--;
            if (local_attacks->soldiers % 10 == 0) {
                //printf ("You have %d soldiers in %d land\n", local_attacks->soldiers/10, i);
                for (int j = 0; j < 8; j++) {
                    if (main_matrix[map[i].X / MAIN_MATRIX_UNITS_COUNT][map[i].Y /
                                                                        MAIN_MATRIX_UNITS_COUNT].local_soldiers[j].X ==
                        0) {
                        //printf("You are going to X = %d, Y = %d\n", local_attacks->X_dis, local_attacks->Y_dis);
                        //count++;
                        //printf ("Count is: %d\n", count);
                        filling_main_matrix_for_attacks(main_matrix, map, local_attacks->X_dis,
                                                        local_attacks->Y_dis, i, j);
                        break;
                    }
                }
            }
            if (local_attacks->soldiers <= 0) {
                local_attacks->finished = true;
                map[local_attacks->id_dis].is_d = false;
            }
        }
        local_attacks = local_attacks->next_attack;
    }

    return;
}




