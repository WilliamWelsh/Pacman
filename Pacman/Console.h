// Copyright 2018 Krystian Stasiowski

#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

enum class Color
{
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  AQUA = 3,
  RED = 4,
  PURPLE = 5,
  YELLOW = 6,
  WHITE = 7,
  GRAY = 8,
  LIGHT_BLUE = 9,
  LIGHT_GREEN = 10,
  LIGHT_AQUA = 11,
  LIGHT_RED = 12,
  LIGHT_PURPLE = 13,
  LIGHT_YELLOW = 14,
  BRIGHT_WHITE = 15
};

class Console
{
public:
  Console() = delete;
  Console(const Console&) = delete;
  Console& operator=(const Console&) = delete;
  Console(Console&&) = delete;
  Console& operator=(Console&&) = delete;
  static void Clear();
  static void ClearLine(unsigned index);
  static void ClearAt(unsigned x, unsigned y, unsigned length);
  static void Resize(unsigned x, unsigned y);
  static void SetCursorPosition(unsigned x, unsigned y);
  static COORD GetCursorPosition();
  static COORD GetSize();
  static void ShowConsoleCursor(bool show);
  static void SetTitle(const std::string& title);
  static void SetColor(Color color);
  static void SetColor(Color color, Color highlight);
  static void Write(const std::string& text, Color color, Color highlight);
  static void Write(const std::string& text, Color color);
  static void Write(const std::string& text);
  static void WriteAt(unsigned x, unsigned y, const std::string& text, Color color, Color highlight);
  static void WriteAt(unsigned x, unsigned y, const std::string& text, Color color);
  static void WriteAt(unsigned x, unsigned y, const std::string& text);
  static void WriteCentered(const std::string& text, Color color, Color highlight);
  static void WriteCentered(const std::string& text, Color color);
  static void WriteCentered(const std::string& text);
  static void WriteLine(const std::string& text, Color color, Color highlight);
  static void WriteLine(const std::string& text, Color color);
  static void WriteLine(const std::string& text);
  static void WriteLineAt(unsigned x, unsigned y, const std::string& text, Color color, Color highlight);
  static void WriteLineAt(unsigned x, unsigned y, const std::string& text, Color color);
  static void WriteLineAt(unsigned x, unsigned y, const std::string& text);
  static void WriteLineCentered(const std::string& text, Color color, Color highlight);
  static void WriteLineCentered(const std::string& text, Color color);
  static void WriteLineCentered(const std::string& text);
  static void WriteList(const std::vector<std::string>& text, Color color, Color highlight);
  static void WriteList(const std::vector<std::string>& text, Color color);
  static void WriteList(const std::vector<std::string>& text);
  static void WriteListCentered(const std::vector<std::string>& text, Color color, Color highlight);
  static void WriteListCentered(const std::vector<std::string>& text, Color color);
  static void WriteListCentered(const std::vector<std::string>& text);
  static void ColorLine(Color color, unsigned length);
  static void ColorLineCentered(Color color, unsigned length);
  static std::string ReadLine();
  static std::string ReadLine(Color color);
  static std::string ReadLine(Color color, Color highlight);
  static std::string ReadLinePrompt(const std::string& prompt, Color color);
  static std::string ReadLinePrompt(const std::string& prompt);
  static std::string ReadLinePrompt(const std::string& prompt, Color color, Color highlight);
  static std::string ReadLineAt(unsigned x, unsigned y);
  static std::string ReadLineAt(unsigned x, unsigned y, Color color);
  static std::string ReadLineAt(unsigned x, unsigned y, Color color, Color highlight);
  static std::string ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt, Color color);
  static std::string ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt);
  static std::string ReadLinePromptAt(unsigned x, unsigned y, const std::string& prompt, Color color, Color highlight);
private:
  static std::string CenterString(const std::string& str);
};