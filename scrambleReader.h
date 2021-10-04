#pragma once
#include "scramble.h"
#include "puzzle.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::string printableMoveNames[18] = { "U", "U2", "U'", "L", "L2", "L'", "F", "F2", "F'", "R", "R2", "R'", "B", "B2", "B'", "D", "D2", "D'" };
std::string moveNames[18] = { "U", "U2", "U'", "L", "L2", "L'", "F", "F2", "F'", "R", "R2", "R'", "B", "B2", "B'", "D", "D2", "D'" };

Puzzle* readScramble(std::string str) {
    Puzzle* p = new Puzzle();
    std::stringstream stream(str);
    std::string line, setName;
    bool permutation = true;

    while (std::getline(stream, line)) {
        std::stringstream tmp(line);
        if ('0' <= line[0] && line[0] <= '9') {
            std::vector<int> currentSet;
            int num;

            while (tmp >> num) {
                currentSet.push_back(num);
            }

            if (permutation) {
                if (setName == "corners")  p->set_pieces<8>(currentSet, p->corners_p);
                if (setName == "edges") p->set_pieces<12>(currentSet, p->edges_p);
                if (setName == "edges_2") p->set_pieces<12>(currentSet, p->edges_2_p);
            }
            else {
                if (setName == "corners") p->set_pieces<8>(currentSet,p->corners_o);
                if (setName == "edges") p->set_pieces<12>(currentSet,p->edges_o);
            }

            permutation = false;
        }
        else if (line == "Moves") {
            std::getline(stream, line);
            std::stringstream moves(line);
            std::string move;
            while (moves >> move) {
                for (int i = 0; i < 18; i++) {
                    if (moveNames[i] == move) {
                        ;
                        p->doMove(i);
                    }
                }
            }
        }
        else {
            std::getline(tmp, setName);
            permutation = true;
        }
    }

    return p;
}

std::vector<Scramble*> readScrambleFile(char* fname) {
    std::vector<Scramble*> scrambles;
    std::ifstream f(fname);
    if (!f.good()) {
        std::cout << "Failed to open scramble file\n" << std::flush;
        return scrambles;
    }

    std::string cmd;
    std::string name;
    int startDepth = 1, maxDepth = 1000, slack = 0, maxSolutions = 2147483647;

    while (!f.eof()) {
        f >> cmd;
        if (cmd == "Scramble") {
            std::getline(f, name);

            std::string scr = "", line;
            while (std::getline(f, line)) {
                if (line == "End") break;
                scr += line + "\n";
            }

            Scramble* s = new Scramble();
            s->name = name;
            s->state = readScramble(scr);
            s->startDepth = startDepth;
            s->maxDepth = maxDepth;
            s->slack = slack;
            s->maxSolutions = maxSolutions;

            scrambles.push_back(s);

            startDepth = 1;
            maxDepth = 1000;
            slack = 0;
            maxSolutions = 2147483647;
        }
       
        else if (cmd == "StartDepth") {
            f >> startDepth;
        }
        else if (cmd == "MaxDepth") {
            f >> maxDepth;
        }
        else if (cmd == "Slack") {
            f >> slack;
        }
        else if (cmd == "MaxSolutions") {
            f >> maxSolutions;
        }
    }

    return scrambles;
}