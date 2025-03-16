#include "Puzzle.hpp"


//puzzle of type "Knowleadge"  with multiple correct answears
class knowledge: public Puzzle
{
    protected:
        std::vector<std::string> ans;
    public:
    knowledge(int x,int y): Puzzle(x,y) {}
    ~knowledge() {} 
    void read_puzzle() override
    {
        std::vector<std::string> random_choice;
        std::string myText;
        std::ifstream MyReadFile("inc/puzzle_elements/puzzle_data/knowledge.txt", std::ios::in);
        while (std::getline (MyReadFile, myText)) 
        {
           if(myText[0]==level) random_choice.push_back(myText);
        }
        MyReadFile.close();
        std::srand(std::time(0));
        int range = random_choice.size();
        int x = int(rand() % range);
        std::string tmp_str=random_choice[x];// the line chosen from the file, each line in the file is a separate puzzles
        int semi_col=0;
        std::string cointainer_str="";
        //proccessing the puzzle information from chosen line in the file, each line cointains level, puzzle text, ans possible correct answears separated by ';'
        for(int sign=0;sign<tmp_str.size();sign++)
        {
            if(tmp_str[sign]==';')
            {
                if(semi_col==1)
                {
                    set_text(cointainer_str);
                    cointainer_str="";
                }
                if(semi_col>=2)
                {
                    ans.push_back(cointainer_str);
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

        window.create(sf::VideoMode(700,500), "My_Game");  
        manager = new text_manager(window);
        read_puzzle();
        std::string user_ans = question_loop(window);
        bool rn=false;
        for(int idx=0;idx<ans.size();idx++)
        {
            if(ans[idx]==user_ans) rn=true;
        }
        if (rn){
            manager->put_text_at_center("Correct!!!", 70, 700);
            return true;
        }
        manager->put_text_at_center("Loser!!!", 70, 700);
        return false;
    }
    // displayes the puzzle to the user and fetches the answear from the user
    std::string question_loop(sf::RenderWindow& window){
        sf::Event event;
        std::string ans;
        std::string communicate = "please enter your answer: ";
        manager->put_text_to_window(20, 20, "here is the puzzle u need to solve: ", 35);
        manager->put_text_to_window(20, 120, puzzle_text, 35);
        manager->put_text_to_window(20, 220, "please enter your answer: ", 35);
        window.display();
        while(true){
            manager->put_text_to_window(20, 20, "here is the puzzle u need to solve: ", 35);
            manager->put_text_to_window(20, 120, puzzle_text, 35);
            while (window.pollEvent(event))
            {
                //here window is also displayed
                if(manager->get_text(20, 220, communicate,ans, event, 25, 35)){
                    window.clear();
                    return ans;
                }
            }
            window.clear();
        }
    }

};