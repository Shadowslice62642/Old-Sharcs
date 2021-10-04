// SHARCS_0.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <chrono>
#include <random>

uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 mt(seed);
std::uniform_int_distribution<> dist(0, 17);

#include "puzzle/puzzle.h"
#include "./scramble/scramble.h" 
#include "./solver/solver.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Loading move tables\n" << std::flush;
        Puzzle hi = Puzzle();
        std::cout << "Loading pruning tables\n" << std::flush;
        Solver s = Solver(hi);
        while (true) {
            std::cout << "Enter name of scramble file: " << std::flush;
            std::string filename;
            std::getline(std::cin, filename);
            std::cin.clear();
            if (filename == "") continue;
            std::cout << "Reading scramble file " << filename << "\n" << std::flush;
            std::vector<Scramble*> scrambles = readScrambleFile((char*)filename.c_str());
            std::cout << "Read " << scrambles.size() << ((scrambles.size() == 1) ? " scramble\n" : " scrambles\n") << std::flush;
            for (int i = 0; i < scrambles.size(); i++) {
                std::cout << "Solving scramble " << i + 1 << ": " << scrambles[i]->name << "\n" << std::flush;
                s.loadScrambles(scrambles[i]);
                s.solve();
            }
            std::cout << "\n" << std::flush;
        }
    }
    else if (argc == 2) {
        Puzzle hi = Puzzle();
        std::string str(argv[1]);
        std::cout << "Loading pruning tables\n" << std::flush;
        Solver s = Solver(hi);
        std::cout << "Reading scramble file " << str << "\n" << std::flush;
        std::vector<Scramble*> scrambles = readScrambleFile(argv[1]);
        std::cout << "Read " << scrambles.size() << ((scrambles.size() == 1) ? " scramble\n" : " scrambles\n") << std::flush;
        for (int i = 0; i < scrambles.size(); i++) {
            std::cout << "Solving scramble " << i + 1 << ": " << scrambles[i]->name << "\n" << std::flush;
            s.loadScrambles(scrambles[i]);
            s.solve();
        }
    }
    return 0;
}
