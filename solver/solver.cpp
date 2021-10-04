#include "solver.h"
#include <ctime>
#include <stack>
#include "puzzle.h"
#include "Scramble.h"

Solver::Solver(Puzzle p) {
    puzzle = p;
    loadPruningTables();
}

void Solver::loadScrambles(Scramble* s) {
    scramble = s;
    puzzle = Puzzle(*(s->state));
}

void Solver::solve() {
    nodes = 0;
    numSolutions = 0;
    bool solved = false;
    int slackCounter = 0;

    time_t t = clock();
    for (int i = scramble->startDepth; i <= scramble->maxDepth; i++) {
        if (solved && slackCounter == scramble->slack) break;
        if (solved) slackCounter++;
        std::cout << "Depth " << i << "\n" << std::flush;
        if (search(i)) solved = true;
        std::cout << "  " << nodes << " nodes visited, " << (float)(clock() - t) / CLOCKS_PER_SEC << "s\n" << std::flush;
    }
}

bool Solver::search(int depth) {
    if (prune(depth)) {
        return false;
    }

    bool s = false;
    //loop through all 6 defined moves
    for (int i = 0; i < 6; i++) {
        //apply each move 3 times in a loop to check and once to reset to original
        for (int j = 0; j < 3; j++) {
            puzzle.coord_move(i);
            soln.push(3 * i + j);
            nodes++;
        }
        //call next level of search. if true, then return the moves
        if (search(depth - 1, i)) s = true;
        soln.pop();
        puzzle.coord_move(i);
    }
    return s;
}

//overload for consistency in recursion, skips currently inverstigated move
bool Solver::search(int depth, int searched) {
    bool s = false;
    if (numSolutions == scramble->maxSolutions) return true;
    if (depth == 0) return checkSolved();
    if (prune(depth)) return false;
    //loop through all 6 defined moves
    for (int i = 0; i < 6; i++) {
        //skip already searched move
        if (i == searched) continue;
        //apply each move 3 times in a loop to check and once to reset to original
        for (int j = 0; j < 3; j++) {
            puzzle.coord_move(i);
            soln.push(3 * i + j);
            nodes++;
        }
        //call next level of search. if true, then return the moves
        if (search(depth - 1, i)) s = true; // L
        soln.pop();
        puzzle.coord_move(i);
    }
    return s;
}

bool Solver::checkSolved() {
    if (puzzle.solved()) {
        std::stack<int> soln2 = soln;
        std::string solution = printableMoveNames[soln2.top()];
        soln2.pop();
        while (!soln2.empty()) {
            solution = printableMoveNames[soln2.top()] + " " + solution;
            soln2.pop();
        }
        std::cout << "  " << solution << "\n" << std::flush;
        numSolutions++;
        return true;
    }
    else return false;
}