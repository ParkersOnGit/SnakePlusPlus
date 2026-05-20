#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <conio.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

uint8_t boardSize[2] = { 28, 28 };
std::vector<std::array<int, 2>> snake;
int apple[2];

enum TileType {
	Board = 0,
	Snake = 1,
	Apple = 2
};

void moveCursor(int x, int y) { std::cout << "\x1b[" << y << ";" << x << "H"; }
void colorConsole(int r, int g, int b) { std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"; }
void clearConsole() { std::cout << "\033[2J\033[1;1H"; }

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
}

void drawBoard() {
	clearConsole();

	for (int y = 1; y <= boardSize[1]; y++) {
		for (int x = 1; x <= boardSize[0]; x++) {
			drawSection(x, y, Board);
		}
		std::cout << "\n";
	}

	moveCursor(1, boardSize[1] + 1);
	colorConsole(0, 0, 0);
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());

	char prevUserInp = 'd';
	int xVec = 1;
	int yVec = 0;
	bool appleEaten = false;

	snake.push_back({ 2, boardSize[1] / 2 });
	apple[0] = 8; apple[1] = boardSize[1] / 2;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	drawBoard();
	drawSection(snake.front()[0], snake.front()[1], Board);
	drawSection(apple[0], apple[1], Apple);

	do {
		drawSection(snake.front()[0], snake.front()[1], Board);

		if (_kbhit()) {
			char userInp = _getch();

			if (userInp == 'w' && prevUserInp != 's') { xVec = 0; yVec = -1; prevUserInp = userInp; }
			else if (userInp == 'a' && prevUserInp != 'd') { xVec = -1; yVec = 0; prevUserInp = userInp; }
			else if (userInp == 's' && prevUserInp != 'w') { xVec = 0; yVec = 1; prevUserInp = userInp; }
			else if (userInp == 'd' && prevUserInp != 'a') { xVec = 1; yVec = 0; prevUserInp = userInp; }
			else if (userInp == 'e') { std::string str; std::getline(std::cin, str); }
			else if (userInp == '`') { break; }
			else if (userInp == 'P') { appleEaten = true; }
		}

		snake.push_back({ snake.back()[0] + xVec, snake.back()[1] + yVec });
		if (!appleEaten) { snake.erase(snake.begin()); }
		else { appleEaten = false; }

		if (snake.back()[0] <= 0 || snake.back()[1] <= 0 || snake.back()[0] > boardSize[0] || snake.back()[1] > boardSize[1]) { break; }
		if (std::find(snake.begin(), snake.end() - 1, snake.back()) != snake.end() - 1) { break; }

		if (snake.back()[0] == apple[0] && snake.back()[1] == apple[1]) {
			appleEaten = true;
			do {
				apple[0] = rd() % boardSize[0] + 1;
				apple[1] = rd() % boardSize[1] + 1;

				if (std::find(snake.begin(), snake.end(), std::array<int, 2> { apple[0], apple[1] }) == snake.end()) {
					break;
				}

			} while (true);

			drawSection(apple[0], apple[1], Apple);
		}

		drawSection(static_cast<int>(snake.back()[0]), static_cast<int>(snake.back()[1]), Snake);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	} while (true);

	moveCursor(1, boardSize[1] + 1);
	colorConsole(0, 0, 0);

	std::cout << "You lose!\nSCORE: " << snake.size() - 1;
	std::cin.get();
	return 0;
}