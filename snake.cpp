#include "snake.h"

HANDLE MySnake::m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
std::atomic_flag snake_lock = ATOMIC_FLAG_INIT;

MySnake::MySnake(int row, int col, int dir, Graph& graph, int len) {
	m_cursor.X = 0;
	m_cursor.Y = 0;
	m_rinit = row;
	m_cinit = col;
	m_dir = dir;
	m_len = len;
	m_body.assign(0, { 0,0 });
	selected = false;
	islegal = ConstructSnake(graph);
}

bool MySnake::Move(int dir, Graph& graph) {
	if (((dir - 1) / 2) == ((m_dir - 1) / 2)) // direction is parallel
		return true;
	std::pair<int, int> oldhead = m_body.back();
	std::pair<int, int> oldtail = m_body.front();
	m_body.pop_front();// delete old tail
	std::pair<int, int> newhead;
	std::pair<int, int> newtail = m_body.front();
	switch (dir) {
	case 1: // go right
		newhead = std::make_pair(oldhead.first, oldhead.second + 1);
		break;
	case 2: // go left
		newhead = std::make_pair(oldhead.first, oldhead.second - 1);
		break;
	case 3: // go up
		newhead = std::make_pair(oldhead.first - 1, oldhead.second);
		break;
	case 4: // go down
		newhead = std::make_pair(oldhead.first + 1, oldhead.second);
		break;
	}
	m_body.push_back(newhead);
	// update data in graph matrix
	graph.m_matrix[oldtail.first][oldtail.second] = 0;
	// touch wall or body
	if (graph.m_matrix[newhead.first][newhead.second] == 1 || graph.m_matrix[newhead.first][newhead.second] == 2) {
		return false;
	}
	else if (graph.m_matrix[newhead.first][newhead.second] == 3) {
		graph.m_matrix[oldtail.first][oldtail.second] = 2;
		m_body.push_front(oldtail);
		graph.food_num--;
		m_len++;
		newtail = oldtail;
	}
	UpDateSnakeInGraph(oldtail, oldhead, newtail, newhead, graph);
	// update dir
	m_dir = dir;
	// update data in graph matrix
	graph.m_matrix[newhead.first][newhead.second] = 2;
	return true;
}

bool MySnake::Step(Graph& graph) {
	std::pair<int, int> oldhead = m_body.back();
	std::pair<int, int> oldtail = m_body.front();
	m_body.pop_front();// delete old tail
	std::pair<int, int> newhead;
	std::pair<int, int> newtail = m_body.front();
	switch (m_dir) {
	case 1: // go right
		newhead = std::make_pair(oldhead.first, oldhead.second + 1);
		break;
	case 2: // go left
		newhead = std::make_pair(oldhead.first, oldhead.second - 1);
		break;
	case 3: // go up
		newhead = std::make_pair(oldhead.first - 1, oldhead.second);
		break;
	case 4: // go down
		newhead = std::make_pair(oldhead.first + 1, oldhead.second);
		break;
	}
	m_body.push_back(newhead);
	// update data in graph matrix
	graph.m_matrix[oldtail.first][oldtail.second] = 0;
	// touch wall or body
	if (graph.m_matrix[newhead.first][newhead.second] == 1 || graph.m_matrix[newhead.first][newhead.second] == 2) {
		return false;
	}
	else if (graph.m_matrix[newhead.first][newhead.second] == 3) {//eat food
		graph.m_matrix[oldtail.first][oldtail.second] = 2;
		m_body.push_front(oldtail);
		graph.food_num--;
		m_len++;
		newtail = oldtail;
	}
	UpDateSnakeInGraph(oldtail, oldhead, newtail, newhead, graph);
	// update data in graph matrix
	graph.m_matrix[newhead.first][newhead.second] = 2;
	return true;
}

bool MySnake::ConstructSnake(Graph& graph) {
	switch (m_dir) {
	case 1:
		if (m_cinit - (m_len - 1) < 1)
			return false;
		for (int i = 0; i < m_len; i++)
			if (graph.m_matrix[m_rinit][m_cinit - i])
				return false;
		for (int i = 0; i < m_len; i++)
			m_body.push_front(std::make_pair(m_rinit, m_cinit - i));
		break;
	case 2:
		if (m_cinit + (m_len - 1) > graph.m_width)
			return false;
		for (int i = 0; i < m_len; i++)
			if (graph.m_matrix[m_rinit][m_cinit + i])
				return false;
		for (int i = 0; i < m_len; i++)
			m_body.push_front(std::make_pair(m_rinit, m_cinit + i));
		break;
	case 3:
		if (m_rinit + (m_len - 1) > graph.m_height)
			return false;
		for (int i = 0; i < m_len; i++)
			if (graph.m_matrix[m_rinit + i][m_cinit])
				return false;
		for (int i = 0; i < m_len; i++)
			m_body.push_front(std::make_pair(m_rinit + i, m_cinit));
		break;
	case 4:
		if (m_rinit - (m_len - 1) < 1)
			return false;
		for (int i = 0; i < m_len; i++)
			if (graph.m_matrix[m_rinit - i][m_cinit])
				return false;
		for (int i = 0; i < m_len; i++)
			m_body.push_front(std::make_pair(m_rinit - i, m_cinit));
		break;
	}
	DrawSnake(graph);
	return true;
}

