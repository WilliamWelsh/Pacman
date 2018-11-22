// Copyright 2018 Krystian Stasiowski

#include "Console.h"

void Console::Clear()
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD topleft = {0, 0};
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;
  GetConsoleScreenBufferInfo(handle, &screen);
  FillConsoleOutputCharacterA(handle, ' ', screen.dwSize.X * screen.dwSize.Y, topleft, &written);
  FillConsoleOutputAttribute(handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topleft, &written);
  SetConsoleCursorPosition(handle, topleft);
}

void Console::ClearLine(unsigned index)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos = {0, index};
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;
  GetConsoleScreenBufferInfo(handle, &screen);
  FillConsoleOutputCharacterA(handle, ' ', screen.dwSize.X, pos, &written);
  FillConsoleOutputAttribute(handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X, pos, &written);
  SetConsoleCursorPosition(handle, pos);
}


void Console::ClearAt(unsigned x, unsigned y, unsigned length)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos = {x, y};
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;
  GetConsoleScreenBufferInfo(handle, &screen);
  FillConsoleOutputCharacterA(handle, ' ', length, pos, &written);
  FillConsoleOutputAttribute(handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, length, pos, &written);
  SetConsoleCursorPosition(handle, pos);
}

void Console::Resize(unsigned x, unsigned y)
{
  HWND console = GetConsoleWindow();
  RECT rect;
  GetWindowRect(console, &rect);
  MoveWindow(console, rect.left, rect.top, x, y, TRUE);
}

void Console::SetCursorPosition(unsigned x, unsigned y)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos = {x, y};
  SetConsoleCursorPosition(handle, pos);
}

COORD Console::GetCursorPosition()
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  GetConsoleScreenBufferInfo(handle, &screen);
  return screen.dwCursorPosition;
}

COORD Console::GetSize()
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  GetConsoleScreenBufferInfo(handle, &screen);
  return screen.dwSize;
}

void Console::ShowConsoleCursor(bool show)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  GetConsoleCursorInfo(handle, &info);
  info.bVisible = show;
  SetConsoleCursorInfo(handle, &info);
}

void Console::SetTitle(const std::string& title)
{
  SetConsoleTitle(title.c_str());
}

void Console::SetColor(Color color)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, static_cast<WORD>(color));
}

void Console::SetColor(Color color, Color highlight)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, static_cast<WORD>(color) | (static_cast<WORD>(highlight) << 4));
}

void Console::Write(const std::string& text, Color color, Color highlight)
{
  SetColor(color, highlight);
  std::cout << text;
}

void Console::WriteAt(unsigned x, unsigned y, const std::string& text)
{
  SetCursorPosition(x, y);
  Write(text);
}

void Console::WriteCentered(const std::string& text, Color color, Color highlight)
{
  Write(CenterString(text), color, highlight);
}

void Console::WriteAt(unsigned x, unsigned y, const std::string& text, Color color)
{
  SetCursorPosition(x, y);
  Write(text, color);
}

void Console::Write(const std::string& text, Color color)
{
  Write(text, color, Color::BLACK);
}

void Console::Write(const std::string& text)
{
  Write(text, Color::WHITE);
}

void Console::WriteAt(unsigned x, unsigned y, const std::string& text, Color color, Color highlight)
{
  SetCursorPosition(x, y);
  Write(text, color, Color::BLACK);
}

void Console::WriteCentered(const std::string& text, Color color)
{
  Write(CenterString(text), color);
}

void Console::WriteCentered(const std::string& text)
{
  WriteCentered(text, Color::WHITE);
}

void Console::WriteLine(const std::string& text, Color color, Color highlight)
{
  Write(text, color, highlight);
  std::cout << std::endl;
}

void Console::WriteLine(const std::string& text, Color color)
{
  WriteLine(text, color, Color::BLACK);
}

void Console::WriteLine(const std::string& text)
{
  WriteLine(text, Color::WHITE);
}

void Console::WriteLineAt(unsigned x, unsigned y, const std::string& text, Color color, Color highlight)
{
  SetCursorPosition(x, y);
  WriteLine(text, color, highlight);
}

void Console::WriteLineAt(unsigned x, unsigned y, const std::string& text)
{
  SetCursorPosition(x, y);
  WriteLine(text);
}

