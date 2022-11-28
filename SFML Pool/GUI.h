#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "Menu.h"

class GUI : public sf::Drawable
{
public:
	//Creacion de puntaje para 2 Jugadores
	GUI();
	~GUI();

	struct Player
	{
		sf::Text numberText;
		sf::Text pointsText;
		int points;
		int number;
	};
	//Selecciona el jugador.
	void setCurrentPlayer(Player);
	//Verifica que numero corresponde a cada jugador.
	void setCurrentPlayer(int);
	Player getCurrentPlayer();
	//Agrega puntos a los jugadores.
	void addPoints(Player);
	bool gamePaused;
	//Reinicia los puntajes
	void restart();
	//Actualiza en tiempo real el puntaje de los jugadores
	void update(sf::RenderWindow* window, bool& isPaused, bool& isRestart, sf::Vector2f mouse, sf::Event event);
private:
	sf::Font font;

	Menu menu;
	Player player1;
	Player player2;
	Player currentPlayer;

	//Muestra por pantalla los jugadores y su puntaje.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};
