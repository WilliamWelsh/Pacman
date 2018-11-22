#include <iostream>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <chrono>
#include "Console.h"

char slots[29][28];
int posX = 22;
int posY = 13;
int score = 0;

int direction = 0; // 0 = right, 1 = left, 2 = up, 3 = down

void writeBoard();
void setColor(int color);
void gameloop();
void handleInput();
void updateScore();

void movePacman();
void moveUp();
void moveDown();
void moveRight();
void moveLeft();

void checkForDot();

void initializeBoard();

void drawT(int row);
void drawRectangle(int row, int pos, int width, int height);

bool isSlotNotWall(int x, int y);

int main()
{
	// Resize console
	RECT r;
	HWND console = GetConsoleWindow();
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 260, 555, TRUE);

	// Center console to middle of screen
	const HWND desktop = GetDesktopWindow();
	GetWindowRect(desktop, &r);
	SetWindowPos(console, 0, r.right / 2 - 130, r.bottom / 2 - 278, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// Set up the game
	Console::ShowConsoleCursor(false);
	system("cls");
	initializeBoard();
	writeBoard();
	updateScore();

	// Start the game and wait for it to end
	std::thread gameloop(gameloop);
	gameloop.join();

	std::cin.ignore();
	return 0;
}

void gameloop()
{
	while (1) // TODO Add a win condition
	{
		handleInput();
		movePacman();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	};
}

void handleInput()
{
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000 || GetAsyncKeyState('D') && 0x8000)
		direction = 0;
	else if (GetAsyncKeyState(VK_LEFT) && 0x8000 || GetAsyncKeyState('A') && 0x8000)
		direction = 1;
	else if (GetAsyncKeyState(VK_UP) && 0x8000 || GetAsyncKeyState('W') && 0x8000)
		direction = 2;
	else if (GetAsyncKeyState(VK_DOWN) && 0x8000 || GetAsyncKeyState('S') && 0x8000)
		direction = 3;
}

void movePacman()
{
  // Delete old pacman
  Console::WriteAt(posY, posX + 2, " ", Color::BRIGHT_WHITE);

	if (direction == 0)
		moveRight();
	else if (direction == 1)
		moveLeft();
	else if (direction == 2)
		moveUp();
	else
		moveDown();

  // Add new pacman
  Console::SetColor(Color::LIGHT_YELLOW);
  Console::SetCursorPosition(posY, posX + 2);
  std::cout << slots[posX][posY] << std::endl;
}

bool isSlotNotWall(int x, int y)
{
	return (slots[x][y] == ' ' || slots[x][y] == (char)249 || slots[x][y] == 'o');
}

// Set the current slot to a space
// then adjust either x or y (int &var + int toAdd)
// then check for a dot/update score
// then set Pacmans to char to something like ^,v,<,>
void adjustSlots(int &var, int toAdd, char newChar)
{
	slots[posX][posY] = ' ';
	var += toAdd;
	checkForDot();
	slots[posX][posY] = newChar;
}

void moveUp()
{
	if (isSlotNotWall(posX-1, posY))
		adjustSlots(posX, -1, 'v');
}

void moveDown()
{
	if (isSlotNotWall(posX+1, posY))
		adjustSlots(posX, 1, '^');
}

void moveRight()
{
	if (isSlotNotWall(posX, posY+1))
		adjustSlots(posY, 1, '<');
	else if (posX == 13 && posY == 27) // Go in the middle right and come out through the left
	{
		slots[posX][posY] = ' ';
		posY = 0;
		checkForDot();
		slots[posX][posY] = '<';
	}
}

void moveLeft()
{
	if (isSlotNotWall(posX, posY-1))
		adjustSlots(posY, -1, '>');
	else if (posX == 13 && posY == 0) // Go in the middle left and come out through the right
	{
		slots[posX][posY] = ' ';
		posY = 27;
		checkForDot();
		slots[posX][posY] = '>';
	}
}

void checkForDot()
{
	if (slots[posX][posY] == (char)249)
	{
		score += 10;
		updateScore();
	}
	else if (slots[posX][posY] == 'o')
	{
		score += 50;
		updateScore();
	}
}

void updateScore()
{
	Console::SetCursorPosition(0, 0);
	setColor(7);
	std::cout << "HIGH SCORE: " << score << std::endl;
}

