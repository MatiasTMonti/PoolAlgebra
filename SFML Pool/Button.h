#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <string>

class Button : public sf::Drawable
{
public:
	Button(std::string, sf::Vector2f pos);
	~Button();
	//Colocar la posicion del boton.
	void setPosition(float x, float y);
	//Chequea si el mouse esta en el boton.
	bool mouseCheck(sf::Vector2f mouse);
	bool pressed;
	//Verifica si el mouse interacciono o no con el boton.
	void updateEvent(sf::Vector2f mouse, sf::Event event);
	//Updatea el chequeo y verificacion entre el mouse y el boton a tiempo real.
	void update(sf::Vector2f mouse, sf::Event event);

	enum btnType
	{
		CONTINUE,
		NEW_GAME,
		EXIT
	};

private:
	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
	//Dibuja el boton.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

