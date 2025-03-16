#pragma once

#include "static_window.hpp"




class Leaderboard: public Static_window{ //class to display leaderboard 
    private:
        sf::Text ids_name, names_name, scores_name; //titles of columns
        std::vector<sf::Text> ids;
        std::vector<sf::Text> names;
        std::vector<sf::Text> scores;
        std::vector<std::tuple<std::string, std::string, int>> leaderboard;
    public:    
        Leaderboard(sf::RenderWindow& my_window): Static_window(my_window){
        
            set_text(title, MyFont, "leaderboard",           80.f,20.f, 70);
            set_text(ids_name, MyFont, "id",                 10.f,90.f, 25);
            set_text(names_name, MyFont, "username",         40.f,90.f, 25);
            set_text(scores_name, MyFont, "bestscore",       200.f,90.f, 25); 

            auto raw_leaderboard = load_leaderboard(); //loads leaderboard from file

            // convert scores to integers
            for (const auto& row : raw_leaderboard) {
                leaderboard.emplace_back(row[0], row[1], std::stoi(row[2]));
            }
            sort_leaderboard();
            set_text_vectors();
        }

        ~Leaderboard(){}

    void sort_leaderboard() { //sort leaderboard according to scores of players
        std::sort(leaderboard.begin(), leaderboard.end(),
            [](const auto& a, const auto& b) {
                return std::get<2>(a) > std::get<2>(b); //descending
            });
    }

    void set_text_vectors() { //creates sf::text for data from leaderboard
        ids.clear();
        names.clear();
        scores.clear();

        int text_size = 20;
        for (size_t i = 0; i < leaderboard.size(); ++i) {
            const auto& [id_str, name_str, score_val] = leaderboard[i];

            sf::Text id;
            set_text(id, MyFont, id_str, 10, 120 + i * (text_size + 2), text_size);
            ids.push_back(id);

            sf::Text name;
            set_text(name, MyFont, name_str, 40, 120 + i * (text_size + 2), text_size);
            names.push_back(name);

            sf::Text score;
            set_text(score, MyFont, std::to_string(score_val), 200, 120 + i * (text_size + 2), text_size);
            scores.push_back(score);
        }
    }

    //draws leaderboard to window
    void draw_child_content() override{ 
        for(sf::Text text: ids){
            window.draw(text);
        }
        for(sf::Text text: names){
            window.draw(text);
        }
        for(sf::Text text: scores){
            window.draw(text);
        }
        window.draw(ids_name);
        window.draw(names_name);
        window.draw(scores_name);
    }

};