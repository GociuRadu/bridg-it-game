#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#define CIRCLE 1
#define SQUARE 2
#define DIAMOND 3

extern int w;
extern int h;
extern bool startGameSelected;
extern int color_player1;
extern int color_player2;

struct AREA {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct BUTTON {
    AREA area;
    char txt[30];
    int color;
    int bkcolor;
    int color_selected;
    int bkcolor_selected;
    bool selected;
    void (*fn)();// ce face butonul
    void (*draw)(BUTTON b);//deseneaza butonul
};

void show_winner(int winner);
void wait_for_click();
bool inside(int x, int y, int left, int top, int right, int bottom);
void menu();
void undo_clicked(int x, int y);
void undo_button();
void init_graphics(int n, int cell_size);
void Main_Menu();
void Count();
void draw(int n);
bool in_game_area(int x, int y);
int get_row(int y);
int get_col(int x);
void draw_cell(int row, int col, int player);
#endif // GRAPH_H_INCLUDED
