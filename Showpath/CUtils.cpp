#include "CUtils.h"

void CUtils::initConsole()
{
    initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void CUtils::restoreConsole()
{
    endwin();
}

void CUtils::gotoxy(int x, int y)
{
    move(y,x);
    refresh();
}

std::pair<int, int> CUtils::getxy()
{
    int x = 0;
    int y = 0;
	getyx(curscr, y, x);
    return std::make_pair(x,y);
}

void CUtils::printRed(std::string text)
{
	attron(COLOR_PAIR(1));
    addstr(text.c_str());
	attroff(COLOR_PAIR(1));
    refresh();
}

void CUtils::printCyan(std::string text)
{
	attron(COLOR_PAIR(2));
	addstr(text.c_str());
	attroff(COLOR_PAIR(2));
	refresh();
}

int CUtils::getFieldValue(const std::string & line)
{
	size_t delim = line.find_first_of('=');
	std::string strValue = line.substr(delim + 1, line.length() - delim - 1);
	return std::stoi(strValue);
}

std::set<std::pair<int, int>> CUtils::printMaze(std::ifstream & fin, int rows, int cols)
{
	std::string line;
	int Y = 0;
	std::set<std::pair<int, int>> walls;
	std::pair<int, int> startingPos;
	while (std::getline(fin, line) && Y < rows) {
		CUtils::printRed(line + "\n");
		size_t _cols = std::min((int)line.size(), cols);
		for (size_t X = 0; X < _cols; X++) {
			if (line.at(X) == '@') {
				startingPos.first = (int)X;
				startingPos.second = Y;
			}
			else if (line.at(X) == '#') {
				walls.insert({ std::make_pair((int)X, Y) });
			}
			else if (line.at(X) == '$') {
				auto tmp = CUtils::getxy();
				attron(COLOR_PAIR(2));
				mvaddch(Y, X, '$');
				attroff(COLOR_PAIR(2));
				CUtils::gotoxy(tmp.first, tmp.second);
			}
		}
		Y++;
	}
	CUtils::gotoxy(startingPos.first, startingPos.second);
	fin.close();
	return walls;
}

void CUtils::showPath(std::ifstream & fin, std::set<std::pair<int, int>>& walls, int rows, int cols)
{
	std::string line;
	std::pair<int, int> location = CUtils::getxy();
	std::pair<int, int> nextLocation;
	std::set<std::pair<int,int>> bookmarks;

	while (std::getline(fin, line)) {
		if (line == "!" || line == "X") return;
		else if (line == "R") {
			if (location.first >= cols - 1) nextLocation.first = 0;
			else nextLocation.first = location.first + 1;
			nextLocation.second = location.second;
		}
		else if (line == "L") {
			if (location.first <= 0) nextLocation.first = cols - 1;
			else nextLocation.first = location.first - 1;
			nextLocation.second = location.second;
		}
		else if (line == "D") {
			if (location.second >= rows - 1) nextLocation.second = 0;
			else nextLocation.second = location.second + 1;
			nextLocation.first = location.first;
		}
		else if (line == "U") {
			if (location.second <= 0) nextLocation.second = rows - 1;
			else nextLocation.second = location.second - 1;
			nextLocation.first = location.first;
		}
		else if (line == "B") {
			nextLocation = location;
			bookmarks.insert(location);
		}

		if (line != "B" && walls.find(nextLocation) == walls.end()) {
			CUtils::gotoxy(nextLocation.first, nextLocation.second);
			if (bookmarks.find(nextLocation) == bookmarks.end()) {
				CUtils::printCyan("*");
				CUtils::gotoxy(nextLocation.first, nextLocation.second);
			}
			location = nextLocation;
		}
		else {
			if (line == "B") CUtils::printCyan(line);
			else {
				CUtils::gotoxy(nextLocation.first, nextLocation.second);
				CUtils::printCyan(" ");
				CUtils::gotoxy(nextLocation.first, nextLocation.second);
				CUtils::printRed("#");
			}
			CUtils::gotoxy(location.first, location.second);
		}
		
		std::cin.get();
		CUtils::gotoxy(location.first, location.second);
	}

	fin.close();
}
