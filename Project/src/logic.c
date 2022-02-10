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

int potion_generator(SDL_Renderer * sdlRenderer, int* X_of_potion, int* Y_of_potion, hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix [SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {

    int random_number = (rand()%1000);
    if (random_number%300 != 0) {
        return 0;
    }


    int line_of_potion = rand()%CELL_NUM;
    *(X_of_potion) = (rand()%(SCREEN_WIDTH-250) + 125)/MAIN_MATRIX_UNITS_COUNT;
    *(Y_of_potion) = (map[line_of_potion].Y+HEXAGON_h/2)/MAIN_MATRIX_UNITS_COUNT;
    if (*(Y_of_potion) > 450/MAIN_MATRIX_UNITS_COUNT || *(Y_of_potion) < 150/MAIN_MATRIX_UNITS_COUNT) {
        return 0;
    }


    //printf("Potion is in X = %d Y = %d\n", *(X_of_potion), *(Y_of_potion));

    if (map[X_Y_to_hexagon((*(X_of_potion))*MAIN_MATRIX_UNITS_COUNT, (*(Y_of_potion))*MAIN_MATRIX_UNITS_COUNT)-1].color_index == 9) {
        return 0;
    }

    for (int i=(*X_of_potion)-5; i<=(*X_of_potion)+5; i++) {
        for (int j=(*Y_of_potion)-5; j<=(*Y_of_potion)+5; j++) {
            if (i>0 && i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT && j>0 && j<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT) {
                if (main_matrix[i][j].Potion == true) {
                    return 0;
                }
            }
        }
    }

    return 1;

}

int AI(int X_s, int Y_s, hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, int myID) {

    //printf ("%d\n", myID);
    int smallest_Delta_D = 100*100;
    int Delta_D;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 9 || (map[i].X == X_s && map[i].Y == Y_s)) {
            continue;
        }
        if (map[i].color_index == map[myID].color_index && (map[i].X != X_s && map[i].Y != Y_s) && map[i].is_d == true && map[i].is_s == false) {
            *(X_dis) = map[i].X;
            *(Y_dis) = map[i].Y;
            return (X_Y_to_hexagon(*(X_dis), *(Y_dis)) - 1);
        }
        if (map[i].is_d == false && map[i].is_s == true && map[i].color_index != map[myID].color_index) {
            Delta_D = Delta_D_calculator(X_s, Y_s, map[i].X, map[i].Y);
            if (Delta_D < smallest_Delta_D) {
                *(X_dis) = map[i].X;
                *(Y_dis) = map[i].Y;
                smallest_Delta_D = Delta_D;
            }
        }

    }

    int temp_id = X_Y_to_hexagon(*(X_dis), *(Y_dis)) - 1;

    if (smallest_Delta_D != 100*100 && map[temp_id].color_index != 9) {
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        //printf ("%d - %d\n", map[X_Y_to_hexagon(*(X_dis), *(Y_dis))-1].color_index, X_Y_to_hexagon(*(X_dis), *(Y_dis))-1);
        return temp_id;
    }

    int my_soldiers = (int)map[myID].soldier;

    smallest_Delta_D = 150*150;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 9 || (map[i].X == X_s && map[i].Y == Y_s)) {
            continue;
        }
        if (map[i].is_d == false && map[i].color_index != 9 && map[i].color_index != map[myID].color_index && map[i].soldier < (float)my_soldiers) {
            Delta_D = Delta_D_calculator(X_s, Y_s, map[i].X, map[i].Y);
            if (Delta_D < smallest_Delta_D) {
                *(X_dis) = map[i].X;
                *(Y_dis) = map[i].Y;
                smallest_Delta_D = Delta_D;
            }
        }
        if (map[i].color_index == 0 && Delta_D<= (HEXAGON_h+30)*(HEXAGON_h+30) && map[i].soldier < (float)my_soldiers) {
            Delta_D = Delta_D_calculator(X_s, Y_s, map[i].X, map[i].Y);
            *(X_dis) = map[i].X;
            *(Y_dis) = map[i].Y;
            return X_Y_to_hexagon(*(X_dis), *(Y_dis)) - 1;
        }

    }

    if (smallest_Delta_D != 150*150) {
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        //printf ("%d - %d\n", map[X_Y_to_hexagon(*(X_dis), *(Y_dis))-1].color_index, X_Y_to_hexagon(*(X_dis), *(Y_dis))-1);
        return X_Y_to_hexagon(*(X_dis), *(Y_dis)) - 1;
    }

    if (map[X_Y_to_hexagon(X_s, Y_s - HEXAGON_h)-1].color_index != 9) {
        //printf ("Hi\n");
        *(X_dis) = X_s;
        *(Y_dis) = Y_s - HEXAGON_h;
        //printf ("X_dis is %d, Y_dis is %d but I'm %d and %d\n", *(X_dis), *(Y_dis), X_s, Y_s);
        return myID - 1;
    }
}

