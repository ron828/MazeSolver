#include "GameManager.h"

int GameManager::start(const path &outFile)
{
	std::ofstream fout;
	if (!outFile.empty())
	{
		fout.open(outFile);
	}
	int steps = 1;
	int hitBookmark = -1;
	bool reachedEnd = false;
	while (steps <= m_maze.maxSteps())
	{
		AbstractAlgorithm::Move move = m_algorithm->move();
		if (fout.is_open())
			fout << move2str(move) << std::endl;
		Maze::TileType type = m_maze.move(move, hitBookmark, reachedEnd);
		if (type == Maze::WALL)
			m_algorithm->hitWall();
		else if (hitBookmark != -1)
			m_algorithm->hitBookmark(hitBookmark);

		if (reachedEnd)
		{
			if (fout.is_open())
			{
				fout << "!" << std::endl;
				fout.close();
			}
			return steps;
		}
		steps++;
	}

	if (fout.is_open())
	{
		fout << "X" << std::endl;
		fout.close();
	}
	return -1;
}

std::string GameManager::move2str(AbstractAlgorithm::Move move)
{
	switch (move)
	{
	case AbstractAlgorithm::LEFT:
		return "L";
		break;
	case AbstractAlgorithm::RIGHT:
		return "R";
		break;
	case AbstractAlgorithm::UP:
		return "U";
		break;
	case AbstractAlgorithm::DOWN:
		return "D";
		break;
	default:
		return "B";
		break;
	}
	return "";
}