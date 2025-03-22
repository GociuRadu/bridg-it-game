#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include "game.h"
#include "graph.h"
#define MAX_BT 20//maximul de butoane active
using namespace std;

struct graphics_settings {
    int bkcolor;
    linesettingstype old_line;
    fillsettingstype old_fill;
};


BUTTON bt_null;
BUTTON bt_start;
BUTTON bt_rules;
BUTTON bt_player;
BUTTON bt_piece_type_text;
BUTTON bt_piece_type_circle;
BUTTON bt_piece_type_square;
BUTTON bt_piece_type_diamond;
BUTTON bt_computer;
BUTTON bt_pieces_number_down;
BUTTON bt_pieces_number;
BUTTON bt_pieces_number_up;
BUTTON bt_undo;
BUTTON bt_exit;
BUTTON bt_game;
BUTTON bt_back;
BUTTON bt_player1_color_text;
BUTTON bt_player2_color_text;
BUTTON bt_draw_player1_color;
BUTTON bt_draw_player2_color;
BUTTON bt_sound;
BUTTON* menu_buttons[MAX_BT];
BUTTON* game_buttons[MAX_BT];
BUTTON* active_buttons[MAX_BT];

AREA undo_area;
AREA game_area;

int cell_size;

int w, h;

int pieces;

int piece_type = CIRCLE;

int color[5] = {RED, BLUE, LIGHTGRAY, MAGENTA, YELLOW};
int color_player1 = RED;
int color_player2 = BLUE;

bool inside(int x, int y, int left, int top, int right, int bottom);
bool inside(int x, int y, AREA area);

void init_graphics(int sz, int csz);
void draw(int sz);
void undo_button();
void menu();
void init_buttons();

void diamond(int xc, int yc, int R) {
    linesettingstype old_line;
    getlinesettings(&old_line);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    line(xc, yc - R, xc + R, yc);
    line(xc + R, yc, xc, yc + R);
    line(xc, yc + R, xc - R, yc);
    line(xc - R, yc, xc, yc - R);
    setlinestyle(old_line.linestyle, 0, old_line.thickness);
}

void draw_button_text(BUTTON b) {
    if(b.area.x1 < 0) {
        return;
    }
    int color = b.color;
    int bkcolor = b.bkcolor;
    int old_color = getcolor();
    int old_bkcolor = getbkcolor();
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    setcolor(color);
    setbkcolor(bkcolor);
    setfillstyle(SOLID_FILL, bkcolor);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(b.area.x1 + (b.area.x2 - b.area.x1 - textwidth(b.txt)) / 2, b.area.y1 + (b.area.y2 - b.area.y1 - textheight(b.txt)) / 2, b.txt);
    setcolor(old_color);
    setbkcolor(old_bkcolor);
    setfillstyle(old_fill.pattern, old_fill.color);
}

void draw_piece_type_button(BUTTON b, int piece_type) {
    int bkcolor = b.selected ? b.bkcolor_selected : b.bkcolor;
    int color = b.selected ? b.color_selected : b.color;
    int old_color = getcolor();
    int old_bkcolor = getbkcolor();
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    setcolor(color);
    setbkcolor(bkcolor);
    setfillstyle(SOLID_FILL, bkcolor);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    switch(piece_type) {
        case(CIRCLE) :
            circle((b.area.x1 + b.area.x2) / 2, (b.area.y1 + b.area.y2) / 2, 10);
            floodfill((b.area.x1 + b.area.x2) / 2, (b.area.y1 + b.area.y2) / 2, color);
            break;
        case(SQUARE) :
            bar(b.area.x1, b.area.y1, b.area.x2, b.area.y2);
            break;
        case(DIAMOND) :
            diamond((b.area.x1 + b.area.x2) / 2, (b.area.y1 + b.area.y2) / 2, 10);
            floodfill((b.area.x1 + b.area.x2) / 2, (b.area.y1 + b.area.y2) / 2, color);
            break;

    }
    setcolor(old_color);
    setbkcolor(old_bkcolor);
    setfillstyle(old_fill.pattern, old_fill.color);
}

void draw_circle_piece_button(BUTTON b) {
    draw_piece_type_button(b, CIRCLE);
}

void draw_square_piece_button(BUTTON b) {
    draw_piece_type_button(b, SQUARE);
}

void draw_diamond_piece_button(BUTTON b) {
    draw_piece_type_button(b, DIAMOND);
}

