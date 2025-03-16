#pragma once

#include <SFML/Graphics.hpp>
#include "menu_class.hpp"

class Button {
public:
    sf::RectangleShape shape;
    sf::RectangleShape back_shape;
    sf::Text text;
    std::string hidden_text; //used to store additional information 
    //eg.return correct type of player while creating a new one/ keeps map name to get it when button is pressed. 
    Button(){};

    Button(sf::Font& font, const std::string message, float x, float y, float width, float height, int fontSize, std::string text_to_hide="") {

        hidden_text=text_to_hide;
        //front rectangle
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color::White); 
        shape.setOutlineThickness(2);

        //back rectangle
        back_shape.setPosition(x+10, y+8);
        back_shape.setSize(sf::Vector2f(width, height));
        back_shape.setFillColor(sf::Color(200,200,200));
        back_shape.setOutlineColor(sf::Color::White);
        back_shape.setOutlineThickness(1);

        //text
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);

        // center the text inside the front rectangle
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x + (width - textBounds.width) / 2,
            y + (height - textBounds.height) / 2 - textBounds.top
        );
    }

    ~Button(){}


    // check if clicked inside the button
    bool isHovered(float mouseX, float mouseY) const {
        return shape.getGlobalBounds().contains(mouseX, mouseY);
    }

    // draw the button
    void draw(sf::RenderWindow& window) {
        window.draw(back_shape);
        window.draw(shape);
        window.draw(text);
    }

    // visual effect when button is pressed
    void press_visual(){
        sf::Vector2f size= shape.getSize();
        shape.setSize(sf::Vector2f(size.x+5,size.y+5));
        shape.setFillColor(sf::Color(50,50,50));
    }

    //coming back to original appearance when button is no longer pressed
    void unpress_visual(){
        sf::Vector2f size= shape.getSize();
        shape.setSize(sf::Vector2f(size.x-5,size.y-5));
        shape.setFillColor(sf::Color(0,0,0));
    }

    //changes text inside already existing button(used in my_stats button)
    void set_text(std::string& mytext){
        text.setString(mytext);
    }

    //operator overload to compare 2 buttons
    bool operator==(const Button& other) const {
        return this->text.getString() == other.text.getString();
    }
};
