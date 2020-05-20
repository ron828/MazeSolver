#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "AbstractAlgorithm.h"
#include "Utils.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
typedef std::pair<int, int> Position;
typedef AbstractAlgorithm::Move Move;

class Maze
{
public:
	Maze(const path &mazefile);
	bool valid() const { return m_valid; }
	int maxSteps() const { return m_maxSteps; }
	int rows() const { return m_rows; }
	int cols() const { return m_cols; }
	Position startPosition() const { return m_startPosition; }
	enum TileType : char
	{
		WALL = '#',
		PASS = ' ',
		PLAYER = '@',
		END = '$'
	};
	TileType tile(const Position &p) const; // get tile type in position p

private:
	int m_maxSteps, m_rows, m_cols;
	vector<vector<TileType>> m_maze;
	Position m_startPosition;
	bool m_valid; // true iff maze is valid (successfully parsed maze file)

	// Maze file header's format is "fieldName = fieldValue"
	// e.g. MaxSteps = 100
	string getFieldName(const string &line) const;
	int getFieldValue(const string &line) const; // returns -1 on error

	// Set maze value from maze header of file,
	// or add error to errors if fail
	void setValue(ifstream &fin, int lineNumber, const string &valueName, int &value, vector<string> &errors);

	bool parseHeader(ifstream &fin, const path &mazefile); // returns false in case of errors
	bool parseMaze(ifstream &fin, const path &mazefile);   // returns false in case of errors
	std::optional<TileType> getTileType(const string &line, int index);

	static const int headerMaxStepsLine;
	static const int headerRowsLine;
	static const int headerColsLine;
	static const string headerMaxStepsStr;
	static const string headerRowsStr;
	static const string headerColsStr;
};