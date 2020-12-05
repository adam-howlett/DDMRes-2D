#include <iostream>
#include <SFML/Graphics.hpp>

bool isMouseOver_Rectangle(sf::ConvexShape *query, sf::RenderWindow *window){
    sf::Vector2i localPosition = sf::Mouse::getPosition(*window);

    if(localPosition.x > query->getPoint(0).x && localPosition.x < query->getPoint(2).x
            && localPosition.y > query->getPoint(0).y && localPosition.y < query->getPoint(2).y){
        return true;
    }
    return false;
}

bool isMouseOver_Circle(sf::CircleShape *query, sf::RenderWindow *window){
    sf::Vector2i localPosition = sf::Mouse::getPosition(*window);

    if(localPosition.x > query->getPosition().x && localPosition.x < query->getPosition().x + 2*query->getRadius()
            && localPosition.y > query->getPosition().y && localPosition.y < query->getPosition().y + 2*query->getRadius()){
        return true;
    }
    return false;
}
