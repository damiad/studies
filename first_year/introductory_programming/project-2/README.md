# WildStrawberry

## Requirements
- Open: Monday, November 22, 2021, 08:30
- Due: Tuesday, December 7, 2021, 20:00

## Description
WildStrawberry is a two-player combinatorial game played on a rectangular board divided into rows and columns.

The game is parameterized by three positive integers:

- FIELDS: the number of board spaces occupied by a player in one move.
- ROWS: the number of rows on the board.
- COLUMNS: the number of columns on the board.

When FIELDS is equal to 2, the game is known as Domineering, and when FIELDS is equal to 3, it is known as Triomineering.

The players, referred to as Left and Right, sit at adjacent sides of the board, not opposite each other. The constants ROWS and COLUMNS determine the size of the board from the perspective of the Right player. The columns are labeled with uppercase letters starting from 'A', and the rows are labeled with lowercase letters starting from 'a'.

The players take turns making moves, starting with Left. Left can pass the right to make the first move to Right.

A player's move consists of placing a block on the board, occupying a set of contiguous free spaces. The block has a width of FIELDS, a height of 1, and is horizontal from the perspective of the player placing it and vertical for the opponent. When playing on paper, players draw a horizontal line across a specific number of free spaces.

If a player cannot make a move when it is their turn or decides not to continue the game, they surrender. The opponent of a player who surrenders wins, even if they cannot make a move either.

## Board Evaluation
The winner of the game is determined at the end, but sometimes during the gameplay, we want to determine the current score. This is called the board evaluation.

The board evaluation from a player's perspective is the difference between the number of blocks they could currently place on the board and the number of blocks their opponent could place on the board at once.


## Task
Write a program that plays as the Right player in the game WildStrawberry with the parameters FIELDS, ROWS, and COLUMNS.

The program starts by optionally receiving the right to make the first move from Left. After that, the program reads the recorded moves of Left and responds as the Right player by writing its moves. Right surrenders when they cannot make any moves. The program ends when one of the players surrenders.

Among all possible moves, the program selects the one that maximizes the board evaluation for the Right player.