void Console::WriteLineCentered(const std::string& text, Color color, Color highlight)
{
  WriteLine(CenterString(text), color, highlight);
}

void Console::WriteLineAt(unsigned x, unsigned y, const std::string& text, Color color)
{
  SetCursorPosition(x, y);
  WriteLine(text, color);
}

void Console::WriteLineCentered(const std::string& text, Color color)
{
  WriteLineCentered(text, color, Color::BLACK);
}

void Console::WriteLineCentered(const std::string& text)
{
  WriteLineCentered(text, Color::WHITE);
}

void Console::WriteList(const std::vector<std::string>& text, Color color, Color highlight)
{
  for (const std::string& s : text)
  {
    WriteLine(s, color, highlight);
  }
}

void Console::WriteList(const std::vector<std::string>& text, Color color)
{
  WriteList(text, color, Color::BLACK);
}

void Console::WriteList(const std::vector<std::string>& text)
{
  for (const std::string& s : text)
  {
    WriteLine(s, Color::WHITE);
  }
}

void Console::WriteListCentered(const std::vector<std::string>& text, Color color, Color highlight)
{
  for (const std::string& s : text)
  {
    WriteLineCentered(s, color, highlight);
  }
}

void Console::WriteListCentered(const std::vector<std::string>& text, Color color)
{
  WriteListCentered(text, color, Color::BLACK);
}

void Console::WriteListCentered(const std::vector<std::string>& text)
{
  for (const std::string& s : text)
  {
    WriteLineCentered(s, Color::WHITE);
  }
}

void Console::ColorLine(Color color, unsigned length)
{
  SetColor(Color::WHITE, color);
  std::string line;
  for (unsigned i = 0; i < length; ++i)
    line += " ";
  std::cout << line;
}

void Console::ColorLineCentered(Color color, unsigned length)
{
  SetColor(Color::WHITE, color);
  unsigned x = (GetSize().X - length) / 2;
  std::string line;
  for (unsigned i = 0; i < length; ++i)
    line += " ";
  SetCursorPosition(x, GetCursorPosition().Y);
  std::cout << line;
}

std::string Console::ReadLine()
{
  std::string input;
  std::getline(std::cin, input);
  return input;
}

std::string Console::ReadLine(Color color)
{
  SetColor(color, Color::BLACK);
  std::string input;
  std::getline(std::cin, input);
  return input;
}

std::string Console::ReadLine(Color color, Color highlight)
{
  SetColor(color, highlight);
  std::string input;
  std::getline(std::cin, input);
  return input;
}

std::string Console::ReadLinePrompt(const std::string& prompt, Color color)
{
  Write(prompt, color);
  return ReadLine();
}

std::string Console::ReadLinePrompt(const std::string& prompt, Color color, Color highlight)
{
  Write(prompt, color, highlight);
  return ReadLine();
}

std::string Console::ReadLinePrompt(const std::string& prompt)
{
  Write(prompt);
  return ReadLine();
}

std::string Console::ReadLineAt(unsigned x, unsigned y)
{
  SetCursorPosition(x, y);
  return ReadLine();
}

std::string Console::ReadLineAt(unsigned x, unsigned y, Color color)
{
  SetCursorPosition(x, y);
  return ReadLine(color);
}

std::string Console::ReadLineAt(unsigned x, unsigned y, Color color, Color highlight)
{
  SetCursorPosition(x, y);
  return ReadLine(color, highlight);
}

std::string Console::ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt, Color color)
{
  SetCursorPosition(x, y);
  return ReadLinePrompt(prompt, color);
}

std::string Console::ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt)
{
  SetCursorPosition(x, y);
  return ReadLinePrompt(prompt);
}

std::string Console::ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt, Color color, Color highlight)
{
  SetCursorPosition(x, y);
  return ReadLinePrompt(prompt, color, highlight);
}

std::string Console::CenterString(const std::string& str)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  int columns;
  GetConsoleScreenBufferInfo(handle, &screen);
  columns = screen.srWindow.Right - screen.srWindow.Left + 1;
  std::string output;
  size_t padding = (columns / 2) - (str.size() / 2);
  for (int i = 0; i < padding; ++i)
  {
    output += ' ';
  }
  return output + str;
}