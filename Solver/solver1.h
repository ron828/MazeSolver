#pragma once
#include "UnlimitedBookmarksAlgo.h"
#include "AlgorithmRegistration.h"

class solver1 : public UnlimitedBookmarksAlgo
{
public:
	solver1(){};

private:
	std::optional<Move> getNextMove() override;
	bool placeBookmark() override;
};