void draw_button(BUTTON b) {//desenez butoane
    if(b.area.x1 < 0) {
        return;
    }
    int bkcolor = b.selected ? b.bkcolor_selected : b.bkcolor;
    int color = b.selected ? b.color_selected : b.color;
    int old_color = getcolor();
    int old_bkcolor = getbkcolor();
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    setcolor(color);
    setbkcolor(bkcolor);
    setfillstyle(SOLID_FILL, bkcolor);
    bar3d(b.area.x1, b.area.y1, b.area.x2, b.area.y2, 0, 1);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(b.area.x1 + (b.area.x2 - b.area.x1 - textwidth(b.txt)) / 2, b.area.y1 + (b.area.y2 - b.area.y1 - textheight(b.txt)) / 2, b.txt);
    setcolor(old_color);
    setbkcolor(old_bkcolor);
    setfillstyle(old_fill.pattern, old_fill.color);
}


void draw_active_buttons() {//aici imi deseneaza doar butoanele active
    for(int i = 0; i < MAX_BT; i++) {
        BUTTON b = *active_buttons[i];
        if(b.draw != NULL) {
            b.draw(b);
        }
    }
}

void start_game() {
    if(bt_player.selected || bt_computer.selected) {
        setbkcolor(CYAN);
        clearviewport();
        winner = EMPTY;
        int sz = new_game(pieces);
        init_graphics(sz, 40);
        memcpy(active_buttons, game_buttons,  sizeof(game_buttons));
        piece_type = CIRCLE;
        color_player1 = RED;
        color_player2 = BLUE;
        vs_computer = bt_computer.selected;
        bt_game.area = game_area;
        bt_undo.area = {game_area.x2 + 1, game_area.y2 - 50, game_area.x2 + 200, game_area.y2};
        if(vs_computer) {
            bt_undo.area = {-1, -1, -1, -1};
        }
        draw_active_buttons();
        draw(sz);
        draw_counter();
    }
}

void show_rules() {
    bt_rules.selected = !bt_rules.selected;
    draw_button(bt_rules);
    if(bt_rules.selected) {
        readimagefile("bridg-it-rules.bmp", 800, 200, 1400, 349);
        readimagefile("tabla.bmp", 900, 350, 1300, 750);
    } else {
        setfillstyle(SOLID_FILL, CYAN);
        bar(800, 200, 1401, 800);
    }
}

void exit_game() {
    cleardevice();
    closegraph();
    exit(0);
}

void back_to_menu() {
    menu();
}


void select_player() {
    bt_player.selected = true;
    bt_computer.selected = false;
    bt_player.draw(bt_player);
    bt_computer.draw(bt_computer);
}

void select_computer() {
    bt_computer.selected = true;
    bt_player.selected = false;
    bt_player.draw(bt_player);
    bt_computer.draw(bt_computer);
}

void in_game_click() {
    int x = mousex();
    int y = mousey();
    int row = get_row(y);
    int col = get_col(x);
    cell_clicked(row, col);
    if(get_winner() != EMPTY) {
        bt_game.area = {-1, -1, -1, -1};
    }
}

void select_circle() {
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    bt_piece_type_circle.selected = true;
    bt_piece_type_square.selected = false;
    bt_piece_type_diamond.selected = false;
    bt_piece_type_circle.draw(bt_piece_type_circle);
    bt_piece_type_square.draw(bt_piece_type_square);
    bt_piece_type_diamond.draw(bt_piece_type_diamond);
    piece_type = CIRCLE;
    draw(matrix_size);
    setfillstyle(old_fill.pattern, old_fill.color);
}

void select_square() {
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    bt_piece_type_square.selected = true;
    bt_piece_type_circle.selected = false;
    bt_piece_type_diamond.selected = false;
    bt_piece_type_circle.draw(bt_piece_type_circle);
    bt_piece_type_square.draw(bt_piece_type_square);
    bt_piece_type_diamond.draw(bt_piece_type_diamond);
    piece_type = SQUARE;
    draw(matrix_size);
    setfillstyle(old_fill.pattern, old_fill.color);
}

void select_diamond() {
    fillsettingstype old_fill;
    getfillsettings(&old_fill);
    bt_piece_type_square.selected = false;
    bt_piece_type_circle.selected = false;
    bt_piece_type_diamond.selected = true;
    bt_piece_type_circle.draw(bt_piece_type_circle);
    bt_piece_type_square.draw(bt_piece_type_square);
    bt_piece_type_diamond.draw(bt_piece_type_diamond);
    piece_type = DIAMOND;
    draw(matrix_size);
    setfillstyle(old_fill.pattern, old_fill.color);
}

void pieces_number_down() {
    if(pieces > 3) {
        pieces--;
        sprintf(bt_pieces_number.txt, "%d", pieces);
        draw_button(bt_pieces_number);
    }
}

