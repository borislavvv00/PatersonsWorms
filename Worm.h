#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Path.h"
#include "Dot.h"
#include "Line.h"

// Worm who move from dot to dot.
struct Worm
{
private:
	std::vector<int> nearbyDotsIds;
	std::vector<Path> nextDotPaths;
	float currentDotXPosition, currentDotYPosition;// Used for the beginning of the line.
	float nextDotXPosition, nextDotYPosition;// Used for the end of the line.

	bool IsThereAPath(std::vector<Path> path, int nextDotId);
	void CreateLine(std::vector<Line> &line);
public:
	int currentDotId;
	
	Worm(int dotId, std::vector<Path> path) : currentDotId(dotId)
	{
		for (std::vector<Path>::iterator it = path.begin(); it < path.end(); it++)
		{
			nearbyDotsIds.push_back(it->id);
		}
	}

	// Takes ids of the dots nearby the current one.
	void ResetNearbyDotsIds(std::vector<Path> path);

	// Check on which dot to move.
	void CheckForTheNextDot(std::vector<Dot> &dots, std::vector<Line> &line);
};