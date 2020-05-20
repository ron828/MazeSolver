#pragma once
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include "MazeInfo.h"
#include "AbstractAlgorithm.h"
#include "Utils.h"

typedef std::unique_ptr<AbstractAlgorithm> algoPtr;

class GameManager
{
public:
	GameManager(const Maze &maze, std::function<algoPtr()> algorithmFactory)
		: m_maze(maze),
		  m_algorithm(algorithmFactory()) {}

	int start(const path &outFile); // returns number of steps took to solve maze. -1 if failed

private:
	MazeInfo m_maze;
	algoPtr m_algorithm;

	std::string move2str(AbstractAlgorithm::Move move);
};