void pieces_number_up() {
    if(pieces < 8) {
        pieces++;
        sprintf(bt_pieces_number.txt, "%d", pieces);
        draw_button(bt_pieces_number);
    }
}

void draw_player_color(BUTTON b) {
    for(int i = 0; i < 5; i++) {
        setfillstyle(SOLID_FILL, color[i]);
        bar(b.area.x1 + i * 50, b.area.y1, b.area.x1 + (i + 1) * 50, b.area.y2 + 1);
    }
}

void set_player1_color() {
    if(get_winner()) {
        return;
    }
    int x = mousex();
    int y = mousey();
    int color = getpixel(x, y);
    if(color != color_player1 && color != color_player2){
        color_player1 = color;
        draw(matrix_size);
    }
}

void set_player2_color() {
    if(get_winner()) {
        return;
    }
    int x = mousex();
    int y = mousey();
    int color = getpixel(x, y);
    if(color != color_player1 && color != color_player2){
        color_player2 = color;
        draw(matrix_size);
    }
}

void sound() {
    bt_sound.selected = !bt_sound.selected;
    bar(10, h - 200, 250, h - 150);
    if(bt_sound.selected == false) {
        strcpy(bt_sound.txt, "Sound: Off");
    } else {
        strcpy(bt_sound.txt, "Sound: On");
    }
    bt_sound.draw(bt_sound);
}

bool click_sound() {
    return bt_sound.selected;
}

void init_buttons() {
    bt_null = (BUTTON){{-1, -1, -1, -1}, "", BLACK, BLACK, BLACK, BLACK, false, NULL, NULL};


    bt_start = (BUTTON){{w / 8, h / 5, w / 8 + 250, h / 5 + 50}, "Start Game", BLACK, LIGHTCYAN, WHITE, BLUE , false, start_game, draw_button};
    bt_rules = (BUTTON){{w / 8, 2 * h / 5, w / 8 + 250, 2 * h / 5 + 50}, "Show Rules", BLACK, LIGHTCYAN, WHITE, BLUE, false, show_rules, draw_button};
    bt_exit = (BUTTON){{w / 8, 3 * h / 5,  w / 8 + 250, 3 * h / 5 + 50}, "Exit", BLACK, LIGHTCYAN, WHITE, WHITE, false, exit_game, draw_button};
    bt_pieces_number_down = (BUTTON) {{w / 8 + 270, h / 5 + 10, w / 8 + 300, h / 5 + 40}, "-", BLACK, LIGHTCYAN, WHITE, WHITE, false, pieces_number_down, draw_button};
    bt_pieces_number = (BUTTON) {{w / 8 + 310, h / 5 + 10, w / 8 + 340, h / 5 + 40}, "5", BLACK, LIGHTCYAN, WHITE, WHITE, false, NULL, draw_button};
    bt_pieces_number_up = (BUTTON) {{w / 8 + 350, h / 5 + 10, w / 8 + 380, h / 5 + 40}, "+", BLACK, LIGHTCYAN, WHITE, WHITE, false, pieces_number_up, draw_button};
    bt_player = (BUTTON) {{w / 8 - 150, h / 5 + 70, w / 8 + 100, h / 5 + 120}, "vs Player", BLACK, LIGHTCYAN, WHITE, BLUE, false, select_player, draw_button};
    bt_computer = (BUTTON) {{w / 8 + 150, h / 5 + 70, w / 8 + 400, h / 5 + 120}, "vs Computer", BLACK, LIGHTCYAN, WHITE, BLUE, false, select_computer, draw_button};


    bt_game = (BUTTON){{-1, -1, -1, -1},"", BLACK, CYAN, BLACK, BLACK, false, in_game_click, NULL};
    bt_piece_type_text = (BUTTON) {{100, h / 5, 260, h / 5 + 50}, "Choose pieces type:", BLACK, CYAN, BLACK, WHITE, false, NULL, draw_button_text};
    bt_player1_color_text = (BUTTON) {{100, h / 5 + 100, 260, h / 5 + 150}, "Color player 1:", BLACK, CYAN, BLACK, WHITE, false, NULL, draw_button_text};
    bt_player2_color_text = (BUTTON) {{100, h / 5 + 200, 260, h / 5 + 250}, "Color player 2:", BLACK, CYAN, BLACK, WHITE, false, NULL, draw_button_text};
    bt_draw_player1_color = (BUTTON) {{50, h / 5 + 150, 299, h / 5 + 190}, "", BLACK, BLACK, BLACK, BLACK, false, set_player1_color, draw_player_color};
    bt_draw_player2_color = (BUTTON) {{50, h / 5 + 250, 299, h / 5 + 290}, "", BLACK, BLACK, BLACK, BLACK, false, set_player2_color, draw_player_color};
    bt_undo = (BUTTON){{game_area.x2 + 1, game_area.y2 - 50, game_area.x2 + 200, game_area.y2 + 50},"UNDO", BLACK, CYAN, BLACK, CYAN, false, undo, draw_button};
    bt_back = (BUTTON) {{10, h - 100, 110, h - 50}, "<--", BLACK, LIGHTCYAN, BLACK, WHITE, false, back_to_menu, draw_button};
    bt_piece_type_circle = (BUTTON) {{100, h / 5 + 60, 120, h / 5 + 80}, "", BLACK, BLACK, RED, RED, true, select_circle, draw_circle_piece_button};
    bt_piece_type_square = (BUTTON) {{150, h / 5 + 60, 170, h / 5 + 80}, "", BLACK, BLACK, RED, RED, false, select_square, draw_square_piece_button};
    bt_piece_type_diamond = (BUTTON) {{200, h / 5 + 60, 220, h / 5 + 80}, "", BLACK, BLACK, RED, RED, false, select_diamond, draw_diamond_piece_button};
    bt_sound = (BUTTON) {{10, h - 200, 250, h - 150}, "Sound: Off", BLACK, CYAN, BLACK, CYAN, false, sound, draw_button};

    for(int i = 0; i < MAX_BT; i++) {
        menu_buttons[i] = &bt_null;
        game_buttons[i] = &bt_null;
    }
    int k = 0;
    menu_buttons[k++] = &bt_start;
    menu_buttons[k++] = &bt_rules;
    menu_buttons[k++] = &bt_exit;
    menu_buttons[k++] = &bt_pieces_number_down;
    menu_buttons[k++] = &bt_pieces_number;
    menu_buttons[k++] = &bt_pieces_number_up;
    menu_buttons[k++] = &bt_player;
    menu_buttons[k++] = &bt_computer;

    k = 0;
    game_buttons[k++] = &bt_undo;
    game_buttons[k++] = &bt_game;
    game_buttons[k++] = &bt_back;
    game_buttons[k++] = &bt_piece_type_text;
    game_buttons[k++] = &bt_piece_type_circle;
    game_buttons[k++] = &bt_piece_type_square;
    game_buttons[k++] = &bt_piece_type_diamond;
    game_buttons[k++] = &bt_player1_color_text;
    game_buttons[k++] = &bt_player2_color_text;
    game_buttons[k++] = &bt_draw_player1_color;
    game_buttons[k++] = &bt_draw_player2_color;
    game_buttons[k++] = &bt_sound;
}


