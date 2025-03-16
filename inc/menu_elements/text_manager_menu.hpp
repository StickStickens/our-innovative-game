#pragma once

#include "temp_communicate.hpp"



class text_manager_menu{
    private:
        sf::RenderWindow& window;
        sf::Font font;
        sf::RectangleShape shape;
    public:
        Temp_communicate* temp_communicate;
        
        text_manager_menu(sf::RenderWindow& some_window):
        window(some_window){
            if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")){
                std::cout<<"seems you don't have arial font. Install it now";
            }
            temp_communicate = new Temp_communicate(font, "", window, 0.1); 
            
        }

        ~text_manager_menu(){
            delete temp_communicate;
        }
        // loc_x, loc_y are window locations
        std::string get_text(int loc_x, int loc_y, std::string communicate, int limit_x, int font_size = 20) {
            
            std::string ans = "";
            sf::Text playerText;
            playerText.setString(communicate);
            playerText.setFont(font);
            playerText.setPosition(sf::Vector2f({loc_x, loc_y}));
            sf::Event event;
            shape.setFillColor(sf::Color::Black);
            shape.setPosition(sf::Vector2f({loc_x, loc_y}));
            shape.setSize(sf::Vector2f({limit_x - loc_x, loc_y}));
            window.draw(shape);
            window.draw(playerText);
            temp_communicate->draw();
            window.display();

            while (true) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::TextEntered) {
                        // Ignore newline and backspace control characters
                        if (event.text.unicode == '\n' || event.text.unicode == '\r') {
                            continue;
                        }
                        if(ans.size()>=limit_x){return ans;}
                        // Handle backspace to remove the last character
                        if (event.text.unicode == 8 && !ans.empty()) {
                            ans.pop_back();
                        } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                            ans += static_cast<char>(event.text.unicode);
                        }

                        // Update the display
                        window.clear();
                        playerText.setString(communicate + ans);
                        window.draw(shape);
                        window.draw(playerText);
                        temp_communicate->draw();
                        window.display();
                    } else if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Enter) {
                            return ans;
                        }
                        if (event.key.code == sf::Keyboard::Escape) {
                            return ";;;";
                        }
                    }
                }
                
            }
        }
        // loc_x, loc_y are window locations
        void put_text_to_window(int loc_x, int loc_y, std::string communicate, int font_size = 20){
            shape.setFillColor(sf::Color::Black);
            shape.setPosition(sf::Vector2f({loc_x, loc_y}));
            sf::Text text_to_put ;
            text_to_put.setFont(font);
            text_to_put.setCharacterSize(font_size);
            text_to_put.setPosition(sf::Vector2f({loc_x, loc_y}));
            text_to_put.setString(communicate);
            shape.setSize(sf::Vector2f({text_to_put.getGlobalBounds().width+10, font_size+10}));
            window.draw(shape);
            window.draw(text_to_put);
        }


};