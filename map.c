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

// basic functions for map
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
        map[i].potion_id = -1;
        map[i].rate = 0.1;
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


void drawCircle(SDL_Renderer *sdlRenderer, int x_int, int y_int, Uint32 color) {
    Sint16 X = (Sint16)x_int;
    Sint16 Y = (Sint16)y_int;
    Sint16 radius = HEXAGON_SIDE/3;
    filledCircleColor(sdlRenderer, X, Y, radius, color);
}
void drawBox(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color) {
    Sint16 X = (Sint16)x;
    Sint16 Y = (Sint16)y;
    Sint16 D = 2*HEXAGON_SIDE/4;
    boxColor(sdlRenderer, X-D/2, Y-D/2, X+D/2, Y+D/2, color);
}
void drawTri(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color) {
    Sint16 D = 2*HEXAGON_SIDE/3;
    Sint16 X1 = (Sint16)x;
    Sint16 Y1 = (Sint16)(y - D/2);
    Sint16 X2 = (Sint16)(x + 0.866025*D/2);
    Sint16 Y2 = (Sint16)(y + D/2);
    Sint16 X3 = (Sint16)(x - 0.866025*D/2);
    Sint16 Y3 = (Sint16)(y + D/2);
    filledTrigonColor(sdlRenderer, X1, Y1, X2, Y2, X3, Y3, color);
}

int all_soldiers_count_in_one_land(hexagonal map[HEXAGON_COUNT], int index) {
    int all_soldiers = (int)map[index].soldier;
    attacks_struct* local_attacks = map[index].attacks;
    while (local_attacks != NULL) {
        if (local_attacks->finished == false) {
            all_soldiers = all_soldiers + local_attacks->soldiers/SOLDIER_GENERATOR_RATE;
        }
        local_attacks = local_attacks->next_attack;
    }
    /*
    if (map[index].color_index != 8 && map[index].color_index != 9) {
        printf("You have %d soldiers in land %d that has %d color index\n", all_soldiers, index,map[index].color_index);
    }
    */
    return all_soldiers;

}

int power(int number) {

    int result = 1;
    for (int i=0; i<number; i++) {
        result = result * 10;
    }

    return result;
}

void int_to_str(char number_str[9], int number) {

    for (int i=0; i<9; i++) {
        number_str[i] = 0;
    }

    if (number == 0) {
        number_str[0] = '0';
        return;
    }

    if (number < 0) {
        number = -number;
    }

    int temp_score = number;
    int digit_counter = 0;

    while (temp_score != 0) {
        temp_score = temp_score/10;
        digit_counter++;
    }

    int counter = 0;
    int temp;

    for (int i=0; i<digit_counter; i++) {
        temp = number/(power(digit_counter-i-1));
        number_str[i] = temp + '0';
        number = number%(power(digit_counter-i-1));
    }


    return;
}



// random map generator
void file_name_change(char file_name[7], int file_name_counter) {

    file_name[2] = file_name_counter%10 + '0';
    file_name_counter = file_name_counter/10;
    file_name[1] = file_name_counter%10 + '0';
    file_name_counter = file_name_counter/10;
    file_name[0] = file_name_counter + '0';

    return;
}

int find_in_players_first_state_by_index(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int index) {


    for (int i=0; i<number_of_players; i++) {
        if (i == index) {
            continue;
        }
        if (players_first_state[i][0] == X && players_first_state[i][1] == Y) {
            return 1;
        }
    }

    return 0;
}

void players_first_state_generator(int number_of_players, int players_first_state[number_of_players][2]) {

    for (int i=0; i<number_of_players; i++) {
        players_first_state[i][0] = 0;
        players_first_state[i][1] = 0;
    }

    for (int i=0; i<number_of_players; i++) {
        players_first_state[i][0] = ((rand())%10000)%(CELL_NUM-2) + 1;
        players_first_state[i][1] = ((rand())%10000)%(CELL_NUM-2) + 1;
        while (find_in_players_first_state_by_index(number_of_players, players_first_state, players_first_state[i][0], players_first_state[i][1], i) != 0) {
            players_first_state[i][0] = (players_first_state[i][0] + 1)%(CELL_NUM-2) + 1;
            players_first_state[i][1] = (players_first_state[i][1] + 1)%(CELL_NUM-2) + 1;
        }
    }
    return;
}

int find_in_players_first_state(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int flag) {

    for (int i=0; i<number_of_players; i++) {
        if (players_first_state[i][0] == X && players_first_state[i][1] == Y) {
            if ((flag == 0 && players_first_state[i][0]%2 == 0) || (flag == 1 && players_first_state[i][0]%2 == 1)) {
                return i+1;
            }
        }
    }
    return 0;
}

