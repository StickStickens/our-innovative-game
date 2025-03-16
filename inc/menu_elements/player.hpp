#pragma once


#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include<sstream>
#include "../map_elements/Map_manager.hpp"
#include "menu_class.hpp"



//abstract class player since each player has its unique priviliges, we store the common attributes and methods here
class Player{ 
    protected:
        int points, vision;
        std::string username, password ;
        int bestscore, id;
    public:
        virtual void update_player(int type, int weight, bool was_solved){} ;
        virtual std::string gettype() const{} ;
        virtual void default_settings(){};
        int get_id(){return id;}
        std::string get_password(){return password;}
        int get_bestscore(){return bestscore;}
        int  get_points() const{ return points;} ;
        int get_vision_range() const{ return vision;} ;
        std::string getname() const{return username;} ;

        virtual ~Player(){
        }
        void game_ended(int time, bool win_or_lost){
            //this function  writes players score to leaderboard if it was the best score of a player
            //it calculates final score of a player
            //should be called at the end of a game
            if(gettype() == "Admin"){return;}
            int final_score;
            final_score=  std::round(60 *points/time);
            if(!win_or_lost){final_score-=100;}
            auto leaderboard=read_leaderboard();
            int best_score = std::stoi(leaderboard[id-1][2]);
            if(best_score < final_score){
                write_score_to_file(final_score, id-1);
            }
        
        }

        // getting the leatherboard information from the file in which its stored
        std::vector <std::vector<std::string>> read_leaderboard(){
            std::vector<std::vector<std::string>>leaderboard;

            std::ifstream file("inc/menu_elements/leaderboard/leaderboard.csv"); 
            if (!file.is_open()) {
                std::cerr << "Error opening file!" << std::endl;
            return {};
            }
            std::string line;
                
            float i=0;
            while (std::getline(file, line)) {
                i+=1;
                std::vector<std::string> words;
                std::stringstream ss(line);
                std::string word;

                while (std::getline(ss, word, ',')) {
                    words.push_back(word);
                }
                leaderboard.push_back(words);
            }
            return leaderboard;
        }
        

        //updating the leatherboard
        void write_score_to_file(int final_score, int id){
            
            auto leaderboard = read_leaderboard();
            leaderboard[id][2] = std::to_string(final_score);

            std::ofstream file("inc/menu_elements/leaderboard/leaderboard.csv");
            if(!file){
                std::cerr<<"error opening leaderboard"<<std::endl;
                return;
            }

            for(int i=0; i<leaderboard.size(); i++){
                file<< leaderboard[i][0] <<","<< leaderboard[i][1] << ","<< leaderboard[i][2]  << "," << leaderboard[i][3]  << "," << leaderboard[i][4] ;
                if(i<leaderboard.size()){file << "\n";}
            }
            file.close();
    
        }
};