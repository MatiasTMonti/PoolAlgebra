#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "Line.h"

class Board
{
public:
	//Crea la mesa y su caja de colision (Hitbox)
	Board();
	~Board();

	std::vector <Line> getBorderLines();
	std::vector <Line> getHoles();

	void render(sf::RenderTarget* target);
private:
	sf::Sprite sprite;
	sf::Texture texture;
	std::vector <Line> lines;
	std::vector <Line> holes;
	//Inicia la las lineas de la mesa.
	void initBorderLines();
	//Inicia la colision de los hoyos de la mesa.
	void initHoles();
};