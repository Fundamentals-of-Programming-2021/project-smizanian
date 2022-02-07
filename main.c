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
#include "global_variables.h"
#include "main_menu.h"





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


// important function prototypes
// map
void read_file(FILE* file_ptr, hexagonal map[HEXAGON_COUNT], Uint32 hexagonal_colors[HEXAGON_COUNT], Uint32 map_colors[10]);
void drawShape(SDL_Renderer * sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, Uint32 map_colors[10]);
void drawHexagon(SDL_Renderer *sdlRenderer, int xcounter, int ycounter, Uint32 hexagon_color, int soldiers, Uint32 map_colors[10], bool potion);
void drawHexagonalMap(SDL_Renderer *sdlRenderer, hexagonal map[HEXAGON_COUNT], Uint32 map_colors[10]);
void drawCircle(SDL_Renderer *sdlRenderer, int x_int, int y_int, Uint32 color);
void drawBox(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);
void drawTri(SDL_Renderer *sdlRenderer, int x, int y, Uint32 color);
int mod(int a, int b);
// random map generator
void file_name_change(char file_name[7], int file_name_counter);
int find_in_players_first_state_by_index(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int index);
void players_first_state_generator(int number_of_players, int players_first_state[number_of_players][2]);
int find_in_players_first_state(int number_of_players, int players_first_state[number_of_players][2], int X, int Y, int flag);
void map_generator(int number_of_players, int players_first_state[number_of_players][2], FILE* map_ptr);
int count_hexagons();
void random_map_generator(int cell_num, int number_of_players, int* map_number);
void fill_file_name(FILE** file_ptr_ptr, int map_number);
void copy_selected_map(int map_number);
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
// new game menu
int which_item_selected_in_new_game_menu(int X_of_mouse, int Y_of_mouse, int X_item1, int Y_item1, int H_items, int W_items, int D_items);
int default_maps_count();
int new_game_menu(SDL_Renderer* sdlNewGameMenuRenderer, int* number_of_players, int* cell_num, int* default_map_number);
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
int all_soldiers_count_in_one_land(hexagonal map[HEXAGON_COUNT], int index);
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