void filling_main_matrix_for_attacks(matrix_unit main_matrix[60][60], hexagonal map[HEXAGON_COUNT], int X_dis, int Y_dis, int i, int j, int X, int Y) {
    //printf ("%d - %d\n", map[X_Y_to_hexagon((X_dis), (Y_dis))-1].color_index, X_Y_to_hexagon((X_dis), (Y_dis))-1);
    //printf ("I'm for land %d with potion", map[i].color_index);
    main_matrix[X/10][Y/10].local_soldiers[j].X = (float)X;
    main_matrix[X/10][Y/10].local_soldiers[j].potion_id = map[i].potion_id;
    main_matrix[X/10][Y/10].local_soldiers[j].Y = (float)Y;
    main_matrix[X/10][Y/10].local_soldiers[j].land_id = map[i].color_index;
    main_matrix[X/10][Y/10].local_soldiers[j].Y_d = Y_dis;
    main_matrix[X/10][Y/10].local_soldiers[j].X_d = X_dis;
    float Delta_D = (float)pow(Delta_D_calculator(X, Y, X_dis, Y_dis), 0.5);
    main_matrix[X/10][Y/10].local_soldiers[j].V_x = (float)map[i].soldier_speed * ((float)(X_dis - X)/Delta_D);
    main_matrix[X/10][Y/10].local_soldiers[j].V_y = (float)map[i].soldier_speed * ((float)(Y_dis - Y)/Delta_D);
    //printf ("Speed is Vx = %f, Yy = %f\n", main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_x, main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_y);
    //printf (" %d\n", main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].potion_id);
}

