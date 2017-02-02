#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>
#include <cassert>

#include "player.h"

using namespace std;



struct Team{

    explicit Team(Player* p1_, Player* p2_){
        p1 = p1_;
        p2 = p2_;
        team_name = to_string();
    }
    std::string to_string(){
        return  string("[") +  p1->name + "+" + p2->name + string("]");
    }
    bool has_player(const Player* p){
        return p->id == p1->id || p->id == p2->id;
    }

    string team_name;
    Player* p1;
    Player* p2;
};


struct MatchCost{
    MatchCost(){
        team1 = 0;
        team2 = 0;
        cost_match_count = 0;
        cost_oppenent = 0;
        cost_partner = 0;
        cost_total = 0;
        has_match = false;
    }
    MatchCost(Team* t1, Team* t2){
        team1 = t1;
        team2 = t2;
        cost_match_count = 0;
        cost_oppenent = 0;
        cost_partner = 0;
        cost_total = 0;
        has_match = false;
    }

    void set_match(){ has_match = true; }

    bool has_team(const Team* t){
        return ( (t->team_name == team1->team_name) ||
                (t->team_name == team2->team_name) );
    }

    bool has_player(const Player* p){
        return team1->has_player(p) || team2->has_player(p);
    }

    bool head_to_head(const Player* a, const Player* b){
        return ( (team1->has_player(a) && team2->has_player(b)) ||
                (team1->has_player(b) && team1->has_player(a)) );
    }

    bool is_partner(const Player* a, const Player* b){
        return ( (team1->has_player(a) && team1->has_player(b)) ||
                (team2->has_player(a) && team2->has_player(b)) );
    }

    void inc_match_count(const Player* p){
        if(has_player(p)) cost_match_count++;
    }

    void inc_head_to_head(const Player* a, const Player* b){
        if(head_to_head(a,b)) cost_oppenent++;
    }

    void inc_partner(const Player* a, const Player* b){
        if(is_partner(a, b)) cost_partner++;
    }

    int calc_total_cost(){
        cost_total = cost_match_count +
                cost_oppenent +
                2*cost_partner;
        return cost_total;
    }

    Team* team1;
    Team* team2;
    int cost_partner;
    int cost_oppenent;
    int cost_match_count;
    int cost_total;
    bool has_match;
};

inline std::vector<Team*> create_teams_in_group(std::vector<string>& group_members){
    std::vector<Player*> players;
    for(size_t i=0; i<group_members.size(); i++){
        players.push_back(new Player(group_members[i]));
    }
    cout << "number of playes in group: " << Player::get_num_players() << endl;
    std::vector<Team*> team_table;
    for(size_t i=0; i<players.size()-1; i++){
        for(size_t j=i+1; j<players.size(); j++){
            Team* team = new Team(players[i], players[j]);
            team_table.push_back(team);
        }
    }
    return team_table;
}

inline void print_team_table(std::vector<Team*>& table){
    for(auto p : table){
        cout << p->team_name << " ";
    }
}


typedef std::pair<Team*, Team*> Match;
typedef std::map<Match, MatchCost*> MatchMatrix;

typedef std::vector<MatchCost*> MatchCostTable;
inline MatchCostTable create_match_cost_table(std::vector<Team*>& teams_in_A,
                                              std::vector<Team*>& teams_in_B){
    MatchCostTable match_table;
    for(auto t1: teams_in_A){
        for(auto t2: teams_in_B){
            MatchCost* match = new MatchCost(t1, t2);
            match_table.push_back(match);
        }
    }
    return match_table;
}

inline MatchMatrix create_match_matrix(MatchCostTable& match_table){
    MatchMatrix match_matrix;
    for(MatchCost* match : match_table){
        std::pair<Team*, Team*> match_key = std::make_pair(match->team1, match->team2);
        match_matrix[match_key] = match;
    }
    return match_matrix;
}

inline void print_match_matrix_cost(MatchMatrix& match_matrix,
                          const std::vector<Team*>& teams_in_A,
                          const std::vector<Team*>& teams_in_B){

    cout << "        ";
    for(const Team* t2 : teams_in_B){
        cout << t2->team_name << "\t";
    }
    cout <<endl;
    for(auto t1 : teams_in_A){
        cout << t1->team_name << "\t";
        for(auto t2 : teams_in_B){
            Match match = std::make_pair(t1,t2);
            assert(match_matrix.find(match) != match_matrix.end());
            int cost = match_matrix[match]->cost_total;
            cout << cost << "\t";
        }
        cout <<endl;
    }
}

inline void print_match_matrix_partner(MatchMatrix& match_matrix,
                          const std::vector<Team*>& teams_in_A,
                          const std::vector<Team*>& teams_in_B){

    cout << "      ";
    for(const Team* t2 : teams_in_B){
        cout << t2->team_name << "\t";
    }
    cout <<endl;
    for(auto t1 : teams_in_A){
        cout << t1->team_name << "\t";
        for(auto t2 : teams_in_B){
            Match match = std::make_pair(t1,t2);
            assert(match_matrix.find(match) != match_matrix.end());
            int cost = match_matrix[match]->cost_partner;
            cout << cost << "\t";
        }
        cout <<endl;
    }
}

