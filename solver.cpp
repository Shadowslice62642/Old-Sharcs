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
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            puzzle.coord_move(i);
            soln.push(3 * i + j);
            nodes++;
        }
        if (search_child(depth - 1, i)) s = true;
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