void writeBoard()
{
	Console::SetCursorPosition(0, 0);
	std::cout << '\n';
	setColor(1); // Blue

	// Top row
	std::cout << (char)201;
	for (int i = 0; i < 12; i++)
		std::cout << (char)205;
	std::cout << (char)209 << (char)209;
	for (int i = 0; i < 12; i++)
		std::cout << (char)205;
	std::cout << (char)187;
	std::cout << std::endl;

	for (unsigned int row = 0; row < 29; row++)
	{
		for (unsigned int column = 0; column < 28; column++)
		{
			// Make the dots/food a color, Pac-Man yellow, and the walls blue
			if (slots[row][column] == (char)249 || slots[row][column] == 'o')
				setColor(7);
			else if (row == 11 && (column == 13 || column == 14))
				setColor(13);
			else
				setColor(1);
			std::cout << slots[row][column];
		}
		std::cout << std::endl;
	}

	// Bottom row
	std::cout << (char)200;
	for (int i = 0; i < 26; i++)
		std::cout << (char)205;
	std::cout << (char)188;
	std::cout << std::endl;
}

void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawRectangle(int row, int pos, int width, int height)
{
	slots[row][pos] = 218; // Top left corner
	slots[row][pos + width + 1] = 191; // Top right corner
	slots[row + height + 1][pos] = 192; // Bottom left corner
	slots[row + height + 1][pos + width + 1] = 217; // Bottom right corner
	// Make the "-'s (width)
	for (int x = 1; x < width + 1; x++)
	{
		slots[row][pos + x] = 196;
		slots[row + height + 1][pos + x] = 196;
	}
	// Make the '|'s (height)
	for (int y = 1; y < height + 1; y++)
	{
		slots[row + y][pos] = 179;
		slots[row + y][pos + width + 1] = 179;
	}

}

void drawT(int row)
{
	// Top capsule
	slots[row][10] = 218; // Top left corner
	slots[row][17] = 191; // Top right corner
	slots[row + 1][10] = 192; // Bottom left corner
	slots[row + 1][17] = 217; // Bottom right corner
	// Top of the T
	for (int i = 1; i < 7; i++)
		slots[row][10 + i] = 196;
	// Bottom parts
	for (int i = 1; i < 3; i++)
	{
		slots[row + 1][10 + i] = 196;
		slots[row + 1][17 - i] = 196;
	}
	slots[row + 1][13] = 191;
	slots[row + 1][14] = 218;
	slots[row + 2][13] = 179;
	slots[row + 2][14] = 179;
	slots[row + 3][13] = 192;
	slots[row + 3][14] = 217;
}