void first_initialization_of_main_matrix(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {
    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {
            main_matrix[i][j].Potion = false;
            main_matrix[i][j].center_of_potion = false;
            main_matrix[i][j].local_potion.potion_id = -1;
            main_matrix[i][j].local_potion.potion_in_use = false;
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

void read_saved_main_matrix(FILE* saved_main_matrix_ptr, matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {

    int bool_temp1;
    int bool_temp2;
    int bool_temp3;
    int potion_time;
    int potion_id;
    float temp_x;
    float temp_y;
    int land_id;
    float temp_V_x;
    float temp_V_y;
    int temp_X_d;
    int temp_Y_d;
    int soldier_potion_id;

    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {
            fscanf(saved_main_matrix_ptr, "%d %d %d %d %d\n", &bool_temp1, &bool_temp2, &potion_time, &bool_temp3, &potion_id);
            main_matrix[i][j].Potion = bool_temp1;
            main_matrix[i][j].center_of_potion = bool_temp2;
            main_matrix[i][j].local_potion.potion_time = potion_time;
            main_matrix[i][j].local_potion.potion_in_use = bool_temp3;
            main_matrix[i][j].local_potion.potion_id = potion_id;
            for (int k=0; k<8; k++) {
                fscanf(saved_main_matrix_ptr, "%f %f %d %f %f %d %d %d\n", &temp_x, &temp_y, &land_id, &temp_V_x, &temp_V_y, &temp_X_d, &temp_Y_d, &soldier_potion_id);
                main_matrix[i][j].local_soldiers[k].X = temp_x;
                main_matrix[i][j].local_soldiers[k].Y = temp_y;
                main_matrix[i][j].local_soldiers[k].land_id = land_id;
                main_matrix[i][j].local_soldiers[k].V_x = temp_V_x;
                main_matrix[i][j].local_soldiers[k].V_y = temp_V_y;
                main_matrix[i][j].local_soldiers[k].X_d = temp_X_d;
                main_matrix[i][j].local_soldiers[k].Y_d = temp_Y_d;
                main_matrix[i][j].local_soldiers[k].potion_id = soldier_potion_id;
            }
        }
    }
    return;
}

void soldier_generator_in_attacks(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i) {
    attacks_struct* local_attacks = map[i].attacks;
    int count = 0;
    int Delta_D;
    int Delta_x;
    int Delta_y;
    while (local_attacks != NULL) {
        count++;
        //printf ("%d is not NULL\n", map[i].color_index);
        if (local_attacks->finished == false) {

            local_attacks->soldiers--;
            Delta_D = (int)pow(Delta_D_calculator(local_attacks->X_dis, local_attacks->Y_dis, map[i].X, map[i].Y), 0.5);
            Delta_x = -(int)(((float)((float)((local_attacks->Y_dis - map[i].Y))/(float)Delta_D))*(float)(HEXAGON_SIDE/4));
            Delta_y = (int)(((float)((float)((local_attacks->X_dis - map[i].X))/(float)Delta_D))*(float)(HEXAGON_SIDE/4));
            //printf ("%d - %d - %d \n", Delta_D, Delta_x, Delta_y);
            if (local_attacks->soldiers % (SOLDIER_GENERATOR_RATE) == 0) {             //// generating first soldier
                //printf ("You have %d soldiers in %d land\n", local_attacks->soldiers/10, i);
                for (int j = 0; j < 8; j++) {
                    if (main_matrix[map[i].X / MAIN_MATRIX_UNITS_COUNT][map[i].Y /MAIN_MATRIX_UNITS_COUNT].local_soldiers[j].X ==0) {
                        //printf("You are going to X = %d, Y = %d\n", local_attacks->X_dis, local_attacks->Y_dis);
                        //count++;
                        //printf ("Count is: %d\n", count);
                        filling_main_matrix_for_attacks(main_matrix, map, local_attacks->X_dis,local_attacks->Y_dis, i, j, map[i].X, map[i].Y);
                        map[local_attacks->id_dis].is_d = true;
                        break;
                    }
                }

                if (local_attacks->soldiers > 0) {
                    local_attacks->soldiers-= SOLDIER_GENERATOR_RATE;
                    for (int j = 0; j < 8; j++) { ////// generating second soldier
                        if (main_matrix[(map[i].X+Delta_x) / MAIN_MATRIX_UNITS_COUNT][(map[i].Y+Delta_y) /MAIN_MATRIX_UNITS_COUNT].local_soldiers[j].X ==0) {
                            //printf("You are going to X = %d, Y = %d\n", local_attacks->X_dis, local_attacks->Y_dis);
                            //count++;
                            //printf ("Count is: %d\n", count);
                            filling_main_matrix_for_attacks(main_matrix, map, (local_attacks->X_dis + Delta_x),(local_attacks->Y_dis + Delta_y), i, j, (map[i].X + Delta_x), (map[i].Y + Delta_y));
                            map[local_attacks->id_dis].is_d = true;
                            break;
                        }
                    }

                    if (local_attacks->soldiers > 0) {
                        local_attacks->soldiers-= SOLDIER_GENERATOR_RATE;
                        for (int j = 0; j < 8; j++) { //// generating third soldier
                            if (main_matrix[(map[i].X-Delta_x) / MAIN_MATRIX_UNITS_COUNT][(map[i].Y-Delta_y) /MAIN_MATRIX_UNITS_COUNT].local_soldiers[j].X ==0) {
                                //printf("You are going to X = %d, Y = %d\n", local_attacks->X_dis, local_attacks->Y_dis);
                                //count++;
                                //printf ("Count is: %d\n", count);
                                filling_main_matrix_for_attacks(main_matrix, map, (local_attacks->X_dis - Delta_x),(local_attacks->Y_dis - Delta_y), i, j, (map[i].X - Delta_x), (map[i].Y - Delta_y));
                                map[local_attacks->id_dis].is_d = true;
                                break;
                            }
                        }
                    }
                }
            }
                //printf ("You have %d soldiers in %d land\n", local_attacks->soldiers/10, i);
                //printf ("You have %d soldiers in %d land\n", local_attacks->soldiers/10, i);
            if (local_attacks->soldiers <= 0) {
                local_attacks->finished = true;
                map[local_attacks->id_dis].is_d = false;
            }
        }
        local_attacks = local_attacks->next_attack;
    }

    return;
}

void set_new_attack(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i, int* X_dis, int* Y_dis, int id_dis) {
    //printf ("Attack\n");
    attacks_struct* local_attacks;
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
    local_attacks->soldiers = SOLDIER_GENERATOR_RATE*map[i].soldier;
    map[i].soldier = 0;
    local_attacks->Y_dis = *(Y_dis);
    local_attacks->X_dis = *(X_dis);
    local_attacks->id_dis = id_dis;
    return;
}



void potion_add_to_conquered_land(hexagonal map[HEXAGON_COUNT], int id_dis, soldier local_soldiers) {

    if (local_soldiers.potion_id != 1) {
        map[id_dis].soldier_speed = 2;
    }
    else if (local_soldiers.potion_id != 5) {
        map[id_dis].rate = 0.1;
    }

    //printf ("Hi, I'm a soldier from land %d with potion %d\n", local_soldiers.land_id, local_soldiers.potion_id);
    if (local_soldiers.potion_id <1) {
        map[id_dis].potion_id = -1;
        map[id_dis].land_potion.potion_id = -1;
        map[id_dis].land_potion.potion_in_use = false;
        return;
    }
    bool potion = false;
    int index = 0;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == local_soldiers.land_id) {
            if (map[i].potion_id > 0) {
                potion = true;
                index = i;
            }
            break;
        }
    }


    if (potion == true) {
        /*
        map[id_dis].potion_id = local_soldiers.potion_id;
        map[id_dis].land_potion.potion_id = (short) local_soldiers.potion_id;
        */
        map[id_dis].land_potion.potion_in_use = true;
        if (index == 0) {
            map[id_dis].potion_id = local_soldiers.potion_id;
            map[id_dis].land_potion.potion_id = (short) local_soldiers.potion_id;
            map[id_dis].land_potion.potion_time = 1000;
            return;
        }
        map[id_dis].potion_id = map[index].potion_id;
        map[id_dis].land_potion.potion_id = map[index].potion_id;
        map[id_dis].land_potion.potion_time = map[index].land_potion.potion_time;
    }
    return;

    //map[id_dis].land_potion.potion_time = map[index].land_potion.

}

void battle(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i, int j, int k) {
    int id_dis = X_Y_to_hexagon(main_matrix[i][j].local_soldiers[k].X_d, main_matrix[i][j].local_soldiers[k].Y_d);
    id_dis--;
    //printf ("Hi, I'm in %d with %f soldiers\n", id_dis, map[id_dis].soldier);
    if (map[id_dis].color_index == main_matrix[i][j].local_soldiers[k].land_id) {
        //printf ("%d - %d\n", map[id_dis].color_index, main_matrix[i][j].local_soldiers[k].land_id);
        map[id_dis].soldier++;
        return;
    }
    if (map[id_dis].color_index != main_matrix[i][j].local_soldiers[k].land_id && map[id_dis].potion_id == 4) {
        map[id_dis].soldier++;
        return;
    }
    int all_soldiers = all_soldiers_count_in_one_land(map, id_dis);
    if (all_soldiers > 0) {
        //printf ("2\n");
        if (main_matrix[i][j].local_soldiers[k].potion_id == 22) {
            //printf ("I'm potion 2\n");
            map[id_dis].soldier -=2;
        }
        else if (main_matrix[i][j].local_soldiers[k].potion_id == 21) {
            //printf ("I'm potion 2\n");
            map[id_dis].soldier -= 0.5;
        }
        else {
            map[id_dis].soldier--;
        }
        return;
    }
    //printf ("3\n");
    //printf ("%d - %d\n", map[id_dis].color_index, main_matrix[i][j].local_soldiers[k].land_id);
    map[id_dis].color_index = main_matrix[i][j].local_soldiers[k].land_id;
    //printf ("%d - %d\n", map[id_dis].color_index, main_matrix[i][j].local_soldiers[k].land_id);
    map[id_dis].soldier = 1;
    map[id_dis].is_d = false;
    map[id_dis].is_s = false;
    potion_add_to_conquered_land(map, id_dis, main_matrix[i][j].local_soldiers[k]);
    //map[id_dis].
    attacks_struct* local_attacks = map[id_dis].attacks;
    /*
    attacks_struct* temp_attack;
    while (local_attacks->next_attack != NULL) {
        temp_attack = local_attacks->next_attack;
        free(local_attacks);
        local_attacks = temp_attack;
    }
    */

    while (local_attacks != NULL) {
        local_attacks->finished = true;
        local_attacks = local_attacks->next_attack;
    }
    return;

}

bool is_attack_source_valid(hexagonal map[HEXAGON_COUNT], int mouse_X, int mouse_Y, int* id_s_user) {
    int id_s = X_Y_to_hexagon(mouse_X, mouse_Y);
    id_s--;
    *(id_s_user) = id_s;

    if (map[id_s].color_index == 0) {
        return true;
    }

    return false;

}

bool is_attack_dis_valid(hexagonal map[HEXAGON_COUNT], int mouse_X, int mouse_Y, int id_s_user, int* id_d_user) {
    int id_d = X_Y_to_hexagon(mouse_X, mouse_Y);
    id_d--;
    *(id_d_user) = id_d;

    if (map[id_d].color_index == 9 || id_s_user == *(id_d_user)) {
        return false;
    }

    return true;

}

bool game_end_check(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], bool* game_finished) {

    int user_land_count = 0;
    int comp_land_count = 0;
    int user_soldier_count = 0;
    int comp_soldier_count = 0;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 0) {
            user_land_count++;
        }
        else if(map[i].color_index != 8 && map[i].color_index != 9) {
            comp_land_count++;
        }
    }

    if (comp_land_count * user_land_count != 0) {
        *(game_finished) = false;
        return false;
    }
    for (int i=0; i<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; j++) {
            for (int k=0; k<8; k++) {
                if (main_matrix[i][j].local_soldiers[k].X != 0) {
                    if (main_matrix[i][j].local_soldiers[k].land_id == 0) {
                        user_soldier_count++;
                    }
                    else if(main_matrix[i][j].local_soldiers[k].land_id != 8 && main_matrix[i][j].local_soldiers[k].land_id != 9) {
                        comp_soldier_count++;
                    }
                }
            }
        }
    }

    if (comp_land_count == 0 && comp_soldier_count == 0) {
        *(game_finished) = true;
        return true;
    }
    else if (user_land_count == 0 && user_soldier_count == 0) {
        *(game_finished) = true;
        return false;
    }
    *(game_finished) = false;
    return false;
}

