#pragma once
#include <iostream>
#include "../moves/perm_move.h"
#include "../moves/ori_move.h"
#include <array>
#include <vector>

typedef uint32_t (*edges_encoding_func)(std::array<char, 12>& pieces);
typedef uint32_t (*corners_encoding_func)(std::array<char, 8>& pieces);
typedef uint32_t (*edges_decoding_func)(std::array<char, 12>& pieces);
typedef uint32_t (*corners_decoding_func)(std::array<char, 8>& pieces);

class Puzzle {
public:
    //coordinate move tables
    uint64_t* epTable = new uint64_t[1330560];
    uint64_t* ep2Table = new uint64_t[1330560];
    uint32_t* cpTable = new uint32_t[241920];
    uint32_t* eoTable = new uint32_t[12288];
    uint32_t* coTable = new uint32_t[13122];

    //cubie piece arrays
    std::array<char, 12> edges_p;
    std::array<char, 12> edges_o;
    std::array<char, 12> edges_2_p;
    std::array<char, 8> corners_o;
    std::array<char, 8> corners_p;

    //piece coordinates
    uint64_t ep;
    uint64_t ep2;
    uint32_t eo;
    uint32_t cp;
    uint32_t co;

    //cubie moves
    perm_move<12> epMoves[6];
    perm_move<8> cpMoves[6];
    ori_move<12, 2> eoMoves[6];
    ori_move<8, 3> coMoves[6];

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
    template<class move_type, int size>
    void make_move_table(move_type doMove[6], uint32_t(*encode)(std::array<char, size>), std::array<char, size>(*decode)(uint32_t), uint32_t max_coord, uint32_t* move_table) {
        std::array<char, size> placeholder;
        for (uint32_t i = 0; i < max_coord; i++) {
            for (int j = 0; j < 6; j++) {
                placeholder = decode(i);
                doMove[j](placeholder);
                move_table[i * 6 + j] = encode(placeholder);
            }
        }
    }

    //utilities (utilities.cpp)
    template<char piece_count>
    void set_pieces(std::vector<int> v, std::array<char, piece_count>& pieces) {
        std::copy(v.begin(), v.end(), pieces.begin());
    }
    void print();
    void doMove(int);


    //encoding (encoding.cpp)
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

    //same as above but with O(pieces) implementation
    template<char piece_count>
    uint32_t encode_p_bitshift(std::array<char, piece_count> pieces) {
        uint32_t t = 0;
        uint32_t seen = (1 << pieces) - 2; //set all pieces to seen
        for (int i = 0; i < piece_count - 1; i++) {
            seen -= 1 << pieces[i]; // mark current piece as unseen
            t *= piece_count - i; // break up factorial
            t += _popcnt64(seen >> pieces[i]);
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
};