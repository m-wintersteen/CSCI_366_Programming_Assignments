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
    this->player = player;
    this->board_size = board_size;
    vector<vector<int> > board(board_size, vector<int>(board_size));

    ofstream actionBoardFile;

    actionBoardFile.open("player_"+std::to_string(player)+".action_board.json");
    {
        cereal::JSONOutputArchive oarc(actionBoardFile);
        oarc(CEREAL_NVP(board));
    }
    actionBoardFile.close();
}


void Client::fire(unsigned int x, unsigned int y) {
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
}


int Client::get_result() {
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
}


string Client::render_action_board(){
}