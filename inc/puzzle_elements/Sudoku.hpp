#pragma once
#include "Puzzle.hpp"



// a puzzle type in which user is asked to solve a sudoku
class Sudoku: public Puzzle
{
    protected:
        std::string ans;
        std::string base;
    public:
    Sudoku(int x,int y): Puzzle(x,y) {}
    ~Sudoku() {}
    void read_puzzle() override
    {
        std::vector<std::string> random_choice;
        std::string myText;
        std::ifstream MyReadFile("inc/puzzle_elements/puzzle_data/sudoku.txt");
        while (std::getline (MyReadFile, myText)) 
        {
           if(myText[0]==level) random_choice.push_back(myText);
        }
        MyReadFile.close();
        std::srand(std::time(0));
        int range = random_choice.size();
        int x = int(rand() % range +0);
        std::string tmp_str=random_choice[x];// the line chosen from the file, each line in the file is a separate puzzles
        int semi_col=0;
        std::string cointainer_str="solve this sudoku, enter position (x,y) and correct digit";
        set_text(cointainer_str);
        cointainer_str="";
        //proccessing the puzzle information from chosen line in the file, each line cointains level, sudoku to solve and solved sudoku a string separated by ';'
        for(int sign=0;sign<tmp_str.size();sign++)
        {
            if(tmp_str[sign]==';')
            {
                if(semi_col==1)
                {
                    base=cointainer_str;
                    cointainer_str="";
                }
                if(semi_col==2)
                {
                    ans=cointainer_str;
                    cointainer_str="";
                }
                semi_col++;
            }
            else
            {
                if(semi_col>0)
                {
                    cointainer_str+=tmp_str[sign];
                }
            }
        }

    }

    bool solver_checker() override
    {
        int tile_size = 80;
        window.create(sf::VideoMode(9*tile_size,9*tile_size), "Sudoku");  
        int amount = 0;

        read_puzzle();

        char base_c[9][9];// to check if changing given poll is allowed
        char user_c[9][9];// editing what user types
        char ans_c[9][9];// the correct solution
        //making grid from the string
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(base[i*9+j]!='.')
                {
                    base_c[i][j]=base[i*9+j];
                    user_c[i][j]=base[i*9+j];
                    amount++;
                }
                else
                {
                    base_c[i][j]=' ';
                    user_c[i][j]=' ';
                }
                
                ans_c[i][j]=ans[i*9+j];
            }
        }

        return solve_sudoku(user_c, base_c, tile_size, amount);
    }
    //here the checking itself happens in this function
    bool verify_correctness(char user_c[9][9]){
            for(int i=0;i<9;i++)
            {
                for(int j=0;j<9;j++)
                {
                    base[i*9+j]=user_c[i][j];
                }
            }
            if (ans==base){
                return true;
            }
            return false;
    }
    //function that displays the sudoku grid
    void display_sudoku( char grid[9][9], int tile_size){
        sf::RectangleShape shape(sf::Vector2f {tile_size, tile_size});
        shape.setOutlineThickness(0.01*tile_size);
        shape.setOutlineColor(sf::Color::Black);
        sf::RectangleShape vertical_line(sf::Vector2f {0.05*tile_size, 9*tile_size});
        sf::RectangleShape horizontal_line(sf::Vector2f {9*tile_size, 0.05*tile_size});
        shape.setFillColor(sf::Color::White);
        vertical_line.setFillColor(sf::Color::Black); 
        horizontal_line.setFillColor(sf::Color::Black);
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                shape.setPosition(sf::Vector2f {i*tile_size, j*tile_size});
                window.draw(shape);
                manager->put_text_to_window((i+0.35)*tile_size, (j+0.14)*tile_size,
                 std::string()+grid[i][j], tile_size*0.6, -1,sf::Color::Black);
            }
        }
        for (int i = 2; i < 8; i+=3) {
            horizontal_line.setPosition(sf::Vector2f {0,(i+0.975)*tile_size});
            window.draw(horizontal_line);
            vertical_line.setPosition(sf::Vector2f {(i+0.975)*tile_size,0});
            window.draw(vertical_line);
        }
        window.display();
    }
    //function that interacts with the user detects the mouse movements and reads users input,
    // user is asked to choose a tile ( with a mouse) and entr an integer in range [0,9]
    // the user is allowed to clear  a tile provided it was entered by him not giving in the initial state of the sudoku
    bool solve_sudoku(char (&grid)[9][9], char (&base)[9][9], 
        int tile_size, int putted){
        manager = new text_manager(window, sf::Color::White);
        int mouse_coords[2] = {-1, -1}; std::string ans, dummy="";
        int counter = 0;
        sf::Event event;
        while(true){
            display_sudoku(grid, tile_size);
            while (window.pollEvent(event))
            {   
                if(event.type == sf::Event::Closed){
                    window.close();
                    return false;
                }

                if(event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left){
                        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                        mouse_coords[0] = localPosition.x/tile_size;
                        mouse_coords[1] = localPosition.y/tile_size; 
                    }
                if(mouse_coords[0]<0 || mouse_coords[0]>8 || mouse_coords[1]<0 || mouse_coords[1]>8){
                    continue;
                }
                if(manager->get_text(mouse_coords[0]*tile_size, mouse_coords[1]*tile_size, 
                     dummy ,ans, event, 0, tile_size*0.6)){
                    window.clear();
                    if(ans=="###error###"){continue;}
                    if(ans[0]<'0' || ans[0]>'9'){
                        manager->put_text_at_center("You Must enter an Integer!!!", tile_size*0.73,
                         1000, sf::Color::Black);
                        return false;
                    }
                    if(base[mouse_coords[0]][mouse_coords[1]]==' '){
                        putted+= grid[mouse_coords[0]][mouse_coords[1]]!=' ' ? 0:1;
                        grid[mouse_coords[0]][mouse_coords[1]] = ans[0];
                        if(grid[mouse_coords[0]][mouse_coords[1]]=='0'){
                            grid[mouse_coords[0]][mouse_coords[1]]=' ';
                            putted--;
                        }
                    }
                    ans="";
                }
            }
            if(putted==81){
                window.clear(sf::Color::White);
                if(verify_correctness(grid)){
                    manager->put_text_at_center("Correct!!!", 70, 700, sf::Color::Black);
                    return true;
                }
                manager->put_text_at_center("Loser!!!", 70, 700, sf::Color::Black);
                return false;
            }
        }
    }
};
