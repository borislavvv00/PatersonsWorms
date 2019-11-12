#pragma once

// The relation which have the current dot with the other six nearby dots.
struct Path
{
	bool isPathBuild;
	int id;

	Path(int id_, bool isPathBuild_) :id(id_), isPathBuild(isPathBuild_) {}
};