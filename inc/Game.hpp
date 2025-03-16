#pragma once

#include "menu_elements/menu_class.hpp"
#include "puzzle_elements/Puzzle_handler.hpp"


#include "map_elements/Map_editor.hpp"
#include "map_elements/Map_manager.hpp"

class Game{
    private:
        Map* game_map;
        Player* player;
        sf::RenderWindow window;
        std::string filename;
        text_manager* manager;
        View_manager* view;
        Puzzle_handler* puzzle;
        sf::Clock clock;
        bool is_running;

    public:

        //constructing the game
        Game(int size){
            Map_manager* helper = new Map_manager(window);
            manager = new text_manager(window);
            Puzzle_handler* puzzle = new Puzzle_handler();
            window.create(sf::VideoMode(20*size,16*size), "Game");
            Menu* menu = new Menu(window, size);
            std::string menu_status = menu->display_menu();
            while(true){
                if(menu_status=="Play"){
                    filename = menu->choose_mapa();
                    helper->read_map_from_file(filename, game_map);
                    this->player = menu->get_player();
                    view = new View_manager(window, game_map);
                    is_running = true;
                    play_game();
                }
                if(menu_status=="Exit"){ break;}
                menu_status = menu->display_menu();
            }
            delete helper;
            //Loading font, as I need it in display menu function
        }
        ~Game(){ delete game_map; delete player; delete puzzle;}
        void move();
        void end_game(bool win_or_lose);
        void play_game();
        void display_menu(int time);
        std::string format_time(int time);

};

//display menu (most of the code is just to display time in good format)
std::string Game::format_time(int time){
    std::string time_info="";
    time_info+= time/3600>9 ? "":"0";
    time_info+= std::to_string(time/3600)+':';
    time_info+= (time/60)%60>9 ? "":"0";
    time_info+= std::to_string((time/60)%60)+':';
    time_info+= time%60>9 ? "":"0";
    time_info+= std::to_string(time%60);
    return time_info;
}

void Game::display_menu(int time){
    std::vector<int> sizes = game_map->get_map_params();
    std::string time_info = "Time: " + format_time(time);
    //create proper time format
    manager->draw_menu_marker(sizes[2]);
    sf::Vector2f text_coords = {sizes[2]*0.2, sizes[2]*0.1};
    if(player->gettype()=="Admin"){
        manager->put_text_to_window(text_coords.x, text_coords.y,
         "Admin gets no points", sizes[2]*0.5);
    }
    else{
        manager->put_text_to_window(text_coords.x, text_coords.y,
         "Score: "+std::to_string(player->get_points()), sizes[2]*0.5);
    }
    text_coords = {sizes[2]*(view->get_window_x()-4), sizes[2]*0.1};
    manager->put_text_to_window(text_coords.x, text_coords.y, time_info,sizes[2]*0.5);
}


//Move player to new coordinates (end if new tile is 4, solve puzzle if new tile is 2, move is 1
// nothing if new tile is 0)
void Game::move(){
    std::pair<int,int> action;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){action = {0,-1};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){action = {0,1};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){action = {1,0};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){action = {-1,0};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){action = {0,-1};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){action = {0,1};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){action = {1,0};}
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){action = {-1,0};}
    else{ return ;}
    std::pair<int,int> old_coords = game_map->get_player_coords();
    std::pair<int, int> new_coords = std::make_pair(old_coords.first+action.first,old_coords.second+action.second);
    if(!game_map->is_within_limits(new_coords)){return;}
    // std::cout<<game_map->get_map(new_coords);
    switch(game_map->get_map_tile(new_coords)){
        case 1:
            game_map->swap_map(old_coords, action);
            return;
        case 2:
            {
            std::cout<<"solve"<<new_coords.first<<"  "<<new_coords.second<<'\n';
            std::pair<int, int> puzzle_data = game_map->get_puzzle_info(new_coords.first, new_coords.second);
            std::cout<<puzzle_data.first<<" "<<puzzle_data.second;
            bool was_solved = puzzle->run(puzzle_data.first, puzzle_data.second);
            player->update_player(puzzle_data.first, puzzle_data.second, was_solved);
            if(player->get_vision_range()<1){end_game(0); return;}
            if(was_solved){game_map->swap_map(old_coords, action);}
            return;
            }
            break;
        case 4:
            end_game(1);
            return;
        case 5:
            game_map->set_map_tile(old_coords, 1);
            game_map->draw_grid(1);
            manager->put_text_to_window((view->get_window_x()-9)*70, 10, "You fell into pit and died", 40, 1000);
            end_game(0);
            return;
        default:
            return;
    }
}


//terminating the game and saving the results
void Game::end_game(bool win_or_lose){
    is_running = false;
    player->game_ended(clock.getElapsedTime().asSeconds(), win_or_lose);
    player->default_settings();
}


//starting new game opennning new sfml window, setting up the new map and all the important components
void Game::play_game(){
    clock.restart();
    sf::Event event;
    while (window.pollEvent(event));
    window.setTitle("Game");
    int size = game_map->get_map_params()[2];
    window.setSize(sf::Vector2u {20*size, 16*size});
    window.setPosition(sf::Vector2i(0,0));
    view->resize_window(game_map->get_player_coords());
    while (true)
    {
        if(!is_running){
            return;}
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                end_game(0);
                return;
            }
        }
        move();
        usleep(20000);
        window.clear();
        //this time is how fast player moves; if you wish, you may modify it
        view->settle_view(game_map->get_player_coords());
        game_map->draw_grid(player->get_vision_range());
        display_menu(clock.getElapsedTime().asSeconds());
        window.display();
    }
}