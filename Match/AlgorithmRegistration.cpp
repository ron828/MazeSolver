#include "AlgorithmRegistration.h"
#include "MatchManager.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> getAlgorithm)
{
    MatchManager::algorithmFactory = std::move(getAlgorithm);
}