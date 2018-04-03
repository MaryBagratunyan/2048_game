#include "2048_game.h"

int Memento::log(int n)
{
	if (n == 0)
		return 0;

	int i = 0;
	while (n >> i)
	{
		++i;
	}

	return i - 1;
}

// store log2(n) in cell, instead of n
BoardMemento Memento::zip(const Board& b)
{
	BoardMemento m = { 0 };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = log(b[i][j]);
		}
	}

	return m;
}

Board Memento::unzip(const BoardMemento& m)
{
	Board b = { 0 };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (m[i][j] == 0)
				b[i][j] = 0;
			else
				b[i][j] = (1 << m[i][j]);
		}
	}

	return b;
}

ScoreMemento Memento::zip_score(int m)
{
	ScoreMemento sm = log(m);
	return sm;
}

int Memento::unzip_score(ScoreMemento sm)
{
	return (1 << sm);
}

Cell Game::get_random_cell()
{
	int index = rand() % free_cells.size();
	Cell c = free_cells[index];
	free_cells.erase(free_cells.begin() + index);
	return c;
}

void Game::add_random_number()
{
	Cell c = get_random_cell();

	int p = rand() % 4;
	if(p == 0)
		b[c.first][c.second] = 4;
	else
		b[c.first][c.second] = 2;
}

void Game::update_free_cells()
{
	free_cells.clear();
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (b[i][j] == 0)
				free_cells.push_back(std::make_pair(i, j));
}

Game::Game() : score(0), game_over(false), full_board(false)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			b[i][j] = 0;
			free_cells.push_back(std::make_pair(i, j));
		}
	}

	Cell c1 = get_random_cell();
	Cell c2 = get_random_cell();
	b[c1.first][c1.second] = 2;
	b[c2.first][c2.second] = 2;

	history.push_back(std::make_pair(bm.zip(b), bm.zip_score(score)));
	index = 0;
}

void Game::push_zeros_to_front(std::vector<int>& v)
{
	std::vector<int> copy = v;
	copy.erase(std::remove(copy.begin(), copy.end(), 0), copy.end());

	std::fill(v.begin(), v.end(), 0);

	int d = v.size() - copy.size();
	for (int i = 0; i < copy.size(); ++i)
		v[i + d] = copy[i];
}

void Game::push__row_by_dir(std::vector<int>& v)
{
	push_zeros_to_front(v);

	for (int i = v.size() - 1; i > 0; --i)
	{
		if (v[i] == v[i - 1])
		{
			v[i] = 2 * v[i];
			v[i - 1] = 0;
			score += v[i];
		}
	}

	push_zeros_to_front(v);
}

void Game::push_board(std::string dir)
{
	if (dir == "R")
	{
		for (int i = 0; i < 4; ++i)
		{
			std::vector<int> v(4);
			for (int j = 0; j < 4; ++j)
			{
				v[j] = b[i][j];
			}

			push__row_by_dir(v);

			for (int j = 0; j < 4; ++j)
			{
				b[i][j] = v[j];
			}
		}
	}

	if (dir == "L")
	{
		for (int i = 0; i < 4; ++i)
		{
			std::vector<int> v(4);
			for (int j = 3; j >= 0; --j)
			{
				v[3 - j] = b[i][j];
			}

			push__row_by_dir(v);

			for (int j = 3; j >= 0; --j)
			{
				b[i][j] = v[3 - j];
			}
		}
	}

	if (dir == "U")
	{
		for (int i = 0; i < 4; ++i)
		{
			std::vector<int> v(4);
			for (int j = 3; j >= 0; --j)
			{
				v[3 - j] = b[j][i];
			}

			push__row_by_dir(v);

			for (int j = 3; j >= 0; --j)
			{
				b[j][i] = v[3 - j];
			}
		}
	}

	if (dir == "D")
	{
		for (int i = 0; i < 4; ++i)
		{
			std::vector<int> v(4);
			for (int j = 0; j < 4; ++j)
			{
				v[j] = b[j][i];
			}

			push__row_by_dir(v);

			for (int j = 0; j < 4; ++j)
			{
				b[j][i] = v[j];
			}
		}
	}

	update_free_cells();
}

void Game::move(std::string dir)
{
	push_board(dir);

	if(free_cells.size() != 0)
		add_random_number();
	else if (full_board)
	{
		if (std::find(possible_ways.begin(), possible_ways.end(), dir)
			== possible_ways.end())
		{
			game_over = true;
			return;
		}
	}
	else
	{
		Board init = b;
		std::array<std::string, 4> dirs = { "R", "L", "D", "U" };
		for (int i = 0; i < 4; ++i)
		{
			push_board(dirs[i]);
			if (free_cells.size() != 0)
				possible_ways.push_back(dirs[i]);
			b = init;
		}

		if (possible_ways.size() == 0)
		{
			game_over = true;
			return;
		}
	}

	++index;
	history.erase(history.begin() + index, history.end());
	history.push_back(std::make_pair(bm.zip(b), bm.zip_score(score)));
}

void Game::print()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << b[i][j];
			std::cout << ' ';
		}
		if (i == 0)
		{
			std::cout << "    score : " << score;
		}
		std::cout << std::endl;
	}
}

bool Game::legal_cmd(std::string s)
{
	return s == "R" || s == "L" || s == "D" || s == "U";
}

void Game::undo()
{
	if (index <= 0)
		std::cout << "Unable to undo\n";
	else
	{
		b = bm.unzip(history[--index].first);
		score = bm.unzip_score(history[index].second);
		print();
	}
}

void Game::redo()
{
	if (index >= history.size() - 1)
		std::cout << "Unable to redo\n";
	else
	{
		b = bm.unzip(history[++index].first);
		score = bm.unzip_score(history[index].second);
		print();
	}
}

void Game::start()
{
	print();

	while (1)
	{
		std::string cmd;
		std::cin >> cmd;

		if (game_over && cmd != "Undo")
		{
			break;
		}

		if (cmd == "Undo")
		{
			undo();
		}
		else if (cmd == "Redo")
		{
			redo();
		}
		else if (cmd == "END")
		{
			break;
		}
		else if (legal_cmd(cmd))
		{
			move(cmd);
			print();
		}
		else
			std::cout << "Try again\n";

		if (game_over)
		{
			std::cout << "Game Over\n";
		}
	}
}