void MySnake::DrawSnake(Graph& graph) {
	while (snake_lock.test_and_set());//lock
	if (selected)
		SetConsoleTextAttribute(m_handle, 0X0C); // red
	else
		SetConsoleTextAttribute(m_handle, 0X0B); // light blue
	for (auto p : m_body) {
		LocateCursor(p.first, p.second, graph);
		std::cout << "¡ö";
		graph.m_matrix[p.first][p.second] = 2;
	}
	// draw tial
	LocateCursor(m_body.front().first, m_body.front().second, graph);
	std::cout << "¡ô";
	// draw head
	LocateCursor(m_body.back().first, m_body.back().second, graph);
	std::cout << "¡ñ";
	snake_lock.clear();
}

void MySnake::UpDateSnakeInGraph(std::pair<int, int> oldtail, std::pair<int, int> oldhead,
	std::pair<int, int> newtail, std::pair<int, int> newhead, Graph& graph) {
	while (snake_lock.test_and_set());//lock
	// clear tail
	SetConsoleTextAttribute(m_handle, 0X00); // black
	LocateCursor(oldtail.first, oldtail.second, graph);
	std::cout << "¡ö";
	// change body to new tail
	if (selected)
		SetConsoleTextAttribute(m_handle, 0X0C); // red
	else
		SetConsoleTextAttribute(m_handle, 0X0B); // light blue
	LocateCursor(newtail.first, newtail.second, graph);
	std::cout << "¡ô";
	// change old head to body
	LocateCursor(oldhead.first, oldhead.second, graph);
	std::cout << "¡ö";
	// draw new head
	LocateCursor(newhead.first, newhead.second, graph);
	std::cout << "¡ñ";
	snake_lock.clear();
}

void MySnake::LocateCursor(int row, int col, Graph& graph) {
	m_cursor.X = (col + graph.m_coffset) * 2;
	m_cursor.Y = row + graph.m_roffset;
	SetConsoleCursorPosition(m_handle, m_cursor);
}

extern bool quit_flag;
void snake_work(MySnake* snake, Graph* graph) {
	const int delay_time = 800;
	while (snake->Step(*graph) && !quit_flag) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
	}
	quit_flag = true;
}

void snake_test(MySnake* snake, Graph* graph) {
	const int delay_time = 400;
	while (!quit_flag) {
		snake->Move(1, *graph);//right
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
		snake->Step(*graph);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));

		snake->Move(3, *graph);//up
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
		snake->Step(*graph);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));

		snake->Move(2, *graph);//left
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
		snake->Step(*graph);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));

		snake->Move(4, *graph);//down
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
		snake->Step(*graph);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
	}
}

void snake_ctrl(std::vector<MySnake>* snakes, Graph* graph) {
	int snake_num_old = 0;
	int snake_num_new = 0;
	snakes->at(snake_num_new).selected = true;
	snakes->at(snake_num_new).DrawSnake(*graph);
	const int delay_time = 100;
	while (true && !quit_flag) {
		if (_kbhit()) {
			UCHAR ch = _getch();
			if (ch == 27) {//press ESC to quit
				break;
			}
			else if (ch == 224) {//move key
				char dir = _getch();//77:right 75:left 72:up 80:down
				if (dir == 77)
					snakes->at(snake_num_new).Move(1, *graph);
				else if (dir == 75)
					snakes->at(snake_num_new).Move(2, *graph);
				else if (dir == 72)
					snakes->at(snake_num_new).Move(3, *graph);
				else if (dir == 80)
					snakes->at(snake_num_new).Move(4, *graph);
				std::this_thread::sleep_for(std::chrono::milliseconds(delay_time));
			}
			else {//other key
				int num = snakes->size();
				if (1 <= ch - '0' && ch - '0' <= num) {
					snake_num_new = ch - '1';
				}
				else if (ch == 9) {
					snake_num_new = (snake_num_new + 1) % num;
				}
				else
					continue;
				snakes->at(snake_num_old).selected = false;
				snakes->at(snake_num_old).DrawSnake(*graph);
				snakes->at(snake_num_new).selected = true;
				snakes->at(snake_num_new).DrawSnake(*graph);
				snake_num_old = snake_num_new;
			}
		}
		else {//no press
			std::this_thread::yield();
		}
	}
}