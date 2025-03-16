# pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <map>

#include <iostream>
//set ups map
class Map{
    protected:
        std::vector<std::vector<int> > map_grid;
        int x_size, y_size;
        int square_size;
        sf::RenderWindow& window;
        std::pair<int,int> player_coords;
        std::map<int, std::pair<int, int>> puzzle_info;
        sf::Color get_color(int num){
            if(num==0){ return sf::Color::Magenta;}
            if(num==1){ return sf::Color::Yellow;}
            if(num==2){ return sf::Color::Blue;}
            if(num==3){ return sf::Color::Red;}
            if(num==4){return sf::Color::Green;}
            if(num==5){return sf::Color {0xB9, 0x7A, 0x57};}
            else{ return sf::Color::Black;}
        }
        int get_manhattan_distance(std::pair<int, int> point1, std::pair<int,int> point2){
        return abs(point1.first-point2.first) + abs(point1.second-point2.second);
        }

    public:
        Map(int x, int y, int size, sf::RenderWindow& my_window): x_size(x), y_size(y)
            , square_size(size), window(my_window){
            x_size++;
            player_coords = {-1,-1};
            this->map_grid = std::vector<std::vector<int> >(x_size, std::vector<int> (y_size, 0));
            for(int i = 0; i < x_size; i++){map_grid[i][0] = 10;}
        }
        //retrieve informations from already made map
        Map(std::vector<std::vector<int> > map, int size, std::map<int, std::pair<int, int>> puzzle_info,
        sf::RenderWindow& my_window, std::pair<int, int> player_coords) : 
        square_size(size), window(my_window) {
            this->x_size = map.size();
            this->y_size = map[0].size();
            this->square_size = size;
            this->map_grid = map;
            this->puzzle_info = puzzle_info;
            this->player_coords = player_coords;
        }
        ~Map(){
            window.close();
        }
        

        //fog_range says how much can be seen from player; range should be taken from Player class
        void draw_grid(int fog_range){

            sf::RectangleShape shape(sf::Vector2f {square_size, square_size});
            for(int i=-fog_range; i<fog_range+1; i++){
                for(int j=-fog_range+abs(i); j<fog_range-abs(i)+1; j++){
                    if(!is_within_limits(std::make_pair(player_coords.first+i,player_coords.second+j))){continue;}
                    shape.setFillColor(get_color(map_grid[player_coords.first+i][player_coords.second+j]));
                    shape.setPosition(sf::Vector2f {(player_coords.first+i)*square_size, (player_coords.second+j)*square_size});
                    window.draw(shape);
                }
            }
        }
        // Swap grid element before and after move (assuming player stands on 1 file (tile?))
        void swap_map(std::pair<int, int> coords1, std::pair<int, int> action){
            player_coords = std::make_pair(coords1.first+action.first, coords1.second+action.second);
            map_grid[player_coords.first][player_coords.second] = 3;
            map_grid[coords1.first][coords1.second] = 1;
        }

        bool is_within_limits(std::pair<int,int> coords){
            return coords.first>=0 && coords.first<x_size && coords.second>=0 && coords.second<y_size;
        }
        //Some getters needed by other classes
        std::pair<int, int> get_puzzle_info(int x, int y){return puzzle_info[100*x+y];}
        std::pair<int, int> get_player_coords(){return player_coords;}
        int get_map_tile(std::pair<int, int> coords){
            return map_grid[coords.first][coords.second];
        }
        std::vector<int> get_map_params(){
            return std::vector<int> {x_size, y_size, square_size};
        }

        void set_map_tile(std::pair<int,int> coords, int type){
            map_grid[coords.first][coords.second] = type;
        }

};