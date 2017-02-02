#ifndef PLAYERS_H
#define PLAYERS_H

#include <string>
#include <cstdint>

class Player{
public:
    explicit Player(std::string name_){
        id = count;
        name = name_;
        point = 50;
        count++;
    }
    Player(const Player& p){
        id = p.id;
        name = p.name;
        point = p.point;
    }

    static uint32_t get_num_players(){
        return count;
    }

    uint32_t id;
    std::string name;
    uint32_t point; // 0-100
private:
    static uint32_t count;
};


#endif // PLAYERS_H
