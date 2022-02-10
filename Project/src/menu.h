//
// Created by smiza on 2/8/2022.
//

#ifndef MAIN_C_MENU_H
#define MAIN_C_MENU_H

// main menu
int Which_item_selected(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items);
int main_menu(char username[100], SDL_Renderer* sdlMenuRenderer, int letter_counter, bool* music, bool last_game);

// scoreboard
int user_count_in_scoreboard();
void read_score_board_file(score_board_struct* users_ptr, int users_counter);
void sort_score_board_file(score_board_struct* users_ptr, int user_counter);
void index_to_rank_in_scoreboard(char temp_username_in_scoreboard_menu_rank[3], int index);
void fill_strings_for_ranks_and_scores(score_board_struct* users_ptr, int user_counter);
void writing_on_scoreboard(SDL_Renderer * sdlScoreboardRenderer, score_board_struct* users_head_ptr, int user_counter);
int Which_item_selected_in_scoreboard(int X_of_mouse, int Y_of_mouse, Uint16 X_item1, Uint16 Y_item1,Uint16 H_items, Uint16 W_items, Uint16 D_items);
int Scoreboard_manu(SDL_Renderer* sdlScoreboardRenderer);

// new game menu
int which_item_selected_in_new_game_menu(int X_of_mouse, int Y_of_mouse, int X_item1, int Y_item1, int H_items, int W_items, int D_items);
int default_maps_count();
int new_game_menu(SDL_Renderer* sdlNewGameMenuRenderer, int* number_of_players, int* cell_num, int* default_map_number);

// user won or lode menu
void user_w_or_l_menu(SDL_Renderer * sdlUserWonRenderer, bool user_won);
#endif //MAIN_C_MENU_H
