#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <chrono>

char slots[28][28];
int posX = 21;
int posY = 13;
int score = 0;

int direction = 0; // 0 = right, 1 = left, 2 = up, 3 = down

void writeBoard();
void setColor(int color);
void gameloop();
void handleInput();

void movePacman();
void moveUp();
void moveDown();
void moveRight();
void moveLeft();

void checkForDot();

void initializeBoard();

void drawRectangle(int row, int pos, int width, int height);
void drawT(int row);

bool isSlotNotWall(int x, int y);

std::string centerText(std::string input, int width);

int main()
{

	RECT r;
	HWND console = GetConsoleWindow();
	// Resize console
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 260, 555, TRUE);

	// Center console to middle of screen
	const HWND desktop = GetDesktopWindow();
	GetWindowRect(desktop, &r);
	SetWindowPos(console, 0, r.right / 2 - 130, r.bottom / 2 - 278, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	initializeBoard();

	std::thread gameloop(gameloop);

	gameloop.join();

	std::cin.ignore();
	return 0;
}

void gameloop()
{
	while (1)
	{
		system("cls");
		writeBoard();
		handleInput();
		movePacman();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	};
}

void handleInput()
{
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
		direction = 0;
	else if (GetAsyncKeyState(VK_LEFT) && 0x8000)
		direction = 1;
	else if (GetAsyncKeyState(VK_UP) && 0x8000)
		direction = 2;
	else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
		direction = 3;
}

void movePacman()
{
	if (direction == 0)
		moveRight();
	else if (direction == 1)
		moveLeft();
	else if (direction == 2)
		moveUp();
	else
		moveDown();
}

bool isSlotNotWall(int x, int y)
{
	return (slots[x][y] == ' ' || slots[x][y] == (char)249);
}

void moveUp()
{
	if (isSlotNotWall(posX-1, posY))
	{
		slots[posX][posY] = ' ';
		posX--;
		checkForDot();
		slots[posX][posY] = 'v';
	}
}

void moveDown()
{
	if (isSlotNotWall(posX+1, posY))
	{
		slots[posX][posY] = ' ';
		posX++;
		checkForDot();
		slots[posX][posY] = '^';
	}
}

void moveRight()
{
	if (isSlotNotWall(posX, posY+1))
	{
		slots[posX][posY] = ' ';
		posY++;
		checkForDot();
		slots[posX][posY] = '<';
	}
	else if (posX == 13 && posY == 27)
	{
		slots[posX][posY] = ' ';
		posY = 0;
		slots[posX][posY] = '<';
	}
}

void moveLeft()
{
	if (isSlotNotWall(posX, posY-1))
	{
		slots[posX][posY] = ' ';
		posY--;
		slots[posX][posY] = '>';
	}
	else if (posX == 13 && posY == 0)
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
		score++;
}

void writeBoard()
{
	setColor(7);
	std::cout << centerText("HIGH SCORE: " + std::to_string(score), 30) << std::endl;

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

	for (unsigned int row = 0; row < 28; row++)
	{
		for (unsigned int column = 0; column < 28; column++)
		{
			// Make the dots/food a color, Pac-Man yellow, and the walls blue
			if (slots[row][column] == '<' || slots[row][column] == '>' || slots[row][column] == '^' || slots[row][column] == 'v')
				setColor(14);
			else if (slots[row][column] == (char)249)
				setColor(7);
			else if (row == 11 && (column == 13 || column == 14))
				setColor(13);
			else
				setColor(1);
			std::cout << slots[row][column];
		}
		//setColor(1); // Blue
		std::cout << std::endl;
	}

	// Bottom row
	std::cout << (char)200;
	for (int i = 0; i < 26; i++)
		std::cout << (char)205;
	std::cout << (char)188;
	std::cout << std::endl;

	setColor(7);
	std::cout << "(" << posX << ", " << posY << ")";
}

void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

std::string centerText(std::string input, int width)
{
	return std::string((width - input.length()) / 2, ' ') + input;
}

void drawRectangle(int row, int pos, int width, int height)
{
	slots[row][pos] = 218; // Top left corner
	slots[row][pos + width + 1] = 191; // Top right corner
	slots[row + height + 1][pos] = 192; // Bottom left corner
	slots[row + height + 1][pos + width + 1] = 217; // Bottom right corner

	for (int x = 1; x < width + 1; x++)
	{
		slots[row][pos + x] = 196;
		slots[row + height + 1][pos + x] = 196;
	}

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

void initializeBoard()
{
	// Build board and map
	for (unsigned int row = 0; row < 28; row++)
		for (unsigned int column = 0; column < 28; column++)
			slots[row][column] = 249;
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
	}
	slots[12][5] = 188;
	slots[12][22] = 200;
	slots[18][5] = 188;
	slots[22][0] = 186;
	slots[22][27] = 186;

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
	}
	slots[14][22] = 201;
	slots[13][5] = ' ';
	slots[13][22] = ' ';
	slots[14][5] = 187;

	// Bottom indents
	slots[23][0] = 200;
	slots[23][1] = 187;
	slots[24][0] = 201;
	slots[24][1] = 188;
	slots[23][26] = 201;
	slots[23][27] = 188;
	slots[24][26] = 200;
	slots[24][27] = 187;

	// Place Pac-Man
	slots[posX][posY] = '<';
}