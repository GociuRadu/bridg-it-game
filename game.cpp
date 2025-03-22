#include <iostream>
#include "game.h"
#include "graph.h"
#include <queue>
#include <string.h>
#include <stdio.h>
#include <graphics.h>
#include <time.h>

using namespace std;
int matrix_size;
int matrix[200][200];
int winner = EMPTY;
int counter;

typedef struct {
    int row;
    int col;
} POZ;

POZ position[100];
bool vs_computer;


void init();

bool is_margin_1(int row, int col, int player) {
    return player == PLAYER_1 ? row == 0 : col == 0;
}

bool is_margin_2(int row, int col, int player) {
    return player == PLAYER_1 ? row == matrix_size - 1 : col == matrix_size - 1;
}

bool OK(int row, int col, int lee[200][200]) {
    return row >= 0 && col >= 0 && row < matrix_size && col < matrix_size && lee[row][col] == 0;
}
bool inside(int row, int col)
{
    return row > 1 && col > 0 && row < matrix_size-1 && col < matrix_size ;
}

bool finished(int row, int col) {
    bool m1 = false; // a fost atinsa prima margine
    bool m2 = false; //a fost atinsa a doua margine
    int Lee[200][200];
    pair <int, int> directions[4] = {{0, -1},{-1, 0}, {0, 1}, {1, 0}};
    queue < pair <int, int > > q;
    int player = matrix[row][col];
    for(int i = 0; i < matrix_size; i++) {
        for(int j = 0; j < matrix_size; j++) {
            if(matrix[i][j] == player) {
                Lee[i][j] = 0;
            } else {
                Lee[i][j] = -1;
            }
        }
    }
    Lee[row][col] = 1;
    q.push({row, col});
    while(!q.empty() && (!m1 || !m2)) {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
            m1 = m1 || is_margin_1(r, c, player);
            m2 = m2 || is_margin_2(r, c, player);
            for(int i = 0; i < 4; i++) {
                int next_r = r + directions[i].first;
                int next_c = c + directions[i].second;
                if(OK(next_r, next_c, Lee)) {
                    Lee[next_r][next_c] = 1;
                    q.push({next_r, next_c});
                }
            }
        }
        return m1 && m2;
    }

int new_game(int pieces) {
    matrix_size = 2 * pieces + 1;
    init();
    counter = 0;
    return matrix_size;
}

void undo() {
    int r = position[counter].row;
    int c = position[counter].col;
    if(counter > 0 && !finished(r, c)) {
        matrix[r][c] = EMPTY;
        counter--;
        redraw(r, c);
    }
}



void init() {
    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++) {
            matrix[i][j] = EMPTY;
        }
    }
    for (int i = 0; i < matrix_size; i++) {
        for(int j = 0; j < matrix_size; j++) {
            if(i % 2 == 0 && j % 2 == 1)
                matrix[i][j] = PLAYER_1;
            else if(i % 2 == 1 && j % 2 == 0)
                matrix[i][j] = PLAYER_2;
        }
    }
}

POZ next_computer_move(int  row,int  col)
{
    delay(100);
    int row_p=row;
    int col_p=col;
    POZ p;

     if(matrix[row_p+2][col_p]==0 && inside(row_p,col_p ) && row_p+3<matrix_size)
    {
        p.row=row_p+2;
        p.col=col_p;

    }
    else

     if(matrix[row_p-2][col_p]==0 && inside(row_p,col_p ) && row_p-2>0)
    {
        p.row=row_p-2;
        p.col=col_p;
    }
else


        do
        {
            p.row = 1+ rand() % (matrix_size -2);
            p.col = 1+ rand() % (matrix_size -2);

        }while(matrix[p.row][p.col] != EMPTY);


    return p;

}



void cell_clicked(int row, int col) {
    if(row <= 0 || row >= matrix_size - 1 || col <= 0 || col >= matrix_size - 1) {
        return;
    }
    if (matrix[row][col] == EMPTY) {
        matrix[row][col] = counter % 2 == 0 ? PLAYER_1 : PLAYER_2;
        draw_cell(row, col, matrix[row][col]);
        counter++;
        draw_counter();
        if(click_sound()) {
            Beep(400, 400);
        }
        position[counter].row = row;
        position[counter].col = col;
        if(finished(row, col)) {
            winner = matrix[row][col];
            show_winner(winner);
            return;
        }
    }
    if(vs_computer && counter % 2 != 0) {
        POZ p = next_computer_move(row,col);
        cell_clicked(p.row, p.col);
    }

}

int get_winner() {
    return winner;
}

