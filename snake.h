#pragma once

#include <iostream>
#include <deque>
#include <Windows.h>
#include "graph.h"
#include <atomic>
#include <chrono>
#include <conio.h>
#include <thread>

class MySnake {
private:
	COORD m_cursor;
	// dir: 1-right, 2-left, 3-up, 4-down
	int m_dir;
	int m_len;
	int m_rinit;
	int m_cinit;
	std::deque<std::pair<int, int>> m_body;
	bool islegal;
public:
	bool selected;
	static HANDLE m_handle;
public:
	MySnake(int row, int col, int dir, Graph& graph, int len = 5);
	bool Move(int dir, Graph& graph);
	bool Step(Graph& graph);
	void DrawSnake(Graph& graph);
	int GetLength(void) { return m_len; }
private:
	bool ConstructSnake(Graph& graph);
	void UpDateSnakeInGraph(std::pair<int, int> oldtail, std::pair<int, int> oldhead,
		std::pair<int, int> newtail, std::pair<int, int> newhead, Graph& graph);
	void LocateCursor(int row, int col, Graph& graph);
};

void snake_test(MySnake* snake, Graph* graph);
void snake_work(MySnake* snake, Graph* graph);
void snake_ctrl(std::vector<MySnake>* snakes, Graph* graph);