#pragma once
#include "Ball.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Sprite.hpp>

void Ball::initVariables()
{
	Max_Points = 20;
	Friction = 1.f;
	COLOR_SHAPE = sf::Color::Red;

	m_radius = 10.f;
	m_angle = 0.f;
	m_center = sf::Vector2f(100.f, 100.f);
	m_velocity = sf::Vector2f(0.f, 0.f);
	m_acceleration = sf::Vector2f(0.f, 0.f);
	m_vertices.setPrimitiveType(sf::TrianglesFan);
	m_vertices.clear();
	m_vertices.resize(Max_Points);
	m_color = COLOR_SHAPE;
	m_mass = m_radius;
	M_PI = 3.14f;

}

void Ball::initShape()
{
	float x = 0.f;
	float y = 0.f;

	for (unsigned long i = 0; i != Max_Points - 1; i++)
	{
		x = m_center.x + m_radius * cos(((360.f) / (static_cast<float>(Max_Points - 2)) * i + m_angle) * M_PI / 180.f);
		y = m_center.y + m_radius * sin(((360.f) / (static_cast<float>(Max_Points - 2)) * i + m_angle) * M_PI / 180.f);
		m_vertices[i] = sf::Vertex(sf::Vector2f(x, y), m_color);
	}
}

Ball::Ball()
{
	initVariables();
	initShape();
}

Ball::Ball(sf::Vector2f center, float radius, sf::Color color)
{
	initVariables();
	m_center = center;
	m_radius = radius;
	COLOR_SHAPE = color;
	m_color = COLOR_SHAPE;
	m_mass = m_radius;
	initShape();
}

Ball::Ball(const Ball& other)
{
	initVariables();
	m_center = other.m_center;
	m_radius = other.m_radius;
	COLOR_SHAPE = other.m_color;
	m_color = COLOR_SHAPE;
}

Ball& Ball::operator=(const Ball& other)
{
	m_vertices.clear();
	m_vertices = other.m_vertices;
	m_center = other.m_center;
	m_radius = other.m_radius;
	COLOR_SHAPE = other.m_color;
	m_color = COLOR_SHAPE;
	return *this;
}

Ball::~Ball()
{
	m_vertices.clear();
}

void Ball::setPosition(float x, float y)
{
	m_center.x = x;
	m_center.y = y;
	initShape();
}

void Ball::setAcceleration(sf::Vector2f acceleration)
{
	m_acceleration.x = acceleration.x;
	m_acceleration.y = acceleration.y;
}


void Ball::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

const sf::Vector2f Ball::getVelocity() const
{
	return m_velocity;
}

void Ball::updateVelocity(float deltaTime)
{
	m_acceleration = -m_velocity * Friction;
	m_velocity += m_acceleration * deltaTime;
	setPosition(m_center.x + m_velocity.x * deltaTime, m_center.y + m_velocity.y * deltaTime);
	setAngle(atan2f(m_velocity.y, m_velocity.x) * 180.f / M_PI);
	if (std::abs(m_velocity.x) < 0.1f || std::abs(m_velocity.y) < 0.1f)
	{
		m_velocity = sf::Vector2f(0.f, 0.f);
	}
}

sf::Vector2f Ball::getPosition()
{
	return m_center;
}

float Ball::getRadius()
{
	return m_radius;
}

float Ball::getMass()
{
	return m_mass;
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices);
}

void Ball::move(sf::Vector2f velocity)
{
	setPosition(m_center.x + velocity.x, m_center.y + velocity.y);
}

void Ball::setAngle(float angle)
{
	m_angle = angle;
}

void Ball::rotate(float angle)
{
	setAngle(m_angle + angle);
}

void Ball::updateCollisionBorder(const sf::RenderWindow& window)
{
	if ((m_center.x + m_radius) > window.getSize().x)
	{
		setPosition(window.getSize().x - m_radius, m_center.y);
		m_velocity.x = -m_velocity.x;
	}
	if ((m_center.x - m_radius) < 0.f)
	{
		setPosition(m_radius, m_center.y);
		m_velocity.x = -m_velocity.x;
	}
	if ((m_center.y + m_radius) > window.getSize().y)
	{
		setPosition(m_center.x, window.getSize().y - m_radius);
		m_velocity.y = -m_velocity.y;
	}
	if ((m_center.y - m_radius) < 0.f)
	{
		setPosition(m_center.x, m_radius);
		m_velocity.y = -m_velocity.y;
	}
}

bool Ball::checkCollisionPoint(const sf::Vector2f& mouse)
{
	float x_mouse = mouse.x;
	float y_mouse = mouse.y;
	if (((x_mouse - m_center.x) * (x_mouse - m_center.x) +
		(y_mouse - m_center.y) * (y_mouse - m_center.y)) <= m_radius * m_radius)
	{
		return true;
	}
	m_color = COLOR_SHAPE;
	return false;
}

void Ball::update(const sf::RenderWindow& window, float deltaTime)
{
	updateCollisionBorder(window);
	updateVelocity(deltaTime);
}

