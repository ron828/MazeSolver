#pragma once
#include <utility>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <unistd.h>
#include <curses.h>

class CUtils {
public:
    static void initConsole();
    static void restoreConsole();
    static void gotoxy(int x, int y);
    static std::pair<int, int> getxy();
    static void printRed(std::string text);
    static void printCyan(std::string text);
    static int getFieldValue(const std::string &line);
    static std::set<std::pair<int, int>> printMaze(std::ifstream &fin, int rows, int cols);
    static void showPath(std::ifstream &fin, std::set<std::pair<int, int>> &walls, int rows, int cols);
};