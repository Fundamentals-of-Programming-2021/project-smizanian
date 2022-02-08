//
// Created by smiza on 2/8/2022.
//

#ifndef MAIN_C_GLOBAL_VARIABLES_H
#define MAIN_C_GLOBAL_VARIABLES_H

// global variables
extern int FPS;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int CELL_NUM;
extern int HEXAGON_h;
extern int HEXAGON_SIDE;
extern int HEXAGON_COUNT;
extern int NUMBER_OF_PLAYERS;
extern int MAIN_MATRIX_UNITS_COUNT;
extern int SOLDIER_GENERATOR_RATE;

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

// Potions
typedef struct game_Potions{
    int potion_time;
    bool potion_in_use;
    short potion_id;
}potion;

// for map
typedef struct my_map_hexagonal{
    int color_index;
    float soldier;
    int X;
    int Y;
    int potion_id;
    potion land_potion;
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

// Soldier
typedef struct game_soldier{
    int potion_id;
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
    bool center_of_potion;
    potion local_potion;
    soldier local_soldiers[8];
}matrix_unit;
#endif //MAIN_C_GLOBAL_VARIABLES_H
