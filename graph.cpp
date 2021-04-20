#include "graph.h"

Graph::Graph(int height, int width, int r_offset, int c_offset) {
	m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_cursor.X = 0;
	m_cursor.Y = 0;
	m_height = height;
	m_width = width;
	m_roffset = r_offset;
	m_coffset = c_offset;
	food_num = 0;
	m_matrix.assign(m_height + 2, std::vector<int>(m_width + 2));
	DrawMap();
}

void Graph::DrawMap(void) { // ¡ö takes two pos
	// row = 0, col = [0, width + 1]
	SetConsoleTextAttribute(m_handle, 0X0E);
	for (int i = 0; i <= m_width + 1; i++) {
		LocateCursor(0, i);
		std::cout << "¡ö";
		m_matrix[0][i] = 1;
	}
	// row = height + 1, col = [0, width + 1]
	for (int i = 0; i <= m_width + 1; i++) {
		LocateCursor(m_height + 1, i);
		std::cout << "¡ö";
		m_matrix[m_height + 1][i] = 1;
	}
	// col = 0, row = [1, height]
	for (int i = 1; i <= m_height; i++) {
		LocateCursor(i, 0);
		std::cout << "¡ö";
		m_matrix[i][0] = 1;
	}
	// col = width + 1, row = [1, height]
	for (int i = 1; i <= m_height; i++) {
		LocateCursor(i, m_width + 1);
		std::cout << "¡ö";
		m_matrix[i][m_width + 1] = 1;
	}
	SetConsoleTextAttribute(m_handle, 0X00);
	for (int i = 1; i <= m_height; i++) {
		for (int j = 1; j <= m_width; j++) {
			LocateCursor(i, j);
			std::cout << "¡ö";
		}
	}
	//GenerateFood();
	return;
}

extern std::atomic_flag snake_lock;
void Graph::GenerateFood(void) {
	int row, col;
	srand(time(nullptr));
	do {
		row = rand() % m_height + 1;
		col = rand() % m_width + 1;
	} while (m_matrix[row][col]);
	m_matrix[row][col] = 3;
	while (snake_lock.test_and_set());//lock
	LocateCursor(row, col);
	SetConsoleTextAttribute(m_handle, 0X0F);
	std::cout << "¡ï";
	snake_lock.clear();
	return;
}

void Graph::LocateCursor(int row, int col) {
	m_cursor.X = (col + m_coffset) * 2;
	m_cursor.Y = row + m_roffset;
	SetConsoleCursorPosition(m_handle, m_cursor);
}

extern bool quit_flag;
void gen_food(Graph* graph) {
	while (!quit_flag) {
		if (graph->food_num == 3) {
			std::this_thread::yield();
		}
		else {
			graph->GenerateFood();
			graph->food_num++;
		}
	}
}