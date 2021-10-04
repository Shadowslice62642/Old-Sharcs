#pragma once
#include <iostream>
#include "perm_move.h"
#include "ori_move.h"
#include <array>
#include <vector>

typedef uint32_t (*edges_encoding_func)(std::array<char, 12>& pieces);
typedef uint32_t (*corners_encoding_func)(std::array<char, 8>& pieces);
typedef uint32_t (*edges_decoding_func)(std::array<char, 12>& pieces);
typedef uint32_t (*corners_decoding_func)(std::array<char, 8>& pieces);

class Puzzle {
public:
    uint64_t* epTable = new uint64_t[1330560];
    uint64_t* ep2Table = new uint64_t[1330560];
    uint32_t* cpTable = new uint32_t[241920];
    uint32_t* eoTable = new uint32_t[12288];
    uint32_t* coTable = new uint32_t[13122];

    std::array<char, 12> edges_p;
    std::array<char, 12> edges_o;
    std::array<char, 12> edges_2_p;
    std::array<char, 8> corners_o;
    std::array<char, 8> corners_p;

    uint64_t ep;
    uint64_t ep2;
    uint32_t eo;
    uint32_t cp;
    uint32_t co;

    //setup functions
    //initialising (puzzle.cpp)
    Puzzle();
    void reset();
    bool solved() {
        return ep == 0 && ep2 == 0 && cp == 0 && eo == 0 && co == 0;
    }
    void loadStandardMoves();

    //move tables (puzzle.cpp)
    void loadMoveTables();
    void make_eptable(perm_move<12> doMove[6]);
    void make_ep2table(perm_move<12> doMove[6]);
    void make_cptable(perm_move<8> doMove[6]);
    void make_cotable(ori_move<8,3> doMove[6]);
    void make_eotable(ori_move<12,2> doMove[6]);

    //utilities
    template<char piece_count>
    void set_pieces(std::vector<int> v, std::array<char, piece_count>& pieces) {
        std::copy(v.begin(), v.end(), pieces.begin());
    }
    void print();
    void doMove(int);


    //encoding
    uint32_t encode_edges_p(std::array<char, 12> edges_p);
    uint32_t encode_edges_2_p(std::array<char, 12> edges_p);
    std::array<char, 12> decode_edges_p(uint32_t t);
    std::array<char, 12> decode_edges_2_p(uint32_t t);
    template<char piece_count>
    uint32_t encode_p(std::array<char, piece_count> pieces) {
        uint32_t t = 0;
        for (int i = 0; i < piece_count - 1; i++) {
            t *= piece_count - i;
            for (int j = i + 1; j < piece_count; j++) {
                if (pieces[i] > pieces[j]) t++;
            }
        }
        return t;
    }

    template<char piece_count>
    std::array<char, piece_count> decode_p(uint32_t t) {
        std::array<char, piece_count> pieces;
        pieces[piece_count - 1] = 1;
        for (int i = piece_count - 2; i >= 0; i--) {
            pieces[i] = 1 + t % (8 - i);
            t /= 8 - i;
            for (int j = i + 1; j < 8; j++) {
                if (pieces[j] >= pieces[i]) pieces[j]++;
            }
        }
        return pieces;
    }

    template<char piece_count, char ori_count>
    uint32_t encode_o(std::array<char, piece_count> pieces) {
        uint32_t t = 0;
        for (int i = 0; i < piece_count-1; i++) {
            t = ori_count * t + pieces[i];
        }
        return t;
    }

    template<char piece_count, char ori_count>
    std::array<char, piece_count> decode_o(uint32_t t) {
        std::array<char, piece_count> pieces;
        char s = 0;
        for (int i = piece_count - 2; i >= 0; i--) {
            pieces[i] = t % ori_count;
            s += ori_count - pieces[i];
            t /= ori_count;
        }
        pieces[piece_count-1] = s % ori_count;
        return pieces;
    }

    void coord_move(int);

private:

    perm_move<12> epMoves[6];
    perm_move<8> cpMoves[6];
    ori_move<12, 2> eoMoves[6];
    ori_move<8, 3> coMoves[6];

    edges_encoding_func ep_encode;
    corners_encoding_func cp_encode;
    edges_encoding_func eo_encode;
    corners_encoding_func co_encode;

    edges_decoding_func ep_decode;
    corners_decoding_func cp_decode;
    edges_decoding_func eo_decode;
    corners_decoding_func co_decode;
};