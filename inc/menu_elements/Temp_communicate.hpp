#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "menu_class.hpp"


class Temp_communicate { //communicate to be displayed in bottom right corner for some time after actions
private:
    sf::Text text;
    sf::Clock timer;
    float displayDuration;  
    sf::RenderWindow& window;
    

public:
    Temp_communicate(sf::Font& font, const std::string& message, sf::RenderWindow& my_window, float time)
        : window(my_window)  {
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Red);  // You can change the color as needed
        displayDuration=time;
    }

    ~Temp_communicate(){
    }

    // activate the message (reset the timer and set the message)
    void activate(const std::string& newMessage) {
        text.setPosition(window.getSize().x-400, window.getSize().y-100);
        text.setString(newMessage);
        timer.restart();  
    }

    // draw the message if it is active
    void draw() {
        if (timer.getElapsedTime().asSeconds() < displayDuration) {
            window.draw(text);
        }
    }
};