bool find_new_username_in_scoreboard(struct users_in_score_board* users_head_ptr, int user_counter, char username[100]) {
    if (users_head_ptr == NULL) {
        return true;
    }

    score_board_struct* temp_i;
    temp_i = users_head_ptr;

    for (int i=0; i<user_counter; i++) {
        if (strcmp(temp_i->name, username) == 0) {
            return false;
        }
        temp_i = temp_i->next_user;

    }

    free(temp_i);

    return true;
}

void add_score(struct users_in_score_board* users_head_ptr, int user_counter, char username[100], bool user_won) {

    score_board_struct* temp_i;
    temp_i = users_head_ptr;

    char scoreboard_name[] = "scoreboard.txt";

    FILE* file_ptr = fopen(scoreboard_name, "w");

    for (int i=0; i<user_counter; i++) {
        fprintf(file_ptr, "%s\n", temp_i->name);
        if (strcmp(temp_i->name, username) == 0) {
            if (user_won == true) {
                switch(CELL_NUM) {
                    case 5:
                        temp_i->score+=45;
                        break;
                    case 6:
                        temp_i->score+=60;
                        break;
                    case 7:
                        temp_i->score+=75;
                        break;
                    case 8:
                        temp_i->score+=90;
                        break;
                    default:
                        temp_i->score+=105;
                }
            }
            else {
                temp_i->score-=30;
            }
        }
        fprintf(file_ptr, "%d\n", temp_i->score);
        temp_i = temp_i->next_user;

    }

    fclose(file_ptr);

    free(temp_i);

    return;
}

