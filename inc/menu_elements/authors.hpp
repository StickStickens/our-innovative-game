#pragma once

#include "static_window.hpp"


class Authors: public Static_window{ //class to display authors of the game
    private:
        sf::Sprite sprite; //for image handling
        sf::Texture texture;
        std::vector<sf::Text> authors;
    public:
    
        Authors(sf::RenderWindow& my_window): Static_window(my_window){       
            if (!texture.loadFromFile("inc/menu_elements/authors_image/creators.jpg")){ //loads image from file
                std::cerr<<"error loading file"<<std::endl;
            }

            //sets image poarameters
            sprite.setTexture(texture); 
            sprite.setPosition(100.f, 100.f);
            sprite.setScale(0.75, 0.75);
            authors = std::vector<sf::Text> (3, sf::Text());
            set_text(title, MyFont, "authors",           20.f,20.f, 70);
            set_text(authors[0], MyFont, "Igor Szymczak",           750.f,120.f, 45);
            set_text(authors[1], MyFont, "Oliwier Necelman",           750.f,205.f, 45);
            set_text(authors[2], MyFont, "Julia Mironska",           750.f,290.f, 45);
        }

        ~Authors(){
        }

        //draws image
        void draw_child_content() override{
            window.draw(sprite);
            for(auto i : authors){
                window.draw(i);
            }
        }

};