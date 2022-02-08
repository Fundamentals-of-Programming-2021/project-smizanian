//
// Created by smiza on 2/8/2022.
//

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "global_variables.h"
#include "map.h"

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

    free(temp_i);
    free(temp_j);

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



void fill_strings_for_ranks_and_scores(score_board_struct* users_ptr, int user_counter) {

    char temp_username_in_scoreboard_menu_rank[3] = {0};
    char temp_username_in_scoreboard_menu_score[9] = {0};

    for (int i=0; i<user_counter; i++) {
        index_to_rank_in_scoreboard(temp_username_in_scoreboard_menu_rank, i+1);
        int_to_str(temp_username_in_scoreboard_menu_score, (users_ptr+i)->score);
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

    free(users_head_ptr);


    SDL_Delay(100);
    SDL_DestroyTexture(score_board_texture);
    SDL_FreeSurface(score_board_image);
    SDL_RenderClear(sdlScoreboardRenderer);

    return item_selected_in_scoreboard;
}




// new game menu
int which_item_selected_in_new_game_menu(int X_of_mouse, int Y_of_mouse, int X_item1, int Y_item1, int H_items, int W_items, int D_items) {
    int item_selected_in_menu = 0;
    if (X_of_mouse>X_item1 && X_of_mouse<(X_item1+W_items) && Y_of_mouse>Y_item1 && Y_of_mouse<(Y_item1+H_items)) {
        item_selected_in_menu = 1;
    }
    else if (X_of_mouse>X_item1 && X_of_mouse<(X_item1+W_items) && Y_of_mouse>Y_item1+H_items+D_items && Y_of_mouse<(Y_item1+H_items*2+D_items)) {
        item_selected_in_menu = 2;
    }
    else {
        item_selected_in_menu = 0;
    }
    return item_selected_in_menu;
}

int default_maps_count() {
    char file_name[] = "000.txt";
    int file_name_counter = 0;
    FILE* map_ptr = fopen(file_name, "r");

    while (map_ptr != NULL) {
        file_name_counter++;
        fclose(map_ptr);
        file_name_change(file_name, file_name_counter);
        map_ptr = fopen(file_name, "r");
    }
    return file_name_counter;
}

int new_game_menu(SDL_Renderer* sdlNewGameMenuRenderer, int* number_of_players, int* cell_num, int* default_map_number) {

    // image of menu
    char image_name[100] = {0};
    strcpy(image_name, "Sohrab_and_Rostam_first_battle3.bmp");
    SDL_Surface * menu_image = SDL_LoadBMP(image_name);
    SDL_Texture * menu_texture = SDL_CreateTextureFromSurface(sdlNewGameMenuRenderer, menu_image);
    SDL_Rect menu_image_dstrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // menu texts and variables
    char main_text[] = "Load a map";
    char default_text[] = "Choose a number between 1 and ";
    char random_text1[] = "Choose number of players [2,6]:";
    char random_text2[] = "Choose number of rows [5,9]:";
    char error[] = "Number is not valid.";
    char item1_text[] = "default maps";
    char item2_text[] = "random map";
    char OK_text[] = "OK";
    int X_of_mouse, Y_of_mouse;
    int item_selected_in_menu = 0;
    Uint32 item1_box_color = 0xffAfAf00; // persian green
    Uint32 item2_box_color = 0xffAfAf00; // persian green
    Uint32 item3_box_color = 0xffAfAf00; // persian green


    // default maps count
    int map_counter = default_maps_count();
    int temp_map_counter = map_counter;
    char map_number[] = "000";
    map_number[2] = map_counter%10 + '0';
    map_counter = map_counter/10;
    map_number[1] = map_counter%10 + '0';
    map_counter = map_counter/10;
    map_number[0] = map_counter + '0';
    int default_map_index_count = 0;
    char default_map_name[4] = {0};
    for (int i=0; i<4; i++) {
        default_map_name[i] = 0;
    }
    map_counter = temp_map_counter;
    printf ("%d\n", map_counter);
    char number_of_players_str[2] = {0};
    char cell_num_str[2] = {0};




    // menu logic variables
    bool random_map = false;
    bool default_map = false;
    bool valid_default_map = true;
    bool valid_number_of_players = true;
    bool valid_cell_num = true;

    int selected_map_number = 0;
    int option = 1;


    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        SDL_SetRenderDrawColor(sdlNewGameMenuRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlNewGameMenuRenderer);


        // background image
        SDL_RenderCopy(sdlNewGameMenuRenderer, menu_texture, NULL, &menu_image_dstrect);


        // texts
        stringColor(sdlNewGameMenuRenderer, 385, 100, main_text, 0xff000000);


        // item boxes
        Uint16 X_item1 = 350;
        Uint16 Y_item1 = 120;
        Uint16 W_items = 150;
        Uint16 H_items = 45;
        Uint16 D_items = 15+30+H_items;

        // drawing item boxes
        boxColor(sdlNewGameMenuRenderer, X_item1, Y_item1, X_item1 + W_items, Y_item1 + H_items, item1_box_color);
        boxColor(sdlNewGameMenuRenderer, X_item1, Y_item1 + H_items + D_items, X_item1 + W_items,Y_item1 + H_items * 2 + D_items, item2_box_color);


        // text in items
        stringColor(sdlNewGameMenuRenderer, X_item1 + W_items / 4 - 10, Y_item1 + H_items / 2 - 5, item1_text, 0xff000000);
        stringColor(sdlNewGameMenuRenderer, X_item1 + W_items / 4, Y_item1 + H_items + D_items + H_items / 2 - 5,item2_text, 0xff000000);


        // item color change
        if (random_map == false && default_map == false) {
            SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
            item_selected_in_menu = which_item_selected_in_new_game_menu(X_of_mouse, Y_of_mouse, X_item1, Y_item1,H_items, W_items, D_items);
            switch (item_selected_in_menu) {
                case 0:
                    item1_box_color = 0xffAfAf00;
                    item2_box_color = 0xffAfAf00;
                    break;
                case 1:
                    item1_box_color = 0xffAfAf00 - 0xA1000000;
                    break;
                case 2:
                    item2_box_color = 0xffAfAf00 - 0xA1000000;
                    break;
            }
        }

        if (default_map == true) {
            boxColor(sdlNewGameMenuRenderer, X_item1, Y_item1+H_items+30, X_item1 + W_items, Y_item1 + H_items*2 + 15, 0xffffffff);
            stringColor(sdlNewGameMenuRenderer, X_item1 - W_items/3, Y_item1 + H_items + 10 , default_text, 0xff000000);
            stringColor(sdlNewGameMenuRenderer, X_item1 + W_items + W_items/3 - 8, Y_item1 + H_items + 10, map_number, 0xff000000);
            stringColor(sdlNewGameMenuRenderer, X_item1+W_items/2-15, Y_item1+H_items+40, default_map_name, 0xff000000);
            if (valid_default_map == false) {
                stringColor(sdlNewGameMenuRenderer, X_item1, Y_item1+H_items*2 + 25, error, 0xff0000ff);

            }
        }
        else if (random_map == true) {
            if (option == 1 || valid_number_of_players == true) {
                boxColor(sdlNewGameMenuRenderer, X_item1, Y_item1 + H_items * 2 + D_items + 25, X_item1 + W_items, Y_item1 + H_items * 2 + D_items + H_items + 25, 0xffffffff);
                stringColor(sdlNewGameMenuRenderer, X_item1 - W_items/4, Y_item1 + H_items * 2 + D_items + 10 , random_text1, 0xff000000);
                stringColor(sdlNewGameMenuRenderer, X_item1+W_items/2, Y_item1 + H_items * 2 + D_items + 45, number_of_players_str, 0xff000000);
                if (valid_number_of_players == false) {
                    stringColor(sdlNewGameMenuRenderer, X_item1, Y_item1 + H_items * 2 + D_items + H_items + 30, error, 0xff0000ff);

                }
            }
            if (option == 2 || option == 3) {
                boxColor(sdlNewGameMenuRenderer, X_item1, Y_item1 + H_items * 3 + D_items + 50, X_item1 + W_items, Y_item1 + H_items * 3 + D_items + H_items + 50, 0xffffffff);
                stringColor(sdlNewGameMenuRenderer, X_item1 - W_items/5, Y_item1 + H_items * 3 + D_items + 10 + 25 , random_text2, 0xff000000);
                stringColor(sdlNewGameMenuRenderer, X_item1+W_items/2, Y_item1 + H_items * 3 + D_items + 45 + 25, cell_num_str, 0xff000000);
                if (valid_cell_num == false) {
                    stringColor(sdlNewGameMenuRenderer, X_item1, Y_item1 + H_items * 3 + D_items + H_items + 55, error, 0xff0000ff);
                }
            }
        }

        if (option == 3) {
            boxColor(sdlNewGameMenuRenderer, X_item1 + W_items/2 - 30, Y_item1 + H_items * 4 + D_items + 65, X_item1 + W_items/2 + 30, Y_item1 + H_items * 4 + D_items + H_items + 65, item3_box_color);
            stringColor(sdlNewGameMenuRenderer, X_item1 + W_items/2 - 5, Y_item1 + H_items * 4 + D_items + 80 , OK_text,  0xff000000);
            SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
            if (X_of_mouse >= (X_item1 + W_items/2 - 30) && X_of_mouse <= (X_item1 + W_items/2 + 30) && Y_of_mouse >= (Y_item1 + H_items * 4 + D_items + 65) && Y_of_mouse <= Y_item1 + H_items * 4 + D_items + H_items + 65) {
                item3_box_color =  0xffAfAf00 - 0xA1000000;
            }
            else {
                item3_box_color =  0xffAfAf00;
            }
        }



        // event handling
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                        if (random_map == false && default_map == false) {
                            SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                            item_selected_in_menu = which_item_selected_in_new_game_menu(X_of_mouse, Y_of_mouse,X_item1, Y_item1, H_items,W_items, D_items);
                            switch (item_selected_in_menu) {
                                case 1:
                                    default_map = true;
                                    item1_box_color = 0xffAfAf00;
                                    break;
                                case 2:
                                    random_map = true;
                                    item2_box_color = 0xffAfAf00;
                                    break;
                            }
                        }
                        if (option == 3) {
                            SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                            if (X_of_mouse >= (X_item1 + W_items/2 - 30) && X_of_mouse <= (X_item1 + W_items/2 + 30) && Y_of_mouse >= (Y_item1 + H_items * 4 + D_items + 65) && Y_of_mouse <= Y_item1 + H_items * 4 + D_items + H_items + 65) {
                                shallExit = SDL_TRUE;
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    if (random_map == true) {
                        if (option == 1) {
                            if (sdlEvent.key.keysym.sym >= '0' && sdlEvent.key.keysym.sym <= '9') {
                                number_of_players_str[0] = sdlEvent.key.keysym.sym;
                                *(number_of_players) = sdlEvent.key.keysym.sym - '0';
                                if ((*number_of_players) <= 6 && (*number_of_players) >= 2) {
                                    valid_number_of_players = true;
                                    option = 2;
                                }
                                else {
                                    *(number_of_players) = 0;
                                    valid_number_of_players = false;
                                    number_of_players_str[0] = 0;
                                }
                            }

                        }
                        else if (option == 2) {
                            if (sdlEvent.key.keysym.sym >= '0' && sdlEvent.key.keysym.sym <= '9') {
                                cell_num_str[0] = sdlEvent.key.keysym.sym;
                                *(cell_num) = sdlEvent.key.keysym.sym - '0';
                                if ((*cell_num) <= 9 && (*cell_num) >= 5) {
                                    valid_cell_num = true;
                                    option = 3;
                                }
                                else {
                                    *(cell_num) = 0;
                                    valid_cell_num = false;
                                    cell_num_str[0] = 0;
                                }
                            }

                        }
                    }
                    if (default_map == true) {
                        if (default_map_index_count >= 0 && default_map_index_count < 3) {
                            if (sdlEvent.key.keysym.sym >= '0' && sdlEvent.key.keysym.sym <= '9') {
                                default_map_name[default_map_index_count] = sdlEvent.key.keysym.sym;
                                selected_map_number = 10*selected_map_number + (sdlEvent.key.keysym.sym - '0');
                                default_map_index_count++;
                                if (selected_map_number-1 <= map_counter) {
                                    valid_default_map = true;
                                    *(default_map_number) = selected_map_number - 1;
                                    option = 3;
                                }
                                if (selected_map_number > map_counter || selected_map_number <= 0) {
                                    printf ("%d\n", selected_map_number);
                                    default_map_index_count = 0;
                                    selected_map_number = 0;
                                    valid_default_map = false;
                                    for (int i=0; i<3; i++) {
                                        default_map_name[i] = 0;
                                    }
                                }
                            }

                        }
                    }
            }

        }

        SDL_RenderPresent(sdlNewGameMenuRenderer);
        SDL_Delay(1000 / FPS);
    }


    SDL_Delay(100);
    SDL_DestroyTexture(menu_texture);
    SDL_FreeSurface(menu_image);
    //SDL_DestroyRenderer(sdlMenuRenderer);
    //SDL_DestroyWindow(sdlMenu);
    SDL_RenderClear(sdlNewGameMenuRenderer);

    if (random_map == true) {
        item_selected_in_menu = 2;
    }
    if (default_map == true) {
        item_selected_in_menu = 1;
    }

    return item_selected_in_menu;
}