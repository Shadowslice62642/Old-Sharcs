#include "Solver.h"

#pragma warning(disable : 4996)

#include <vector>
#include <map>
#include <fstream>
#include <string.h>
#include <algorithm>
#include "puzzle.h"

char * edges_p_edges_o_table = new char[1362493440];
char * edges_2_p_edges_o_table = new char[1362493440];
char * corners_p_corners_o_table = new char[88179840];

//implements bfs to generate pruning table for heuristic
//overloaded on types of coordinates for consistency in generation
void Solver::make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint32_t Puzzle::*coord_1, uint32_t Puzzle::*coord_2) {
    char depth = 0;
    memset(prune_table, -1, pos_count);
    time_t t = clock();
    prune_table[puzzle.*coord_1 + puzzle.*coord_2 * coord_1_count] = 0;
    uint64_t newPositions = 1, totalPositions = 1;
    while (newPositions != 0) {
        newPositions = 0;
        for (uint64_t pos = 0; pos < pos_count; pos++) {
            if (prune_table[pos] != depth) continue;
            for (int i = 0; i < 6; i++) {
                //break combined coordinate into constituent parts
                puzzle.*coord_1 = pos % coord_1_count;
                puzzle.*coord_2 = (uint32_t)pos / coord_1_count;
                for (int j = 0; j < 3; j++) {
                    //get coordinate from non-packed array
                    puzzle.*coord_2 = puzzle.cpTable[6 * puzzle.*coord_2 + i];
                    puzzle.*coord_1 = puzzle.coTable[6 * puzzle.*coord_1 + i];
                    if (prune_table[puzzle.*coord_1 + puzzle.*coord_2 * coord_1_count] == -1) {
                        prune_table[puzzle.*coord_1 + puzzle.*coord_2 * coord_1_count] = depth + 1;
                        newPositions++;
                    }
                }
                puzzle.coord_move(i);
            }
        }
        depth++;
        totalPositions += newPositions;
        std::cout << (int)depth << "\t" << newPositions << "\t" << totalPositions << "\n" << std::flush;
    }

    std::cout << "\nTable generation finished (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << "s)\n" << std::flush;

    t = clock();
    std::cout << "Table saved (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << "s)\n\n" << std::flush;
}

//implements bfs to generate pruning table for heuristic
//overloaded on types of coordinates for consistency in generation
void Solver::make_pruning_table(char* prune_table, uint64_t pos_count, uint64_t coord_1_count, uint64_t Puzzle::* coord_1, uint32_t Puzzle::* coord_2, uint64_t* moveTable) {
    char depth = 0;
    uint64_t ep_mask = (1 << 20) - 1;
    memset(prune_table, -1, pos_count);
    time_t t = clock();
    prune_table[0] = 0;
    uint64_t newPositions = 1, totalPositions = 1;
    while (totalPositions != pos_count) {
        newPositions = 0;
        for (uint64_t pos = 0; pos < pos_count; pos++) {
            if (prune_table[pos] != depth) continue;
            for (int i = 0; i < 6; i++) {
                //break combined coordinate into constituent parts
                puzzle.*coord_1 = pos % coord_1_count;
                puzzle.*coord_2 = (uint32_t)pos / coord_1_count;
                for (int j = 0; j < 3; j++) {
                    //extract new coordinate from packed array
                    puzzle.*coord_1 = moveTable[puzzle.*coord_1 * 2 + i / 3];
                    puzzle.*coord_1 >>= (20 * (i % 3));
                    puzzle.*coord_1 &= ep_mask;
                    //get coordinate from non-packed array
                    puzzle.*coord_2 = puzzle.eoTable[6 * puzzle.*coord_2 + i];
                    if (prune_table[puzzle.*coord_1 + puzzle.*coord_2 * coord_1_count] == -1) {
                        prune_table[puzzle.*coord_1 + puzzle.*coord_2 * coord_1_count] = depth + 1;
                        newPositions++;
                    }
                }
                //same as in loop but it restores to original so no point looking up
                puzzle.*coord_1 = moveTable[puzzle.*coord_1 * 2 + i / 3];
                puzzle.*coord_1 >>= (20 * (i % 3));
                puzzle.*coord_1 &= ep_mask;
                puzzle.*coord_2 = puzzle.eoTable[6 * puzzle.*coord_2 + i];
            }
        }
        depth++;
        totalPositions += newPositions;
        std::cout << (int)depth << "\t" << newPositions << "\t" << totalPositions << "\n" << std::flush;
    }

    std::cout << "\nTable generation finished (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << "s)\n" << std::flush;

    t = clock();
    std::cout << "Table saved (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << "s)\n\n" << std::flush;
}

//checks if there is a saved pruning table, if not, generate.
void Solver::loadPruningTables() {
    FILE* f;
    f = fopen("edges_p_edges_o.table", "rb");
    if (f == NULL) {
        std::cout << "Generating complete pruning table \"edges_p_edges_o_table\"\n" << std::flush;
        std::cout << "Depth\tNew\tTotal\n0\t1\t1\n" << std::flush;
        make_pruning_table(edges_p_edges_o_table, 1362493440, 665280, &Puzzle::ep, &Puzzle::eo, puzzle.epTable);
        f = fopen("edges_p_edges_o.table", "wb");
        fwrite(edges_p_edges_o_table, 1, 1362493440, f);
        fclose(f);
    }
    else {
        fread(edges_p_edges_o_table, 1, 1362493440, f);
        fclose(f);
    }
    f = fopen("edges_2_p_edges_o.table", "rb");
    if (f == NULL) {
        std::cout << "Generating complete pruning table \"edges_2_p_edges_o_table\"\n" << std::flush;
        std::cout << "Depth\tNew\tTotal\n0\t1\t1\n" << std::flush;
        make_pruning_table(edges_2_p_edges_o_table, 1362493440, 665280, &Puzzle::ep2, &Puzzle::eo, puzzle.ep2Table);
        f = fopen("edges_2_p_edges_o.table", "wb");
        fwrite(edges_2_p_edges_o_table, 1, 1362493440, f);
        fclose(f);
    }
    else {
        fread(edges_2_p_edges_o_table, 1, 1362493440, f);
        fclose(f);
    }
    f = fopen("corners_p_corners_o.table", "rb");
    if (f == NULL) {
        std::cout << "Generating complete pruning table \"corners_p_corners_o_table\"\n" << std::flush;
        std::cout << "Depth\tNew\tTotal\n0\t1\t1\n" << std::flush;
        make_pruning_table(corners_p_corners_o_table, 88179840, 40320, &Puzzle::cp, &Puzzle::co);
        f = fopen("corners_p_corners_o.table", "wb");
        fwrite(corners_p_corners_o_table, 1, 88179840, f);
        fclose(f);
    }
    else {
        fread(corners_p_corners_o_table, 1, 88179840, f);
        fclose(f);
    }
}

//calculates heuristic value by max(depth)+curr depth and returns if it is higher than the current prune setting
bool Solver::prune(int depth) {
    if (edges_p_edges_o_table[puzzle.ep + puzzle.eo * 665280] > depth) return true;
    if (edges_2_p_edges_o_table[puzzle.ep2 + puzzle.eo * 665280] > depth) return true;
    if (corners_p_corners_o_table[puzzle.cp + puzzle.co * 40320] > depth) return true;
    return false;
}