#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <conio.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

uint8_t board[28][28] = {};
std::vector<std::array<int, 2>> snake;
int apple[2] = { 8, 8 };

int boardSizeX() {
	return sizeof(board) / sizeof(board[0]);
}
int boardSizeY() {
	return sizeof(board) / sizeof(board[1]);
}

enum TileType {
	Board = 0,
	Snake = 1,
	Apple = 2
};

void moveCursor(int x, int y) {
	std::cout << "\x1b[" << y << ";" << x << "H";
}

void colorConsole(int r, int g, int b) {
	std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m";
}

void clearConsole() {
	std::cout << "\033[2J\033[1;1H";
}

void drawBoard() {
	clearConsole();

	// draw empty
	for (int y = 0; y < boardSizeY(); y++) {
		for (int x = 0; x < boardSizeX(); x++) {
			x % 2 - y % 2 == 0 ? colorConsole(45, 45, 45) : colorConsole(35, 35, 35);
			std::cout << "  ";
		}
		std::cout << "\n";
	}

	moveCursor(1, boardSizeY() + 1);
	colorConsole(0, 0, 0);
}

void drawSection(int x, int y, TileType type) {
	switch (type) {
	case Board:
		moveCursor(x * 2 - 1, y);
		x % 2 - y % 2 == 0 ? colorConsole(45, 45, 45) : colorConsole(35, 35, 35);
		std::cout << "  ";
		break;
	case Snake:
		moveCursor(x * 2 - 1, y);
		x % 2 - y % 2 == 0 ? colorConsole(10, 205, 122) : colorConsole(10, 185, 102);
		std::cout << "  ";
		break;
	case Apple:
		moveCursor(x * 2 - 1, y);
		colorConsole(230, 20, 12);
		std::cout << "  ";
		break;
	}

	moveCursor(1, boardSizeY() + 1);
	colorConsole(0, 0, 0);
}

int main() {
	char userInp = 'd';
	int xVec = 1;
	int yVec = 0;
	bool appleEaten = false;

	snake.push_back({ 2, boardSizeY() / 2 });

	drawBoard();
	drawSection(snake.front()[0], snake.front()[1], Board);

	do {
		drawSection(snake.front()[0], snake.front()[1], Board);

		// get input
		if (_kbhit()) {
			userInp = _getch();

			if (userInp == 'w') { xVec = 0; yVec = -1; }
			if (userInp == 'a') { xVec = -1; yVec = 0; }
			if (userInp == 's') { xVec = 0; yVec = 1; }
			if (userInp == 'd') { xVec = 1; yVec = 0; }
			if (userInp == 'e') { std::string str; std::getline(std::cin, str); }
			if (userInp == '`') { break; }
		}

		// move snake
		snake.push_back({ snake.back()[0] + xVec, snake.back()[1] + yVec });
		if (!appleEaten) { snake.erase(snake.begin()); }
		else { appleEaten = false; }

		// snake wall detection
		if (snake.back()[0] <= 0 || snake.back()[1] <= 0 || snake.back()[0] > boardSizeX() || snake.back()[1] > boardSizeY()) {
			break;
		}
		// snake self detection
		// not yet implemented

		// apple detection
		if (snake.back()[0] == apple[0] && snake.back()[1] == apple[1]) {
			appleEaten = true;
			srand(static_cast<unsigned>(time(nullptr)));
			apple[0] = rand() & boardSizeX();
			srand(static_cast<unsigned>(time(nullptr)));
			apple[1] = rand() & boardSizeY();
		}

		// render 
		drawSection(apple[0], apple[1], Apple);

		for (int i = 0; i < snake.size(); i++) {
			drawSection(static_cast<int>(snake[i][0]), static_cast<int>(snake[i][1]), Snake);
		}

		// sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	} while (true);

	std::cout << "You lose!\nSCORE: " << snake.size() - 1;
	std::cin.get();
	return 0;
}