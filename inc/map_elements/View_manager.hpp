#pragma once

#include<SFML/Graphics.hpp>
#include "Map.hpp"
#include <iostream>



// a classs resposible for the correct display of everything in sfml
class View_manager{
    private:
        sf::RenderWindow& window;
        int window_x, window_y, map_x, map_y, view_x, view_y;
        sf::View view;
        int tile_size;
    public:
        View_manager(sf::RenderWindow& my_window, Map* viewed_map):
        window(my_window){
            tile_size = viewed_map->get_map_params()[2];
            window_x = window.getSize().x/tile_size;
            window_y = window.getSize().y/tile_size;
            map_x = viewed_map->get_map_params()[0];
            map_y = viewed_map->get_map_params()[1];
            view = window.getDefaultView();
            view_x = view.getCenter().x/tile_size;
            view_y = view.getCenter().y/tile_size;
        }
        View_manager(sf::RenderWindow& my_window, int x_size, int y_size, int tile_size):
        window(my_window){
            this->tile_size = tile_size;
            window_x = window.getSize().x/tile_size;
            window_y = window.getSize().y/tile_size;
            map_x = x_size;
            map_y = y_size;
            view = window.getDefaultView();
            view_x = view.getCenter().x/tile_size;
            view_y = view.getCenter().y/tile_size;
        }
        void settle_view(std::pair<int, int> coords){
            sf::Vector2f center = {view_x, view_y};
            if(coords.first-view_x <= -window_x/4){center.x=coords.first+window_x/4;}
            if(coords.first-view_x >= window_x/4){center.x=coords.first-window_x/4;}
            if(coords.second-view_y <= -window_x/4){center.y=coords.second+window_y/4;}
            if(coords.second-view_y >= window_x/4){center.y=coords.second-window_y/4;}
            check_correctness(center);
            center.x = center.x*tile_size;
            center.y = center.y*tile_size;
            view.setCenter(center);
            view_x = view.getCenter().x/tile_size;
            view_y = view.getCenter().y/tile_size;
            window.setView(view);
        }
        void move_view(std::string direction){
            sf::Vector2f move ={0.0f, 0.0f};
            if(direction=="up"){move.y=-tile_size;}
            if(direction=="down"){move.y=tile_size;}
            if(direction=="left"){move.x=-tile_size;}
            if(direction=="right"){move.x=tile_size;}
            sf::Vector2f center=view.getCenter();
            center+=move;
            center.x/=tile_size; center.y/=tile_size;
            check_correctness(center);
            center.x*=tile_size; center.y*=tile_size;
            if(center==view.getCenter()+move){
                view.move(move);
                window.setView(view);
            }
        }

        void check_correctness(sf::Vector2f& center){
            center.x = center.x<window_x/2 ? window_x/2:center.x;
            center.y = center.y<window_y/2 ? window_y/2:center.y;
            center.x = center.x>map_x-window_x/2 ? map_x-window_x/2:center.x;
            center.y = center.y>map_y-window_y/2 ? map_y-window_y/2:center.y;
        }
        sf::Vector2f get_mouse_position(){
            return window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
        int get_window_x(){ return window_x;}
        int get_window_y(){ return window_y;}

        bool is_in_view(std::pair<int, int> coords){
            sf::Vector2f center = view.getCenter();
            center.x/=tile_size; center.y/=tile_size;
            return (coords.first>=center.x-window_x/2 && coords.first<=center.x+window_x/2 
                && coords.second>=center.y-window_y/2 && coords.second<=center.y+window_y/2);
        }
        void resize_window(std::pair<int, int> coords){
            window_x = window.getSize().x;
            window_y = window.getSize().y;
            view.setSize(window_x, window_y);
            window_x /= tile_size; window_y /= tile_size;
            settle_view(coords);
        }
};