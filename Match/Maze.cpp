#include "Maze.h"

const int Maze::headerMaxStepsLine{2};
const int Maze::headerRowsLine{3};
const int Maze::headerColsLine{4};
const string Maze::headerMaxStepsStr{"MaxSteps"};
const string Maze::headerRowsStr{"Rows"};
const string Maze::headerColsStr{"Cols"};

Maze::Maze(const path &mazefile) : m_maxSteps(0), m_rows(0), m_cols(0), m_valid(false)
{
	ifstream fin(mazefile);
	if (!fin.is_open())
	{
		cout << "Failed to open maze file " << mazefile << endl;
		return;
	}

	m_valid = parseHeader(fin, mazefile) && parseMaze(fin, mazefile);
	fin.close();
}

Maze::TileType Maze::tile(const Position &p) const
{
	return m_maze[p.first][p.second];
}

void Maze::setValue(ifstream &fin, int lineNumber, const string &valueName, int &value, vector<string> &errors)
{
	bool error = false;
	string fieldName, line;
	if (std::getline(fin, line))
	{
		fieldName = getFieldName(line);
		value = getFieldValue(line);
	}
	else
	{
		error = true;
	}

	if (fieldName != valueName || error || value < 1)
	{
		errors.push_back("expected in line " + std::to_string(lineNumber) + " - " + valueName + " = <num>\ngot: " + line);
	}
}

bool Maze::parseHeader(ifstream &fin, const path &mazefile)
{
	// Skip 1st line
	string line;
	std::getline(fin, line);

	vector<string> errors;
	setValue(fin, headerMaxStepsLine, headerMaxStepsStr, m_maxSteps, errors);
	setValue(fin, headerRowsLine, headerRowsStr, m_rows, errors);
	setValue(fin, headerColsLine, headerColsStr, m_cols, errors);

	if (errors.size() > 0)
	{
		cout << "Bad maze file header in " << mazefile << ":" << endl;
		for (const auto &e : errors)
		{
			cout << e << endl;
		}
		return false;
	}
	return true;
}

bool Maze::parseMaze(ifstream &fin, const path &mazefile)
{
	string line;
	vector<string> errors;
	int playerCounter = 0, endCounter = 0;
	for (int row = 0; row < m_rows; row++)
	{
		if (!std::getline(fin, line))
			line = "";
		m_maze.push_back(vector<TileType>{});
		line.erase(line.find_last_not_of("\r") + 1); // ignore '\r'
		for (int col = 0; col < m_cols; col++)
		{
			std::optional<TileType> tile = getTileType(line, col);
			if (tile.has_value())
			{
				if (tile.value() == END)
					endCounter++;
				else if (tile.value() == PLAYER)
				{
					playerCounter++;
					m_startPosition = {row, col};
				}
				m_maze.back().push_back(tile.value());
			}
			else
			{
				string wrongChar = string(1, line.at(col));
				if (wrongChar == "\t")
					wrongChar = "TAB";
				errors.push_back("Wrong character in maze: " + wrongChar +
								 " in row " + std::to_string(row + 1) + ", col " + std::to_string(col + 1));
			}
		}
	}

	bool parseSuccessful = true;
	if (playerCounter != 1 || endCounter != 1 || errors.size() > 0)
	{
		cout << "Bad maze in maze file " << mazefile << ":" << endl;
		parseSuccessful = false;
	}
	if (playerCounter == 0)
		cout << "Missing @ in maze" << endl;
	if (endCounter == 0)
		cout << "Missing $ in maze" << endl;
	if (playerCounter > 1)
		cout << "More than one @ in maze" << endl;
	if (endCounter > 1)
		cout << "More than one $ in maze" << endl;
	if (errors.size() > 0)
	{
		for (const auto &e : errors)
		{
			cout << e << endl;
		}
	}
	return parseSuccessful;
}

std::optional<Maze::TileType> Maze::getTileType(const string &line, int index)
{
	if ((int)line.length() <= index)
		return PASS;
	else if (line.at(index) == PASS)
		return PASS;
	else if (line.at(index) == WALL)
		return WALL;
	else if (line.at(index) == PLAYER)
		return PLAYER;
	else if (line.at(index) == END)
		return END;

	return {};
}

string Maze::getFieldName(const string &line) const
{
	size_t delim = line.find_first_of('=');
	string name = line.substr(0, delim);
	return Utils::trim(name);
}

int Maze::getFieldValue(const string &line) const
{
	size_t delim = line.find_first_of('=');
	string strValue = line.substr(delim + 1, line.length() - delim - 1);
	Utils::trim(strValue);
	int value = -1;
	try
	{
		size_t end;
		value = std::stoi(strValue, &end);

		// Part of the value wasn't digits - unacceptable
		if (strValue[end] != '\0')
		{
			return -1;
		}
	}

	// Failed to convert or out of range
	catch (...)
	{
		return -1;
	}

	return value;
}