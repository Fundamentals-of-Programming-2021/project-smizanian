#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const int FPS = 60;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
int CELL_NUM = 7;
int HEXAGON_h = 0;
int HEXAGON_SIDE = 0;
int HEXAGON_COUNT = 72;

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

void random_map_generator() {

    char file_name[] = "000.txt";
    int file_name_counter = 0;
    FILE* map_ptr = fopen(file_name, "r");

    while (map_ptr != NULL) {
        file_name_counter++;
        fclose(map_ptr);
        file_name_change(file_name, file_name_counter);
        map_ptr = fopen(file_name, "r");
    }

    map_ptr = fopen(file_name, "w");

    srand(time(NULL));


    ////////////////////////// random number of players ///////////////////////
    /*
    int number_of_players = (rand())%4 + 2;
    */

    // getting number of players
    int number_of_players = 0;
    printf ("Enter a number between 2 to 6:");
    scanf ("%d", &number_of_players);
    while (!(number_of_players>=2 && number_of_players<=6)) {
        printf ("Enter a valid number:");
        scanf ("%d", &number_of_players);
    }


    // getting cell num
    printf ("Enter a number between 5 to 55:");
    scanf ("%d", &CELL_NUM);
    while (!(CELL_NUM>=5 && CELL_NUM<=55)) {
        printf ("Enter a valid number:");
        scanf ("%d", &CELL_NUM);
    }
    HEXAGON_h = (SCREEN_HEIGHT/CELL_NUM)/1 + 1;
    HEXAGON_SIDE = (HEXAGON_h/(2*0.866025))/1 + 1;


    int players_first_state[number_of_players][2];
    players_first_state_generator(number_of_players, players_first_state);

    HEXAGON_COUNT = count_hexagons();
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

int main()
{
    random_map_generator();
    printf ("Side = %d \n", HEXAGON_SIDE);
    printf("Hello world!\n");

    return 0;
}

