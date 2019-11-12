#include <SFML/Graphics.hpp>
#include <vector>
#include "Worm.h"
#define WINDOW_SIZE 630
#define DOTS_RADIUS 3
#define DOTS_DISTANCE 7
#define DOTS_PER_ROW 15
#define TIME_FOR_SLEEP_MILLISECONDS 600
int Dot::count = -1;

/* The scheme shows how dots are connected to their nearby dots.
 *
 *		#--#--#--#--#
 *		\ /\ /\ /\ /\
 *		 #--#--#--#--#
 *		/ \/ \/ \/ \/
 *		#--#--#--#--# 
 *
 */

// Checks if the searched nearby dot exists.
void CheckNearbyDot(std::vector<Dot> &dots, std::vector<Dot>::iterator &it, int index)
{
	if (index < 0)
	{
		// Checks for dots with lower index then the current one.
		if (it >= dots.begin() - index)
		{
			it->SetPaths((it + index)->id);
		}
	}
	else
	{
		// Checks for dots with higher index then the current one.
		if (it < dots.end() - index)
		{
			it->SetPaths((it + index)->id);
		}
	}
}

void SetDotsAtTheBeginningOfTheRowPaths(std::vector<Dot> &dots, std::vector<Dot>::iterator it, int &rowIndex)
{
	if (it->id % 15 == 0)
	{
		rowIndex++;

		// Dot of an odd row.
		if (it->id % 2 != 0)
		{
			// Down right nearby dot.
			CheckNearbyDot(dots, it, 16);

			// Up right nearby dot.
			CheckNearbyDot(dots, it, -14);
		}
	}
}

void SetDotsAtTheEndOfTheRowPaths(std::vector<Dot> &dots, std::vector<Dot>::iterator it)
{
	if (it->id % 15 == 14)
	{
		if (it->id % 2 == 0)
		{
			// Up left nearby dot.
			CheckNearbyDot(dots, it, -16);

			// Down left nearby dot.
			CheckNearbyDot(dots, it, 14);
		}
	}
}

// Dots that aren't at the beginning nor at the end of the row.
void SetDotsAtMiddleOfTheRowPaths(std::vector<Dot> &dots, std::vector<Dot>::iterator it, int rowIndex)
{
	// Dot that isn't at the begging of the row.
	if (it->id % 15 != 0)
	{
		// Dot at the beginning of the row isn't nearby the dot at the end of the previous row. 
		CheckNearbyDot(dots, it, -1);

		// Dot that isn't at end of the row.
		if (it->id % 15 != 14)
		{
			if (rowIndex % 2 == 0)// Even rows.
			{
				// Down right nearby dot.
				CheckNearbyDot(dots, it, 14);

				// Up left nearby dot.
				CheckNearbyDot(dots, it, -16);
			}
			else// Odd row.
			{
				// Down right nearby dot.
				CheckNearbyDot(dots, it, 16);

				// Up right nearby dot.
				CheckNearbyDot(dots, it, -14);
			}
		}
	}
}

void SetDotsPathsAtTheBeginning(std::vector<Dot> &dots)
{
	int rowIndex = -1; // Used to check if the row is odd or even.

	// Checks for the six nearby dots.
	for (std::vector<Dot>::iterator it = dots.begin(); it < dots.end(); it++)
	{
		CheckNearbyDot(dots, it, -15);
		CheckNearbyDot(dots, it, 15);

		SetDotsAtTheBeginningOfTheRowPaths(dots, it, rowIndex);
		SetDotsAtTheEndOfTheRowPaths(dots, it);
		SetDotsAtMiddleOfTheRowPaths(dots, it, rowIndex);

		// Dot at the end of the row isn't nearby the dot at the beginning of the next row.
		if (it->id % 15 != 14)
		{
			CheckNearbyDot(dots, it, 1);
		}
	}
}

// Set dots position, color and radius at the beginning of the simulation.
void SetDots(std::vector<Dot> &dots)
{
	int row = 0;
	int column = 0;

	for (std::vector<Dot>::iterator it = dots.begin(); it < dots.end(); it++)
	{
		it->shape.setPosition(sf::Vector2f(5 + column * DOTS_RADIUS * DOTS_DISTANCE, 5 + row * DOTS_RADIUS * DOTS_DISTANCE * 2));
		it->shape.setRadius(DOTS_RADIUS);
		it->shape.setFillColor(sf::Color::White);

		column += 2;

		// column increase by 2 so we need to divide it by 2 to make the comparetoin.
		if (column / 2 == DOTS_PER_ROW)
		{
			row++;

			if (row % 2 == 0)// If row is even.
			{
				column = 0;
			}
			else // If row is odd.
			{
				column = 1;
			}
		}
	}
}

