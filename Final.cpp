#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <vector>
#include <string>
using namespace std;
struct allblock {
    int x;
    int y;
    bool if_live;
    bool if_live1;
};

char temporary_map[15][15];
char Map[15][15];
bool block[4][4];
int x_of_block;
int y_of_block;

bool block1[4][4] = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool block2[4][4] = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool block3[4][4] = { true, false, false, false, true, true, false, false, true, false, false, false, false, false, false, false };
bool block4[4][4] = { true, true, false, false, false, true, true, false, false, false, false, false, false, false, false, false };
bool block5[4][4] = { false, true, true, false, true, true, false, false, false, false, false, false, false, false, false, false };
bool block6[4][4] = { true, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false };
bool block7[4][4] = { false, false, true, false, true, true, true, false, false, false, false, false, false, false, false, false };
bool block8[4][4] = { true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false };
bool block9[4][4] = { true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false };

bool player1;

bool p1_used_block[9] = { false, false, false, false, false, false, false, false, false };
bool p2_used_block[9] = { false, false, false, false, false, false, false, false, false };

int using_block = 0;

int final_map[15][15];
int unkown_number = 0;
int search_array[60];
int search_array_number = 0;
int number_of_p1 = 0;
int number_of_p2 = 0;

int final_map1[15][15];
int final_map2[15][15];

allblock blocklife1[30];
allblock blocklife2[30];

int block_begin1 = 0;
int block_end1 = 0;

int block_begin2 = 0;
int block_end2 = 0;

bool boundary[48];
int boundary_number = 0;

int live_or_dead_map[15][15];

std::vector<std::string> player_history;
bool auto_build = true; // assume: input from file -> stdin


void introduction()
{
    printf(" welcome to game that called 'block out' \n");
    printf(" following show how to control this game \n");
    printf("\n w : up \n");
    printf("\n s : down \n");
    printf("\n a : left \n");
    printf("\n d : right \n");
    printf("\n 1 : 口 \n");
    printf("\n 2 : 口 \n");
    printf("\n 3 : 口\n");
    printf("     口口\n");
    printf("     口\n");
    printf("\n 4 : 口口 \n");
    printf("       口口 \n");
    printf("\n 5 :   口口\n");
    printf("     口口\n");
    printf("\n 6 : 口\n");
    printf("     口口口\n");
    printf("\n 7 :     口\n");
    printf("     口口口\n");
    printf("\n 8 : 口口口口\n");
    printf("\n 9 : 口口 \n");
    printf("     口口 \n");
    printf("\n c : change directiono of block \n");
    printf("\n y : make a decision to set up the block in here\n");
    printf("\n Are you ready\n");
    return;
}

void reduction_block()
{
    for (int iii = 0; iii < 4; iii++) {
        for (int jjj = 0; jjj < 4; jjj++) {
            block[iii][jjj] = false;
        }
    }
}

void reduction_map()
{
    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            Map[i][j] = '+';
        }
    }
    for (int ii = 0; ii < 15; ii++) {
        Map[ii][0] = '-';
        Map[0][ii] = '-';
        Map[ii][14] = '-';
        Map[14][ii] = '-';
    }
    Map[4][4] = '*';
    Map[4][10] = '*';
    Map[10][4] = '*';
    Map[10][10] = '*';
    return;
}

void printf_temporary_map()
{
	//system("clear");
	system("tput reset");

    if (player1 == true) {
        printf("\n               Player1:O               \n\n");
    } else {
        printf("\n              Player2:AI               \n\n");
    }

    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            temporary_map[i][j] = Map[i][j];
        }
    }

    for (int ii = 0; ii < 4; ii++) {
        for (int jj = 0; jj < 4; jj++) {
            if (block[ii][jj]) {
                if (player1) {
                    temporary_map[x_of_block + ii][y_of_block + jj] = 'O';
                } else {
                    temporary_map[x_of_block + ii][y_of_block + jj] = 'X';
                }
            }
        }
    }

    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            if (temporary_map[i][j] == '*') {
                printf(" \e[33m%c\e[0m ", temporary_map[i][j]);
            } else if (temporary_map[i][j] == 'O') {
                printf(" \e[32m%c\e[0m ", temporary_map[i][j]);
            } else if (temporary_map[i][j] == 'X') {
                printf(" \e[31m%c\e[0m ", temporary_map[i][j]);
            } else {
                printf(" %c ", temporary_map[i][j]);
            }
        }
        printf("\n");
    }

    return;
}

