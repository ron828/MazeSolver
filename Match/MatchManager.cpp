#include "MatchManager.h"

const string MatchManager::mazeFileExtension{".maze"};
const string MatchManager::algoFileExtension{".so"};
const int MatchManager::cellPadding{2};
Factory MatchManager::algorithmFactory{nullptr};

MatchManager::MatchManager(const path &mazeDir, const path &algoDir, const path &outDir, int numThreads)
{
	m_numThreads = numThreads;
	m_outDir = outDir;

	// Find mazes in mazeDir
	for (const auto &p : std::filesystem::directory_iterator(mazeDir))
	{
		if (!p.path().extension().compare(mazeFileExtension))
		{
			Maze maze(p.path());
			if (maze.valid())
			{
				string name = Utils::filename(p.path());
				m_mazes.push_back({name, std::move(maze)});
				updateCellWidth(m_mazesCellWidth, name);
			}
		}
	}

	// Find algorithms in algoDir
	for (const auto &p : std::filesystem::directory_iterator(algoDir))
	{
		if (!p.path().extension().compare(algoFileExtension))
		{
			if (registerAlgorithm(p.path()))
			{
				updateCellWidth(m_algosCellWidth, Utils::filename(p.path()));
			}
		}
	}
	buildWorkQueue();
}

MatchManager::~MatchManager()
{
	// Release all std::function which point to an open shared object
	// so we can safely close it
	algorithmFactory = nullptr;
	for (auto &algorithm : m_algorithms)
	{
		algorithm.second = nullptr;
	}

	// Close open shared objects
	for (auto &handle : m_sharedObjectsHandles)
	{
		dlclose(handle);
	}
}

bool MatchManager::registerAlgorithm(const path &file)
{
	algorithmFactory = nullptr;
	void *handle = dlopen(file.c_str(), RTLD_NOW);
	if (!handle)
	{
		cout << "Failed to load algorithm file " << file << endl;
		return false;
	}

	if (!algorithmFactory)
	{
		cout << "Failed to register algorithm " << file << endl;
		dlclose(handle);
		return false;
	}

	m_sharedObjectsHandles.push_back(handle);
	m_algorithms.push_back({Utils::filename(file), algorithmFactory});
	return true;
}

void MatchManager::buildWorkQueue()
{
	for (const auto &maze : m_mazes)
	{
		for (const auto &algo : m_algorithms)
		{
			m_workQueue.push(GameInstance(maze.first, maze.second, algo.first, algo.second));
		}
	}
}

void MatchManager::start()
{
	vector<thread> threads;

	// main thread also does work, so create only
	// m_numThreads-1 new threads
	int newThreads = m_numThreads - 1;
	for (int i = 0; i < newThreads; i++)
	{
		threads.push_back(thread(&MatchManager::runGames, this));
	}

	runGames();

	for (auto &thread : threads)
	{
		thread.join();
	}

	if (m_printResults)
		printResults();
}

void MatchManager::runGames()
{
	while (true)
	{
		std::unique_ptr<GameInstance> gameInstance;

		// Minimize locking to this scope
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			if (m_workQueue.empty())
				return;
			gameInstance = std::make_unique<GameInstance>(m_workQueue.front());
			m_workQueue.pop();
		}

		path outFile = getOutputFileName(gameInstance->mazeName, gameInstance->algoName);
		string result = " ";
		if (!outFile.empty() && std::filesystem::exists(outFile))
		{
			std::lock_guard<std::mutex> guard(m_printMutex);
			cout << "Output file " << outFile << " already exists" << endl;
		}
		else
		{
			GameManager game(gameInstance->maze, gameInstance->algoFactory);
			result = std::to_string(game.start(outFile));
			m_printResults = true;
		}

		// Minimize locking to this scope
		{
			std::lock_guard<std::mutex> guard(m_resultsMutex);
			m_results[gameInstance->algoName][gameInstance->mazeName] = result;
			updateCellWidth(m_mazesCellWidth, result);
		}
	}
}

path MatchManager::getOutputFileName(const string &maze, const string &algorithm) const
{
	if (m_outDir.empty())
		return path();
	path outFile = m_outDir / maze;
	outFile += algorithm + ".output";
	return outFile;
}

void MatchManager::updateCellWidth(size_t &currentWidth, const string &candidate)
{
	if (candidate.length() > currentWidth)
	{
		currentWidth = candidate.length();
	}
}

void MatchManager::printResults()
{
	m_algosCellWidth += cellPadding;
	m_mazesCellWidth += cellPadding;
	string lineSeparator((m_algosCellWidth + 1) + ((m_mazesCellWidth + 1) * m_mazes.size()) + 1, '-');
	cout << lineSeparator << endl;

	// Print mazes row
	cout << "|" << string(m_algosCellWidth, ' ');
	const auto &mazesMap = m_results.at(m_algorithms[0].first);
	for (const auto &m : mazesMap)
	{
		cout << "|" << padString(m.first, m_mazesCellWidth);
	}
	cout << "|" << endl;

	// Print algorithms results
	for (const auto &[algoName, resultsMap] : m_results)
	{
		cout << lineSeparator << endl;
		cout << "|" << padString(algoName, m_algosCellWidth);

		for (const auto &r : resultsMap)
		{
			cout << "|" << padString(r.second, m_mazesCellWidth);
		}
		cout << "|" << endl;
	}
	cout << lineSeparator << endl;
}

string MatchManager::padString(const string &str, int cellWidth) const
{
	int paddingBefore = (cellWidth - str.length()) / 2;
	int paddingAfter = cellWidth - (paddingBefore + str.length());
	return string(paddingBefore, ' ') + str + string(paddingAfter, ' ');
}