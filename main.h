#pragma once
#include "graph.h"
#include "snake.h"
#include <vector>
#include <iomanip>

#define GRAPH_WIDTH 40
#define GRAPH_HEIGHT 25
#define ROWOFFSET 2
#define COLOFFSET 2

inline void HideCursor(bool hidden) {
	CONSOLE_CURSOR_INFO m_curinfo = { 1, !hidden };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_curinfo);
}

inline void LocateCursor(int row, int col) {
	COORD cursor;
	cursor.X = col * 2;
	cursor.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

void disp_scroe(std::vector<MySnake>* snakes, int row, int col);
void Introduction(int row, int col);
void count_down(void);