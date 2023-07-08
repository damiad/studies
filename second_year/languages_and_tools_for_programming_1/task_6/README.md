# Qualify from from the Group

Qualify from from the Group is a board game where the player's goal is to qualify from the group at the World Cup. Players can accumulate wealth through organization and playing matches.

The game can be played by 2 to 11 players. Each player starts with 1000 Zdzislaws and begins the game on the "Start of the Season" field. Players move around a cyclic board consisting of 12 fields. During their turn, unless the player is on pause:
* They roll two dice.
* They move a number of fields equal to the sum of the dice values.
* They perform an action associated with the field they land on (if there is any action associated with that field).

The player with the most Zdzislaws at the end of the tournament qualifies from the group and goes down in history.

There are different types of fields with associated actions:
* Start of the Season: When a player passes or stops on this field, they receive 50 Zdzislaws.
* Goal: When a player stops on this field, they receive a goal bonus.
* Penalty Kick: When a player stops on this field, they have to pay Szczesny for saving the penalty kick.
* Bookmaker: When a player stops on this field, every third player wins a bet in Zdzislaws, while the others lose. The betting series starts with the first winner.
* Yellow Card: The player waits for a certain number of rounds.
* Match: When a player passes this field, they have to play a match and pay a fee for playing it. The player who stops on this field collects all the fees paid for played matches multiplied by the match weight according to FIFA.

A match can be a friendly match, a points match, or a final. The match weight is 1 for a friendly match, 2.5 for a points match, and 4 for a final.

If a player runs out of money to pay the fees at any point during the game, they go bankrupt immediately. Bankruptcy means the player ends the game and is eliminated from the tournament. If there is only one player remaining in the game, the game ends.

Each field on the board has a name. The fields are as follows:
1. Start of the Season
2. Match against San Marino: A chance to win, friendly match, the fee for playing the match is 160 Zdzislaws.
3. Day off from training: You rest and enjoy your chosen drinks, nothing happens, the coach might jump on you.
4. Match against Liechtenstein: It's getting harder now, friendly match, the fee for playing the match is 220 Zdzislaws.
5. Yellow Card: You fouled Lewandowski brutally, you receive a yellow card and wait for three rounds.
6. Match against Mexico: Formerly an opening match, points match, the fee for playing the match is 300 Zdzislaws.
7. Match against Saudi Arabia: Formerly a match for everything, but today it's a points match, the fee for playing the match is 280 Zdzislaws.
8. Bookmaker: When a player stops on this field, every third player wins 100 Zdzislaws for the bet, while the others lose 100 Zdzislaws.
9. Match against Argentina: Usually an honor match, but there is a chance to win 0:2, points match, the fee for playing the match is 250 Zdzislaws.
10. Goal: After VAR verification, the referee allows the goal, and you receive a bonus of 120 Zdzislaws.
11. Match against France: Round of 16, and that would be it, but it's the final, the fee for playing the match is 400 Zdzislaws.
12. Penalty Kick: This time you fouled Lewandowski in the penalty area, and you pay Szczesny for saving the penalty, 180 Zdzislaws.

You should provide an implementation of the `WorldCup2022` class that inherits from the `WorldCup` class defined in the `worldcup.h` header, available in the repository in the task directory.

Hints:
* Design the necessary classes along with their hierarchies. Where appropriate, separate classes representing interfaces.
* There should be classes representing at least the field, board, and player.
* The design should consider the possibility of adding new fields to the board or changing the arrangement of fields.
* Design with loose coupling and high cohesion.
* Principles that help achieve loose coupling and high cohesion include GRASP patterns, SOLID principles, and GoF design patterns.
* Explain the design assumptions in comments.