void transfer() // I can't understand this
{

    bool row = false;

    bool col = false;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[0][j] == true) {
                row = true;
                break;
            }
        }
        if (row == true) {
            break;
        } else if (row == false) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 4; j++) {
                    block[i][j] = block[i + 1][j];
                }
            }
            for (int k = 0; k < 4; k++) {
                block[3][k] = false;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[j][0] == true) {
                col = true;
                break;
            }
        }
        if (col == true) {
            break;
        } else if (col == false) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 4; j++) {
                    block[j][i] = block[j][i + 1];
                }
            }
            for (int k = 0; k < 4; k++) {
                block[k][3] = false;
            }
        }
    }
}

void player(int player, FILE *sourcefp = stdin)
{

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block[i][j] = false;
        }
    }

    if (player == 1) {
        player1 = true;
    } else {
        player1 = false;
    }

    printf_temporary_map();

	std::string history;
    while (1) {
        char order;
        int PC_1;
        int PC_2[2];
        int PC_3;
        bool block_sel = false;
        bool overlapped = false;
        bool Out_Range = false;
        bool Connected_Star = false;
        bool temp[4][4];
        bool pass = true;
        bool AI_Error = true;
        if (not auto_build) {
	        sourcefp = stdin;
        }

        
        if (player == 1) {
			// key map [w, a, s, d], [1-9], [c], [y], (new) r for reload???, s
	        if (fscanf(sourcefp, "%c", &order) == EOF && sourcefp != stdin) {
				printf("File ended. Switch to stdin.\n");
				auto_build = false;
				scanf("%c", &order);
			}
			history += order;
			
            if (order == 'w') {
                x_of_block -= 1;
                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        if (block[ii][jj]) {
                            if (x_of_block + ii == 0) {
                                Out_Range = true;
                                x_of_block += 1;
                                break;
                            }
                        }
                    }
                    if (Out_Range)
                        break;
                }
                printf_temporary_map();
                if (Out_Range) {
                    printf("\nThe Area is out of Range!\n");
                }
            } else if (order == 's') {
                x_of_block += 1;
                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        if (block[ii][jj]) {
                            if (x_of_block + ii == 14) {
                                Out_Range = true;
                                x_of_block -= 1;
                                break;
                            }
                        }
                    }
                    if (Out_Range)
                        break;
                }
                printf_temporary_map();
                if (Out_Range) {
                    printf("\nThe Area is out of Range!\n");
                }
            } else if (order == 'a') {
                y_of_block -= 1;
                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        if (block[ii][jj]) {
                            if (y_of_block + jj == 0) {
                                Out_Range = true;
                                y_of_block += 1;
                                break;
                            }
                        }
                    }
                    if (Out_Range)
                        break;
                }
                printf_temporary_map();
                if (Out_Range) {
                    printf("\nThe Area is out of Range!\n");
                }
            } else if (order == 'd') {
                y_of_block += 1;
                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        if (block[ii][jj]) {
                            if (y_of_block + jj == 14) {
                                Out_Range = true;
                                y_of_block -= 1;
                                break;
                            }
                        }
                    }
                    if (Out_Range)
                        break;
                }
                printf_temporary_map();
                if (Out_Range) {
                    printf("\nThe Area is out of Range!\n");
                }
            } else if (order == '1') {
                if (!p1_used_block[0]) {
                    using_block = 1;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block1[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '2') {
                if (!p1_used_block[1]) {
                    using_block = 2;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block2[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '3') {
                if (!p1_used_block[2]) {
                    using_block = 3;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block3[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '4') {
                if (!p1_used_block[3]) {
                    using_block = 4;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block4[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '5') {
                if (!p1_used_block[4]) {
                    using_block = 5;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block5[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '6') {
                if (!p1_used_block[5]) {
                    using_block = 6;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block6[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '7') {
                if (!p1_used_block[6]) {
                    using_block = 7;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block7[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '8') {
                if (!p1_used_block[7]) {
                    using_block = 8;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block8[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == '9') {
                if (!p1_used_block[8]) {
                    using_block = 9;
                    for (int nn = 0; nn < 4; nn++) {
                        for (int mm = 0; mm < 4; mm++) {
                            block[nn][mm] = block9[nn][mm];
                        }
                    }
                    printf_temporary_map();
                } else {
                    printf_temporary_map();
                    printf("\nThe Block has been used!\n");
                }
            }

            else if (order == 'c') {
                for (int nn = 0; nn < 4; nn++) {
                    for (int mm = 0; mm < 4; mm++) {
                        temp[nn][mm] = block[nn][mm];
                    }
                }
                block[0][0] = temp[3][0];
                block[0][1] = temp[2][0];
                block[0][2] = temp[1][0];
                block[0][3] = temp[0][0];
                block[1][0] = temp[3][1];
                block[1][1] = temp[2][1];
                block[1][2] = temp[1][1];
                block[1][3] = temp[0][1];
                block[2][0] = temp[3][2];
                block[2][1] = temp[2][2];
                block[2][2] = temp[1][2];
                block[2][3] = temp[0][2];
                block[3][0] = temp[3][3];
                block[3][1] = temp[2][3];
                block[3][2] = temp[1][3];
                block[3][3] = temp[0][3];

                for (int ii = 0; ii < 4; ii++) {
                    for (int jj = 0; jj < 4; jj++) {
                        if (block[ii][jj]) {
                            if (x_of_block + ii < 1 || x_of_block + ii > 13) {
                                Out_Range = true;
                                break;
                            } else if (y_of_block + jj < 1 || y_of_block + jj > 13) {
                                Out_Range = true;
                                break;
                            }
                        }
                    }
                    if (Out_Range)
                        break;
                }

                if (Out_Range) {
                    block[0][0] = temp[0][0];
                    block[0][1] = temp[0][1];
                    block[0][2] = temp[0][2];
                    block[0][3] = temp[0][3];
                    block[1][0] = temp[1][0];
                    block[1][1] = temp[1][1];
                    block[1][2] = temp[1][2];
                    block[1][3] = temp[1][3];
                    block[2][0] = temp[2][0];
                    block[2][1] = temp[2][2];
                    block[2][2] = temp[2][2];
                    block[2][3] = temp[2][3];
                    block[3][0] = temp[3][0];
                    block[3][1] = temp[3][1];
                    block[3][2] = temp[3][2];
                    block[3][3] = temp[3][3];
                }
                printf_temporary_map();
                if (Out_Range) {
                    printf("\nThe Area is out of Range!\n");
                }
            }

            else if (order == 'y') {
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (block[i][j]) {
                            block_sel = true;
                        }
                    }
                }

                if (block_sel) {
                    for (int ii = 0; ii < 4; ii++) {
                        for (int jj = 0; jj < 4; jj++) {
                            if (block[ii][jj]) {
                                if (Map[x_of_block + ii][y_of_block + jj] == '*') {
                                    Connected_Star = true;
                                } else if (player) {
                                    if (Map[x_of_block + ii + 1][y_of_block + jj    ] == 'O' || Map[x_of_block + ii - 1][y_of_block + jj    ] == 'O' ||
                                        Map[x_of_block + ii + 1][y_of_block + jj + 1] == 'O' || Map[x_of_block + ii + 1][y_of_block + jj - 1] == 'O' ||
                                        Map[x_of_block + ii - 1][y_of_block + jj + 1] == 'O' || Map[x_of_block + ii - 1][y_of_block + jj - 1] == 'O' ||
                                        Map[x_of_block + ii    ][y_of_block + jj + 1] == 'O' || Map[x_of_block + ii    ][y_of_block + jj - 1] == 'O') {
                                        Connected_Star = true;
                                    }
                                }
                            }
                        }
                    }

                    if (Connected_Star) {
                        for (int ii = 0; ii < 4; ii++) {
                            for (int jj = 0; jj < 4; jj++) {
                                if (block[ii][jj]) {
                                    if (Map[x_of_block + ii][y_of_block + jj] == 'O' || Map[x_of_block + ii][y_of_block + jj] == 'X') {
                                        overlapped = true;
                                        break;
                                    }
                                }
                            }
                            if (overlapped)
                                break;
                        }

                        if (overlapped) {
                            printf_temporary_map();
                            printf("\nPlease Select other places for the chosen block! The Area is overlapped!\n");
                        }

                        else {
                            if (using_block == 1) {
                                p1_used_block[0] = true;
                            } else if (using_block == 2) {
                                p1_used_block[1] = true;
                            } else if (using_block == 3) {
                                p1_used_block[2] = true;
                            } else if (using_block == 4) {
                                p1_used_block[3] = true;
                            } else if (using_block == 5) {
                                p1_used_block[4] = true;
                            } else if (using_block == 6) {
                                p1_used_block[5] = true;
                            } else if (using_block == 7) {
                                p1_used_block[6] = true;
                            } else if (using_block == 8) {
                                p1_used_block[7] = true;
                            } else if (using_block == 9) {
                                p1_used_block[8] = true;
                            }
                            printf_temporary_map();
                            for (int i = 1; i < 14; i++) {
                                for (int j = 1; j < 14; j++) {
                                    Map[i][j] = temporary_map[i][j];
                                }
                            }

							// commit history
							player_history.push_back(history);
							history.clear();
							
                            return;
                        }
                    } else {
                        printf_temporary_map();
                        printf("\nPlease Select other places for the chosen block! The Selection is invalid!\n");
                    }

                }

                else if (!block_sel) {
                    printf_temporary_map();
                    x_of_block = 1;
                    y_of_block = 1;
                    printf("\nPlease Select one kind of Block(Input 1~9!!)\n");
                }

            }

            else {
                if (!block_sel) {
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (block[i][j]) {
                                block_sel = true;
                            }
                        }
                    }
                    if (!block_sel) {
                        printf_temporary_map();
                        x_of_block = 1;
                        y_of_block = 1;
                        printf("\nPlease Select one kind of Block(Input 1~9!!)\n");
                    }
                } else {
                    printf("\nPlease Input Correct Instruction!\n");
                }
            }

        }

        //----------------------------------Player2---------------------------------------//

        else if (player == 0) {

            while (AI_Error == true) {
                overlapped = false;
                Out_Range = false;
                Connected_Star = false;
                AI_Error = false;

				if (auto_build) {
					int scanned = fscanf(sourcefp, "%d,%d,%d,%d,", &PC_1, &PC_2[0], &PC_2[1], &PC_3);
					if (scanned != 4) {
						printf("Error on loading commands. Switch to random generate.\n");
						auto_build = false;
						continue;
					}
				} else {
					PC_1 = (rand() % 9 + 1);
					PC_2[0] = (rand() % 13 + 1);
					PC_2[1] = (rand() % 13 + 1);
					PC_3 = (rand() % 4 + 1);
				}
                if (PC_1 < 1 || PC_1 > 9) {
                    AI_Error = true;
                } else if (p2_used_block[PC_1 - 1] == true) {
                    AI_Error = true;
                } else {
                    if ((PC_2[0] < 1 || PC_2[0] > 13) || (PC_2[1] < 1 || PC_2[1] > 13)) {
                        AI_Error = true;
                    } else if ((PC_2[0] > 0 && PC_2[0] < 14) && (PC_2[1] > 0 && PC_2[1] < 14)) {
                        if (PC_3 < 1 || PC_3 > 4) {
                            AI_Error = true;
                            pass = false;
                        }

                        else {
                            using_block = PC_1;
                            x_of_block = PC_2[0];
                            y_of_block = PC_2[1];

                            switch (PC_1) {
                                case 1:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block1[nn][mm];
                                        }
                                    }
                                    break;
                                case 2:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block2[nn][mm];
                                        }
                                    }
                                    break;
                                case 3:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block3[nn][mm];
                                        }
                                    }
                                    break;
                                case 4:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block4[nn][mm];
                                        }
                                    }
                                    break;
                                case 5:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block5[nn][mm];
                                        }
                                    }
                                    break;
                                case 6:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block6[nn][mm];
                                        }
                                    }
                                    break;
                                case 7:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block7[nn][mm];
                                        }
                                    }
                                    break;
                                case 8:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block8[nn][mm];
                                        }
                                    }
                                    break;
                                case 9:
                                    for (int nn = 0; nn < 4; nn++) {
                                        for (int mm = 0; mm < 4; mm++) {
                                            block[nn][mm] = block9[nn][mm];
                                        }
                                    }
                                    break;
                            }

                            for (int i = 1; i < PC_3; i++) {
                                for (int nn = 0; nn < 4; nn++) {
                                    for (int mm = 0; mm < 4; mm++) {
                                        temp[nn][mm] = block[nn][mm];
                                    }
                                }
                                block[0][0] = temp[3][0];
                                block[0][1] = temp[2][0];
                                block[0][2] = temp[1][0];
                                block[0][3] = temp[0][0];
                                block[1][0] = temp[3][1];
                                block[1][1] = temp[2][1];
                                block[1][2] = temp[1][1];
                                block[1][3] = temp[0][1];
                                block[2][0] = temp[3][2];
                                block[2][1] = temp[2][2];
                                block[2][2] = temp[1][2];
                                block[2][3] = temp[0][2];
                                block[3][0] = temp[3][3];
                                block[3][1] = temp[2][3];
                                block[3][2] = temp[1][3];
                                block[3][3] = temp[0][3];
                            }

                            transfer();

                            for (int ii = 0; ii < 4; ii++) {
                                for (int jj = 0; jj < 4; jj++) {
                                    if (block[ii][jj] == true) {
                                        PC_2[0] = PC_2[0] + ii;
                                        PC_2[1] = PC_2[1] + jj;
                                        if ((PC_2[0] < 1 || PC_2[0] > 13) || (PC_2[1] < 1 || PC_2[1] > 13)) {
                                            Out_Range = true;
                                            PC_2[0] = PC_2[0] - ii;
                                            PC_2[1] = PC_2[1] - jj;
                                            break;
                                        }
                                        PC_2[0] = PC_2[0] - ii;
                                        PC_2[1] = PC_2[1] - jj;
                                    }
                                }
                                if (Out_Range)
                                    break;
                            }

                            for (int ii = 0; ii < 4; ii++) {
                                for (int jj = 0; jj < 4; jj++) {
                                    if (block[ii][jj]) {
                                        if (Map[x_of_block + ii][PC_2[1] + jj] == '*') {
                                            Connected_Star = true;
                                        } else {
                                            if (Map[x_of_block + ii + 1][y_of_block + jj    ] == 'X' || Map[x_of_block + ii - 1][y_of_block + jj    ] == 'X' ||
												Map[x_of_block + ii + 1][y_of_block + jj + 1] == 'X' ||	Map[x_of_block + ii + 1][y_of_block + jj - 1] == 'X' ||
												Map[x_of_block + ii - 1][y_of_block + jj + 1] == 'X' ||	Map[x_of_block + ii - 1][y_of_block + jj - 1] == 'X' ||
												Map[x_of_block + ii    ][y_of_block + jj + 1] == 'X' ||	Map[x_of_block + ii    ][y_of_block + jj - 1] == 'X') {
                                                Connected_Star = true;
                                            }
                                        }
                                    }
                                }
                            }

                            for (int ii = 0; ii < 4; ii++) {
                                for (int jj = 0; jj < 4; jj++) {
                                    if (block[ii][jj]) {
                                        if (Map[x_of_block + ii][y_of_block + jj] == 'O' ||
											Map[x_of_block + ii][y_of_block + jj] == 'X') {
                                            overlapped = true;
                                            break;
                                        }
                                    }
                                }
                            }

                            if (Out_Range) {
                                for (int nn = 0; nn < 4; nn++) {
                                    for (int mm = 0; mm < 4; mm++) {
                                        block[nn][mm] = false;
                                    }
                                }
                                AI_Error = true;
                            } else if (!Connected_Star) {
                                for (int nn = 0; nn < 4; nn++) {
                                    for (int mm = 0; mm < 4; mm++) {
                                        block[nn][mm] = false;
                                    }
                                }
                                AI_Error = true;
                            } else if (overlapped) {
                                for (int nn = 0; nn < 4; nn++) {
                                    for (int mm = 0; mm < 4; mm++) {
                                        block[nn][mm] = false;
                                    }
                                }
                                AI_Error = true;
                            }
                        }
                    }
                }
            }

            switch (using_block) {
                case 1:
                    p2_used_block[0] = true;
                    break;
                case 2:
                    p2_used_block[1] = true;
                    break;
                case 3:
                    p2_used_block[2] = true;
                    break;
                case 4:
                    p2_used_block[3] = true;
                    break;
                case 5:
                    p2_used_block[4] = true;
                    break;
                case 6:
                    p2_used_block[5] = true;
                    break;
                case 7:
                    p2_used_block[6] = true;
                    break;
                case 8:
                    p2_used_block[7] = true;
                    break;
                case 9:
                    p2_used_block[8] = true;
                    break;
            }

			// commit player2 history
			player_history.push_back(std::to_string(PC_1) + "," + 
									 std::to_string(PC_2[0]) + "," +
									 std::to_string(PC_2[1]) + "," +
									 std::to_string(PC_3) + ",");
            printf_temporary_map();
            for (int i = 1; i < 14; i++) {
                for (int j = 1; j < 14; j++) {
                    Map[i][j] = temporary_map[i][j];
                }
            }
            return;
        }
    }
}

