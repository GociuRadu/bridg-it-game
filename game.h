#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#define EMPTY 0
#define PLAYER_1 1
#define PLAYER_2 2

extern int matrix_size;
extern int matrix[200][200];
extern int counter;
extern bool vs_computer;
extern int winner;

bool click_sound();
bool finished(int row, int col);
void new_start();
void undo();
void init();
void redraw(int row, int col);
void draw_counter();
void moves_number(char s[100], int cx);
int new_game(int pieces);
bool game_end();
void cell_clicked(int row, int col);
int get_winner();
#endif // GAME_H_INCLUDED