bool check_potion_in_use(int land_id_of_soldier, hexagonal map[HEXAGON_COUNT]) {
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == land_id_of_soldier) {
            if (map[i].potion_id != -1) {
                return true;
            }
            else {
                return false;
            }
        }
    }
}

void add_potion_to_land_and_soldiers(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], matrix_unit main_matrix_temp[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int land_id_of_soldier, int index_i, int index_j) {

    // add to lands
    int potion_id = main_matrix[index_i][index_j].local_potion.potion_id;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == land_id_of_soldier) {
            map[i].land_potion.potion_time = main_matrix[index_i][index_j].local_potion.potion_time;
            map[i].land_potion.potion_id = (short)potion_id;
            map[i].land_potion.potion_in_use = true;
            map[i].potion_id = potion_id;
            if (potion_id == 1) {
                map[i].soldier_speed = 4;
            }
            else if (potion_id == 5) {
                map[i].rate = 0.2;
            }
        }
    }

    // add to soldiers
    for (int i=0; i<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; j++) {
            for (int k=0; k<8; k++) {
                // adding to soldiers in main matrix
                if (main_matrix[i][j].local_soldiers[k].X != 0 && main_matrix[i][j].local_soldiers[k].land_id == land_id_of_soldier) {
                    if (potion_id == 1) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 1;
                        main_matrix[i][j].local_soldiers[k].V_x*=2;
                        main_matrix[i][j].local_soldiers[k].V_y*=2;
                    }
                    else if (potion_id == 22) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 22;
                    }
                    else if (potion_id == 21) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 21;
                    }
                    else if (potion_id == 3) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 3;
                    }
                    else if (potion_id == 4) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 4;
                    }
                    else if (potion_id == 5) {
                        main_matrix[i][j].local_soldiers[k].potion_id = 5;
                    }
                }
                // adding to soldiers in main matrix temp
                if (main_matrix_temp[i][j].local_soldiers[k].X != 0 && main_matrix_temp[i][j].local_soldiers[k].land_id == land_id_of_soldier) {
                    if (potion_id == 1) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 1;
                        main_matrix_temp[i][j].local_soldiers[k].V_x*=2;
                        main_matrix_temp[i][j].local_soldiers[k].V_y*=2;
                    }
                    else if (potion_id == 22) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 22;
                    }
                    else if (potion_id == 21) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 21;
                    }
                    else if (potion_id == 3) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 3;
                    }
                    else if (potion_id == 4) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 4;
                    }
                    else if (potion_id == 5) {
                        main_matrix_temp[i][j].local_soldiers[k].potion_id = 5;
                    }
                }
            }
        }
    }


    // free potion
    // find center of potion
    int x_center;
    int y_center;
    for (int i=index_i-1; i<=index_i+1; i++) {
        for (int j=index_j-1; j<=index_j+1; j++) {
            if (i>0 && i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT && j>0 && j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT) {
                if (main_matrix[i][j].center_of_potion == true) {
                    x_center = i;
                    y_center = j;
                    break;
                }
            }
        }
    }
    // free all of potion
    for (int i=x_center-1; i<=x_center+1; i++) {
        for (int j=y_center-1; j<=y_center+1; j++) {
            if (i>0 && i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT && j>0 && j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT) {
                //printf ("| %d - %d |", 10*i+5, 10*j+5);
                main_matrix[i][j].Potion = false;
                main_matrix[i][j].center_of_potion = false;
                main_matrix[i][j].local_potion.potion_id = -1;
            }
        }
    }

    for (int i=x_center-1; i<=x_center+1; i++) {
        for (int j=y_center-1; j<=y_center+1; j++) {
            if (i>0 && i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT && j>0 && j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT) {
                //printf ("| %d| ",main_matrix[i][j].Potion);
            }
        }
    }
    //printf ("\n");


    /*
    main_matrix[index_i][index_j].Potion = false;
    main_matrix[index_i][index_j].center_of_potion = true;
    main_matrix[index_i][index_j].local_potion.potion_id = -1;
     */
    return;
}

