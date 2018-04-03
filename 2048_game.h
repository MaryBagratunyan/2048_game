#pragma once

#include <array>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

typedef std::array<std::array<int, 4>, 4> Board;
typedef std::pair<int, int> Cell;
typedef std::array<std::array<char, 4>, 4> BoardMemento;
typedef char ScoreMemento;

/*
	Using Memento in order to store value of each cell in one byte instead of 4 bytes.
	This is efficient, because we are going to save the history of board conditions, in order to be able to undo and redo the game.
*/

class Memento
{
private:
	int log(int n);
public:
	BoardMemento zip(const Board& b);
	Board unzip(const BoardMemento& m);
	ScoreMemento zip_score(int m);
	int unzip_score(ScoreMemento sm);
};

class Game
{
private:
	Board b;
	int score;
	Memento bm;
	std::vector<std::pair<BoardMemento, ScoreMemento>> history;
	std::vector<Cell> free_cells;
	int index;
	bool game_over;
	bool full_board;
	std::vector<std::string> possible_ways;


private:
	void move(std::string dir);
	Cell get_random_cell();
	void add_random_number();
	void push_zeros_to_front(std::vector<int>& v);
	void push__row_by_dir(std::vector<int>& v);
	void push_board(std::string dir);
	void update_free_cells();
	bool legal_cmd(std::string s);
	void print();

	void undo();
	void redo();

public:
	Game();
	void start();
};