#include "solver1.h"

REGISTER_ALGORITHM(solver1)

std::optional<solver1::Move> solver1::getNextMove()
{
	// Try going in the same direction first
	if (!m_moves.empty())
	{
		if (newMove(m_moves.back()))
			return m_moves.back();
	}

	std::shuffle(m_possibleMoves.begin(), m_possibleMoves.end(), m_generator);
	for (const auto &m : m_possibleMoves)
	{
		if (newMove(m))
			return m;
	}
	return {};
}

bool solver1::placeBookmark()
{
	// Place bookmark on first move
	if (m_bookmarks.empty())
		return true;

	// Place bookmark if changed direction
	// m_moves.back() and m_moves.end()[-2] are the two last moves
	if (!m_backtracking && (m_moves.size() > 1) && (m_moves.back() != m_moves.end()[-2]))
	{
		Move move = reverseMove(m_moves.back());
		Coords prevCoords = nextCoords(m_currentCoords, move);
		if (!hasBookmark(m_currentCoords) && !hasBookmark(prevCoords))
		{
			return true;
		}
	}
	return false;
}