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

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
    std::streampos fsize = 0;

    file->seekg( 0, std::ios::end );
    fsize = file->tellg();
    file->close();

    return fsize;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    this->board_size = board_size;

    // Check board size
    this->p1_setup_board.open(p1_setup_board);
    this->p2_setup_board.open(p2_setup_board);

    int expected_size = (board_size+1)*board_size;

    if (get_file_length(&this->p1_setup_board) != expected_size or get_file_length(&this->p2_setup_board) != expected_size){
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

    // Open file
    ifstream file;
    if(player == 1){
        file.open("player_2.setup_board.txt");
    }
    else{
        file.open("player_1.setup_board.txt");
    }

    string line1;
    int line = 0;

    while ( getline (file,line1))
    {
        for(int i = 0; i < board_size; i++){
            if(line == y and i == x) {
                if ( line1.at(i) != '_' ) {
                    file.close();
                    return HIT;
                }
                else {
                    file.close();
                    return MISS;
                }
            }
        }

        line++;
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