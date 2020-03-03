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
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    //Setting variables
    this->player = player;
    this->board_size = board_size;
    vector<vector<int> > board(board_size, vector<int>(board_size));

    //Writing a new action board using output archive
    ofstream actionBoardFile;

    actionBoardFile.open("player_"+std::to_string(player)+".action_board.json");
    {
        cereal::JSONOutputArchive oarc(actionBoardFile);
        oarc(CEREAL_NVP(board));
    }
    actionBoardFile.close();
}


void Client::fire(unsigned int x, unsigned int y) {
    //writing shot to file using output archive
    ofstream shotFile;

    shotFile.open("player_"+std::to_string(player)+".shot.json");
    {
        cereal::JSONOutputArchive oarc(shotFile);
        oarc(CEREAL_NVP(x));
        oarc(CEREAL_NVP(y));
    }
    shotFile.close();
}


bool Client::result_available() {
    //using filesystem exists
    if(std::__fs::filesystem::exists("player_"+std::to_string(player)+".result.json")){
        return true;
    }
    else{
        return false;
    }
}


int Client::get_result() {
    //Reading result
    int result;

    ifstream resultFile;

    resultFile.open("player_"+std::to_string(player)+".result.json");
    {
        cereal::JSONInputArchive iarc(resultFile);
        iarc(CEREAL_NVP(result));
    }
    resultFile.close();

    //Remove result file after getting result
    string file = "player_"+std::to_string(player)+".result.json";
    const char* removeFile = file.c_str();

    remove(removeFile);

    //Logic for getting result and handling errors
    if(result == HIT){
        return HIT;
    }
    else if(result == MISS){
        return MISS;
    }
    else if(result == OUT_OF_BOUNDS){
        return OUT_OF_BOUNDS;
    }
    else{
        throw ClientException("ERROR: Bad Result");
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    vector<vector<int> > board(board_size, vector<int>(board_size));
    string file = "player_" + std::to_string(player) + ".action_board.json";
    ifstream boardFile;

    boardFile.open(file);
    {
        cereal::JSONInputArchive iarc(boardFile);
        iarc(CEREAL_NVP(board));
    }
    boardFile.close();

    board[y][x] = result;

    ofstream actionBoardFile;

    actionBoardFile.open(file);
    {
        cereal::JSONOutputArchive oarc(actionBoardFile);
        oarc(CEREAL_NVP(board));
    }
    actionBoardFile.close();
}


string Client::render_action_board(){
}