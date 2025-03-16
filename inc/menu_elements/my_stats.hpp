#pragma once

#include "static_window.hpp"

class My_stats: public Static_window{ //class to display stats of a logged player
    private:
        sf::Text id;
        sf::Text username;
        sf::Text bestscore;
        sf::Text type;
        sf::Text password;
    
    public:
    
        My_stats(sf::RenderWindow& my_window, Player* player): Static_window(my_window){
        set_text(title, MyFont, "my stats" ,           20.f,20.f, 70);
        
        set_text(id, MyFont, "id number: " + std::to_string( player->get_id()),                20.f,100.f, 30);
        set_text(username, MyFont, "username: "+ player->getname(),           20.f,140.f, 30);
        set_text(type, MyFont, "player type: " + player->gettype(),           20.f,180.f, 30);
        set_text(password, MyFont, "password: "+ player->get_password(),       20.f,220.f, 30);
        set_text(bestscore, MyFont, "bestscore: "+ std::to_string( player->get_bestscore()),    20.f,260.f, 30);
    }

    ~My_stats(){}

    // displayes the info
    void draw_child_content() override{
        window.draw(id);
        window.draw(username);
        window.draw(bestscore);
        window.draw(type);
        window.draw(password);
    }

};