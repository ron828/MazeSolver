#include "Maze.h"

/*
    Maze class holds the maze itself and does not change after its creation
    and therefore reusable by different algorithms, while MazeInfo
    holds information about a specific game such as the current position
    and bookmarks positions and therefore is tied to a single run of 
    an algorithm on a maze.

    This is so we don't need to create the entire maze over and over
    for each algorithm+maze run. Instead we create the more minimal MazeInfo
    which references the appropriate Maze
*/

class MazeInfo
{
public:
    MazeInfo(const Maze &maze) : m_maze(maze), m_currentPosition(m_maze.startPosition()) {}
    int maxSteps() const { return m_maze.maxSteps(); }

    // Update m_currentPosition according to selectedMove and return the
    // TileType of the new position
    Maze::TileType move(Move selectedMove, int &hitBookmark, bool &reachedEnd);

private:
    const Maze &m_maze;
    Position m_currentPosition;
    int m_bookmarkSequence = 1;
    std::map<Position, int> m_bookmarks; // positions of bookmarks and their sequence number

    Position getNextPosition(Position currentP, Move move); // next position according to move
};