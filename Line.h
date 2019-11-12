#pragma once
#include <SFML/Graphics.hpp>

struct Line
{
	sf::Vertex line[2];

	Line(float startX, float startY, float endX, float endY)
	{
		line[0].position = sf::Vector2f(startX, startY);
		line[0].color = sf::Color::Blue;
		line[1].position = sf::Vector2f(endX, endY);
		line[1].color = sf::Color::Blue;
	}
};