void wait_for_click() {
    while(true) {
        if(ismouseclick(WM_LBUTTONDOWN)) {
            clearmouseclick(WM_LBUTTONDOWN);
            int x = mousex();
            int y = mousey();
            for(int i = 0; i < MAX_BT; i++) {
                if(inside(x, y, (*active_buttons[i]).area)) {
                    if((*active_buttons[i]).fn != NULL) {
                        (*active_buttons[i]).fn();
                    }
                    break;
                }
            }
        }
    }
}

void menu() {
    w = getwindowwidth();
    h = getwindowheight();
    setbkcolor(CYAN);
    cleardevice();

    setcolor(RED);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 6);
    outtextxy(w / 3 + (2 * w / 3 - w / 3 - textwidth("BRIDG-IT")) / 2, 50 + (100 - 50 - textheight("BRIDG-IT")) / 2, "BRIDG-IT");
    pieces = 5;
    init_buttons();
    memcpy(active_buttons, menu_buttons, sizeof(menu_buttons));
    draw_active_buttons();
}



bool inside(int x, int y, int left, int top, int right, int bottom) {
    return (x >= left && x <= right && y >= top && y <= bottom);
}

bool inside(int x, int y, AREA area) {
    return inside(x, y, area.x1, area.y1, area.x2, area.y2);
}



void draw_counter() {
    char tmp[20];
    setfillstyle(SOLID_FILL, CYAN);
    sprintf(tmp, "Moves: %d", counter);
    bar(game_area.x2 + 50, game_area.y1 - 50, game_area.x2 + 400, game_area.y1 - 50 + 100);
    setcolor(BLACK);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(game_area.x2 + 50, game_area.y1 - 50, tmp);
}





void draw_circle(int x1, int y1, int x2, int y2, int color) {
    setcolor(color);
    circle((x1 + x2) / 2, (y1 + y2) /2 , cell_size / 2 - 2);
    setfillstyle(SOLID_FILL, color);
    floodfill((x1 + x2) / 2, (y1 + y2) / 2, color);
}

