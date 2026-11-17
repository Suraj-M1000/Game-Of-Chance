
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player{
    private:
        int uid;
        std::string name;
        int credits;
        int highscore;

        void update_highscore();

    public:
        Player();    // constructor declaration
        Player(int uid, const std::string& name, int startingCredits);

        int get_uid() const;     // There exists a function called get_uid, which returns an int.
        void set_uid(int uid);

        int get_credits() const;
        void set_credits(int credits);

        void add_credits(int amount);
        bool deduct_credits(int amount);

        const std::string& get_name() const;
        void set_name(const std::string &name);

        int get_highscore() const;
        void set_highscore(int highscore);
};

#endif
