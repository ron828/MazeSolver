#include "Utils.h"
#include "MatchManager.h"

int main(int argc, char **argv)
{
    path mazeDir = std::filesystem::current_path();
    path algoDir = std::filesystem::current_path();
    path outDir;
    int numThreads = 1;
    bool success = Utils::parseArgs(argc, argv, mazeDir, algoDir, outDir, numThreads);
    if (!success)
        return EXIT_FAILURE;

    MatchManager match(mazeDir, algoDir, outDir, numThreads);
    match.start();
    return EXIT_SUCCESS;
}