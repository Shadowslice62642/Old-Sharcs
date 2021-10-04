#pragma warning(disable : 4996)
#include "puzzle.h"
#include <array>
#include <stdio.h>

Puzzle::Puzzle() {
    reset();
    loadStandardMoves();
    loadMoveTables();
}

void Puzzle::loadStandardMoves() {
    //cpMoves
    cpMoves[0] = perm_move<8>(0, 3, 2, 1); // U
    cpMoves[1] = perm_move<8>(4, 7, 6, 5); // D
    cpMoves[2] = perm_move<8>(2, 3, 6, 7); // R
    cpMoves[3] = perm_move<8>(0, 1, 4, 5); // L
    cpMoves[4] = perm_move<8>(0, 5, 6, 3); // F
    cpMoves[5] = perm_move<8>(1, 2, 7, 4); // B
    //epMoves
    epMoves[0] = perm_move<12>(0, 3, 2, 1); // U
    epMoves[1] = perm_move<12>(8, 11, 10, 9); // D
    epMoves[2] = perm_move<12>(3, 6, 11, 7); // R
    epMoves[3] = perm_move<12>(1, 4, 9, 5); // L
    epMoves[4] = perm_move<12>(0, 5, 10, 6); // F
    epMoves[5] = perm_move<12>(2, 7, 8, 4); // B
    //coMoves
    coMoves[0] = ori_move<8, 3>(0, 3, 2, 1, 0, 0, 0, 0); // U
    coMoves[1] = ori_move<8, 3>(4, 7, 6, 5, 0, 0, 0, 0); // D
    coMoves[2] = ori_move<8, 3>(2, 3, 6, 7, 1, 2, 1, 2); // R
    coMoves[3] = ori_move<8, 3>(0, 1, 4, 5, 1, 2, 1, 2); // L
    coMoves[4] = ori_move<8, 3>(0, 5, 6, 3, 2, 1, 2, 1); // F
    coMoves[5] = ori_move<8, 3>(1, 2, 7, 4, 1, 2, 1, 2); // B
    //eoMoves
    eoMoves[0] = ori_move<12, 2>(0, 3, 2, 1, 0, 0, 0, 0); // U
    eoMoves[1] = ori_move<12, 2>(8, 11, 10, 9, 0, 0, 0, 0); // D
    eoMoves[2] = ori_move<12, 2>(3, 6, 11, 7, 0, 0, 0, 0); // R
    eoMoves[3] = ori_move<12, 2>(1, 4, 9, 5, 0, 0, 0, 0); // L
    eoMoves[4] = ori_move<12, 2>(0, 5, 10, 6, 1, 1, 1, 1); // F
    eoMoves[5] = ori_move<12, 2>(2, 7, 8, 4, 1, 1, 1, 1); // B
}

void Puzzle::make_eptable(perm_move<12> doMove[6]) {
    for (uint32_t i = 0; i < 665280; i++) {
        for (int j = 0; j < 6; j++) {
            //get current cubie configuration for update
            edges_p = decode_edges_p(i);
            doMove[j](edges_p);
            //edges_p coords are 20 bits long so 3 can be packed into one 64-bit entry.
            //time loss from extraction appears to be outweighed by cache friendliness and sequential calling of new coordinates by pruning tables and solver
            epTable[i * 2 + j / 3] <<= 20;
            epTable[i * 2 + j / 3] += encode_edges_p(edges_p);
        }
    }
    //save move table for future sessions.
    FILE* f = fopen("ep.table", "wb");
    fwrite(epTable, 8, 1330560, f);
    fclose(f);
}

void Puzzle::make_ep2table(perm_move<12> doMove[6]) {
    for (uint32_t i = 0; i < 665280; i++) {
        for (int j = 0; j < 6; j++) {
            //get current cubie configuration for update
            edges_2_p = decode_edges_2_p(i);
            doMove[j](edges_2_p);
            //edges_2_p coords are 20 bits long so 3 can be packed into one 64-bit entry.
            //time loss from extraction appears to be outweighed by cache friendliness and sequential calling of new coordinates by pruning tables and solver
            ep2Table[i * 2 + j / 3] <<= 20;
            ep2Table[i * 2 + j / 3] += encode_edges_2_p(edges_2_p);
        }
    }
    //save move table for future sessions.
    FILE* f = fopen("ep2.table", "wb");
    fwrite(ep2Table, 8, 1330560, f);
    fclose(f);
}

//checks for pregenerated move tables. If not found, then generate.
void Puzzle::loadMoveTables() {
    FILE* f;
    f = fopen("ep.table", "rb");
    if (f == NULL) {
        make_eptable(epMoves);
    }
    else {
        fread(epTable, 8, 1130560, f);
        fclose(f);
    }
    f = fopen("ep2.table", "rb");
    if (f == NULL) {
        make_ep2table(epMoves);
    }
    else {
        fread(ep2Table, 8, 1130560, f);
        fclose(f);
    }
    f = fopen("eo.table", "rb");
    if (f == NULL) {
        make_eotable(eoMoves);
    }
    else {
        fread(eoTable, 4, 12288, f);
        fclose(f);
    }
    f = fopen("cp.table", "rb");
    if (f == NULL) {
        make_cptable(cpMoves);
    }
    else {
        fread(cpTable, 4, 241920, f);
        fclose(f);
    }
    f = fopen("co.table", "rb");
    if (f == NULL) {
        make_cotable(coMoves);
    }
    else {
        fread(coTable, 4, 13122, f);
        fclose(f);
    }
}

void Puzzle::make_eotable(ori_move<12, 2> doMove[6]) {
    for (uint32_t i = 0; i < 2048; i++) {
        for (int j = 0; j < 6; j++) {
            edges_o = decode_o<12, 2>(i);
            doMove[j](edges_o);
            eoTable[i * 6 + j] = encode_o<12, 2>(edges_o);
        }
    }
    FILE* f = fopen("eo.table", "wb");
    fwrite(eoTable, 4, 12288, f);
    fclose(f);
}

void Puzzle::make_cptable(perm_move<8> doMove[6]) {
    for (uint32_t i = 0; i < 40320; i++) {
        for (int j = 0; j < 6; j++) {
            corners_p = decode_p<8>(i);
            doMove[j](corners_p);
            cpTable[6 * i + j] = encode_p<8>(corners_p);
        }
    }
    FILE* f = fopen("cp.table", "wb");
    fwrite(cpTable, 4, 241920, f);
    fclose(f);
}

void Puzzle::make_cotable(ori_move<8, 3> doMove[6]) {
    for (uint32_t i = 0; i < 2187; i++) {
        for (int j = 0; j < 6; j++) {
            decode_o<8, 3>(i);
            doMove[j](corners_o);
            coTable[6 * i + j] = encode_o<8, 3>(corners_o);
        }
    }
    FILE* f = fopen("co.table", "wb");
    fwrite(coTable, 4, 13122, f);
    fclose(f);
}