inline void print_match_matrix(MatchMatrix& match_matrix,
                          const std::vector<Team*>& teams_in_A,
                          const std::vector<Team*>& teams_in_B){

    cout << "        ";
    for(const Team* t2 : teams_in_B){
        cout << t2->team_name << "\t";
    }
    cout <<endl;
    for(auto t1 : teams_in_A){
        cout << t1->team_name << "\t";
        for(auto t2 : teams_in_B){
            Match match = std::make_pair(t1,t2);
            assert(match_matrix.find(match) != match_matrix.end());
            MatchCost* cost = match_matrix[match];
            bool has_match = cost->has_match;
            if(has_match){
                cout << "(" << cost->cost_partner << "|"
                     << cost->cost_partner << "|"
                     << cost->cost_partner << ")\t";
            }else{
                cout << "0 \t";
            }
        }
        cout <<endl;
    }
}

inline void assign_match(MatchCostTable& match_table){
    match_table[0]->has_match = true;
}

inline void eval_match_cost(MatchMatrix& match_matrix, Team* t1, Team* t2){
    Match match = std::make_pair(t1,t2);
    assert(match_matrix.find(match) != match_matrix.end());

    // set match
    match_matrix[match]->has_match = true;

    // increase number of matches for each player
    for(auto it: match_matrix){
        MatchCost* p = it.second;
        p->inc_match_count(t1->p1);
        p->inc_match_count(t1->p2);
        p->inc_match_count(t2->p1);
        p->inc_match_count(t2->p2);
    }

    // evaluate head to head
    for(auto it : match_matrix){
        MatchCost* p = it.second;
        // check opponent
        p->inc_head_to_head(t1->p1, t2->p1);
        p->inc_head_to_head(t1->p1, t2->p2);
        p->inc_head_to_head(t1->p2, t2->p1);
        p->inc_head_to_head(t1->p2, t2->p2);
    }

    // evaluate team partner
    for(auto it : match_matrix){
        MatchCost* p = it.second;
        p->inc_partner(t1->p1, t1->p2);
        p->inc_partner(t2->p1, t2->p2);
    }

    for(auto it : match_matrix){
        MatchCost* p = it.second;
        p->calc_total_cost();
    }
}

inline void sort_match_table(MatchCostTable& match_table){
    std::sort(match_table.begin(), match_table.end(),
              [](MatchCost* a, MatchCost* b){
        return a->cost_total < b->cost_total;
    });
}

int main(int argc, char *argv[])
{

    int nplayers = 6;
    if(argc>1)
        string arg = (argv[1]);

    std::cout << "num players: " << nplayers << std::endl;

    // init
    std::vector<string> groupA = {"a1", "a2", "a3", "a4", "a5"};
    std::vector<string> groupB = {"b1", "b2", "b3", "b4", "b5"};
    std::cout << "\ngourpA: " <<endl;
    for(auto p:groupA){
        std::cout << p << " ";
    }
    std::cout << "\ngourpA: " <<endl;
    for(auto p:groupB){
        std::cout << p << " ";
    }
    std::cout << endl <<endl;

    // create team table
    std::vector<Team*> team_tableA = create_teams_in_group(groupA);
    print_team_table(team_tableA);
    cout << "team A size: " << team_tableA.size() << endl;

    std::vector<Team*> team_tableB = create_teams_in_group(groupB);
    print_team_table(team_tableB);
    cout << "team B size: " << team_tableB.size() << endl;

    MatchCostTable match_table = create_match_cost_table(team_tableA, team_tableB);
    MatchMatrix match_matrix = create_match_matrix(match_table);

    cout << "matrix size: " << match_matrix.size() <<endl;
    print_match_matrix(match_matrix, team_tableA, team_tableB);

    // init team 0,0
    assign_match(match_table);
    eval_match_cost(match_matrix, match_table[0]->team1, match_table[0]->team2);
    sort_match_table(match_table);
    print_match_matrix_cost(match_matrix, team_tableA, team_tableB);

    //
    cout << "\\n cost partner: " <<endl;
    print_match_matrix_partner(match_matrix, team_tableA, team_tableB);

    const int num_matches = 12;
    for(int i=1; i<num_matches; i++){
        cout << "\n ------- add match " << i << " ------\n";
        cout << " (x|y|z): (match cost| parter cost | head to head costs)" <<endl;

        system("pause");

        eval_match_cost(match_matrix, match_table[0]->team1, match_table[0]->team2);
        sort_match_table(match_table);
        print_match_matrix(match_matrix, team_tableA, team_tableB);
    }

    return 0;
}