void search_block1(int x, int y)
{
    final_map1[x][y] = 1;
    blocklife1[block_end1].x = x;
    blocklife1[block_end1].y = y;
    block_end1++;

    //up
    if (final_map1[x - 1][y] == 0) {
        search_block1(x - 1, y);
    } else if (final_map1[x - 1][y] == 2) {
        if (y > 0 && y < 13) {
            boundary[0 + y - 1] = true;
            boundary_number++;
        }
    } else {
    }
    //right
    if (final_map1[x][y + 1] == 0) {
        search_block1(x, y + 1);
    } else if (final_map1[x][y + 1] == 2) {
        if (x > 0 && x < 13) {
            boundary[12 + x - 1] = true;
            boundary_number++;
        }
    } else {
    }
    //down
    if (final_map1[x + 1][y] == 0) {
        search_block1(x + 1, y);
    } else if (final_map1[x + 1][y] == 2) {
        if (y > 0 && y < 13) {
            boundary[24 + 13 - y] = true;
            boundary_number++;
        }
    } else {
    }
    //left
    if (final_map1[x][y - 1] == 0) {
        search_block1(x, y - 1);
    } else if (final_map1[x][y - 1] == 2) {
        if (x > 0 && x < 13) {
            boundary[36 + 13 - x] = true;
            boundary_number++;
        }
    } else {
    }

    //end
    return;
}