void map_generator(int number_of_players, int players_first_state[number_of_players][2], FILE* map_ptr) {

    int xcounter = 0;
    int ycounter = 0;

    int i = 0;
    int j = 0;

    int find = 0;

    int flag = 0;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        j=0;

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {

            // borders
            if (xcounter==0 || ycounter<(HEXAGON_h/2+1) || abs(SCREEN_HEIGHT-ycounter)<=(HEXAGON_h/2 + 1) || abs(SCREEN_WIDTH-xcounter)<=(HEXAGON_h/2 + 1)) {
                fprintf(map_ptr, "%d %d %d %d\n", 9, 0, xcounter, ycounter);
                ycounter = ycounter + HEXAGON_h;
                j++;
                continue;
            }


            // players
            find = find_in_players_first_state(number_of_players, players_first_state, 2*i, j, flag);
            if (find != 0) {
                printf ("Player-A!\n");
                fprintf(map_ptr, "%d %d %d %d\n", find-1, (rand())%30 + 51, xcounter, ycounter);
                ycounter = ycounter + HEXAGON_h;
                j++;
                continue;
            }

            // gray and black lands
            if (((rand())%1000)%(7 - (CELL_NUM/number_of_players)%4) == 0) {
                fprintf(map_ptr, "%d %d %d %d\n", 9, 0, xcounter, ycounter);
            }
            else {
                fprintf(map_ptr, "%d %d %d %d\n", 8, 65, xcounter, ycounter);
            }
            j++;
            ycounter = ycounter + HEXAGON_h;

        }

        i++;

        ycounter = 0;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }

    flag = 1;


    i = 0;

    xcounter = HEXAGON_SIDE + HEXAGON_SIDE/2;
    ycounter = HEXAGON_h/2;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        j = 0;

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {

            // borders
            if (xcounter==0 || ycounter<(HEXAGON_h/2+1) || abs(SCREEN_HEIGHT-ycounter)<=(HEXAGON_h/2 + 1) || abs(SCREEN_WIDTH-xcounter)<=(HEXAGON_h/2 + 1)) {
                fprintf(map_ptr, "%d %d %d %d\n", 9, 0, xcounter, ycounter);
                ycounter = ycounter + HEXAGON_h;
                j++;
                continue;
            }


            // players
            find = find_in_players_first_state(number_of_players, players_first_state, 2*i-1, j, flag);
            if (find != 0) {
                printf ("Player-B!\n");
                fprintf(map_ptr, "%d %d %d %d\n", find-1, (rand())%30 + 51, xcounter, ycounter);
                ycounter = ycounter + HEXAGON_h;
                j++;
                continue;
            }

            // gray and black lands
            if (((rand())%1000)%(7 - (CELL_NUM/number_of_players)%4) == 0) {
                fprintf(map_ptr, "%d %d %d %d\n", 9, 0, xcounter, ycounter);
            }
            else {
                fprintf(map_ptr, "%d %d %d %d\n", 8, 65, xcounter, ycounter);
            }

            j++;
            ycounter = ycounter + HEXAGON_h;

        }

        i++;

        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }
    return;
}

int count_hexagons() {
    int hexagon_counter = 0;
    int xcounter = 0;
    int ycounter = 0;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {

            ycounter = ycounter + HEXAGON_h;
            hexagon_counter++;
        }

        ycounter = 0;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }

    xcounter = HEXAGON_SIDE + HEXAGON_SIDE/2;
    ycounter = HEXAGON_h/2;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {

            ycounter = ycounter + HEXAGON_h;
            hexagon_counter++;
        }
        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;
    }

    return hexagon_counter;
}

void random_map_generator(int cell_num, int number_of_players, int* map_number) {

    char file_name[] = "000.txt";
    int file_name_counter = 0;
    FILE* map_ptr = fopen(file_name, "r");

    while (map_ptr != NULL) {
        file_name_counter++;
        fclose(map_ptr);
        file_name_change(file_name, file_name_counter);
        map_ptr = fopen(file_name, "r");
    }
    *(map_number) = file_name_counter;
    map_ptr = fopen(file_name, "w");

    srand(time(NULL));

    CELL_NUM = cell_num;
    HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
    HEXAGON_SIDE = (int)(HEXAGON_h/(2*0.866025))/1 + 1;


    int players_first_state[number_of_players][2];
    players_first_state_generator(number_of_players, players_first_state);

    HEXAGON_COUNT = count_hexagons();
    fprintf(map_ptr, "%d\n", CELL_NUM);
    fprintf(map_ptr, "%d\n", HEXAGON_COUNT);
    fprintf(map_ptr, "%d\n", number_of_players);


    map_generator(number_of_players, players_first_state, map_ptr);

    printf ("%d\n", number_of_players);

    for (int i=0; i<number_of_players; i++) {
        printf ("%d - %d \n", players_first_state[i][0], players_first_state[i][1]);
    }

    fclose(map_ptr);
    return;
}