void delete_potion_from_land_and_soldiers(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int land_id, int potion_id, int index) {


    map[index].land_potion.potion_time = 0;
    map[index].land_potion.potion_id = -1;
    map[index].land_potion.potion_in_use = false;
    map[index].potion_id = -1;

    if (potion_id == 1) {
        map[index].soldier_speed = 2;
    }
    else if (potion_id == 5) {
        map[index].rate = 0.1;
    }

    // delete from soldiers
    int land_id_of_soldier = map[index].color_index;
    for (int i=0; i<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; j++) {
            for (int k=0; k<8; k++) {
                if (main_matrix[i][j].local_soldiers[k].X != 0 && main_matrix[i][j].local_soldiers[k].land_id == land_id_of_soldier && main_matrix[i][j].local_soldiers[k].potion_id != -1) {
                    if (potion_id == 1) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                        main_matrix[i][j].local_soldiers[k].V_x/=2;
                        main_matrix[i][j].local_soldiers[k].V_y/=2;
                    }
                    else if (potion_id == 22) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                    }
                    else if (potion_id == 21) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                    }
                    else if (potion_id == 3) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                    }
                    else if (potion_id == 4) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                    }
                    else if (potion_id == 5) {
                        main_matrix[i][j].local_soldiers[k].potion_id = -1;
                    }
                }
            }
        }
    }


    return;

}


void add_potion_to_main_matrix_as_index(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int X_of_potion, int Y_of_potion) {

    main_matrix[X_of_potion][Y_of_potion].center_of_potion = true;
    int local_potion_id = rand()%5+1;
    //main_matrix[i][j].local_potion.potion_time = rand()%100000 + 1000;
    int local_potion_time = rand()%1000 + 1000;
    if (local_potion_id == 2) {
        int first_digit = rand()%2+1;
        local_potion_id = local_potion_id*10+first_digit;
    }
    //printf("Potion was generated in location %d and %d with id", 10 * X_of_potion + 5, 10 * Y_of_potion + 5);
    for (int i=X_of_potion-1; i<=X_of_potion+1; i++) {
        for (int j=Y_of_potion-1; j<=Y_of_potion+1; j++) {
            if (i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT && i>0 && j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT && j>0) {
                main_matrix[i][j].Potion = true;
                main_matrix[i][j].local_potion.potion_in_use = false;
                main_matrix[i][j].local_potion.potion_id = local_potion_id;
                main_matrix[i][j].local_potion.potion_time = local_potion_time;

            }
        }
    }
    //printf (" %d\n", local_potion_id);

    return;
}



/*
 main_matrix[X_of_potion][Y_of_potion].Potion = true;
    printf("Potion was generated in location %d and %d \n", 10 * X_of_potion + 5, 10 * Y_of_potion + 5);
    main_matrix[X_of_potion][Y_of_potion].local_potion.potion_in_use = false;
    //main_matrix[X_of_potion][Y_of_potion].local_potion.potion_id = rand()%5 + 1;
    main_matrix[X_of_potion][Y_of_potion].local_potion.potion_id = 1;
    //main_matrix[X_of_potion][Y_of_potion].local_potion.potion_time = rand()%100000 + 1000;
    main_matrix[X_of_potion][Y_of_potion].local_potion.potion_time = 1000;
 */