// Build the map
void initializeBoard()
{
	// Initialize the array with dots (food things)
	for (unsigned int row = 0; row < 29; row++)
		for (unsigned int column = 0; column < 28; column++)
			slots[row][column] = 249;
	// Draw some of the vertical edges
	for (int i = 0; i < 8; i++)
	{
		slots[i][0] = 186;
		slots[i][27] = 186;
	}

	// Boxes
	drawRectangle(1, 2, 2, 1);
	drawRectangle(1, 7, 3, 1);
	drawRectangle(1, 16, 3, 1);
	drawRectangle(1, 22, 2, 1);
	
	// Draw center boxdrawRectangle(11, 10, 6, 3)
	slots[11][10] = 201;
	slots[11][17] = 187;
	slots[15][10] = 200;
	slots[15][17] = 188;
	
	for (int i = 1; i < 6 + 1; i++)
	{
		slots[11][10 + i] = 205;
		slots[15][10 + i] = 205;
	}
	
	for (int i = 1; i < 3 + 1; i++)
	{
		slots[11 + i][10] = 186;
		slots[11 + i][17] = 186;
	}

	// Capsules
	drawRectangle(5, 2, 2, 0);
	drawRectangle(5, 22, 2, 0);
	drawRectangle(14, 7, 0, 3);
	drawRectangle(14, 19, 0, 3);
	drawRectangle(20, 7, 3, 0);
	drawRectangle(20, 16, 3, 0);

	// T shapes
	drawT(5);
	drawT(17);
	drawT(23);

	// Top middle thing
	for (int i = 0; i < 4; i++)
	{
		slots[i][13] = 179;
		slots[i][14] = 179;
	}

	// Map border/edges
	slots[8][0] = 200;
	slots[8][27] = 188;
	for (int i = 0; i < 4; i++)
	{
		slots[8][i + 1] = 205;
		slots[8][23 + i] = 205;

		slots[18][4 - i] = 205;
		slots[18][23 + i] = 205;
	}
	slots[8][5] = 187;
	slots[8][22] = 201;
	slots[18][0] = 201;
	slots[18][27] = 187;
	slots[18][22] = 200;

	for (int i = 0; i < 3; i++)
	{
		slots[i + 9][5] = 186;
		slots[i + 9][22] = 186;
		slots[i + 15][5] = 186;
		slots[i + 15][22] = 186;

		slots[i + 19][0] = 186;
		slots[i + 19][27] = 186;
		slots[i + 25][0] = 186;
		slots[i + 25][27] = 186;
		
		// Remove dots inside boxes
		slots[2][i + 8] = ' ';
		slots[2][i + 17] = ' ';
		slots[12][i + 11] = ' ';
		slots[12][i + 14] = ' ';
		slots[13][i + 11] = ' ';
		slots[13][i + 14] = ' ';
		slots[14][i + 11] = ' ';
		slots[14][i + 14] = ' ';
	}
	slots[2][3] = ' ';
	slots[2][4] = ' ';
	slots[2][23] = ' ';
	slots[2][24] = ' ';
	slots[12][5] = 188;
	slots[12][22] = 200;
	slots[18][5] = 188;
	slots[22][0] = 186;
	slots[22][27] = 186;
	slots[28][0] = 186;
	slots[28][27] = 186;

	for (int row = 0; row < 3; row++)
		for (int column = 0; column < 5; column++)
		{
			slots[row + 9][column] = ' ';
			slots[row + 9][23 + column] = ' ';
			slots[row + 15][column] = ' ';
			slots[row + 15][23 + column] = ' ';
		}
	for (int i = 0; i < 5; i++)
	{
		slots[12][i] = 205;
		slots[13][i] = ' ';
		slots[13][23 + i] = ' ';
		slots[14][i] = 205;
		slots[14][23 + i] = 205;
		slots[12][23 + i] = 205;
		slots[11+i][9] = ' ';
		slots[11+i][18] = ' ';
	}
	slots[14][22] = 201;
	slots[13][5] = ' ';
	slots[13][22] = ' ';
	slots[14][5] = 187;
	// Get rid of SOME dots (center area)
	for (int i = 0; i < 10; i++)
	{
		slots[10][9 + i] = ' ';
		slots[16][9 + i] = ' ';
	}
	for (int i = 0; i < 2; i++)
	{
		slots[8 + i][12] = ' ';
		slots[8 + i][15] = ' ';
		slots[9][13+i] = ' ';
		slots[13][7 + i] = ' ';
		slots[13][19 + i] = ' ';
		slots[17 + i][9] = ' ';
		slots[17 + i][18] = ' ';
	}

	// Bottom indents
	slots[23][0] = 200;
	slots[23][1] = 205;
	slots[23][2] = 187;
	slots[24][0] = 201;
	slots[24][1] = 205;
	slots[24][2] = 188;
	slots[23][25] = 201;
	slots[23][26] = 205;
	slots[23][27] = 188;
	slots[24][25] = 200;
	slots[24][26] = 205;
	slots[24][27] = 187;

	// First 'E'/F Thing
	drawRectangle(5, 7, 0, 6);
	drawRectangle(8, 8, 2, 0);
	slots[8][8] = 192;
	slots[9][8] = 218;

	// Second 'E'/F Thing
	drawRectangle(5, 19, 0, 6);
	drawRectangle(8, 16, 2, 0);
	slots[8][19] = 217;
	slots[9][19] = 191;

	// First L
	drawRectangle(20, 4, 0, 3);
	drawRectangle(20, 2, 2, 0);
	slots[21][4] = 191;
	slots[21][5] = 179;

	// Second L
	drawRectangle(20, 22, 0, 3);
	drawRectangle(20, 22, 2, 0);
	slots[21][22] = 179;
	slots[21][23] = 218;
	
	// First very bottom thing (not T)
	drawRectangle(23, 7, 0, 2);
	drawRectangle(26, 2, 8, 0);
	slots[26][7] = 217;
	slots[26][8] = 192;

	// Second very bottom thing (not T)
	drawRectangle(23, 19, 0, 2);
	drawRectangle(26, 16, 8, 0);
	slots[26][19] = 217;
	slots[26][20] = 192;

	// Place bigger dots
	slots[3][1] = 'o';
	slots[3][26] = 'o';
	slots[22][1] = 'o';
	slots[22][26] = 'o';

	// Place Pac-Man
	slots[posX][posY] = '<';
}