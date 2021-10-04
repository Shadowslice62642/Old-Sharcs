#pragma once
#include <stack>
#include "Scramble.h"

class Solver
{
public:
    Scramble* scramble;
    Puzzle puzzle;
    std::stack<int> soln;
    uint64_t nodes;
    int numSolutions;
    std::string printableMoveNames[18] = { "U", "U2", "U'", "L", "L2", "L'", "F", "F2", "F'", "R", "R2", "R'", "B", "B2", "B'", "D", "D2", "D'" };
    std::string moveNames[18] = { "U", "U2", "U'", "L", "L2", "L'", "F", "F2", "F'", "R", "R2", "R'", "B", "B2", "B'", "D", "D2", "D'" };

    Solver(Puzzle puzzle);
	void loadScrambles(Scramble* s);
	void solve();
	bool search(int depth);
    bool prune(int);
    bool checkSolved();
    void make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint32_t Puzzle::* coord_1, uint32_t Puzzle::* coord_2);
    void make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint64_t Puzzle::* coord_1, uint32_t Puzzle::* coord_2, uint64_t* moveTable);
    void loadPruningTables();

    bool search_child(int depth, int searched) {
        bool s = false;
        if (numSolutions == scramble->maxSolutions) return true;
        if (depth == 0) return checkSolved();
        if (prune(depth)) return false;
        for (int i = 0; i < 6; i++) {
            if (i == searched) continue;
            for (int j = 0; j < 3; j++) {
                puzzle.coord_move(i);
                soln.push(3 * i + j);
                nodes++;
            }
            if (search_child(depth - 1, i)) s = true; // L
            soln.pop();
            puzzle.coord_move(i);
        }
        return s;
    }
};

