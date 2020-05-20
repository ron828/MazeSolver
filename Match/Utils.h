#pragma once
#include <filesystem>
#include <string>
#include <thread>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::filesystem::path;

class Utils
{
public:
	// Set match directories according to given arguments.
	// Returns false if given a bad argument
	static bool parseArgs(int argc, char **argv, path &mazeDir, path &algoDir, path &outDir, int &numThreads);

	// Remove spaces before and after str
	static string &trim(string &str);

	// Returns filename (without extension)
	static string filename(const path &p);

private:
	// Flags given as program arguments
	static const string mazeDirFlag;
	static const string algoDirFlag;
	static const string outDirFlag;
	static const string threadsFlag;

	// Sets numThreads to the number given as argument in argv[index + 1]
	// Returns false on failure
	static bool setNumOfThreads(int argc, char **argv, int index, const string &flag, int &numThreads);

	// Sets pathVarToSet to the path given as argument in argv[index + 1].
	// Returns false on failure
	static bool setPath(int argc, char **argv, int index, const string &flag, path &pathVarToSet);

	// True iff path p exists
	static bool checkPath(const path &p);
};