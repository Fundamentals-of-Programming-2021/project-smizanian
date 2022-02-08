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
#include "map.h"
#include "menu.h"
#include "logic.h"




int main() {

    // SDL_Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    printf ("Hello!\n");

    // random set
    srand(time(0));

    // audio
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("Hans Zimmer-Rush.wav", &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    time_t music_time_0 = time(NULL);
    time_t music_time = music_time_0 + 6*60 + 19;


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


    // new game file variables
    char file_name[] = "000.txt";
    FILE* mapcpy_ptr = NULL;
    FILE* file_ptr = NULL;


    if (item_selected_in_main_menu == 1) { /////////////////////// new game
        // new game menu
        int cell_num = CELL_NUM;
        int number_of_players = 0;
        int map_number = 0;
        int type_of_new_game = 0;
        type_of_new_game = new_game_menu(sdlRenderer, &number_of_players, &cell_num, &map_number);

        if (type_of_new_game == 1) {
            printf ("You selected map No. %d\n", map_number);
            file_name_change(file_name, map_number);
            puts(file_name);
            file_ptr = fopen(file_name, "r");
            mapcpy_ptr = fopen("map.txt", "w");
            if (file_ptr == NULL || mapcpy_ptr == NULL) {
                printf ("ERROR\n");
                return 0;
            }
            copy_selected_map(file_ptr, mapcpy_ptr);
        }
        else if (type_of_new_game == 2) {
            random_map_generator(cell_num, number_of_players, &map_number);
            file_name_change(file_name, map_number);
            file_ptr = fopen(file_name, "r");
            mapcpy_ptr = fopen("map.txt", "w");
            if (file_ptr == NULL || mapcpy_ptr == NULL) {
                printf ("ERROR\n");
                return 0;
            }
            copy_selected_map(file_ptr, mapcpy_ptr);
        }
    }

    // closing files
    fclose(file_ptr);
    fclose(mapcpy_ptr);









    /////////////////////////////////////////////////////////////// game starts //////////////////////////////////////////////////////////



    // reading a map
    FILE* map_ptr;
    map_ptr = fopen("map.txt", "r");
    if (map_ptr == NULL) {
        printf ("Error opening file. \n");
    }
    fscanf(map_ptr, "%d\n", &CELL_NUM);
    HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
    HEXAGON_SIDE = (int)(HEXAGON_h/(2*0.866025))/1 + 1;
    fscanf(map_ptr, "%d\n", &HEXAGON_COUNT);
    fscanf(map_ptr, "%d\n", &NUMBER_OF_PLAYERS);

    hexagonal map[HEXAGON_COUNT];


    // colors
    Uint32 map_colors[10] = {0xffff00ff, 0xff800000, 0xff0000ff, 0xff800080, 0xff808000, 0xff008080, 0xffff00ff, 0x80ff80ff, 0xffc0c0c0, 0xffffff00};
    Uint32 potion_colors[5] = {0xff3e1e85, 0xff8da05a, 0xffd0b196, 0xff79a8c8, 0xff2f2f3c};
    Uint32 hexagonal_colors[HEXAGON_COUNT];
    int potion_color_id = 0;

    // read file function
    read_file(map_ptr, map, hexagonal_colors,map_colors);

    // mouse place variables
    int mouse_X = 0;
    int mouse_Y = 0;

    // image variables
    char image_name[100] = {"image.bmp"};

    // potion variables
    int X_of_potion;
    int Y_of_potion;
    int potion_flag;
    //long long int potion_count;

    // logic variables and first initializations for main matrix
    matrix_unit main_matrix[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    matrix_unit main_matrix_temp[SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT][SCREEN_HEIGHT/MAIN_MATRIX_UNITS_COUNT];
    first_initialization_of_main_matrix(main_matrix);
    first_initialization_of_main_matrix(main_matrix_temp);


    // variables for soldier count in lands
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

        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(sdlRenderer);

        // drawing the map
        drawHexagonalMap(sdlRenderer, map, map_colors);


        // audio
        if (music_time >= music_time_0 + 6*60 + 19) { // audio length is 6:19
            int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
            SDL_PauseAudioDevice(deviceId, 0);
            music_time_0 = time(NULL);
        }
        music_time = time(NULL);


        // logical part of code
        // lands soldier adding and attacks managing in lands
        soldier_adding_and_attack_managing(map, main_matrix);


        // potions
        potion_flag = potion_generator(sdlRenderer, &X_of_potion, &Y_of_potion, map, main_matrix);
        if (potion_flag == 1 && main_matrix[X_of_potion][Y_of_potion].Potion == false) {
            add_potion_to_main_matrix_as_index(main_matrix, X_of_potion, Y_of_potion);
        }


        // attacks
        computer_attacks(map, &X_dis, &Y_dis, main_matrix);


        // drawing main matrix
        drawing_main_matrix(sdlRenderer, main_matrix, map, map_colors, potion_colors);



        // main matrix conflict checking
        main_matrix_conflict_checking(main_matrix, main_matrix_temp);

        // main matrix temp copy
        main_matrix_temp_copy(main_matrix, main_matrix_temp, map);




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

    fclose(map_ptr);

    if (game_finished == true) {
        add_score(users_head_ptr, user_counter, username, user_won);
        user_w_or_l_menu(sdlRenderer, user_won);
    }
    if (game_finished == false) {
        FILE* game_ptr = NULL;
        FILE* last_map_ptr = NULL;
        game_ptr = fopen("map.txt", "r");
        last_map_ptr = fopen("lst.txt", "w");
        if (game_ptr == NULL || last_map_ptr == NULL) {
            printf ("ERROR\n");
            return 0;
        }
        copy_selected_map(map_ptr, last_map_ptr);
    }


    SDL_Delay(100);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    printf("\nHello World\n");

    SDL_Quit();

    return 0;
}
