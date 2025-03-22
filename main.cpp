#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <unistd.h>
#include "game.h"
#include "graph.h"
using namespace std;


int main(){
    initwindow(1600, 800);
    menu();
    wait_for_click();
}

