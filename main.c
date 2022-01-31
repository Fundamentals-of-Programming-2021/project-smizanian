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
// for map
typedef struct my_map_hexagonal{
    int color_index;
    int soldier;
} hexagonal;

// for scoreboard
typedef struct users_in_score_board{
    char name[1000];
    char rank_in_string[3];
    int score;
    char score_in_string[9];
    struct users_in_score_board* next_user;
}score_board_struct;





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




int main() {

    // SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

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

    puts(username);






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


