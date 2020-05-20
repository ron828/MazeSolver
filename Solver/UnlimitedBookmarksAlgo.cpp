#include "UnlimitedBookmarksAlgo.h"

UnlimitedBookmarksAlgo::UnlimitedBookmarksAlgo()
{
	m_generator.seed(m_randomDevice());
	m_visitedCoords.insert({0, 0});
	m_currentCoords = {0, 0};
}

UnlimitedBookmarksAlgo::Move UnlimitedBookmarksAlgo::move()
{

	if (placeBookmark())
	{
		m_bookmarks.push_back(m_currentCoords);
		return BOOKMARK;
	}

	std::optional<Move> move = getNextMove();
	if (move.has_value())
	{
		m_steps++;
		m_backtracking = false;
		Coords newCoords = nextCoords(m_currentCoords, move.value());
		m_moves.push_back(move.value());
		m_visitedCoords.insert(newCoords);
		m_currentCoords = newCoords;
	}

	// No new move - backtrack
	else
	{
		if (m_moves.empty())
			return BOOKMARK; // stuck
		move = reverseMove(m_moves.back());
		cancelLastMove();
	}

	return move.value();
}

void UnlimitedBookmarksAlgo::hitWall()
{
	cancelLastMove();
}

void UnlimitedBookmarksAlgo::hitBookmark(int seq)
{
	if (seq >= 1 && seq <= (int)m_bookmarks.size())
	{

		if (m_backtracking && (m_lastBookmark == m_bookmarks[seq - 1]))
		{
			m_currentCoords = m_bookmarks[seq - 1];
		}

		// Update location to where bookmark was set
		else if (!m_backtracking)
		{
			Coords oldCoords = m_currentCoords;
			m_currentCoords = m_bookmarks[seq - 1];
			m_lastBookmark = m_currentCoords;

			// Mark previous coordinates as visited
			Coords prevCoords = m_currentCoords;
			Coords prevOldCoords = oldCoords;
			for (auto ritr = m_moves.rbegin(); ritr != m_moves.rend(); ++ritr)
			{
				Move prevMove = reverseMove(*ritr);
				prevCoords = nextCoords(prevCoords, prevMove);
				prevOldCoords = nextCoords(prevOldCoords, prevMove);
				if (m_currentCoords == prevCoords)
					return;
				m_visitedCoords.insert(prevCoords);
				updateSurroundingTiles(prevCoords, prevOldCoords);
			}
		}
	}
}

bool UnlimitedBookmarksAlgo::newMove(Move move)
{
	Coords coords = nextCoords(m_currentCoords, move);
	return !visitedCoords(coords);
}

Coords UnlimitedBookmarksAlgo::nextCoords(const Coords &coords, Move move) const
{
	Coords nextCoords;
	switch (move)
	{
	case Move::UP:
		nextCoords.first = coords.first - 1;
		nextCoords.second = coords.second;
		break;
	case Move::DOWN:
		nextCoords.first = coords.first + 1;
		nextCoords.second = coords.second;
		break;
	case Move::LEFT:
		nextCoords.first = coords.first;
		nextCoords.second = coords.second - 1;
		break;
	case Move::RIGHT:
		nextCoords.first = coords.first;
		nextCoords.second = coords.second + 1;
		break;
	default:
		break;
	}
	return nextCoords;
}

bool UnlimitedBookmarksAlgo::visitedCoords(const Coords &coords) const
{
	return m_visitedCoords.find(coords) != m_visitedCoords.end();
}

void UnlimitedBookmarksAlgo::cancelLastMove()
{
	Move move = reverseMove(m_moves.back());
	m_currentCoords = nextCoords(m_currentCoords, move);
	m_moves.pop_back();
	m_steps--;
	m_backtracking = true;
}

UnlimitedBookmarksAlgo::Move UnlimitedBookmarksAlgo::reverseMove(Move m) const
{
	switch (m)
	{
	case Move::UP:
		return Move::DOWN;
	case Move::DOWN:
		return Move::UP;
	case Move::LEFT:
		return Move::RIGHT;
	case Move::RIGHT:
		return Move::LEFT;
	default:
		return Move::RIGHT;
	}
}

bool UnlimitedBookmarksAlgo::hasBookmark(const Coords &coords) const
{
	return std::find(m_bookmarks.begin(), m_bookmarks.end(), coords) != m_bookmarks.end();
}

void UnlimitedBookmarksAlgo::updateSurroundingTiles(const Coords &tileToCheck, const Coords &oldCoords)
{
	for (const auto &m : m_possibleMoves)
	{
		Coords coords = nextCoords(oldCoords, m);
		if (visitedCoords(coords))
		{
			Coords newCoords = nextCoords(tileToCheck, m);
			m_visitedCoords.insert(newCoords);
		}
	}
}