void draw_rectangle(int x1, int y1, int x2, int y2, int color){
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
}

void draw_diamond(int xc, int yc, int color) {
    setcolor(color);
    linesettingstype old_line;
    getlinesettings(&old_line);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    line(xc, yc - cell_size / 2, xc + cell_size / 2, yc);
    line(xc + cell_size / 2, yc, xc, yc + cell_size / 2);
    line(xc, yc + cell_size / 2, xc - cell_size / 2, yc);
    line(xc - cell_size  / 2, yc, xc, yc - cell_size / 2);
    setfillstyle(SOLID_FILL, color);
    floodfill(xc, yc, color);
    setlinestyle(old_line.linestyle, old_line.upattern, old_line.thickness);
}

void draw_piece(int row, int col, int player) {
    int x1 = game_area.x1 + col * cell_size;
    int x2 = game_area.x1 + (col + 1) * cell_size - 1;
    int y1 = game_area.y1 + row * cell_size;
    int y2 = game_area.y1 + (row + 1) * cell_size - 1;
    int color = player == PLAYER_1 ? color_player1 : color_player2;
    switch(piece_type){
        case 1:
            draw_circle(x1, y1, x2, y2, color);
            break;
        case 2:
            draw_rectangle(x1, y1, x2, y2, color);
            break;
        case 3:
            draw_diamond((x1 + x2) / 2, (y1 + y2) / 2, color);
            break;
    }
}

void draw_line(int row, int col, int player) {
    int color;
    color = (player == PLAYER_1 ? color_player1 : color_player2);
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    if ((player == PLAYER_1 && row % 2 == 0) || (player == PLAYER_2 && row % 2 == 1)) {//oriz
        bar(game_area.x1 + col * cell_size , game_area.y1 + row * cell_size + cell_size / 2 - 2,
             game_area.x1 + (col + 1) * cell_size - 2, game_area.y1 + row * cell_size + cell_size / 2 + 2);
    } else { // vert
        bar(game_area.x1 + col * cell_size + cell_size / 2 - 2 , game_area.y1 + row * cell_size,
            game_area.x1 + col * cell_size + cell_size / 2 + 2, game_area.y1 + (row + 1) * cell_size - 2);
    }
}

void margin(int row, int col, int player) {
    if(player == EMPTY && row != col && abs(row - col) != matrix_size - 1){
        if(row == 0 || row == matrix_size - 1){
            draw_line(row, col, PLAYER_1);
        } else
            if(col == 0 || col == matrix_size - 1){
                draw_line(row, col, PLAYER_2);
            }
    }
}
void draw_cell(int row, int col, int player) {
    if (player != EMPTY) {
        if (row % 2 == col % 2) {
            draw_line(row, col, player);
        } else {
            draw_piece(row, col, player);
        }
    }
}



void init_graphics(int n, int csz) {
    cell_size = csz;
    game_area.x1 = (w - n * cell_size) / 2;
    game_area.y1 = (h - n * cell_size) / 2;
    game_area.x2 = game_area.x1 + n * cell_size - 1;
    game_area.y2 = game_area.y1 + n * cell_size - 1;
}

void draw(int n) {
    setfillstyle(SOLID_FILL, CYAN);
    bar(game_area.x1, game_area.y1, game_area.x2, game_area.y2);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            margin(i, j, matrix[i][j]);
        }
    }
    for(int i = 0; i < n ; i++) {
        for (int j = 0; j < n ; j++) {
            draw_cell(i, j, matrix[i][j]);
        }
    }
}

void redraw(int row, int col) {
    setfillstyle(SOLID_FILL, CYAN);
    bar(game_area.x1 + col * cell_size - 1, game_area.y1 + row * cell_size - 1, game_area.x1 + (col + 1) * cell_size, game_area.y1 + (row + 1)  * cell_size - 1);
    draw_counter();
}

int get_row(int y) {
    return (y - game_area.y1) / cell_size;
}

int get_col(int x) {
    return (x - game_area.x1) / cell_size;
}

bool in_game_area(int x, int y) {
    return x >= game_area.x1 && x <= game_area.x2 && y >= game_area.y1 && y <= game_area.y2;
}

void show_winner(int winner)
{
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);

    if (winner == PLAYER_1)
    {
        setcolor(color_player1);
        outtextxy(30, 30, "Winner is Player 1!");
    }
    else if (winner == PLAYER_2) {
        setcolor(color_player2);
        outtextxy(30, 30, "Winner is Player 2!");
    }

}