int main() {

    // SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
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


    // audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("Hans Zimmer-Rush.wav", &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    int length_counter = 0;



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


    // main while of menu (handling "back to main menu" item in scoreboard)
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



    // username
    if (username[0] == 0) {
        strcpy(username, "new user 1");
    }

    // saving username
    char scoreboard_name[] = "scoreboard.txt";
    // reading scoreboard
    int user_counter = user_count_in_scoreboard();
    score_board_struct* users_head_ptr = NULL;
    score_board_struct* users_ptr = NULL;
    users_head_ptr = (score_board_struct*)malloc(user_counter*sizeof(score_board_struct));
    read_score_board_file(users_head_ptr, user_counter);
    // finding username in scoreboard file
    bool new_username = find_new_username_in_scoreboard(users_head_ptr, user_counter, username);
    if (new_username == true) {
        FILE *scoreboard_ptr = fopen(scoreboard_name, "a");
        printf ("NEW USERNAME\n");
        users_ptr = users_head_ptr;
        for (int i=0; i<user_counter; i++) {
            if ( i != user_counter-1) {
                users_ptr = users_ptr->next_user;
            }
        }
        users_ptr->next_user = (score_board_struct*) malloc(sizeof(score_board_struct));
        users_ptr = users_ptr->next_user;
        users_ptr->score = 0;
        strcpy(users_ptr->name, username);
        user_counter++;
        fprintf(scoreboard_ptr, "%s\n%d\n", username, 0);
        fclose(scoreboard_ptr);
    }

    puts(username);


    if (item_selected_in_main_menu == 1) { /////////////////////// new game
        // new game menu
        int cell_num = CELL_NUM;
        int number_of_players = 0;
        int map_number = 0;
        int type_of_new_game = 0;
        type_of_new_game = new_game_menu(sdlRenderer, &number_of_players, &cell_num, &map_number);


        if (type_of_new_game == 1) {
            printf ("You selected map No. %d\n", map_number);
            //fill_file_name(&file_ptr, map_number);
            //copy_selected_map(map_number);
        }
        else if (type_of_new_game == 2) {
            random_map_generator(cell_num, number_of_players, &map_number);
            //fill_file_name(&file_ptr, map_number);
            // copy_selected_map(map_number);
        }
    }









    /////////////////////////////////////////////////////////////// game starts //////////////////////////////////////////////////////////



    // reading a map
    FILE* file_ptr;
    file_ptr = fopen("map.txt", "r");
    if (file_ptr == NULL) {
        printf ("Error opening file. \n");
    }
    fscanf(file_ptr, "%d\n", &CELL_NUM);
    HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
    HEXAGON_SIDE = (int)(HEXAGON_h/(2*0.866025))/1 + 1;
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
    long long int potion_count;

    // logic variables and first initializations for main matrix
    matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    matrix_unit main_matrix_temp[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    first_initialization_of_main_matrix(main_matrix);
    first_initialization_of_main_matrix(main_matrix_temp);


    // variables for soldier count in lands
    int soldier_count_in_lands = 0;
    attacks_struct* local_attacks = NULL;

    // variables for end of the game
    bool game_finished = false;
    bool user_won = false;

    // logical variables for AI and attack
    int X_dis = 0;
    int Y_dis = 0;
    int id_dis = 0;
    bool attack_valid = false;
    int id_s_user = 0;
    int id_d_user = 0;





    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        //printf ("%lld\n", potion_count);
        potion_count = 0;

        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlRenderer);

        // drawing the map
        drawHexagonalMap(sdlRenderer, map, map_colors);


        // audio
        if (length_counter <= 0) {
            printf ("0\n");
            int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
            SDL_PauseAudioDevice(deviceId, 0);
            length_counter = (2*60+20)*1000; // audio length is 6:19
        }
        length_counter = length_counter - 1000/FPS;
        printf ("%d\n", length_counter);


        // logical part of code
        // lands soldier adding and attacks managing in lands
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


        // potions
        potion_flag = potion_generator(sdlRenderer, &X_of_potion, &Y_of_potion, map, main_matrix);
        if (potion_flag == 1 && main_matrix[X_of_potion][Y_of_potion].Potion == false) {
            add_potion_to_main_matrix_as_index(main_matrix, X_of_potion, Y_of_potion);
        }


        // attacks
        for (int i=0; i<HEXAGON_COUNT; i++) {
            if (map[i].color_index == 9 || map[i].color_index == 8 || map[i].color_index == 0) {
                continue;
            }
            if (rand()%500 == 50) {
                id_dis = AI(map[i].X, map[i].Y, map, &X_dis, &Y_dis, i);
                if (map[id_dis].color_index != 9 && X_dis > 5 && Y_dis > 5) {
                    set_new_attack(map, main_matrix, i, &X_dis, &Y_dis, id_dis);
                }
            }
        }













        // drawing main matrix
        for (int i=0; i<SCREEN_WIDTH/MAIN_MATRIX_UNITS_COUNT; i++) {
            for (int j=0; j<SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT; j++) {

                // drawing potions
                if (main_matrix[i][j].Potion == true && main_matrix[i][j].center_of_potion == true) {
                    filledCircleColor(sdlRenderer, (Sint16)(10*i+5), (Sint16)(10*j+5), 10, 0xffffffff);
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



        // main matrix conflict checking
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

        // main matrix temp copy
        bool free_unit = true;
        bool potion_in_use = true;
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
                            printf ("Potion was in %d and %d. Now its for land id %d\n", 10*i+5, 10*j+5, land_id_of_soldier);
                            add_potion_to_land_and_soldiers(map, main_matrix, land_id_of_soldier, i, j);
                            main_matrix[i][j].Potion = false;
                            main_matrix[i][j].local_potion.potion_id = -1;
                        }
                    }
                }

            }

        }
        first_initialization_of_main_matrix(main_matrix_temp);




        // rendering
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);

        // check if user won
        if (game_finished && user_won) {
            printf ("Game finished and you won\n");
            SDL_Delay(1000);
            shallExit = SDL_TRUE;
        }
        else if (game_finished && !user_won) {
            printf ("Khak too sar-e bozet bokonan ke as computer bakhti osgol\n");
            SDL_Delay(1000);
            shallExit = SDL_TRUE;
        }
        user_won = game_end_check(map, main_matrix, &game_finished);


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
                        if (game_finished == false) {
                            attack_valid = is_attack_source_valid(map, mouse_X, mouse_Y, &id_s_user);
                            //printf("You clicked on X %d, Y %d, id %d\n", mouse_X, mouse_Y,X_Y_to_hexagon(mouse_X, mouse_Y));
                            //printf("Color of your clicked hexagon is in index %d\n",map[X_Y_to_hexagon(mouse_X, mouse_Y) - 1].color_index);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                        SDL_GetMouseState(&mouse_X, &mouse_Y);
                        if (game_finished == false) {
                            attack_valid =attack_valid * is_attack_dis_valid(map, mouse_X, mouse_Y, id_s_user, &id_d_user);
                            if (attack_valid) {
                                //printf("Googooli-e man, dorost hamle kardi\n");
                                X_dis = map[id_d_user].X;
                                Y_dis = map[id_d_user].Y;
                                set_new_attack(map, main_matrix, id_s_user, &X_dis, &Y_dis, id_d_user);
                            }
                            else {
                                //printf ("Dorost hamle kon olagh\n");
                            }
                        }
                    }
            }
        }
    }

    fclose(file_ptr);

    if (game_finished == true) {
        add_score(users_head_ptr, user_counter, username, user_won);
        user_w_or_l_menu(sdlRenderer, user_won);
    }


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
    score_to_score_in_scoreboard(number_of_soldiers, soldiers);

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
int mod(int a, int b) {
    return a % b;
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
        if (map[i].is_d == false && map[i].is_s == true) {
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

void filling_main_matrix_for_attacks(matrix_unit main_matrix[60][60], hexagonal map[HEXAGON_COUNT], int X_dis, int Y_dis, int i, int j) {
    //printf ("%d - %d\n", map[X_Y_to_hexagon((X_dis), (Y_dis))-1].color_index, X_Y_to_hexagon((X_dis), (Y_dis))-1);
    //printf ("I'm for land %d with potion", map[i].color_index);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].X = (float)map[i].X;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].potion_id = map[i].potion_id;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].Y = (float)map[i].Y;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].land_id = map[i].color_index;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].Y_d = Y_dis;
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].X_d = X_dis;
    float Delta_D = (float)pow(Delta_D_calculator(map[i].X, map[i].Y, X_dis, Y_dis), 0.5);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_x = (float)map[i].soldier_speed * ((float)(X_dis - map[i].X)/Delta_D);
    main_matrix[map[i].X/10][map[i].Y/10].local_soldiers[j].V_y = (float)map[i].soldier_speed * ((float)(Y_dis - map[i].Y)/Delta_D);
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

void soldier_generator_in_attacks(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int i) {
    attacks_struct* local_attacks = map[i].attacks;
    int count = 0;
    while (local_attacks != NULL) {
        count++;
        //printf ("%d is not NULL\n", map[i].color_index);
        if (local_attacks->finished == false) {

            local_attacks->soldiers--;
            if (local_attacks->soldiers % SOLDIER_GENERATOR_RATE == 0) {
                //printf ("You have %d soldiers in %d land\n", local_attacks->soldiers/10, i);
                for (int j = 0; j < 8; j++) {
                    if (main_matrix[map[i].X / MAIN_MATRIX_UNITS_COUNT][map[i].Y /MAIN_MATRIX_UNITS_COUNT].local_soldiers[j].X ==0) {
                        //printf("You are going to X = %d, Y = %d\n", local_attacks->X_dis, local_attacks->Y_dis);
                        //count++;
                        //printf ("Count is: %d\n", count);
                        filling_main_matrix_for_attacks(main_matrix, map, local_attacks->X_dis,local_attacks->Y_dis, i, j);
                        map[local_attacks->id_dis].is_d = true;
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

int drawSoldiers(SDL_Renderer * sdlRenderer, int x, int y, int d, Uint32 color, Uint32 map_colors[10]) {
    Sint16 X = (Sint16)x;
    Sint16 Y = (Sint16)y;
    if (color == map_colors[0] || color == map_colors[5]) {
        filledCircleColor(sdlRenderer, X, Y, d, color);
        return 0;
    }
    else if (color == map_colors[1] ||color == map_colors[3] || color == map_colors[6]) {
        boxColor(sdlRenderer, X-d, Y-d, X+d, Y+d, color);
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

void potion_add_to_conquered_land(hexagonal map[HEXAGON_COUNT], int id_dis, soldier local_soldiers) {

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
        map[id_dis].potion_id = local_soldiers.potion_id;
        map[id_dis].land_potion.potion_id = (short) local_soldiers.potion_id;
        map[id_dis].land_potion.potion_in_use = true;
        if (index == 0) {
            map[id_dis].land_potion.potion_time = 1000;
            return;
        }
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
                temp_i->score+=45;
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

void user_w_or_l_menu(SDL_Renderer * sdlUserWonRenderer, bool user_won) {

    // image of menu

    char image_name1[100] = {0};
    strcpy(image_name1, "usrwn1.bmp");
    SDL_Surface *menu_image = SDL_LoadBMP(image_name1);
    SDL_Texture *menu_texture = SDL_CreateTextureFromSurface(sdlUserWonRenderer, menu_image);
    SDL_Rect menu_image_dstrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    char image_name2[100] = {0};
    strcpy(image_name2, "usrwn2.bmp");
    SDL_Surface *menu_image2 = SDL_LoadBMP(image_name2);
    SDL_Texture *menu_texture2 = SDL_CreateTextureFromSurface(sdlUserWonRenderer, menu_image2);
    SDL_Rect menu_image_dstrect2 = {SCREEN_WIDTH / 4, 40, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    if (user_won == false) {
        strcpy(image_name1, "usrls1.bmp");
        menu_image = SDL_LoadBMP(image_name1);
        menu_texture = SDL_CreateTextureFromSurface(sdlUserWonRenderer, menu_image);


        strcpy(image_name2, "usrls2.bmp");
        menu_image2 = SDL_LoadBMP(image_name2);
        menu_texture2 = SDL_CreateTextureFromSurface(sdlUserWonRenderer, menu_image2);
    }

    // menu texts and variables
    char text[] = "OK";
    Uint32 item_box_color = 0xffAfAf00; // persian green
    int X_of_mouse = 0;
    int Y_of_mouse = 0;

    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        SDL_SetRenderDrawColor(sdlUserWonRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlUserWonRenderer);


        // background image
        SDL_RenderCopy(sdlUserWonRenderer, menu_texture, NULL, &menu_image_dstrect);
        SDL_RenderCopy(sdlUserWonRenderer, menu_texture2, NULL, &menu_image_dstrect2);


        // item boxes
        Uint16 X_item1 = 280;
        Uint16 Y_item1 = 500;
        Uint16 W_items = 40;
        Uint16 H_items = 40;


        // drawing item boxes
        boxColor(sdlUserWonRenderer, X_item1, Y_item1, X_item1+W_items, Y_item1+H_items, item_box_color);



        // text in items
        stringColor(sdlUserWonRenderer, X_item1+W_items/3, Y_item1+H_items/2 - 5, text, 0xff000000);


        // item color change
        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
        if (X_of_mouse >= X_item1 && X_of_mouse<= X_item1+W_items && Y_of_mouse>=Y_item1 && Y_of_mouse<=Y_item1+H_items) {
            item_box_color = 0xffAfAf00 - 0xA1000000;
        }
        else {
            item_box_color = 0xffAfAf00;
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
                        SDL_GetMouseState(&X_of_mouse, &Y_of_mouse);
                        if (X_of_mouse >= X_item1 && X_of_mouse<= X_item1+W_items && Y_of_mouse>=Y_item1 && Y_of_mouse<=Y_item1+H_items) {
                            shallExit = SDL_TRUE;
                        }

                    }
                    break;
            }

        }

        SDL_RenderPresent(sdlUserWonRenderer);
        SDL_Delay(1000 / FPS);
    }


    SDL_Delay(100);
    SDL_DestroyTexture(menu_texture);
    SDL_FreeSurface(menu_image);
    SDL_DestroyTexture(menu_texture2);
    SDL_FreeSurface(menu_image2);
    //SDL_DestroyRenderer(sdlMenuRenderer);
    //SDL_DestroyWindow(sdlMenu);
    SDL_RenderClear(sdlUserWonRenderer);
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

void add_potion_to_land_and_soldiers(hexagonal map[HEXAGON_COUNT], matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT], int land_id_of_soldier, int index_i, int index_j) {

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
    printf("Potion was generated in location %d and %d with id", 10 * X_of_potion + 5, 10 * Y_of_potion + 5);
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
    printf (" %d\n", local_potion_id);

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


void fill_file_name(FILE** file_ptr_ptr, int map_number) {
    char file_name[] = "000.txt";
    file_name_change(file_name, map_number);
    *(file_ptr_ptr) = fopen(file_name, "r");
}

void copy_selected_map(int map_number) {

    char file_name[] = "000.txt";
    char map_name[] = "map.txt";
    char text[100000] = {0};
    file_name_change(file_name, map_number);
    FILE* file_ptr = fopen(file_name, "r");
    if (file_ptr != NULL) {
        printf ("read file is not NULL\n");
        //return;
    }
    FILE* map_ptr = fopen(map_name, "w");
    if (map_ptr != NULL) {
        printf ("map file is not NULL\n");
        //return;
    }
    fscanf(file_ptr, " %[^\0]s", text);
    puts(text);
    //fprintf(map_ptr, "%s", text);
    //fprintf(map_ptr, "%d\n", 3);

    fclose(file_ptr);
    fclose(map_ptr);
    //printf ("Hi\n");
    return;
}







