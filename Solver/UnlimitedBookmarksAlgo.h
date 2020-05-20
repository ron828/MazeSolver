#pragma once
#include <vector>
#include <set>
#include <random>
#include <optional>
#include <algorithm>
#include "AbstractAlgorithm.h"

using std::set;
using std::vector;

typedef AbstractAlgorithm::Move Move;
typedef std::pair<int, int> Coords;

class UnlimitedBookmarksAlgo : public AbstractAlgorithm
{
public:
	UnlimitedBookmarksAlgo();
	Move move() override;
	void hitWall() override;
	void hitBookmark(int seq) override;

protected:
	// Returns the algorithm's decision for the next move.
	// Empty value if no new move found.
	virtual std::optional<Move> getNextMove() = 0;

	// If true, place bookmark in the current turn instead of moving
	virtual bool placeBookmark() = 0;

	vector<Move> m_possibleMoves{RIGHT, LEFT, UP, DOWN};
	bool m_backtracking = false; // so we know not to leave bookmarks while backtracking
	vector<Move> m_moves;		 // keep track of all moves made
	set<Coords> m_visitedCoords; // easily query if visited given coordinates
	Coords m_currentCoords;		 // coordinates of the current location
	vector<Coords> m_bookmarks;  // m_bookmarks[i] gives the position of the ith bookmark
	Coords m_lastBookmark;		 // mark last encountered bookmark before backtracking
	int m_steps = 0;			 // steps since last placed bookmark

	std::random_device m_randomDevice;
	std::mt19937 m_generator;

	bool newMove(Move move);								  // true iff haven't made this move yet from current location
	Coords nextCoords(const Coords &coords, Move move) const; // get coords relative to coords in the direction of move
	bool visitedCoords(const Coords &coords) const;			  // true iff already been to coords
	void cancelLastMove();									  // go back to previous tile
	Move reverseMove(Move m) const;							  // return the opposite direction of move
	bool hasBookmark(const Coords &coords) const;			  // true iff placed a bookmark in coords

	// When updating the coordinates of tileTocheck after a bookmark was hit,
	// also update the coordinates of the tiles around it if needed
	void updateSurroundingTiles(const Coords &tileToCheck, const Coords &oldCoords);
};