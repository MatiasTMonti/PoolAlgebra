#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <cmath>

class Ball : public sf::Drawable
{
private:
	unsigned int Max_Points;
	float Friction;
	sf::VertexArray m_vertices;
	sf::Color COLOR_SHAPE;
	sf::Color m_color;
	sf::Vector2f viscosity;
	sf::Vector2f m_center;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float m_radius;
	float m_angle;
	float m_mass;
	float M_PI;
	//Inciar las variables principales de las pelotas.
	void initVariables();
	//Iniciar la forma de las pelotas.
	void initShape();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:

	Ball();
	Ball(sf::Vector2f center, float radius, sf::Color color);
	~Ball();
	
	Ball(const Ball&);
	//Sirve para la creacion de las pelotas por igual usando el operador.
	Ball& operator=(const Ball&); 
	//Posicionar la pelota
	void setPosition(float x, float y);
	//Posicionar un angulo.
	void setAngle(float angle);
	//Colocarle acceleracion.
	void setAcceleration(sf::Vector2f acceleration);
	//Colocarle velocidad.
	void setVelocity(sf::Vector2f velocity);

	sf::Vector2f getPosition();
	const sf::Vector2f getVelocity() const;
	float getRadius();
	float getMass();

	//Posicionar la pelota utilizando la velocidad para el movimiento.
	void move(sf::Vector2f velocity);
	//Rotar la pelota utilizando el angulo.
	void rotate(float angle);

	//Updatea en tiempo real la velocidad
	void updateVelocity(float deltaTime);
	//Chequea cuando el mouse esta por encima de una pelota.
	bool checkCollisionPoint(const sf::Vector2f& mouse);
	//Chequea las colisiones con los bordes en tiempo real.
	void updateCollisionBorder(const sf::RenderWindow& window);

	//Updatea la velocidad y las colisiones de las pelotas.
	void update(const sf::RenderWindow& window, float deltaTime);
};
