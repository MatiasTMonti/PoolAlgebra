#include "Game.h"
#include "Ball.h"
#include "Board.h"
#include "GUI.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <string>

const bool Game::running() const
{
	return window->isOpen();
}

void Game::initWindow()
{
	videoMode = sf::VideoMode(1600, 900);
	window = new sf::RenderWindow(videoMode, "TP Pool", sf::Style::Default);
	window->setFramerateLimit(60);
}

void Game::initGame()
{
	restartGame = false;
	gui = new GUI();
	board = new Board();
	black = new Ball(sf::Vector2f(800, 500), ballRadius, sf::Color::Black);
	//Inicializacion de las pelotas blancas
	for (int i = 0; i != 5; i++)
	{
		for (int j = 0; j != i + 1; j++)
		{
			Ball* newBall = new Ball(sf::Vector2f(800 - i * (ballRadius + 2) + (2 * ballRadius + 4) * j, 250 - 2 * ballRadius * i),
				ballRadius, sf::Color::White);
			whiteBalls.push_back(newBall);
		}
	}
	dragged = false;
	move = false;
	goal = false;

	// INIT SOUNDS
	buffer = new sf::SoundBuffer;
	buffer->loadFromFile("./sfx/Circles.wav");
	sound[CIRCLES].setBuffer(*buffer);
	buffer = new sf::SoundBuffer;
	buffer->loadFromFile("./sfx/CircleBoard.wav");
	sound[CIRCLEBOARD].setBuffer(*buffer);
	buffer = new sf::SoundBuffer;
	buffer->loadFromFile("./sfx/Cue.wav");
	sound[CUE].setBuffer(*buffer);
	buffer = new sf::SoundBuffer;
	buffer->loadFromFile("./sfx/CircleHole.wav");
	sound[CIRCLEHOAL].setBuffer(*buffer);
}

Game::Game()
{
	initWindow();
	initGame();
}

Game::~Game()
{
	whiteBalls.clear();
	delete black;
	delete board;
	delete gui;
	delete window;
}

void Game::restart()
{
	gui->restart();
	black->setPosition(800, 500);
	black->setVelocity(sf::Vector2f(0.f, 0.f));
	int count = 0;
	//Setear la posicion de las pelotas y su velocidad.
	for (int i = 0; i != 5; i++)
	{
		for (int j = 0; j != i + 1; j++)
		{
			whiteBalls[count]->setPosition(800 - i * (ballRadius + 2) + (2 * ballRadius + 4) * j, 250 - 2 * ballRadius * i);
			whiteBalls[count]->setVelocity(sf::Vector2f(0.f, 0.f));
			count++;
		}
	}
	dragged = false;
	move = false;
	goal = false;
	restartGame = false;
}

void Game::pollEvents()
{
	while (window->pollEvent(sfmlEvent))
	{
		switch (sfmlEvent.type)
		{
		case sf::Event::Closed: //Cierre del juego.
			window->close();
			break;
		case sf::Event::KeyPressed: //Pausa
			if (sfmlEvent.key.code == sf::Keyboard::Escape)
			{
				gui->gamePaused = 1 - gui->gamePaused;
			}
		case sf::Event::MouseButtonPressed: //Chequeo del mouse hacia una de las pelotas.
			if (sfmlEvent.mouseButton.button == sf::Mouse::Right)
			{
				if (black->checkCollisionPoint(m_mouse))
				{
					draggedBall = black;
					dragged = true;
				}
				for (auto whiteBall : whiteBalls)
				{
					if (whiteBall->checkCollisionPoint(m_mouse))
					{
						draggedBall = whiteBall;
						dragged = true;
					}
				}
			}
			break;
		case sf::Event::MouseButtonReleased: //Chequea cuando el mouse esta seleccionando y arrastrando con una pelota.
			if (sfmlEvent.mouseButton.button == sf::Mouse::Right && dragged)
			{
				draggedBall->setVelocity(sf::Vector2f((draggedBall->getPosition().x - m_mouse.x),
					(draggedBall->getPosition().y - m_mouse.y)));
				dragged = false;
				move = true;

				playSFX(CUE, draggedBall);
			}
			break;

		case sf::Event::MouseMoved:
			m_mouse.x = sfmlEvent.mouseMove.x;
			m_mouse.y = sfmlEvent.mouseMove.y;
		}
	}
}

