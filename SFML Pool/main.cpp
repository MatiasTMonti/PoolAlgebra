#include <SFML/Graphics.hpp>

int main()
{
    // INICIO DE PROYECTO, ESTO SOLO ES PARA VERIFICAR QUE TODO FUNCIONE CORRECTAMENTE.
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Pool");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}