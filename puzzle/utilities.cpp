#include "puzzle.h"
#include <numeric>

void Puzzle::reset() {
    std::iota(corners_p.begin(), corners_p.end(), 1);
    edges_2_p = { 7, 7, 7, 7, 7, 7, 7, 8, 9, 10, 11, 12 };
    edges_p = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6 };
    edges_o = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    corners_o = { 0, 0, 0, 0, 0, 0, 0, 0 };
    ep = 0;
    ep2 = 0;
    eo = 0;
    cp = 0;
    co = 0;
} 

void Puzzle::print() {
    std::cout << "corners\n";
    for (int i = 0; i < 8; i++) {
        std::cout << (int)corners_p[i] << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < 8; i++) {
        std::cout << (int)corners_o[i] << " ";
    }
    std::cout << "\n";
    std::cout << "edges\n";
    for (int i = 0; i < 12; i++) {
        std::cout << (int)edges_p[i] << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < 12; i++) {
        std::cout << (int)edges_o[i] << " ";
    }
    std::cout << "\n";
    std::cout << "edges_2\n";
    for (int i = 0; i < 12; i++) {
        std::cout << (int)edges_2_p[i] << " ";
    }
    std::cout << "\n";
    std::cout << "\n" << std::flush;
}

void Puzzle::doMove(int i) {
    epMoves[i](edges_p);
    epMoves[i](edges_2_p);
    eoMoves[i](edges_o);
    cpMoves[i](corners_p);
    coMoves[i](corners_o);
}

void Puzzle::coord_move(int i) {
    //extract new coordinate from packed array
    uint64_t ep_mask = (1 << 20) - 1;
    ep = epTable[ep * 2 + i / 3];
    ep >>= (20 * (i % 3));
    ep &= ep_mask;
    ep2 = ep2Table[ep2 * 2 + i / 3];
    ep2 >>= (20 * (i % 3));
    ep2 &= ep_mask;
    //get new coordinates from non-packed arrays
    cp = cpTable[cp * 6 + i];
    co = coTable[co * 6 + i];
    eo = eoTable[eo * 6 + i];
}