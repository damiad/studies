#ifndef SQUARE_H
#define SQUARE_H

#include <memory>
#include <string>

#include "player.h"

class Square {
public:
    virtual ~Square() = default;

    std::string const& getName() const {
        return name;
    }
    virtual void reset() = 0;
    virtual void onPassing(std::shared_ptr<Player> player) = 0;
    virtual void onStanding(std::shared_ptr<Player> player) = 0;

protected:
    const std::string name;
    Square(std::string const& _name) : name(_name) {}
};

class SeasonStart : public Square {
public:
    ~SeasonStart() = default;
    SeasonStart() : Square("Początek sezonu") {}
    void reset() override {}
    void onPassing(std::shared_ptr<Player> player) override {
        player->addMoney(startPay);
    }
    void onStanding(std::shared_ptr<Player> player) override {
        player->addMoney(startPay);
    }
private:
    static constexpr unsigned int startPay = 50;
};

class Goal : public Square {
public:
    ~Goal() = default;
    Goal(unsigned int _goalPay) : Square("Gol"), goalPay(_goalPay) {}
    void reset() override {}
    void onPassing(std::shared_ptr<Player> player) override {(void) player;}
    void onStanding(std::shared_ptr<Player> player) override {
        player->addMoney(goalPay);
    }
private:
    const unsigned int goalPay;
};

class Penalty : public Square {
public:
    ~Penalty() = default;
    Penalty(unsigned int _penaltyPay) : Square("Rzut karny"),
        penaltyPay(_penaltyPay) {}
    void reset() override {}
    void onPassing(std::shared_ptr<Player> player) override {(void) player;}
    void onStanding(std::shared_ptr<Player> player) override {
        player->substractMoney(penaltyPay);
    }
private:
    const unsigned int penaltyPay;
};

class Bookie : public Square {
public:
    ~Bookie() = default;
    Bookie(unsigned int _winAmount, unsigned int _loseAmount) :
        Square("Bukmacher"), winAmount(_winAmount), loseAmount(_loseAmount) {}
    void reset() override {playersPassed = 0;}
    void onPassing(std::shared_ptr<Player> player) override {(void) player;}
    void onStanding(std::shared_ptr<Player> player) override {
        if (playersPassed == 0)
            player->addMoney(winAmount);
        else
            player->substractMoney(loseAmount);

        playersPassed = (playersPassed + 1) % playerCycle;
    }
private:
    unsigned int winAmount;
    unsigned int loseAmount;
    static constexpr int playerCycle = 3;
    unsigned int playersPassed = 0;
};

class YellowCard : public Square {
public:
    ~YellowCard() = default;
    YellowCard(uint64_t _suspensionTime) : Square("Żółta kartka"),
        suspensionTime(_suspensionTime) {}
    void reset() override {}
    void onPassing(std::shared_ptr<Player> player) override {(void) player;}
    void onStanding(std::shared_ptr<Player> player) override {
        player->addSuspension(suspensionTime);
    }
private:
    uint64_t suspensionTime;
};

class Match : public Square {
public:
    ~Match() = default;
    Match(std::string const& withWho, unsigned int _playingFee, float _fifaWeight) :
        Square("Mecz " + withWho), playingFee(_playingFee), totalMoney(0),
        fifaWeight(_fifaWeight) {}
    void reset() override { totalMoney = 0;}
    void onPassing(std::shared_ptr<Player> player) override {
        totalMoney += player->substractMoney(playingFee);
    }
    void onStanding(std::shared_ptr<Player> player) override {
        player->addMoney((unsigned int)(fifaWeight * totalMoney));
        totalMoney = 0;
    }
private:
    unsigned int playingFee;
    unsigned int totalMoney;
    float fifaWeight;
};

class Empty : public Square {
public:
    ~Empty() = default;
    Empty() : Square("Dzień wolny od treningu") {}
    void reset() override {}
    void onPassing(std::shared_ptr<Player> player) override {(void) player;}
    void onStanding(std::shared_ptr<Player> player) override {(void) player;}
};

#endif