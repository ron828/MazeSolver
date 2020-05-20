#include "Utils.h"

const string Utils::mazeDirFlag{"-maze_path"};
const string Utils::algoDirFlag{"-algorithm_path"};
const string Utils::outDirFlag{"-output"};
const string Utils::threadsFlag{"-num_threads"};

bool Utils::parseArgs(int argc, char **argv, path &mazeDir, path &algoDir, path &outDir, int &numThreads)
{
	for (int i = 1; i < argc; i += 2)
	{
		if (string(argv[i]) == mazeDirFlag)
		{
			if (!setPath(argc, argv, i, mazeDirFlag, mazeDir))
				return false;
		}
		else if (string(argv[i]) == algoDirFlag)
		{
			if (!setPath(argc, argv, i, algoDirFlag, algoDir))
				return false;
		}
		else if (string(argv[i]) == outDirFlag)
		{
			if (!setPath(argc, argv, i, outDirFlag, outDir))
				return false;
		}
		else if (string(argv[i]) == threadsFlag)
		{
			if (!setNumOfThreads(argc, argv, i, threadsFlag, numThreads))
				return false;
		}
		else
		{
			cout << "Unknown argument " << argv[i] << endl;
			cout << "Usage: " << argv[0]
				 << " [-maze_path <path>] [-algorithm_path <algorithm path>] [-output <output path>]"
					" [-num_threads <num>]"
				 << endl;
			return false;
		}
	}
	return true;
}

bool Utils::setNumOfThreads(int argc, char **argv, int index, const string &flag, int &numThreads)
{
	bool success = false;
	if (argc < index + 2)
	{
		cout << "Expected number of threads after " << flag << endl;
	}
	else
	{
		try
		{
			numThreads = std::stoi(argv[index + 1]);
			if (numThreads < 1)
				cout << "Expected number of threads at least 1. got " << numThreads << endl;
			else
			{
				int maxThreads = (int)std::thread::hardware_concurrency();
				if (maxThreads != 0 && numThreads > maxThreads)
				{
					cout << "Machine supports up to " << maxThreads
						 << " threads. Settings number of threads to "
						 << maxThreads << " instead of " << numThreads << endl;
					numThreads = maxThreads;
				}
				success = true;
			}
		}

		catch (...)
		{
			cout << "Expected a number after " << flag << ", got " << argv[index + 1] << endl;
		}
	}
	return success;
}

bool Utils::setPath(int argc, char **argv, int index, const string &flag, path &pathVarToSet)
{
	if (argc < index + 2)
	{
		cout << "Expected path after " << flag << endl;
	}
	else if (checkPath(argv[index + 1]))
	{
		pathVarToSet = argv[index + 1];
		return true;
	}
	return false;
}

bool Utils::checkPath(const path &p)
{
	if (!std::filesystem::exists(p))
	{
		cout << "Path doesn't exist: " << p << endl;
		return false;
	}
	return true;
}

string &Utils::trim(string &str)
{
	string spaces = "\t\n\v\f\r ";
	str.erase(0, str.find_first_not_of(spaces));
	str.erase(str.find_last_not_of(spaces) + 1);
	return str;
}

string Utils::filename(const path &p)
{
	return p.filename().replace_extension("");
}