void search_block2(int x, int y)
{
    final_map2[x][y] = 1;
    blocklife2[block_end2].x = x;
    blocklife2[block_end2].y = y;
    block_end2++;

    //up
    if (final_map2[x - 1][y] == 0) {
        search_block2(x - 1, y);
    } else if (final_map2[x - 1][y] == 2) {
        if (y > 0 && y < 13) {
            boundary[0 + y - 1] = true;
            boundary_number++;
        }
    } else {
    }
    //right
    if (final_map2[x][y + 1] == 0) {
        search_block2(x, y + 1);
    } else if (final_map2[x][y + 1] == 2) {
        if (x > 0 && x < 13) {
            boundary[12 + x - 1] = true;
            boundary_number++;
        }
    } else {
    }
    //down
    if (final_map2[x + 1][y] == 0) {
        search_block2(x + 1, y);
    } else if (final_map2[x + 1][y] == 2) {
        if (y > 0 && y < 13) {
            boundary[24 + 13 - y] = true;
            boundary_number++;
        }
    } else {
    }
    //left
    if (final_map2[x][y - 1] == 0) {
        search_block2(x, y - 1);
    } else if (final_map2[x][y - 1] == 2) {
        if (x > 0 && x < 13) {
            boundary[36 + 13 - x] = true;
            boundary_number++;
        }
    } else {
    }

    //end
    return;
}

