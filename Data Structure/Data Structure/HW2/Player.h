//Clss file for hw2 by Tianxin Zhou
//Edited date 1/30/2017

#include <vector>
#include <string>
class Player{
    private:
        std::string name;
        int death;
        int kill;
        float kdr;
        std::vector<std::string> champion;
    
    public:
        Player();
        Player(std::string aname, std::string achampion);
        std::string getName() const;
        int getDeath() const;
        int getKill() const;
        std::string getChampion() const;
        float getKdr() const;
        void addDeath();
        void addKill();
        void setName();
        void addChampion(std::string achampion);
        void setKdr();
    
        
    
    
    
};

bool sortHelper (const Player& player1, const Player& player2);