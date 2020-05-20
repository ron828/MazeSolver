#include "MazeInfo.h"

Maze::TileType MazeInfo::move(Move selectedMove, int &hitBookmark, bool &reachedEnd)
{
	hitBookmark = -1;
	reachedEnd = false;
	Position nextP = getNextPosition(m_currentPosition, selectedMove);
	Maze::TileType type = m_maze.tile(nextP);

	if (selectedMove == Move::BOOKMARK)
	{
		m_bookmarks.insert({m_currentPosition, m_bookmarkSequence});
		m_bookmarkSequence++;
	}
	else
	{
		if (type != Maze::WALL)
		{
			m_currentPosition = nextP;
			auto it = m_bookmarks.find(m_currentPosition);
			if (it != m_bookmarks.end())
				hitBookmark = it->second;
		}
		if (type == Maze::END)
			reachedEnd = true;
	}
	return type;
}

Position MazeInfo::getNextPosition(Position currentP, Move move)
{
	Position nextP;
	int rows = m_maze.rows();
	int cols = m_maze.cols();
	switch (move)
	{
	case Move::UP:
		nextP.second = currentP.second;
		if (currentP.first <= 0)
			nextP.first = rows - 1;
		else
			nextP.first = currentP.first - 1;
		break;
	case Move::DOWN:
		nextP.second = currentP.second;
		if (currentP.first >= rows - 1)
			nextP.first = 0;
		else
			nextP.first = currentP.first + 1;
		break;
	case Move::RIGHT:
		nextP.first = currentP.first;
		if (currentP.second >= cols - 1)
			nextP.second = 0;
		else
			nextP.second = currentP.second + 1;
		break;
	case Move::LEFT:
		nextP.first = currentP.first;
		if (currentP.second <= 0)
			nextP.second = cols - 1;
		else
			nextP.second = currentP.second - 1;
		break;
	case Move::BOOKMARK:
		nextP = currentP;
		break;
	}
	return nextP;
}