bool decide_if_live()
{
    if (boundary_number > 1) {
        for (int ii = 0; ii < 48; ii++) {
            for (int jj = 0; jj < boundary_number; jj++) {
                int index = ((ii + jj) % 48);
                if (boundary[index] == true && jj == boundary_number - 1) {
                    return false;
                }
                if (boundary[index] == false) {
                    break;
                }
            }
        }
        return true;
    }
    return false;
}

void clear_live_or_dead_map()
{
    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            live_or_dead_map[i][j] = 0;
        }
    }
    for (int ii = 0; ii < 15; ii++) {
        live_or_dead_map[ii][0] = 2;
        live_or_dead_map[0][ii] = 2;
        live_or_dead_map[ii][14] = 2;
        live_or_dead_map[14][ii] = 2;
    }
    return;
}

void dead_or_life2_search(int x, int y)
{
    live_or_dead_map[x][y] = 1;

    //up
    if (live_or_dead_map[x - 1][y] == 0) {
        dead_or_life2_search(x - 1, y);
    } else {
    }
    //right
    if (live_or_dead_map[x][y + 1] == 0) {
        dead_or_life2_search(x, y + 1);
    } else {
    }
    //down
    if (live_or_dead_map[x + 1][y] == 0) {
        dead_or_life2_search(x + 1, y);
    } else {
    }
    //left
    if (live_or_dead_map[x][y - 1] == 0) {
        dead_or_life2_search(x, y - 1);
    } else {
    }

    //end
    return;
}

