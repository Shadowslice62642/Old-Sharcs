#include "puzzle.h"

void Puzzle::reset() {
    corners_p[0] = 1;
    corners_p[1] = 2;
    corners_p[2] = 3;
    corners_p[3] = 4;
    corners_p[4] = 5;
    corners_p[5] = 6;
    corners_p[6] = 7;
    corners_p[7] = 8;
    corners_o[0] = 0;
    corners_o[1] = 0;
    corners_o[2] = 0;
    corners_o[3] = 0;
    corners_o[4] = 0;
    corners_o[5] = 0;
    corners_o[6] = 0;
    corners_o[7] = 0;
    edges_p[0] = 1;
    edges_p[1] = 2;
    edges_p[2] = 3;
    edges_p[3] = 4;
    edges_p[4] = 5;
    edges_p[5] = 6;
    edges_p[6] = 7;
    edges_p[7] = 7;
    edges_p[8] = 7;
    edges_p[9] = 7;
    edges_p[10] = 7;
    edges_p[11] = 7;
    edges_o[0] = 0;
    edges_o[1] = 0;
    edges_o[2] = 0;
    edges_o[3] = 0;
    edges_o[4] = 0;
    edges_o[5] = 0;
    edges_o[6] = 0;
    edges_o[7] = 0;
    edges_o[8] = 0;
    edges_o[9] = 0;
    edges_o[10] = 0;
    edges_o[11] = 0;
    edges_2_p[0] = 6;
    edges_2_p[1] = 6;
    edges_2_p[2] = 6;
    edges_2_p[3] = 6;
    edges_2_p[4] = 6;
    edges_2_p[5] = 6;
    edges_2_p[6] = 7;
    edges_2_p[7] = 8;
    edges_2_p[8] = 9;
    edges_2_p[9] = 10;
    edges_2_p[10] = 11;
    edges_2_p[11] = 12;
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
    uint64_t ep_mask = (1 << 20) - 1;
    cp = cpTable[cp * 6 + i];
    ep = epTable[ep * 2 + i / 3];
    ep >>= (20 * (i % 3));
    ep &= ep_mask;
    ep2 = ep2Table[ep2 * 2 + i / 3];
    ep2 >>= (20 * (i % 3));
    ep2 &= ep_mask;
    co = coTable[co * 6 + i];
    //std::cout << eo << std::endl;
    eo = eoTable[eo * 6 + i];
}