#include "solver2.h"

REGISTER_ALGORITHM(solver2)

std::optional<Move> solver2::getNextMove()
{
	std::shuffle(m_possibleMoves.begin(), m_possibleMoves.end(), m_generator);
	for (const auto &m : m_possibleMoves)
	{
		if (newMove(m))
			return m;
	}
	return {};
}

bool solver2::placeBookmark()
{
	// Place bookmark on first move
	if (m_bookmarks.empty())
		return true;

	if (!m_backtracking && !hasBookmark(m_currentCoords) && m_steps >= m_stepsThreshold)
	{
		m_steps = 0;
		return true;
	}
	return false;
}