// drawing map
void drawShape(SDL_Renderer * sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, Uint32 map_colors[10]) {

    if (hexagon_color == map_colors[0] || hexagon_color == map_colors[5] || hexagon_color == map_colors[8]) {
        drawCircle(sdlRenderer, xcounter, ycounter, hexagon_color);
        return;
    }
    else if (hexagon_color == map_colors[1] || hexagon_color == map_colors[3] || hexagon_color == map_colors[6]) {
        drawBox(sdlRenderer, xcounter, ycounter, hexagon_color);
        return;
    }
    else {
        drawTri(sdlRenderer, xcounter, ycounter, hexagon_color);
    }
    return;
}

void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, int soldiers, Uint32 map_colors[10], bool potion) {

    char number_of_soldiers[9] = {0};
    int_to_str(number_of_soldiers, soldiers);

    Sint16 X[6] = {(Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter+HEXAGON_SIDE), (Sint16)(xcounter+HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE/2), (Sint16)(xcounter-HEXAGON_SIDE), (Sint16)(xcounter-HEXAGON_SIDE/2)};
    Sint16 Y[6] = {(Sint16)(ycounter+HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter-HEXAGON_SIDE*0.866025), (Sint16)(ycounter), (Sint16)(ycounter+HEXAGON_SIDE*0.866025)};
    if (hexagon_color != 0xffffff00) {
        filledPolygonColor(sdlRenderer, X, Y, 6, hexagon_color - 0x4f000000);
        drawShape(sdlRenderer, xcounter, ycounter, hexagon_color, map_colors);
        if (potion == true) {
            circleColor(sdlRenderer, xcounter, ycounter, HEXAGON_SIDE/2, 0xff00ffff);
        }
        stringColor(sdlRenderer, xcounter-8, ycounter+15, number_of_soldiers, 0xff000000);

    }
    else {
        filledPolygonColor(sdlRenderer, X, Y, 6, hexagon_color);
    }

    //X[0]+=1; X[1]+=1; X[2]+=1; X[3]-=1; X[4]-=1; X[5]-=1;
    //Y[0]+=1; Y[2]-=1; Y[3]-=1; Y[4]+=1;
    //polygonColor(sdlRenderer, X, Y, 6, hexagon_color);
}

void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 map_colors[10]) {
    int xcounter = 0;
    int ycounter = 0;
    int hexagon_counter = 0;
    bool potion = false;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            if (map[hexagon_counter].potion_id > 0) {
                //printf ("%d\n", map[hexagon_counter].potion_id);
                potion = true;
            }
            drawHexagon(sdlRenderer, xcounter, ycounter, map_colors[map[hexagon_counter].color_index],all_soldiers_count_in_one_land(map, hexagon_counter), map_colors, potion);
            //printf ("X is: %d , Y is: %d\n", xcounter, ycounter);
            hexagon_counter++;
            ycounter = ycounter + HEXAGON_h;
            potion = false;
        }
        ycounter = 0;
        xcounter = xcounter + 3*HEXAGON_SIDE;
    }

    xcounter = HEXAGON_SIDE + HEXAGON_SIDE/2;
    ycounter = HEXAGON_h/2;

    while (xcounter < (SCREEN_WIDTH+(HEXAGON_h/2))) {

        while (ycounter<(SCREEN_HEIGHT+(HEXAGON_h/2))) {
            if (map[hexagon_counter].potion_id > 0) {
                //printf ("%d\n", map[hexagon_counter].potion_id);
                potion = true;
            }
            drawHexagon(sdlRenderer, xcounter, ycounter, map_colors[map[hexagon_counter].color_index],all_soldiers_count_in_one_land(map, hexagon_counter), map_colors, potion);
            //printf("X is: %d , Y is: %d\n", xcounter, ycounter);
            hexagon_counter++;
            ycounter = ycounter + HEXAGON_h;
            potion = false;
        }

        ycounter = HEXAGON_h/2;
        xcounter = xcounter + 3*HEXAGON_SIDE;

    }

    //printf ("Hexagons = %d\n", hexagon_counter);
}



int mod(int a, int b) {
    return a % b;
}