void Game::collisionCircleLine(Ball* circle, Line* line)
{
	sf::Vector2f p = sf::Vector2f(circle->getPosition());	//Centro del circulo
	sf::Vector2f s = line->getPoints()[0].position;			//Punto de inciio de la linea
	sf::Vector2f e = line->getPoints()[1].position;			//Punto final de la linea.
	sf::Vector2f ps = p - s;
	sf::Vector2f se = e - s;
	float lengthLine = (e.x - s.x) * (e.x - s.x) + (e.y - s.y) * (e.y - s.y);
	float t = ((ps.x * se.x) + (ps.y * se.y)) / lengthLine; //Punto normal de la linea.
	sf::Vector2f st;
	st.x = s.x + t * se.x;
	st.y = s.y + t * se.y;

	sf::Vector2f distance = p - st;
	float distanceBetween = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

	sf::Vector2f normal = distance / distanceBetween;
	float dotProductNormal = circle->getVelocity().x * normal.x +
		circle->getVelocity().y * normal.y;

	sf::Vector2f tangential = sf::Vector2f(-normal.y, normal.x);
	float dotProductTangential = circle->getVelocity().x * tangential.x +
		circle->getVelocity().y * tangential.y;

	float overlap = distanceBetween - circle->getRadius();
	if (distanceBetween <= circle->getRadius()) //Si hubo colision, setea la posicion calculando las variables de la pelota y direccion de la misma.
	{
		if (t > -0.f && t < 1.f)
		{
			circle->setPosition(p.x - distance.x * overlap / distanceBetween,
				p.y - distance.y * overlap / distanceBetween);
			circle->setVelocity(sf::Vector2f(-normal.x * dotProductNormal + tangential.x * dotProductTangential,
				-normal.y * dotProductNormal + tangential.y * dotProductTangential));

			playSFX(CIRCLEBOARD, circle);
		}
	}
}

void Game::collisionCircleHole(Ball* circle, Line* hole)
{
	sf::Vector2f pointsHole = hole->getPoints()[0].position;
	sf::Vector2f points2Hole = hole->getPoints()[1].position;
	sf::Vector2f positionCircle = circle->getPosition();

	sf::Vector2f pointCenterHole = points2Hole - pointsHole;
	sf::Vector2f positionDistance = positionCircle - pointsHole;

	float lengthLine = (points2Hole.x - pointsHole.x) * (points2Hole.x - pointsHole.x) + (points2Hole.y - pointsHole.y) * (points2Hole.y - pointsHole.y);
	float circleDistance = ((positionDistance.x * pointCenterHole.x) + (positionDistance.y * pointCenterHole.y)) / lengthLine;

	sf::Vector2f holeCircleDistance;
	holeCircleDistance.x = pointsHole.x + circleDistance * pointCenterHole.x;
	holeCircleDistance.y = pointsHole.y + circleDistance * pointCenterHole.y;

	sf::Vector2f distance = positionCircle - holeCircleDistance;
	float distanceBetween = sqrtf(distance.x * distance.x + distance.y * distance.y);

	if (distanceBetween <= circle->getRadius() && (circleDistance > -0.f && circleDistance < 1.f)) //Si hubo colision, suma un punto al jugador y mueve la pelota a la esquina de la pantalla.
	{

		playSFX(CIRCLEHOAL, circle);

		goal = true;
		static int pos = 0;
		circle->setVelocity(sf::Vector2f(0.f, 0.f));
		circle->setPosition(ballRadius + pos, ballRadius);
		pos += 2 * ballRadius;
	}
}

