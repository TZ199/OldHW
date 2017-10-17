//Tianxin Zhou
//Added Custom case 2/01/2017
#include <vector>
#include <string>
class Player{
    private:
        std::string name;
        int death;
        int kill;
        float kdr;
        std::vector<std::string> champion;
        int latekill;
    
    public:
        Player();
        Player(std::string aname, std::string achampion);
        std::string getName() const;
        int getDeath() const;
        int getKill() const;
        std::string getChampion() const;
        float getKdr() const;
        int getLatekill() const;
        void addDeath();
        void addKill();
        void setName();
        void addChampion(std::string achampion);
        void setKdr();
        void addLatekill();
        
    
        

    
    
};

bool sortHelper (const Player& player1, const Player& player2);
//for custom
bool sortHelper2(const Player& player1, const Player& player2);