bool decide_if_2_1(int p1)
{
    clear_live_or_dead_map();
    if (p1 == 1) {
        for (int aa = block_begin1; aa < block_end1; aa++) {
            live_or_dead_map[blocklife1[aa].x][blocklife1[aa].y] = 3;
        }
    }
    if (p1 == 2) {
        for (int aa = block_begin2; aa < block_end2; aa++) {
            live_or_dead_map[blocklife2[aa].x][blocklife2[aa].y] = 3;
        }
    }
    dead_or_life2_search(1, 1);
    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            if (live_or_dead_map[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

void clear_search_array()
{
    for (int i = 0; i < search_array_number; i++) {
        search_array[i] = -1;
    }
    search_array_number = 0;
    return;
}

void search_number(int x, int y)
{

    final_map[x][y] = 1;
    unkown_number += 1;

    for (int aa = 0; aa < 30; aa++) {
        if (x == blocklife1[aa].x && y == blocklife1[aa].y && blocklife1[aa].if_live == false) {
            unkown_number += 1;
        }
        if (x == blocklife2[aa].x && y == blocklife2[aa].y && blocklife2[aa].if_live == false) {
            unkown_number += 1;
        }
    }
    //up
    if (final_map[x + 1][y] == 0) {
        search_number(x + 1, y);
    } else if (final_map[x + 1][y] == 1) {
        final_map[x + 1][y] = 1;
    } else {
        search_array[search_array_number] = final_map[x + 1][y];
        search_array_number++;
    }
    //down
    if (final_map[x - 1][y] == 0) {
        search_number(x - 1, y);
    } else if (final_map[x - 1][y] == 1) {
        final_map[x - 1][y] = 1;
    } else {
        search_array[search_array_number] = final_map[x - 1][y];
        search_array_number++;
    }
    //left
    if (final_map[x][y - 1] == 0) {
        search_number(x, y - 1);
    } else if (final_map[x][y - 1] == 1) {
        final_map[x][y - 1] = 1;
    } else {
        search_array[search_array_number] = final_map[x][y - 1];
        search_array_number++;
    }
    //right
    if (final_map[x][y + 1] == 0) {
        search_number(x, y + 1);
    } else if (final_map[x][y + 1] == 1) {
        final_map[x][y + 1] = 1;
    } else {
        search_array[search_array_number] = final_map[x][y + 1];
        search_array_number++;
    }
    //end
    return;
}

void decide_whose_number()
{
    bool p1 = false;
    bool p2 = false;
    bool side = false;
    for (int i = 0; i < search_array_number; i++) {
        if (search_array[i] == 2) {
            side = true;
        } else if (search_array[i] == 3) {
            p1 = true;
        } else if (search_array[i] == 4) {
            p2 = true;
        }
    }
    if (p1 == true && p2 == false) {
        number_of_p1 += unkown_number;
        unkown_number = 0;
    } else if (p1 == false && p2 == true) {
        number_of_p2 += unkown_number;
        unkown_number = 0;
    } else {
        unkown_number = 0;
    }
    return;
}

int main(int argc, char *argv[])
{
    //--------initialization----------------
    reduction_block();

    introduction();

    reduction_map();
    //--------initialization----------------

	
	//--------   load game  ----------------
	FILE* source = stdin;
	auto_build = false;
	if (argc == 2) {
		FILE* opened = fopen(argv[1], "r");
		if (opened) {
			source = opened;
			auto_build = true;
		}
	}

	//--------   game loop  ----------------
    for (int k = 0; k < 9; k++) {
        x_of_block = 1;
        y_of_block = 1;
        player(1, source);
        x_of_block = 1;
        y_of_block = 1;
        player(0, source);
    }
	if (source != stdin) {
		fclose(source);
	}
	

    int p1num = 0;
    int p2num = 0;
    //new begin
    for (int aaa = 0; aaa < 48; aaa++) {
        boundary[aaa] = false;
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (Map[i][j] == '-') {
                final_map1[i][j] = 2;
                final_map2[i][j] = 2;
            } else if (Map[i][j] == 'O') {
                final_map1[i][j] = 0;
                final_map2[i][j] = 3;
                p1num++;
            } else if (Map[i][j] == 'X') {
                final_map1[i][j] = 3;
                final_map2[i][j] = 0;
                p2num++;
            } else {
                final_map1[i][j] = 3;
                final_map2[i][j] = 3;
            }
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (final_map1[i][j] == 0) {

                search_block1(i, j);

                if (decide_if_live() || decide_if_2_1(1)) {
                    for (int aa = block_begin1; aa < block_end1; aa++) {
                        blocklife1[aa].if_live = true;
                    }
                } else {
                    for (int aa = block_begin1; aa < block_end1; aa++) {
                        blocklife1[aa].if_live = false;
                    }
                }
                block_begin1 = block_end1;
                for (int aaa = 0; aaa < 48; aaa++) {
                    boundary[aaa] = false;
                }
                boundary_number = 0;
            }
        }
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (final_map2[i][j] == 0) {
                search_block2(i, j);

                if (decide_if_live() || decide_if_2_1(2)) {
                    for (int aa = block_begin2; aa < block_end2; aa++) {
                        blocklife2[aa].if_live = true;
                    }
                } else {
                    for (int aa = block_begin2; aa < block_end2; aa++) {
                        blocklife2[aa].if_live = false;
                    }
                }
                block_begin2 = block_end2;
                for (int aaa = 0; aaa < 48; aaa++) {
                    boundary[aaa] = false;
                }
                boundary_number = 0;
            }
        }
    }
    for (int aaa = 0; aaa < 30; aaa++) {
        if (blocklife1[aaa].if_live == false) {
            Map[blocklife1[aaa].x][blocklife1[aaa].y] = '+';
        }
        if (blocklife2[aaa].if_live == false) {
            Map[blocklife2[aaa].x][blocklife2[aaa].y] = '+';
        }
    }
    printf(" ");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf(" %c ", Map[i][j]);
        }
        printf(" \n ");
    }
    //new end
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (Map[i][j] == '-') {
                final_map[i][j] = 2;
            } else if (Map[i][j] == 'O') {
                final_map[i][j] = 3;
            } else if (Map[i][j] == 'X') {
                final_map[i][j] = 4;
            } else {
                final_map[i][j] = 0;
            }
        }
    }
    printf(" ");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf(" %d ", final_map[i][j]);
        }
        printf(" \n ");
    }

    for (int i = 1; i < 14; i++) {
        for (int j = 1; j < 14; j++) {
            if (final_map[i][j] == 0) {

                search_number(i, j);

                decide_whose_number();

                clear_search_array();
            }
        }
    }
    printf("territory of p1: %d\n", number_of_p1);
    printf("territory of p2: %d\n", number_of_p2);
    if (number_of_p1 > number_of_p2) {
        printf("P1 win\n");
    } else if (number_of_p1 < number_of_p2) {
        printf("P2 win\n");
    } else {
        printf("DRAW");
    }

	char save_play;
	printf("save play? [y/n] ");
	std::fflush(stdin);
	scanf("%c", &save_play);
	if (save_play != 'n') {
		printf("Save as: ");
		char save_file_name[100];
		scanf("%s", save_file_name);
		FILE* fp = fopen(save_file_name, "w");
		for (const std::string &history : player_history) {
			fprintf(fp, "%s", history.c_str());
		}
		fclose(fp);
	}

    return 0;
}
