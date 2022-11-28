#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <vector>

#include "Button.h"

class Menu : public sf::Drawable
{
public:
	Menu();
	~Menu();
	//Verifica la posicion del mouse con respecto a los botones en tiempo real.
	void update(sf::RenderWindow* window, bool& isPaused, bool& isRestart, sf::Vector2f mouse, sf::Event event);
	std::map <Button::btnType, Button*> buttons;

private:
	sf::RectangleShape background;
	//Dibuja el tablero y los botones.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
