//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <regex>



char p1_board[BOARD_SIZE][BOARD_SIZE];
char p2_board[BOARD_SIZE][BOARD_SIZE];
/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    // Check if files exist
    if ( !std::__fs::filesystem::exists(p1_setup_board) or !std::__fs::filesystem::exists(p2_setup_board) ) {
        throw ServerException("ERROR: Board does not exist");
    }

    // Create var
    ifstream infilep1,infilep2;
    bool board_size_flag = true;
    this->board_size = board_size;

    // Open files
    infilep1.open(p1_setup_board);
    infilep2.open(p2_setup_board);

    string line1;
    string line2;
    int line = 0;
    while ( getline (infilep1,line1) &&  getline (infilep2,line2))
    {
        for(int i = 0; i < board_size; i++){
            p1_board[line][i] = line1.at(i);
        }
        // Validate board size
        if ( line1.length() != board_size ) {
            board_size_flag = false;
        }

        for(int i = 0; i < board_size; i++){
            p2_board[line][i] = line2.at(i);
        }

        if ( line2.length() != board_size ) {
            board_size_flag = false;
        }
        line++;
    }
    infilep1.close();
    infilep2.close();

    //Throw error if bad board size
    if (!board_size_flag) {
        throw ServerException("ERROR: Wrong Board Size");
    }
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (player < 1) {
        throw ServerException("ERROR: Player number too low in Evaluate Shot");
    }
    if ( player > MAX_PLAYERS ) {
        throw ServerException("ERROR: Player number too high in Evaluate Shot");
    }
    // Check bounds
    if ( x >= BOARD_SIZE or y >= BOARD_SIZE ) {
        return OUT_OF_BOUNDS;
    }

    if ( player == 1 ){
        if ( p2_board[y][x] != '_' ) {
            return HIT;
        }
        else {
            return MISS;
        }

    }
    else {
        if ( p1_board[y][x] != '_') {
            return HIT;
        }
        else {
            return MISS;
        }
    }

}


int Server::process_shot(unsigned int player) {
    if (player < 1) {
        throw ServerException("ERROR: Player number too low in Process Shot");
    }
    if ( player > MAX_PLAYERS ) {
        throw ServerException("ERROR: Player number too high in Process Shot");
    }
    ifstream pfile;

    pfile.open("player_"+std::to_string(player)+".shot.json");

    int x = 0;
    int y = 0;
    if (pfile.is_open())
    {
        cereal::JSONInputArchive arc(pfile);
        arc(x,y);
        string file = "player_"+std::to_string(player)+".shot.json";
        const char* removeFile = file.c_str();

        remove(removeFile);

        int result = evaluate_shot(player,x,y);
        ofstream ofile;
        ofile.open("player_"+std::to_string(player)+".result.json");
        {
            cereal::JSONOutputArchive oarc(ofile);
            oarc(CEREAL_NVP(result));
        }

        pfile.close();
        ofile.close();

        return SHOT_FILE_PROCESSED;
    }
    else{
        return NO_SHOT_FILE;
    }
}