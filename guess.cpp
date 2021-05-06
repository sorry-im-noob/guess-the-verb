#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <array>
#include <cassert>

class Player {
private:
    std::string name{};
    int score{0};
    std::vector<std::string> words;
    std::string wrong{};
    std::string correct{};
    std::string randomString{};
    char guess{};
    std::string warning{};
    unsigned int left{};
    int chance{};
    std::string file{};
public:
    Player ()
    {
        getWords ();
        getIndentity ();
        getScore ();
        reset ();
    }

    void getRandomString()
    {
        randomString = words[std::rand ()%words.size ()];
    }

    void getChar(std::string text)
    {
        std::cout << text;
        std::cin>>guess;
        std::cin.ignore(100,'\n');
        guess = std::tolower(guess);
    }
    void layout ()
    {
        std::cout << "---------------------------\nGUESS THE VERB\n";
        std::cout << "computer: '" << warning << "'\n";
        warning = "";
        std::cout << "name\t: [" << name << "]\nscore\t: " << score << "\tchance: ";
        for (int i=0;i<chance;++i)
            std::cout << "[] ";
        std::cout << "\n---------------------------\n\n";
        std::cout << "  ";
        for (unsigned int i=0;i<randomString.size ();++i)
        {
            if (correct.find(randomString[i])<correct.size())
            {
                std::cout << static_cast<char>(std::toupper(randomString[i])) << " ";
                ++left;
            }else
            {
                std::cout << "- ";
            }
        }
        std::cout << "\n\n";
    }

    void reset ()
    {
        wrong = "";
        correct = "";
        getRandomString ();
        guess = '\0';
        left = 0;
        chance = 5;
    }
    void checks ()
    {
        if (randomString.find(guess)<randomString.size() && correct.find(guess)>=correct.size())
        {
            correct.push_back (guess);
        }else if (correct.find(guess)<correct.size() || wrong.find(guess)<wrong.size())
        {
            warning = "you already entered that character before";
            --chance;
        }else if (guess!='\0'){
            warning = "wrong!";
            wrong.push_back(guess);
            --chance;
        }
        guess = '\0';
        left = 0;
    }
    bool iswin ()
    {
        
        if (randomString.size()==left)
        {
            std::cout << "congratulation player [" << name << "] you have won\n" << randomString << "\n";
            std::ofstream ss (file);
            ss << ++score;
            ss.close ();
            return true;
        }else if (chance<=0)
        {
            std::cout << "player [" << name << "] you lost, the answer is [" << randomString << "], you are really a dumbass\n";
            return true;
        }
        return false;
    }

    void getScore ()
    {
        std::ifstream ss (file);
        if (!ss.is_open ())
        {
            ss.close ();
            std::ofstream mk (file);
            mk << '0';
            mk.close ();
            warning = "getting score failed!! please try again";
            return;
        }
        ss >> score;
        ss.close ();
    }

    void getIndentity ()
    {
        std::ifstream ss (".ident.txt");
        if (!ss.is_open ())
        {
            ss.close ();
            std::ofstream mk (".ident.txt");
            std::cout  << "enter your name: ";
            std::getline (std::cin,name);
            file = "." + name + ".txt";
            mk << name;
            mk.close ();
            return;            
        }
        std::getline(ss,name);
        file = "." + name + ".txt";
        ss.close ();
    }

    void getWords ()
    {
        std::ifstream ss ("verbs.txt");
        assert (ss.is_open() && "unable to open file, the file has been deleted");
        std::string value;
        while (ss>>value)
        {
            words.push_back(value);
        }
    }
    friend class Display;
};

class Display {
public:
    void menu (const Player &plr)
    {
        std::cout << "welcome ";
        if  (plr.name.size ())
            std::cout << "player [" << plr.name << "]";
        std::cout << "\n";
        std::cout << "--------------------\n\n";
        std::cout << "[01] play\n";
        std::cout << "[99] quit\n\n";
    }

    bool check ()
    {
        int choice;
        std::cout << "choose one: ";
        std::cin>>choice;
        std::cin.ignore (100,'\n');
        return (choice == 01);
    }
};

int main ()
{
    std::srand(std::time(0));
    Player player;
    Display display;
    display.menu (player);
    bool play = display.check ();
    bool win = false;
    while (play)
    {
        player.checks ();
        player.layout();
        win = player.iswin ();
        if (!win)
            player.getChar("guess a character: ");
        else
        {
            display.menu (player);
            play = display.check ();
            player.reset ();
        }
    }
    return 0;
}