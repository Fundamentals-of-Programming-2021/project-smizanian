//
// Created by smiza on 2/8/2022.
//

#ifndef MAIN_C_MAP_H
#define MAIN_C_MAP_H

// basic functions for map
void read_file(FILE* file_ptr, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT], Uint32 map_colors[10]);
void read_saved_map(FILE* map_ptr, FILE* saved_potion_datas_ptr, hexagonal map[HEXAGON_COUNT]);
void drawCircle(SDL_Renderer *sdlRenderer, int x_int, int y_int, Uint32 color);
void drawBox(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);
void drawTri(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);
int all_soldiers_count_in_one_land(hexagonal map[HEXAGON_COUNT], int index);
int power(int number);
void int_to_str(char number_str[9], int number);

// random map generator
void file_name_change(char file_name[7], int file_name_counter);
int find_in_players_first_state_by_index(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int index);
void players_first_state_generator(int number_of_players, int players_first_state[number_of_players][2]);
int find_in_players_first_state(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int flag);
void map_generator(int number_of_players, int players_first_state[number_of_players][2], FILE* map_ptr);
int count_hexagons();
void random_map_generator(int cell_num, int number_of_players, int* map_number);

// copy selected map
void copy_selected_map(FILE* file_ptr, FILE* map_ptr);

// saving the map
void save_map_and_main_matrix(FILE* last_map_ptr, FILE* potion_data_ptr, FILE* main_matrix_save_ptr, hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT]);

// drawing map
void drawShape(SDL_Renderer * sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, Uint32 map_colors[10]);
void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, int soldiers, Uint32 map_colors[10], Uint32 potion_colors[5], int potion);
void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 map_colors[10], Uint32 potion_colors[5], int map_number);
int mod(int a, int b);

#endif //MAIN_C_MAP_H
