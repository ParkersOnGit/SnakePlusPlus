#include <iostream>
#include <vector>
#include <array>
#include <conio.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

std::array<int, 2> boardSize = { 28, 28 };
std::vector<std::array<int, 2>> snake = {{ 2, boardSize[1] / 2 }};
std::array<int, 2> apple = { 8, boardSize[1] / 2 };

enum TileType {
	Board = 0,
	Snake = 1,
	Apple = 2
};

struct Vector2i {
	int x;
	int y;
	Vector2i(int xVal = 0, int yVal = 0) {
		x = xVal; y = yVal;
	}
};

void moveCursor(int x, int y) { std::cout << "\x1b[" << y << ";" << x << "H"; }
void colorConsole(int r, int g, int b) { std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"; }
void clearColor() { std::cout << "\x1b[0m"; }
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
	clearColor();
}

int main() {
	std::random_device rand;

	char prevUserInp = 'd';
	Vector2i Direction = Vector2i(1, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	drawBoard();
	drawSection(snake.front()[0], snake.front()[1], Board);
	drawSection(apple[0], apple[1], Apple);

	do {
		// Draw the board on the section of snake that no longer exists.
		drawSection(snake.front()[0], snake.front()[1], Board);

		// Get the user input.
		if (_kbhit()) {
			char userInp = _getch();

			if (userInp == 'w' && prevUserInp != 's') { Direction.x = 0; Direction.y = -1; prevUserInp = userInp; }
			else if (userInp == 'a' && prevUserInp != 'd') { Direction.x = -1; Direction.y = 0; prevUserInp = userInp; }
			else if (userInp == 's' && prevUserInp != 'w') { Direction.x = 0; Direction.y = 1; prevUserInp = userInp; }
			else if (userInp == 'd' && prevUserInp != 'a') { Direction.x = 1; Direction.y = 0; prevUserInp = userInp; }
		}

		// Update snake position.
		snake.push_back({ snake.back()[0] + Direction.x, snake.back()[1] + Direction.y });

		// Check if apple is eaten. If not, erase last section of snake.
		if (snake.back()[0] == apple[0] && snake.back()[1] == apple[1]) {
			do {
				apple[0] = rand() % boardSize[0] + 1;
				apple[1] = rand() % boardSize[1] + 1;
			} while (std::find(snake.begin(), snake.end(), std::array<int, 2> { apple[0], apple[1] }) != snake.end());
			drawSection(apple[0], apple[1], Apple);
		}
		else { snake.erase(snake.begin()); }

		// Check if snake is out of bounds, or snake has collided with itself.
		if (snake.back()[0] <= 0 || snake.back()[1] <= 0 || snake.back()[0] > boardSize[0] || snake.back()[1] > boardSize[1]) { break; }
		if (std::find(snake.begin(), snake.end() - 1, snake.back()) != snake.end() - 1) { break; }

		// Draw snake section.
		drawSection(static_cast<int>(snake.back()[0]), static_cast<int>(snake.back()[1]), Snake);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	} while (true);

	moveCursor(1, boardSize[1] + 1);
	clearColor();

	std::cout << "You lose!\nSCORE: " << snake.size() - 1;
	std::cin.get();
	return 0;
}