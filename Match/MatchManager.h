#pragma once
#include <vector>
#include <map>
#include <filesystem>
#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <dlfcn.h>
#include "Utils.h"
#include "GameManager.h"
#include "AbstractAlgorithm.h"

using std::atomic;
using std::cout;
using std::endl;
using std::map;
using std::mutex;
using std::pair;
using std::queue;
using std::string;
using std::thread;
using std::vector;
using std::filesystem::path;

typedef std::function<std::unique_ptr<AbstractAlgorithm>()> Factory;

// Info required to run a single game of algorithm+maze.
// MatchManager's m_workQueue holds these instances, and each thread
// takes one instance at a time and runs a game on it
struct GameInstance
{
	GameInstance(const string &mazeName, const Maze &maze, const string &algoName, const Factory &factory)
		: mazeName(mazeName), maze(maze), algoName(algoName), algoFactory(factory) {}

	const string &mazeName;
	const Maze &maze;
	const string &algoName;
	const Factory &algoFactory;
};

class MatchManager
{
public:
	MatchManager(const path &mazeDir, const path &algoDir, const path &outDir, int numThreads);
	~MatchManager();

	// Do not allow copy or assignment to avoid releasing resources
	// more than once
	MatchManager(const MatchManager &) = delete;
	MatchManager &operator=(const MatchManager &) = delete;

	void start();

	// Loaded algorithms register their factory here
	static Factory algorithmFactory;

private:
	// Find maze/algorithm files according to these files extensions
	static const string mazeFileExtension;
	static const string algoFileExtension;

	// Pad cells for better readability in summary table
	static const int cellPadding;

	// Each pair is the maze/algorithm name and the maze itself/algorithm factory
	vector<pair<string, Maze>> m_mazes;
	vector<pair<string, Factory>> m_algorithms;
	path m_outDir;
	int m_numThreads;

	queue<GameInstance> m_workQueue;			// threads take GameInstances to run games on from this queue
	mutex m_queueMutex;							// lock queue before taking next GameInstance from it
	mutex m_printMutex;							// lock before printing errors
	map<string, map<string, string>> m_results; // algorithm name -> (maze name -> maze result)
	mutex m_resultsMutex;						// lock m_results before adding current game's result to it
	atomic<bool> m_printResults = false;		// if all games were skipped
												// (because of existing output files) don't print results

	size_t m_algosCellWidth = 1;		   // cell width of algorithms names
	size_t m_mazesCellWidth = 1;		   // cell width of mazes columns
	vector<void *> m_sharedObjectsHandles; // so we can close all shared objects when we're done

	// Add algorithm to m_algorithms
	bool registerAlgorithm(const path &file);

	// Add all runs of algorithm+maze (game instances) to work queue
	void buildWorkQueue();

	// Threads will run this function.
	// Take games from work queue and run them
	void runGames();

	// Returns output file path for maze and algorithm in the format of maze_algorithm.output
	// or empty path if m_outDir is empty
	path getOutputFileName(const string &maze, const string &algorithm) const;

	// If length of candidate greater than current width, update it
	void updateCellWidth(size_t &currentWidth, const string &candidate);
	void printResults();

	// Returns a padded str so it is centered in a cell
	string padString(const string &str, int cellWidth) const;
};