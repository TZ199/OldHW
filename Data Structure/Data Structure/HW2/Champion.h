//Clss file for hw2 by Tianxin Zhou
//Edited date 1/30/2017

#include <vector>
#include <string>
class Champion{
    private:
        std::string name;
        int win;
        int lose;
        float winrate;
        int minion;
        std::string people;
    
    public:
        Champion();
        Champion(std::string aname, int check);
        std::string getName() const;
        int getLose() const;
        int getWin() const;
        int getMinion() const;
        std::string getPeople() const;
        float getWinrate() const;
        void addLose();
        void addWin();
        void setName();
        void setWinrate();
        void addMinion();
        void setPeople(std::string people);
    
        

    
    
};

bool c_sortHelper (const Champion& champion1, const Champion& champion2);