void soldier_adding_and_attack_managing(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {

    int soldier_count_in_lands = 0;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        soldier_count_in_lands = all_soldiers_count_in_one_land(map, i);
        if (map[i].color_index != 9 && map[i].color_index != 8 && soldier_count_in_lands < 150) {
            // lands soldier adding
            if (soldier_count_in_lands < 150 && (soldier_count_in_lands + map[i].rate) > 150) {
                map[i].soldier = 150 - (soldier_count_in_lands - map[i].soldier);
            } else {
                map[i].soldier += map[i].rate;
            }

        }
        else if (map[i].color_index != 9 && map[i].color_index != 8 && soldier_count_in_lands >= 150 && map[i].land_potion.potion_id == 3) {
            map[i].soldier += map[i].rate;
        }
        // land potion time managing
        if (map[i].potion_id > 0) {
            map[i].land_potion.potion_time -= 10;
            if (map[i].land_potion.potion_time <= 0) {
                //printf ("my_potion_finished ");
                delete_potion_from_land_and_soldiers(map, main_matrix, map[i].color_index, map[i].potion_id, i);
                //printf ("%d\n", map[i].potion_id);
            }
        }
        if (map[i].potion_id != -1) {
            //printf ("I have potion\n");
        }
        // attacks managing in lands
        if (map[i].color_index != 9 && map[i].color_index != 8) {
            soldier_generator_in_attacks(map, main_matrix, i);
        }



    }
}

void computer_attacks(hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {

    int id_dis;
    for (int i=0; i<HEXAGON_COUNT; i++) {
        if (map[i].color_index == 9 || map[i].color_index == 8 || map[i].color_index == 0) {
            continue;
        }
        if (rand()%500 == 50) {
            id_dis = AI(map[i].X, map[i].Y, map, X_dis, Y_dis, i);
            if (map[id_dis].color_index != 9 && *X_dis > 5 && *Y_dis > 5) {
                set_new_attack(map, main_matrix, i, X_dis, Y_dis, id_dis);
            }
        }
    }
}


void main_matrix_conflict_checking(matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], matrix_unit main_matrix_temp[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]) {
    int conflict_checking = -1;
    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {
            //main_matrix_temp[i][j].Potion = main_matrix[i][j].Potion;
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
                        main_matrix_temp[(int)main_matrix[i][j].local_soldiers[k].X/10][(int)main_matrix[i][j].local_soldiers[k].Y/10].local_soldiers[conflict_checking].potion_id = main_matrix[i][j].local_soldiers[k].potion_id;
                        main_matrix[i][j].local_soldiers[k].X = 0;
                    }
                }
                conflict_checking = -1;
            }
        }
    }
}

void main_matrix_temp_copy(matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], matrix_unit main_matrix_temp[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], hexagonal map[HEXAGON_COUNT]) {
    bool free_unit = true;
    bool potion_in_use = true;
    int potion_count = 0;
    int land_id_of_soldier = 9;
    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j = 0; j < SCREEN_HEIGHT / MAIN_MATRIX_UNITS_COUNT; j++) {
            free_unit = true;
            potion_in_use = true;
            land_id_of_soldier = 9;
            //main_matrix[i][j].Potion = main_matrix_temp[i][j].Potion;
            for (int k=0; k<8; k++) {
                main_matrix[i][j].local_soldiers[k].V_y = main_matrix_temp[i][j].local_soldiers[k].V_y;
                main_matrix[i][j].local_soldiers[k].V_x = main_matrix_temp[i][j].local_soldiers[k].V_x;
                main_matrix[i][j].local_soldiers[k].Y = main_matrix_temp[i][j].local_soldiers[k].Y;
                main_matrix[i][j].local_soldiers[k].X = main_matrix_temp[i][j].local_soldiers[k].X;
                main_matrix[i][j].local_soldiers[k].X_d = main_matrix_temp[i][j].local_soldiers[k].X_d;
                main_matrix[i][j].local_soldiers[k].Y_d = main_matrix_temp[i][j].local_soldiers[k].Y_d;
                main_matrix[i][j].local_soldiers[k].land_id = main_matrix_temp[i][j].local_soldiers[k].land_id;
                main_matrix[i][j].local_soldiers[k].potion_id = main_matrix_temp[i][j].local_soldiers[k].potion_id;
                if (main_matrix[i][j].local_soldiers[k].X != 0) {
                    free_unit = false;
                    land_id_of_soldier = main_matrix[i][j].local_soldiers[k].land_id;
                }
            }
            if (main_matrix[i][j].Potion == true) {
                potion_count++;
                if (free_unit == false) {
                    potion_in_use = check_potion_in_use(land_id_of_soldier, map);
                    if (potion_in_use == false) {
                        //printf("False\n");
                        //printf ("Potion %d was in %d and %d. Now its for land id %d\n", main_matrix[i][j].local_potion.potion_id, 10*i+5, 10*j+5, land_id_of_soldier);
                        add_potion_to_land_and_soldiers(map, main_matrix, main_matrix_temp, land_id_of_soldier, i, j);
                        main_matrix[i][j].Potion = false;
                        main_matrix[i][j].local_potion.potion_id = -1;
                    }
                }
            }

        }

    }
    first_initialization_of_main_matrix(main_matrix_temp);
}



