#include "Worm.h"

bool Worm::IsThereAPath(std::vector<Path> path, int nextDotId)
{
	for (std::vector<Path>::iterator it = path.begin(); it < path.end(); it++)
	{
		if (it->id == nextDotId)
		{
			return it->isPathBuild;
		}
	}
}

void Worm::CreateLine(std::vector<Line> &line)
{
	line.push_back(Line(currentDotXPosition, currentDotYPosition, nextDotXPosition, nextDotYPosition));
}

void Worm::CheckForTheNextDot(std::vector<Dot> &dots, std::vector<Line> &line)
{
	for (std::vector<int>::iterator it = nearbyDotsIds.begin(); it < nearbyDotsIds.end(); it++)
	{
		for (std::vector<Dot>::iterator it1 = dots.begin(); it1 < dots.end(); it1++)
		{
			// Change current dot color to normal.
			if (currentDotId == it1->id)
			{
				it1->shape.setFillColor(sf::Color::White);

				currentDotXPosition = it1->shape.getPosition().x;
				currentDotYPosition = it1->shape.getPosition().y;
				break;
			}
		}

		for (std::vector<Dot>::iterator it1 = dots.begin(); it1 < dots.end(); it1++)
		{
			// Move to the next dot.
			if (it1->id == *it && IsThereAPath(it1->path, currentDotId) == false)
			{
				// Sets that the current dot has a path to next dot.
				for (std::vector<Dot>::iterator it2 = dots.begin(); it2 < dots.end(); it2++)
				{
					if (it2->id == currentDotId)
					{
						it2->BuildPath(it1->id);
						break;
					}
				}

				// Sets that the next dot has a path to the current dot.
				it1->BuildPath(currentDotId);

				// Change the current dot.
				currentDotId = it1->id;
				it1->shape.setFillColor(sf::Color::Red);

				nextDotXPosition = it1->shape.getPosition().x;
				nextDotYPosition = it1->shape.getPosition().y;

				CreateLine(line);

				// Ensures that the loop will end here and prevents of getting second dot id.
				it = nearbyDotsIds.end() - 1;
				break;
			}
		}
	}
	 
	// Set new dot's nearby dots' ids.
	for (std::vector<Dot>::iterator it1 = dots.begin(); it1 < dots.end(); it1++)
	{
		if (currentDotId == it1->id)
		{
			it1->BuildPath(currentDotId);
			ResetNearbyDotsIds(it1->path);
			break;
		}
	}
}

void Worm::ResetNearbyDotsIds(std::vector<Path> path)
{
	nearbyDotsIds.clear();

	for (std::vector<Path>::iterator it = path.begin(); it < path.end(); it++)
	{
		nearbyDotsIds.push_back(it->id);
	}
}