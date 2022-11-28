#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <array>
#include <SFML/Audio.hpp>

#include "GUI.h"
#include "Board.h"
#include "Ball.h"
#include "Line.h"

class Game
{
private:
	enum sfxType
	{
		CIRCLES,
		CIRCLEBOARD,
		CUE,
		CIRCLEHOAL,

		COUNT
	};

	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	sf::Event sfmlEvent;
	sf::Vector2f m_mouse;
	sf::SoundBuffer* buffer;
	std::array<sf::Sound, COUNT> sound;
	const float	ballRadius = 9.f;
	void initWindow();
	void initGame();


	sf::Font font;
	sf::Text FPS;

public:
	Game();
	~Game();
	GUI* gui;
	Board* board;
	Ball* black;
	std::vector <Ball*> whiteBalls;
	Ball* draggedBall;
	Line* direction;
	sf::Clock clock;
	//Inicia el Juego.
	const bool running() const;
	bool restartGame;
	bool dragged;
	bool move;
	bool goal;

	//Verifica las interacciones del teclado y mouse en el juego.
	void pollEvents();
	//Chequeo cuando una pelota choca con un borde de la mesa.
	void collisionCircleLine(Ball* circle, Line* line);
	//Chequeo entre dos pelotas.
	void collisionCircles(Ball* ball1, Ball* ball2);
	//Chequeo cuando una pelota colisiona con un hoyo de la mesa.
	void collisionCircleHole(Ball* circle, Line* hole);
	//Inicia el sonido.
	void playSFX(const int sfxType, const Ball* obj);
	//Muestra la direccion de la linea hacia donde va la pelota con el mouse arrastrado en tiempo real.
	void updateDirectionLine();
	void restart();
	//Sirve para realizar todos los updates en tiempo real.
	void updateAllCollisions();
	//Verifica cuando realizas un punto con el jugador.
	void updateGameLogic();
	//Update del juego.
	void update();
	//Dibuja todo el juego.
	void render();

};

