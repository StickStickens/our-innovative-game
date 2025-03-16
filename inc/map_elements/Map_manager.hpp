#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include "Map_editor.hpp"


class Map_manager{
    public:
        std::vector<std::vector<int> > map;
        std::map<int, std::pair<int, int>> puzzle_info;
        std::pair<int, int> player_coords;
        sf::RenderWindow& my_window;
        int size_info[3];
        Map_manager(sf::RenderWindow& window): my_window(window) {};

        //looks hard, but actually it just scans the lines, divide it by ' ' and then
        // puts into good location of the map / puzzle info
        // Template gives option to create map/map_editor at once
        template<class T>
        // the map info are saved in a file and they need to be recovered from it to start the game
        void read_map_from_file(std::string file_name, T*& return_value){
            puzzle_info.clear();
            std::ifstream f_in(file_name);
            for(int i=0;i<3;i++){f_in>>size_info[i];}
            // size_info[1]++;
            map = std::vector<std::vector<int> > (size_info[0], std::vector<int>(size_info[1]));
            for(int i=0;i<size_info[0];i++){
                for(int j=0;j<size_info[1];j++){
                    f_in>>map[i][j];
                    if(map[i][j]==3){ player_coords = std::make_pair(i,j);}
                }
            }
            int puzzle_size=0; f_in>>puzzle_size;
            int puzzle_id, puzzle_type, puzzle_diff;
            for(int i=0; i<puzzle_size; i++){
                f_in>>puzzle_id>>puzzle_type>>puzzle_diff;
                puzzle_info.insert({puzzle_id, std::make_pair(puzzle_type, puzzle_diff)});
            }
            f_in.close();
            return_value = new T(map, size_info[2], puzzle_info, my_window, player_coords);
        }
        template<class T>
        // the map after its creation in "map_editor" needs to be saved to a file
        void write_map_to_file(std::string filename, T* map){
            std::ofstream f_out(filename);
            std::vector<int> params = map->get_map_params();
            std::pair<int, int> dummy_info;
            puzzle_info.clear();
            f_out<<params[0]<<' '<<params[1]<<' '<<params[2]<<' '<<'\n';
            for(int i=0;i<params[0];i++){
                for(int j=0;j<params[1];j++){
                    f_out<<map->get_map_tile(std::make_pair(i,j))<<' ';
                    if(map->get_map_tile(std::make_pair(i,j))==2){
                        puzzle_info.insert({100*i+j, map->get_puzzle_info(i,j)});
                    }
                }
                f_out<<'\n';
            }
            f_out<<puzzle_info.size()<<'\n';
            for(auto i : puzzle_info){
                f_out<<std::to_string(i.first)<<' ';
                f_out<<std::to_string(i.second.first)<<' ';
                f_out<<std::to_string(i.second.second)<<' ';
            }
            f_out.close();
        }
};