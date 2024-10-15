#pragma once
#ifndef CHESS_H
#define CHESS_H
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>

using std::string;
using std::cout;
using std::cin;
using std::vector;
using std::abs;
using std::to_string;
using std::endl;
using std::abs;

class Chess {
public:
    short pos[8][8];
    vector<string> moveList;
    bool turn;
    bool running;

    void StartGame(bool text);
    //StartGame(text, start color, fen, white castled, black castled)
    void StartGame(bool text, bool stColor, string inputfen, short whiteCastled, short blackCastled);
    void NextTurn(string move1);
    void EndGame();
    void ShowBoard();
    void ShowPawnTrail();
    void StrictPieceInput();
    //void ShowMoves();
//private:
    float moves;
    bool inCheck;
    bool canCastleLongWhite;
    bool canCastleLongBlack;
    bool canCastleShortWhite;
    bool canCastleShortBlack;
    bool useText;
    short wkingH, wkingD, bkingH, bkingD;
    bool showPawnTrail;
    short wkingMatrix[3][3];
    short bkingMatrix[3][3];
    short wleftRookD;
    short wrightRookD;
    short bleftRookD;
    short brightRookD;
    bool strictpieceinput;
    bool doubleCheck;
    short checkV;
    short apD;
    short apH;
    short winvisP;
    short binvisP;
    char flags[8][8];
    //char attackingFlags[8][8];

    void SetBoard(string inputFen);
    bool isInCheck(bool color, short depthChange);
    void GenerateMoveList();
    void UpdateFlagList(short colorCase);
    void UpdateKingMatrix(bool color, short checkCase);
    void ShowKingMatrix(bool color);
};


#endif