void Game::collisionCircles(Ball* ball1, Ball* ball2)
{
	sf::Vector2f distance = ball1->getPosition() - ball2->getPosition();
	float distanceBetween = (sqrtf((distance.x * distance.x) + (distance.y * distance.y)));
	if (ball1 != ball2)
	{
		if (distanceBetween < (ball1->getRadius() + ball2->getRadius())) //Calculos para la colision entre las pelotas.
		{
			float overlap = (distanceBetween - ball1->getRadius() - ball2->getRadius()) / 2.f;
			float moveX = (overlap * (ball1->getPosition().x - ball2->getPosition().x) / distanceBetween);
			float moveY = (overlap * (ball1->getPosition().y - ball2->getPosition().y) / distanceBetween);
			ball1->setPosition(ball1->getPosition().x - moveX, ball1->getPosition().y - moveY);
			ball2->setPosition(ball2->getPosition().x + moveX, ball2->getPosition().y + moveY);

			sf::Vector2f normal;
			normal.x = distance.x / distanceBetween;
			normal.y = distance.y / distanceBetween;

			sf::Vector2f tangential;
			tangential.x = -normal.y;
			tangential.y = normal.x;

			float dotProductTangential1 = ball1->getVelocity().x * tangential.x +
				ball1->getVelocity().y * tangential.y;
			float dotProductTangential2 = ball2->getVelocity().x * tangential.x +
				ball2->getVelocity().y * tangential.y;

			float dotProductNormal1 = ball1->getVelocity().x * normal.x + ball1->getVelocity().y * normal.y;
			float dotProductNormal2 = ball2->getVelocity().x * normal.x + ball2->getVelocity().y * normal.y;

			float m1 = (2.f * ball2->getMass() * dotProductNormal2 +
				dotProductNormal1 * (ball1->getMass() - ball2->getMass())) /
				(ball1->getMass() + ball2->getMass());
			float m2 = (2.f * ball1->getMass() * dotProductNormal1 +
				dotProductNormal2 * (ball2->getMass() - ball1->getMass())) /
				(ball1->getMass() + ball2->getMass());
			ball1->setVelocity(dotProductTangential1 * tangential + m1 * normal);
			ball2->setVelocity(dotProductTangential2 * tangential + m2 * normal);

			playSFX(CIRCLES, ball1);
		}
	}
}

void Game::playSFX(const int sfxType, const Ball* obj)
{
	sf::Vector2f vol = obj->getVelocity();
	float volume = sqrtf(vol.x * vol.x + vol.y * vol.y) / 5.f;
	sound[sfxType].setVolume(volume);
	sound[sfxType].play();
}

void Game::updateGameLogic()
{
	if (goal)
	{
		gui->addPoints(gui->getCurrentPlayer());
		goal = false;
	}
	if (move && draggedBall->getVelocity() == sf::Vector2f(0.f, 0.f))
	{
		move = false;
		gui->setCurrentPlayer(1 - gui->getCurrentPlayer().number);
	}
}

void Game::updateAllCollisions()
{
	for (auto whiteBall : whiteBalls)
	{
		for (Line line : board->getBorderLines())
		{
			collisionCircleLine(whiteBall, &line);
			collisionCircleLine(black, &line);
		}
		for (auto whiteBall2 : whiteBalls)
		{
			collisionCircles(whiteBall, whiteBall2);
		}
		collisionCircles(black, whiteBall);
		for (auto hole : board->getHoles())
		{
			collisionCircleHole(whiteBall, &hole);
		}
	}
}

void Game::updateDirectionLine()
{

	if (dragged)
	{
		sf::Vector2f distance = (m_mouse - draggedBall->getPosition());
		float distanceBetween = sqrt(distance.x * distance.x + distance.y * distance.y);
		sf::Vector2f invert = distance / distanceBetween;
		sf::Color directionColor = sf::Color(255, (255 - ((int)distanceBetween / 2) % 255), 0);
		if (distanceBetween > 510) { directionColor = sf::Color::Red; }
		direction = new Line(draggedBall->getPosition().x, draggedBall->getPosition().y,
			draggedBall->getPosition().x - distanceBetween * invert.x,
			draggedBall->getPosition().y - distanceBetween * invert.y, directionColor);
	}
}

void Game::update()
{
	// FPS
	float deltaTime = 0.f;
	deltaTime = clock.restart().asSeconds();

	pollEvents();

	updateDirectionLine();
	gui->update(window, gui->gamePaused, restartGame, m_mouse, sfmlEvent);
	updateGameLogic();
	if (!gui->gamePaused)
	{
		for (auto whiteBall : whiteBalls)
		{
			whiteBall->update(*window, deltaTime);
		}
		black->update(*window, deltaTime);
		updateAllCollisions();
	}
	if (restartGame)
	{
		restart();
	}
}

void Game::render()
{
	window->clear(sf::Color(0, 100, 0));

	board->render(window);
	for (auto i : whiteBalls)
	{
		window->draw(*i);
	}
	window->draw(*black);
	window->draw(*gui);

	if (dragged)
	{
		window->draw(*direction);
	}

	window->draw(FPS);

	window->display();
}
