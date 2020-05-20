#include "CUtils.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cout << "give maze and output files" << std::endl;
		return -1;
	}

	std::ifstream maze(argv[1]);
	std::ifstream path(argv[2]);

	if (maze.is_open()) {
		std::string line;
		std::getline(maze, line); // name
		std::getline(maze, line); // max steps
		std::getline(maze, line); // rows
		int rows = CUtils::getFieldValue(line);
		std::getline(maze, line); // cols
		int cols = CUtils::getFieldValue(line);

		CUtils::initConsole();
		auto walls = CUtils::printMaze(maze, rows, cols);

		if (path.is_open()) {
			CUtils::showPath(path, walls, rows, cols);
		}        
		std::cin.get();
    	CUtils::restoreConsole();
	}	
}