// graphic of soldiers and potions
int drawSoldiers(SDL_Renderer * sdlRenderer, int x, int y, int d, Uint32 color, Uint32 map_colors[10]) {
    Sint16 X = (Sint16)x;
    Sint16 Y = (Sint16)y;
    if (color == map_colors[0] || color == map_colors[5]) {
        filledCircleColor(sdlRenderer, X, Y, d, color);
        return 0;
    }
    else if (color == map_colors[1] ||color == map_colors[3] || color == map_colors[6]) {
        boxColor(sdlRenderer, X-2*d/3, Y-2*d/3, X+2*d/3, Y+2*d/3, color);
        return 0;
    }
    else {
        Sint16 X1 = (Sint16)x;
        Sint16 Y1 = (Sint16)(y - d);
        Sint16 X2 = (Sint16)(x + 0.866025*d);
        Sint16 Y2 = (Sint16)(y + d);
        Sint16 X3 = (Sint16)(x - 0.866025*d);
        Sint16 Y3 = (Sint16)(y + d);
        filledTrigonColor(sdlRenderer, X1, Y1, X2, Y2, X3, Y3, color);
    }
    return 0;
}

void drawing_main_matrix(SDL_Renderer* sdlRenderer, matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], hexagonal map[HEXAGON_COUNT], Uint32 map_colors[10], Uint32 potion_colors[5]) {

    int potion_color_id;
    for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
        for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {

            // drawing potions
            if (main_matrix[i][j].Potion == true && main_matrix[i][j].center_of_potion == true) {
                potion_color_id = main_matrix[i][j].local_potion.potion_id - 1;
                if (potion_color_id > 5) { // potion id is 21 or 22
                    potion_color_id = 2 - 1; // (21 or 22)%10 - 1
                }
                filledCircleColor(sdlRenderer, (Sint16)(10*i+5), (Sint16)(10*j+5), 10, potion_colors[potion_color_id]);
            }

            /*
            if (main_matrix[i][j].Potion == true) {
                filledCircleColor(sdlRenderer, (Sint16)(10*i+5), (Sint16)(10*j+5), 5, 0xffffffff);
            }
             */


            // drawing soldiers and moving for next state
            for (int k=0; k<8; k++) {
                if (main_matrix[i][j].local_soldiers[k].X != 0) {
                    //filledCircleColor(sdlRenderer, 10*i+5, 10*j+5, 5, map_colors[main_matrix[i][j].local_soldiers[k].land_id]);
                    if (main_matrix[i][j].local_soldiers[k].potion_id == 22) {
                        drawSoldiers(sdlRenderer, main_matrix[i][j].local_soldiers[k].X/10*10+5, main_matrix[i][j].local_soldiers[k].Y/10*10+5, 7, map_colors[main_matrix[i][j].local_soldiers[k].land_id], map_colors); // 0xfff00fff Pink
                    }
                    else if (main_matrix[i][j].local_soldiers[k].potion_id == 21) {
                        drawSoldiers(sdlRenderer, main_matrix[i][j].local_soldiers[k].X/10*10+5, main_matrix[i][j].local_soldiers[k].Y/10*10+5, 3, map_colors[main_matrix[i][j].local_soldiers[k].land_id], map_colors); // 0xfff00fff Pink
                    }
                    else {
                        drawSoldiers(sdlRenderer, main_matrix[i][j].local_soldiers[k].X / 10 * 10 + 5,main_matrix[i][j].local_soldiers[k].Y / 10 * 10 + 5, 5,map_colors[main_matrix[i][j].local_soldiers[k].land_id],map_colors); // 0xfff00fff Pink
                    }
                    // checking if you arrived
                    if (pow(Delta_D_calculator(main_matrix[i][j].local_soldiers[k].X, main_matrix[i][j].local_soldiers[k].Y, main_matrix[i][j].local_soldiers[k].X_d, main_matrix[i][j].local_soldiers[k].Y_d), 0.5)<= HEXAGON_SIDE/3) {
                        battle(map, main_matrix, i, j, k);
                        //printf ("%d\n", map[X_Y_to_hexagon(main_matrix[i][j].local_soldiers[k].X, main_matrix[i][j].local_soldiers[k].Y)-1].color_index);
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
}