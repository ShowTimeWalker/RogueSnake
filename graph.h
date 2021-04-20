#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <atomic>
#include <thread>

class Graph {
private:
	HANDLE m_handle;
	COORD m_cursor;
	int m_height;
	int m_width;
	int m_roffset;
	int m_coffset;
	// matrix: 1-wall; 2-snake; 3-food
	std::vector<std::vector<int>> m_matrix;
public:
	int food_num;
	friend class MySnake;
	Graph(int height = 0, int width = 0, int r_offset = 0, int c_offset = 0);
	void GenerateFood(void);
	int GetWidth(void) { return m_width; }
	int GetHeight(void) { return m_height; }
	int GetRoffset(void) { return m_roffset; }
	int GetCoffset(void) { return m_coffset; }
private:
	void DrawMap(void);
	void LocateCursor(int x, int y);
};

void gen_food(Graph* graph);

/*
0 = ��ɫ    8 = ��ɫ    1 = ����      9 = ��ɫ
2 = ����    A = ��ɫ    3 = ����      B = ��ǳ��
C = ��ɫ    4 = ����    5 = ��ɫ      D = ����
6 = ��ɫ    E = ����    7 = ��ɫ      F = ����
*/