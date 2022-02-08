//
// Created by smiza on 2/8/2022.
//

#ifndef MAIN_C_LOGIC_H
#define MAIN_C_LOGIC_H

// logic
int Delta_D_calculator(int X1, int Y1, int X2, int Y2);
int X_Y_to_hexagon(int mouse_X, int mouse_Y);
int potion_generator(SDL_Renderer * sdlRenderer, int* X_of_potion, int* Y_of_potion, hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix [SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
int AI(int X_s, int Y_s, hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, int myID);
void filling_main_matrix_for_attacks(matrix_unit main_matrix[60][60], hexagonal map[HEXAGON_COUNT], int X_dis, int Y_dis, int i, int j);
void first_initialization_of_main_matrix(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
void soldier_generator_in_attacks(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i);
int drawSoldiers(SDL_Renderer * sdlRenderer, int x, int y, int d, Uint32 color, Uint32 map_colors[10]);
void set_new_attack(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i, int* X_dis, int* Y_dis, int id_dis);
void potion_add_to_conquered_land(hexagonal map[HEXAGON_COUNT], int id_dis, soldier local_soldiers);
void battle(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i, int j, int k);
bool is_attack_source_valid(hexagonal map[HEXAGON_COUNT], int mouse_X, int mouse_Y, int* id_s_user);
bool is_attack_dis_valid(hexagonal map[HEXAGON_COUNT], int mouse_X, int mouse_Y, int id_s_user, int* id_d_user);
bool game_end_check(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], bool* game_finished);
bool find_new_username_in_scoreboard(struct users_in_score_board* users_head_ptr, int user_counter, char username[100]);
void add_score(struct users_in_score_board* users_head_ptr, int user_counter, char username[100], bool user_won);
void user_w_or_l_menu(SDL_Renderer * sdlUserWonRenderer, bool user_won);
bool check_potion_in_use(int land_id_of_soldier, hexagonal map[HEXAGON_COUNT]);
void add_potion_to_land_and_soldiers(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int land_id_of_soldier, int index_i, int index_j);
void delete_potion_from_land_and_soldiers(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int land_id, int potion_id, int index);
void add_potion_to_main_matrix_as_index(matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int X_of_potion, int Y_of_potion);
void soldier_adding_and_attack_managing(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
void computer_attacks(hexagonal map[HEXAGON_COUNT], int* X_dis, int* Y_dis, matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
void main_matrix_conflict_checking(matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], matrix_unit main_matrix_temp[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);
void main_matrix_temp_copy(matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], matrix_unit main_matrix_temp[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], hexagonal map[HEXAGON_COUNT]);


// graphic of soldiers and potions
void drawing_main_matrix(SDL_Renderer* sdlRenderer, matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], hexagonal map[HEXAGON_COUNT], Uint32 map_colors[10], Uint32 potion_colors[5]);

#endif //MAIN_C_LOGIC_H
