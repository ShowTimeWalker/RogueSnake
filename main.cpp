#include "main.h"

bool quit_flag = false;
extern std::atomic_flag snake_lock;

int main() {
	HideCursor(true);
	Graph graph(GRAPH_HEIGHT, GRAPH_WIDTH, ROWOFFSET, COLOFFSET);
	std::vector<MySnake> snakes;
	std::vector<std::thread> snake_threads;

	// initial snakes begin @params: row, col, dir, graph, len
	snakes.push_back(MySnake(5, 10, 4, graph, 4)); // snake_1
	snakes.push_back(MySnake(15, 10, 4, graph, 4)); // snake_2
	snakes.push_back(MySnake(5, 30, 4, graph, 4)); // snake_3
	snakes.push_back(MySnake(15, 30, 4, graph, 4)); // snake_4
	// initial snakes end

	// introduction
	Introduction(10, 46);
	while (true) {
		if (_getch() == ' ')
			break;
	}
	// register and join
	for (int i = 0; i < snakes.size(); i++)
		snake_threads.push_back(std::thread(snake_work, &snakes[i], &graph));
	//snake_threads.push_back(std::thread(snake_work, &snakes.back(), &graph));
	
	for (auto& th : snake_threads)
		th.detach();
	
	// generate food thread
	std::thread gen_food_thread(gen_food, &graph);
	gen_food_thread.detach();

	// display score
	std::thread disp_score_thread(disp_scroe, &snakes, 22, 46);
	disp_score_thread.detach();

	// count dwon
	std::thread countdown_thread(count_down);
	countdown_thread.detach();

	// user control thread
	std::thread console_control_thread(snake_ctrl, &snakes, &graph);
	console_control_thread.join();

	while (snake_lock.test_and_set());//lock
	LocateCursor(20, 20);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0X0C); // red
	std::cout << "GG" << std::endl;
	LocateCursor(22, 18);
	std::cout << "按ESC退出" << std::endl;
	snake_lock.clear();
	while (true) {
		if (_getch() == 27) // press ESC
			break;
	}
	return 0;
}

void Introduction(int row, int col) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0X0F); // white
	LocateCursor(row++, col);
	std::cout << "游戏说明";
	LocateCursor(row++, col);
	std::cout << "TAB键   切换贪吃蛇";
	LocateCursor(row++, col);
	std::cout << "1/2/3/4 选择贪吃蛇";
	LocateCursor(row++, col);
	std::cout << "方向键  控制贪吃蛇";
	row++;
	LocateCursor(row++, col);
	std::cout << "总得分与蛇长度乘积正相关";
	LocateCursor(row++, col);
	std::cout << "红色：当前选中的贪吃蛇";
	LocateCursor(row++, col);
	std::cout << "青色：非激活贪吃蛇";
	LocateCursor(row++, col);
	std::cout << "尽可能让每条蛇都成长吧！";
	row++;
	LocateCursor(row++, col);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0X0C); // white
	std::cout << "按空格键开始！";
}

void disp_scroe(std::vector<MySnake>* snakes, int row, int col) {
	while (!quit_flag) {
		int y = row, x = col;
		while (snake_lock.test_and_set());//lock
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0X0F); // white
		int score = 1;
		for (int i = 0; i < snakes->size(); i++) {
			LocateCursor(y++, x);
			std::cout << "Snake" << i + 1 << ": " << snakes->at(i).GetLength();
			score *= (snakes->at(i).GetLength() - 3);
		}
		LocateCursor(y + 1, x);
		std::cout << "Score: " << score - 1;
		snake_lock.clear();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void count_down(void) {
	const int CNT = 300;
	for (int i = 0; i <= CNT; i++) {
		if (quit_flag)
			break;
		while (snake_lock.test_and_set());//lock
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0X0F); // white
		LocateCursor(5, 46);
		std::cout << "倒计时: " << std::setfill('0') << std::setw(3) <<CNT - i;
		snake_lock.clear();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	quit_flag = true;
}