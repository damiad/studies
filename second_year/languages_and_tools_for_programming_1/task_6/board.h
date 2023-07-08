#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <string>

#include "player.h"
#include "square.h"

class Board {
protected:
	const std::vector<std::shared_ptr<Square>> squares;

public:
	~Board() = default;
	Board(const std::vector<std::shared_ptr<Square>>& new_squares) :
		squares(new_squares) {}
	void reset(){
		for(auto square : squares)
			square->reset();
	}
	std::shared_ptr<Square> getSquare(size_t index) { return squares[index]; }
	size_t size() const noexcept { return squares.size(); }
};

class Worldcup2022Board : public Board {
public:
	Worldcup2022Board() : Board( {
		std::make_shared<SeasonStart>(),
		std::make_shared<Match>("z San Marino", 160 , 1.0),
		std::make_shared<Empty>(),
		std::make_shared<Match>("z Lichtensteinem", 220, 1.0),
		std::make_shared<YellowCard>(3),
		std::make_shared<Match>("z Meksykiem", 300, 2.5),
		std::make_shared<Match>("z Arabią Saudyjską", 280, 2.5),
		std::make_shared<Bookie>(100, 100),
		std::make_shared<Match>("z Argentyną", 250, 2.5),
		std::make_shared<Goal>(120),
		std::make_shared<Match>("z Francją", 400, 4.0),
		std::make_shared<Penalty>(180)
	}) {}
};

#endif