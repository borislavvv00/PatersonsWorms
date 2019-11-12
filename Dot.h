#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Path.h"

// Dots in the field.
struct Dot
{
	std::vector<Path> path;
	sf::CircleShape shape;

	int id;
	static int count;// Use to set id for each dot.

	Dot()
	{
		id = ++count;
	}

	void ClearPaths()
	{
		path.clear();
	}

	// Create the possible path between nearby dots.
	void SetPaths(int id_)
	{
		path.push_back(Path(id_, false));
	}

	// Sets that the path between two dots is built.
	void BuildPath(int id_)
	{
		for (std::vector<Path>::iterator it = path.begin(); it < path.end(); it++)
		{
			if (id_ == it->id)
			{
				it->isPathBuild = true;
			}
		}
	}
};