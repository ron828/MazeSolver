#pragma once
#include "UnlimitedBookmarksAlgo.h"
#include "AlgorithmRegistration.h"

class solver2 : public UnlimitedBookmarksAlgo
{
public:
	solver2(){};

private:
	const int m_stepsThreshold = 10; // place bookmark after how many new steps

	std::optional<Move> getNextMove() override;
	bool placeBookmark() override;
};