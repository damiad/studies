#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include <memory>
#include <string>
#include <exception>
#include <vector>

#include "worldcup.h"
#include "board.h"
#include "square.h"
#include "player.h"

class TooManyDiceException : public std::exception {};
class TooFewDiceException : public std::exception {};
class TooManyPlayersException : public std::exception {};
class TooFewPlayersException : public std::exception {};

// Kostki do gry
class Dice {
private:
	std::vector<std::shared_ptr<Die>> dice;
public:
	~Dice() = default;
	Dice() = default;

	void addDie(std::shared_ptr<Die> die) { dice.push_back(die); }

	unsigned int roll() {
		unsigned int sum = 0;
		for (auto die : dice) sum += (unsigned int)die->roll();
		return sum;
	}
	uint64_t size() { return dice.size(); }
};

// Domyślnie jest skonfigurowana tablica wyników, która nic nie robi.
class DefaultScoreBoard : public ScoreBoard {
public:
	~DefaultScoreBoard() = default;
	DefaultScoreBoard() = default;
	void onRound(unsigned int roundNo) override {(void) roundNo;}
	virtual void onTurn(std::string const& playerName,
	                    std::string const& playerStatus, std::string const& squareName,
	                    unsigned int money) {(void) playerName; (void)playerStatus; (void) squareName; (void) money;}
	virtual void onWin(std::string const& playerName) {(void) playerName;}
};


class WorldCup2022 : public WorldCup {
private:
	std::shared_ptr<Dice> dice;
	std::shared_ptr<ScoreBoard> scoreboard;
	std::shared_ptr<Board> board;
	std::vector<std::shared_ptr<Player>> players;
	uint64_t active_players;
public:
	~WorldCup2022() = default;
	WorldCup2022()
		: dice(std::make_shared<Dice>()),
		  scoreboard(std::make_shared<DefaultScoreBoard>()),
		  board(std::make_shared<Worldcup2022Board>()) {}

	void addDie(std::shared_ptr<Die> die) override {
		if (die == nullptr)
			return;
		dice->addDie(die);
	}

	void addPlayer(std::string const& name) override { players.push_back(std::make_shared<Player>(name)); active_players++; }


	// Konfiguruje tablicę wyników. Domyślnie jest skonfigurowana tablica wyników, która nic nie robi.
	void setScoreBoard(std::shared_ptr<ScoreBoard> new_scoreboard) override { scoreboard = new_scoreboard; }

	void prepGame() {
		for (auto player : players)
			player->reset();
		board->reset();
		active_players = players.size();
	}

	void play(unsigned int rounds) override {
		if (dice->size() > 2) throw TooManyPlayersException();
		if (dice->size() < 2) throw TooFewDiceException();
		if (players.size() > 11) throw TooManyPlayersException();
		if (players.size() < 2) throw TooFewPlayersException();
		prepGame();
		for (unsigned int round = 0; round < rounds; round++) {
			scoreboard->onRound(round);
			for (std::shared_ptr<Player> player : players) {
				if (!player->isBankrupt()) {
					if (player->isSuspended()) player->decreaseSuspension();
					if (!player->isSuspended()) { // robi ruch
						unsigned int rolled = dice->roll();
						unsigned short position;
						for (unsigned int index = 1; index < rolled; index++) {
							position = (player->getPosition() + index) % board->size();
							board->getSquare(position)->onPassing(player);
							if (player->isBankrupt()) break;
						}
						if (!player->isBankrupt()) {
							board->getSquare((player->getPosition() + rolled) % board->size())->onStanding(
							    player);
						} else
							active_players--;
						// zawsze konczy na wyrzuconym polu
						player->changePosition((player->getPosition() + rolled) % board->size());
					}
				}
				scoreboard->onTurn(player->getName(), player->getStatus(),
				                   board->getSquare(player->getPosition())->getName(), player->getMoney());
				if (active_players == 1) { //znajdź winnera, zakoncz gre
					for (std::shared_ptr<Player> player : players) {
						if (!player->isBankrupt()) {scoreboard->onWin(player->getName()); return; }
					}
				}
			}
		}
		unsigned int max = 0;
		size_t winner; // znajdujemy zwycięzce
		for (size_t j = 0; j < players.size(); j++) {
			if (players[j]->getMoney() > max) {
				max = players[j]->getMoney();
				winner = j;
			}
		}
		scoreboard->onWin(players[winner]->getName());
		return;
	}
};
#endif