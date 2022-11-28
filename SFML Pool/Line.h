#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Line : public sf::Drawable, sf::Transformable
{
public:
	Line();
	Line(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);
	~Line();

	sf::VertexArray getPoints();

private:
	sf::VertexArray m_points;
	sf::Color m_color;
	//Muestra la linea que muestra la direccion de donde va la pelota.
	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
