#pragma once
#include <stack>
#include "../scramble/Scramble.h"

class Solver
{
public:
    Scramble* scramble;
    Puzzle puzzle;
    std::stack<int> soln;
    uint64_t nodes;
    int numSolutions;
    std::string printableMoveNames[6] = { "U", "L", "F", "R", "B", "D" };
    std::string moveNames[6] = { "U", "L", "F", "R", "B", "D" };

    Solver(Puzzle puzzle);
    //set up functions
    void make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint32_t Puzzle::* coord_1, uint32_t Puzzle::* coord_2);
    void make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint64_t Puzzle::* coord_1, uint32_t Puzzle::* coord_2, uint64_t* moveTable);
    void loadScrambles(Scramble* s);
    void loadPruningTables();

    //solving functions
	void solve();
    bool prune(int);
    bool checkSolved();
    bool search(int depth);
    //overload for consistency in calling recursively
    bool search(int depth, int searched);

};