// Set worms start positions at the beginning of the simulation.
void PlaceWorms(sf::Window &window, sf::Vector2i mousePosition, std::vector<Dot>::iterator &it, std::vector<Worm> &worm, bool &isMouseLeftButtonPressed, bool &isMouseRigthButtonPressed)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isMouseLeftButtonPressed = true;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		isMouseRigthButtonPressed = true;
	}

	mousePosition = sf::Mouse::getPosition(window);

	//if mouse is over a dot.
	if ((mousePosition.x >= it->shape.getPosition().x && mousePosition.x <= it->shape.getPosition().x + it->shape.getRadius()))
	{
		if ((mousePosition.y >= it->shape.getPosition().y && mousePosition.y <= it->shape.getPosition().y + it->shape.getRadius()))
		{
			// Left button pressed placed the worm.
			if (isMouseLeftButtonPressed == true)
			{
				isMouseLeftButtonPressed = false;

				// Indicates where the worm is by changing the dot's color.
				if (it->shape.getFillColor() == sf::Color::White)
				{
					it->shape.setFillColor(sf::Color::Red);

					// Add worm.
					worm.push_back(Worm(it->id, it->path));
				}
			}
			
			// Rigth button pressed removes the worm.
			if (isMouseRigthButtonPressed == true)
			{
				isMouseRigthButtonPressed = false;

				// Changes back dot's color to normal.
				if (it->shape.getFillColor() == sf::Color::Red)
				{
					it->shape.setFillColor(sf::Color::White);

					// Delete worm.
					for (std::vector<Worm>::iterator it1 = worm.begin(); it1 < worm.end(); it1++)
					{
						if (it1->currentDotId == it->id)
						{
							if (it1 != worm.end() - 1)
							{
								it1 = worm.erase(it1);
							}
						}
					}
				}
			}
		}
	}
}

// Displays the lines.
void DrawLines(sf::RenderWindow &window, std::vector<Line> &line)
{
	for (std::vector<Line>::iterator it = line.begin(); it < line.end(); it++)
	{
		window.draw(it->line, 2,sf::Lines);
	}
}

// Displays the dots.
void DrawDots(sf::RenderWindow &window, std::vector<Dot> &dots)
{
	for (std::vector<Dot>::iterator it = dots.begin(); it < dots.end(); it++)
	{
		window.draw(it->shape);
	}
}

// Change dots colors and creats paths when worms moves.
void UpdateDots(sf::RenderWindow &window, sf::Vector2i mousePosition, std::vector<Dot> &dots, std::vector<Line> &line, std::vector<Worm> &worm, bool isSimulationReady, bool &isMouseLeftButtonPressed, bool &isMouseRigthButtonPressed)
{
	for (std::vector<Dot>::iterator it = dots.begin(); it < dots.end(); it++)
	{
		if (isSimulationReady == true)
		{
			for (std::vector<Worm>::iterator it1 = worm.begin(); it1 < worm.end(); it1++)
			{
				it1->CheckForTheNextDot(dots, line);
			}

			break;
		}
		else
		{
			PlaceWorms(window, mousePosition, it, worm, isMouseLeftButtonPressed, isMouseRigthButtonPressed);
		}
	}
}

void ClearDotsPaths(std::vector<Dot> &dots)
{
	for (std::vector<Dot>::iterator it = dots.begin(); it < dots.end(); it++)
	{
		it->ClearPaths();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Paterson's worms");

	std::vector<Dot> dots(225);
	sf::Vector2i mousePosition;
	bool isSimulationReady = false;
	bool isMouseLeftButtonPressed = false;
	bool isMouseRigthButtonPressed = false;

	std::vector<Worm> worm;
	std::vector<Line> line;

	SetDots(dots);
	SetDotsPathsAtTheBeginning(dots);

	while (window.isOpen())
	{
		while (true)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::S)// Start the simulatoin.
					{
						isSimulationReady = true;
					}
					else if (event.key.code == sf::Keyboard::A)// Stop and make new simulation.
					{
						line.clear();
						ClearDotsPaths(dots);
						SetDotsPathsAtTheBeginning(dots);
						isSimulationReady = false;
					}
				}
			}

			UpdateDots(window, mousePosition, dots, line, worm, isSimulationReady, isMouseLeftButtonPressed, isMouseRigthButtonPressed);

			window.clear();
			DrawDots(window, dots);
			DrawLines(window, line);
			window.display();

			// Slows the simulation.
			if (isSimulationReady == true)
			{
				sf::Time time = sf::milliseconds(TIME_FOR_SLEEP_MILLISECONDS);
				sf::sleep(time);
			}
		}
	}

	return 0;
}