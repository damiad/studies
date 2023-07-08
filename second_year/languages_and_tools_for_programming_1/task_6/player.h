#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

class Player {
public:
    ~Player() = default;
    Player(std::string const& new_name) : name(new_name), money(1000),
        suspension(0), bankrupt(false), position(0) {}
    void reset() {
        money = 1000;
        suspension = 0;
        bankrupt = false;
        position = 0;
    }
    void addMoney(unsigned int amount) { money += amount; }

    unsigned int substractMoney(unsigned int amount) {
        if (amount > money) {
            bankrupt = true;
            unsigned int res = money;
            money = 0;
            return res;
        } else {
            money -= amount;
            return amount;
        }
    }

    [[nodiscard]] bool isBankrupt() const { return bankrupt; }
    [[nodiscard]] bool isSuspended() const { return suspension > 0; }

    void addSuspension(uint64_t suspensionTime) { suspension += suspensionTime; }
    void decreaseSuspension() { suspension--; }

    void changePosition(uint64_t new_position) { position = new_position; }

    [[nodiscard]] const std::string& getName() const { return name; }
    std::string getStatus() const {
        if (bankrupt)
            return "*** bankrut ***";
        else if (suspension > 0)
            return std::string("*** czekanie: ") + std::to_string(suspension) + " ***";
        return "w grze";
    }
    [[nodiscard]] uint64_t getPosition() const { return position; }
    [[nodiscard]] unsigned int getMoney() const { return money; }

private:
    const std::string name;
    unsigned int money;
    uint64_t suspension;
    bool bankrupt;
    uint64_t position;
};

#endif