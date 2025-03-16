# pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>


//class containing functions used to display text in sfml window
class text_manager{
    private:
        sf::RenderWindow& window;
        sf::Font font;
        sf::Color background;
    public:
        text_manager(sf::RenderWindow& some_window, sf::Color back_color = sf::Color::Black):
        window(some_window), background(back_color){
            if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")){
                std::cout<<"Why you don't have Arial? Are you crazy!?";
            }
        }
        // loc_x, loc_y are window locations
        bool get_text(int loc_x, int loc_y, std::string& communicate, std::string& current_ans,
            sf::Event& event, int limit_x=20, int font_size=20, sf::Color text_color = sf::Color::White){
            sf::Vector2f loc=convert_coords(loc_x, loc_y);
            sf::Text playerText;
            playerText.setCharacterSize(font_size);
            playerText.setString(communicate+current_ans);
            playerText.setFont(font);
            playerText.setFillColor(text_color);
            playerText.setPosition(sf::Vector2f({loc.x, loc.y}));
            if (event.type == sf::Event::TextEntered){
                // equal to esc button
                if(event.text.unicode==27){
                    clear_text(loc.x, loc.y, playerText, font_size);
                    current_ans="###error###";
                    return true;
                }
                // equal to enter button
                if(event.text.unicode==13){
                    clear_text(loc.x, loc.y, playerText, font_size);
                    return true;
                }
                // equal to backspace button
                if(event.text.unicode==8 ){
                    if(current_ans.size()!=0){
                        clear_text(loc.x, loc.y, playerText, font_size);
                        current_ans.erase(current_ans.begin()+current_ans.size()-1);
                    }
                }
                else{
                    current_ans +=event.text.unicode;                           
                }
                if(current_ans.size()>limit_x){
                    clear_text(loc.x, loc.y, playerText, font_size);
                    current_ans.pop_back();
                    return true;
                }
                clear_text(loc.x, loc.y, playerText, font_size);
                playerText.setString(communicate+current_ans);
            }
            window.draw(playerText);
            window.display();
            return false;
        }

        void clear_text(int loc_x, int loc_y, sf::Text text, int font_size=20){
            sf::RectangleShape shape;
            shape.setPosition(sf::Vector2f({loc_x, loc_y-0.2*font_size}));
            sf::Vector2f text_loc{text.getString().getSize()*font_size*0.7, font_size*1.8};

            shape.setSize(text_loc);
            shape.setFillColor(background);
            window.draw(shape);
        }
        // loc_x, loc_y are window locations
        void put_text_to_window(int loc_x, int loc_y, std::string communicate, int font_size = 20, int time=-1,
                sf::Color text_color = sf::Color::White){
            sf::Vector2f loc=convert_coords(loc_x, loc_y);
            sf::Text text_to_put ;
            text_to_put.setFont(font);
            text_to_put.setFillColor(text_color);
            text_to_put.setCharacterSize(font_size);
            text_to_put.setPosition(sf::Vector2f({loc.x, loc.y}));
            text_to_put.setString(communicate);
            clear_text(loc.x, loc.y, text_to_put, font_size);
            window.draw(text_to_put);
            if(time!=-1){
                window.display();
                usleep(1000*time);
                clear_text(loc.x, loc.y, text_to_put, font_size);
            }
        }
        void Set_background_color(sf::Color back_color){ background = back_color;}
        sf::Vector2f convert_coords(int loc_x, int loc_y){
            return window.mapPixelToCoords(sf::Vector2i {loc_x, loc_y});

        }
        void draw_menu_marker(int tile_size){
            sf::Vector2f text_coords;
            sf::RectangleShape menu_marker(sf::Vector2f{window.getSize().x,tile_size});
            text_coords = convert_coords(0,0);
            menu_marker.setPosition(text_coords.x, text_coords.y);
            menu_marker.setFillColor(sf::Color::Black);
            window.draw(menu_marker);
        }
        void put_text_at_center(std::string communicate, int font_size = 20, int time=-1,
                sf::Color text_color = sf::Color::White){
            sf::Vector2u window_size = window.getSize();
            sf::Vector2f text_coords(window_size.x/2.f, window_size.y/2.f);
            sf::Text text_to_put;
            text_to_put.setFont(font);
            text_to_put.setCharacterSize(font_size);
            text_to_put.setString(communicate);
            text_to_put.setPosition(0,0);
            int loc_x = text_coords.x - (text_to_put.getGlobalBounds().width/2);
            int loc_y = text_coords.y - (text_to_put.getGlobalBounds().height/2);
            put_text_to_window(loc_x, loc_y, communicate, font_size, time, text_color);   
        }


};