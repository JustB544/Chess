#include "Chess.h"

#define NULL 0


void Chess::SetBoard(string inputFen) {
    short h = 7;
    short d = 0;
    short check;
    for (int i = 0; i < inputFen.length(); i++) {
        if (inputFen[i] == '/') {
            h--;
            d = 0;
            continue;
        }
        check = (::tolower(inputFen[i]) == inputFen[i]) ? 10 : 0;
        if (::tolower(inputFen[i]) == 'p') {
            pos[h][d] = 1 + check;
            d++;
        }
        else if (::tolower(inputFen[i]) == 'b') {
            pos[h][d] = 2 + check;
            d++;
        }
        else if (::tolower(inputFen[i]) == 'n') {
            pos[h][d] = 3 + check;
            d++;
        }
        else if (::tolower(inputFen[i]) == 'r') {
            pos[h][d] = 4 + check;
            if (check == 10 && h == 7) {
                if (bleftRookD == NULL) {
                    bleftRookD = d;
                }
                else if (brightRookD == NULL) {
                    brightRookD = d;
                }
                else {
                    bleftRookD = -1;
                }
            }
            else if (check == 0 && h == 0) {
                if (wleftRookD == NULL) {
                    wleftRookD = d;
                }
                else if (wrightRookD == NULL) {
                    wrightRookD = d;
                }
                else {
                    wleftRookD = -1;
                }
            }
            else {
                (check == 10) ? bleftRookD : wleftRookD = -1;
            }
            d++;
        }
        else if (::tolower(inputFen[i]) == 'q') {
            pos[h][d] = 5 + check;
            d++;
        }
        else if (::tolower(inputFen[i]) == 'k') {
            pos[h][d] = 6 + check;
            d++;
        }
        else {
            for (int l = 0; l < ((int)inputFen[i] - 48); l++) {
                pos[h][d] = 0;
                if (d > 7) {
                    cout << "Error: " << inputFen[i];
                    Chess::EndGame();
                    return;
                }
                d++;
            }
        }
    }
}


bool Chess::isInCheck(bool color, short depthChange) {
    bool check1 = false;
    bool check2 = false;
    short ckingH, okingH, ckingD, okingD, travelPosH, travelPosD;
    short colorV = (color) ? 5 : -5;

    if (moves == 0 && depthChange == 0) {
        for (int i = 0; i < 8; i++) {
            for (int l = 0; l < 8; l++) {
                if (pos[i][l] == 11 - colorV && !check1) {
                    check1 = true;
                    ckingH = i;
                    ckingD = l;
                }
                else if (pos[i][l] == 11 + colorV && !check2) {
                    check2 = true;
                    okingH = i;
                    okingD = l;
                }
                else if (pos[i][l] == 6 || pos[i][l] == 16) {
                    cout << "Error Code 1.1: impossible number of kings.\n";
                    Chess::ShowBoard();
                    Chess::EndGame();
                    return true;
                }
            }
        }

        if (!check1 || !check2) {
            cout << "Error Code 1.2: impossible number of kings.\n";
            Chess::ShowBoard();
            Chess::EndGame();
            return true;
        }

        if (abs(ckingH - okingH) < 2 && abs(ckingD - okingD) < 2) {
            cout << "Error Code 2: touching kings.\n";
            Chess::EndGame();
            return true;
        }

        if (color) {
            wkingH = ckingH;
            wkingD = ckingD;
            bkingH = okingH;
            bkingD = okingD;
        }
        else {
            bkingH = ckingH;
            bkingD = ckingD;
            wkingH = okingH;
            wkingD = okingD;
        }

        if (wkingH != 0) {
            wleftRookD = -1;
        }
        else {
            if (wleftRookD > wrightRookD) {
                moves = wrightRookD;
                wrightRookD = wleftRookD;
                wleftRookD = moves;
                moves = 0;
            }
            if (wrightRookD - wkingD > wrightRookD - wleftRookD || wrightRookD - wkingD < 0) {
                wleftRookD = -1;
            }
        }
        if (bkingH != 7) {
            bleftRookD = -1;
        }
        else {
            if (bleftRookD > brightRookD) {
                moves = brightRookD;
                brightRookD = bleftRookD;
                bleftRookD = moves;
                moves = 0;
            }
            if (brightRookD - bkingD > brightRookD - bleftRookD || brightRookD - bkingD < 0) {
                bleftRookD = -1;
            }
        }

    }
    else if (color) {
        ckingH = wkingH;
        ckingD = wkingD + depthChange;
        okingH = bkingH;
        okingD = bkingD;
    }
    else {
        ckingH = bkingH;
        ckingD = bkingD + depthChange;
        okingH = wkingH;
        okingD = wkingD;
    }

    if ((color) ? (ckingH > 1) : (ckingH < 6)) {
        if (ckingD > 0) {
            if (pos[ckingH + colorV / 5][ckingD - 1] == 6 + colorV) {
                return true;
            }
        }
        if (ckingD < 7) {
            if (pos[ckingH + colorV / 5][ckingD + 1] == 6 + colorV) {
                return true;
            }
        }
    }

    if (ckingH + 2 <= 7) {
        if (ckingD + 1 <= 7 && pos[ckingH + 2][ckingD + 1] == 8 + colorV) {
            return true;
        }
        if (ckingD - 1 >= 0 && pos[ckingH + 2][ckingD - 1] == 8 + colorV) {
            return true;
        }
    }
    if (ckingH - 2 >= 0) {
        if (ckingD + 1 <= 7 && pos[ckingH - 2][ckingD + 1] == 8 + colorV) {
            return true;
        }
        if (ckingD - 1 >= 0 && pos[ckingH - 2][ckingD - 1] == 8 + colorV) {
            return true;
        }
    }
    if (ckingH + 1 <= 7) {
        if (ckingD + 2 <= 7 && pos[ckingH + 1][ckingD + 2] == 8 + colorV) {
            return true;
        }
        if (ckingD - 2 >= 0 && pos[ckingH + 1][ckingD - 2] == 8 + colorV) {
            return true;
        }
    }
    if (ckingH - 1 <= 7) {
        if (ckingD + 2 <= 7 && pos[ckingH - 1][ckingD + 2] == 8 + colorV) {
            return true;
        }
        if (ckingD - 2 >= 0 && pos[ckingH - 1][ckingD - 2] == 8 + colorV) {
            return true;
        }
    }

    travelPosH = ckingH;
    travelPosD = ckingD;
    while (travelPosH < 7) {
        travelPosH++;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 9 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosH = ckingH;
    while (travelPosH > 0) {
        travelPosH--;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 9 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosH = ckingH;
    while (travelPosD < 7) {
        travelPosD++;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 9 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosD = ckingD;
    while (travelPosD > 0) {
        travelPosD--;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 9 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosD = ckingD;
    travelPosH = ckingH;
    while (travelPosH < 7 && travelPosD < 7) {
        travelPosH++;
        travelPosD++;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 7 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosH = ckingH;
    travelPosD = ckingD;
    while (travelPosH < 7 && travelPosD > 0) {
        travelPosH++;
        travelPosD--;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 7 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosH = ckingH;
    travelPosD = ckingD;
    while (travelPosH > 0 && travelPosD > 0) {
        travelPosH--;
        travelPosD--;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 7 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    travelPosH = ckingH;
    travelPosD = ckingD;
    while (travelPosH > 0 && travelPosD < 7) {
        travelPosH--;
        travelPosD++;
        if (pos[travelPosH][travelPosD] != 0) {
            if (pos[travelPosH][travelPosD] == 10 + colorV || pos[travelPosH][travelPosD] == 7 + colorV) {
                return true;
            }
            else {
                break;
            }
        }
    }
    return false;
}


void Chess::StartGame(bool text) {
    if (running) {
        char answer;
        cout << "Are you sure you want to end the ongoing game? (y/n): ";
        cin >> answer;
        if (::tolower(answer) == 'n') {
            cout << endl;
            return;
        }
        else if (::tolower(answer) != 'y') {
            cout << "\nInvalid response received\nContinuing game...\n";
            return;
        }
    }
    moves = 0;
    running = true;
    inCheck = false;
    turn = true;
    canCastleLongWhite = true;
    canCastleLongBlack = true;
    canCastleShortWhite = true;
    canCastleShortBlack = true;
    useText = text;
    showPawnTrail = false;
    strictpieceinput = false;
    wkingH = 0;
    wkingD = 4;
    bkingH = 7;
    bkingD = 4;
    wrightRookD = 7;
    wleftRookD = 0;
    bleftRookD = 0;
    brightRookD = 7;
    winvisP = -1;
    binvisP = -1;
    Chess::SetBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    Chess::UpdateFlagList(0);
    Chess::UpdateKingMatrix(true, 0);
    Chess::UpdateKingMatrix(false, 0);
    Chess::GenerateMoveList();
    if (useText) {
        Chess::ShowBoard();
        if (running) {
            cout << ((turn) ? "White" : "Black") << " to move:\n";
        }
    }
}


void Chess::StartGame(bool text, bool stColor, string inputfen, short whiteCastled, short blackCastled) {
    if (running) {
        char answer;
        cout << "Are you sure you want to end the ongoing game? (y/n): ";
        cin >> answer;
        if (::tolower(answer) == 'n') {
            cout << endl;
            return;
        }
        else if (::tolower(answer) != 'y') {
            cout << "\nInvalid response received\nContinuing game...\n";
            return;
        }
    }
    SetBoard(inputfen);
    if (Chess::isInCheck(!stColor, 0)) {
        if (!running) {
            return;
        }
        cout << "Error code 3: " << ((stColor) ? "black is in check on white's turn.\n" : "white is in check on black's turn.\n");
        Chess::EndGame();
        return;
    }
    moves = 0;
    running = true;
    inCheck = Chess::isInCheck(stColor, 0);
    turn = stColor;
    if (whiteCastled == 0 && wleftRookD != -1) {
        canCastleLongWhite = true;
        canCastleShortWhite = true;
    }
    else if (whiteCastled == 1 && wleftRookD != -1) {
        canCastleLongWhite = true;
        canCastleShortWhite = false;
    }
    else {
        canCastleLongWhite = false;
        canCastleShortWhite = false;
    }
    if (blackCastled == 0 && bleftRookD != -1) {
        canCastleLongBlack = true;
        canCastleShortBlack = true;
    }
    else if (blackCastled == 1 && bleftRookD != -1) {
        canCastleLongBlack = true;
        canCastleShortBlack = false;
    }
    else {
        canCastleLongBlack = false;
        canCastleShortBlack = false;
    }
    useText = text;
    showPawnTrail = false;
    strictpieceinput = false;
    winvisP = -1;
    binvisP = -1;
    Chess::UpdateFlagList(0);
    Chess::UpdateKingMatrix(true, 0);
    Chess::UpdateKingMatrix(false, 0);
    Chess::GenerateMoveList();
    if (useText) {
        Chess::ShowBoard();
        if (running) {
            cout << ((turn) ? "White" : "Black") << " to move:\n";
        }
    }
}


void Chess::EndGame() {
    moves = -1;
    running = false;
    useText = false;
    showPawnTrail = false;
    for (int i = 0; i < 8; i++) {
        for (int l = 0; l < 8; l++) {
            pos[i][l] = 0;
        }
    }
}


void Chess::ShowBoard() {
    cout << "Piece = white/black\nEmpty Square = 0\nPawn = P/p\nRook = R/r\nKnight = N/n\nBishop = B/b\nQueen = Q/q\nKing = +/-\n";
    for (int i = 7; i >= 0; i--) {
        cout << i << " ";
        for (int l = 0; l < 8; l++) {
            if (pos[i][l] == 0) {
                cout << "0  ";
            }
            else if (pos[i][l] == 1) {
                cout << "P  ";
            }
            else if (pos[i][l] == 2) {
                cout << "B  ";
            }
            else if (pos[i][l] == 3) {
                cout << "N  ";
            }
            else if (pos[i][l] == 4) {
                cout << "R  ";
            }
            else if (pos[i][l] == 5) {
                cout << "Q  ";
            }
            else if (pos[i][l] == 6) {
                cout << "+  ";
            }
            else if (pos[i][l] == 11) {
                cout << "p  ";
            }
            else if (pos[i][l] == 12) {
                cout << "b  ";
            }
            else if (pos[i][l] == 13) {
                cout << "n  ";
            }
            else if (pos[i][l] == 14) {
                cout << "r  ";
            }
            else if (pos[i][l] == 15) {
                cout << "q  ";
            }
            else if (pos[i][l] == 16) {
                cout << "-  ";
            }
            else if (pos[i][l] == 7 || pos[i][l] == 17) {
                if (showPawnTrail) {
                    cout << "*  ";
                }
                else {
                    cout << "0 ";
                }
            }
            else {
                cout << "Error code 4.1: corrupt position\n";
                Chess::EndGame();
                return;
            }
        }
        cout << "\n";
    }
    cout << "  0  1  2  3  4  5  6  7\n";
}


void Chess::UpdateFlagList(short colorCase) {
    bool directionalPin, diagonalPin;
    short colorV, dis2k1, dis2k2, travelPosH, travelPosD, storeH, storeD;

    for (int i = 0; i < 8; i++) {
        for (int l = 0; l < 8; l++) {
            flags[i][l] = NULL;
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int l = 0; l < 8; l++) {
            directionalPin = false;
            diagonalPin = false;
            if (pos[i][l] == 0) {
                if (flags[i][l] == NULL) {
                    flags[i][l] = '~';
                }
                continue;
            }
            if (colorCase == 0) {
                colorV = (pos[i][l] > 10) ? 1 : 0;
            }
            else if (colorCase == 1) {
                if (pos[i][l] < 10) {
                    continue;
                }
                colorV = 1;
            }
            else if (colorCase == 2) {
                if (pos[i][l] > 10) {
                    continue;
                }
                colorV = 0;
            }
            if (pos[i][l] == 5 + 10 * colorV) {
                if (colorV == 1) {
                    if (i == wkingH) {
                        flags[i][l] = '0';
                        directionalPin = true;
                    }
                    else if (l == wkingD) {
                        flags[i][l] = '1';
                        directionalPin = true;
                    }
                    else if (wkingH - i == wkingD - l) {
                        flags[i][l] = '2';
                        diagonalPin = true;
                    }
                    else if (wkingH - i == l - wkingD) {
                        flags[i][l] = '3';
                        diagonalPin = true;
                    }
                    else {
                        flags[i][l] = 'A';
                        continue;
                    }
                }
                else {
                    if (i == bkingH) {
                        flags[i][l] = 'g';
                        directionalPin = true;
                    }
                    else if (l == bkingD) {
                        flags[i][l] = 'h';
                        directionalPin = true;
                    }
                    else if (bkingH - i == bkingD - l) {
                        flags[i][l] = 'i';
                        diagonalPin = true;
                    }
                    else if (bkingH - i == l - bkingD) {
                        flags[i][l] = 'j';
                        diagonalPin = true;
                    }
                    else {
                        flags[i][l] = 'D';
                        continue;
                    }
                }
            }
            else if (pos[i][l] == 4 + 10 * colorV) {
                if (colorV == 1) {
                    if (i == wkingH) {
                        flags[i][l] = '4';
                        directionalPin = true;
                    }
                    else if (l == wkingD) {
                        flags[i][l] = '5';
                        directionalPin = true;
                    }
                    else {
                        flags[i][l] = 'B';
                        continue;
                    }
                }
                else {
                    if (i == bkingH) {
                        flags[i][l] = 'k';
                        directionalPin = true;
                    }
                    else if (l == bkingD) {
                        flags[i][l] = 'l';
                        directionalPin = true;
                    }
                    else {
                        flags[i][l] = 'E';
                        continue;
                    }
                }
            }
            else if (pos[i][l] == 2 + 10 * colorV) {
                if (colorV == 1) {
                    if (wkingH - i == wkingD - l) {
                        flags[i][l] = '6';
                        diagonalPin = true;
                    }
                    else if (wkingH - i == l - wkingD) {
                        flags[i][l] = '7';
                        diagonalPin = true;
                    }
                    else {
                        flags[i][l] = 'C';
                        continue;
                    }
                }
                else {
                    if (bkingH - i == bkingD - l) {
                        flags[i][l] = 'm';
                        diagonalPin = true;
                    }
                    else if (bkingH - i == l - bkingD) {
                        flags[i][l] = 'n';
                        diagonalPin = true;
                    }
                    else {
                        flags[i][l] = 'F';
                        continue;
                    }
                }
            }
            else {
                if (flags[i][l] == NULL) {
                    flags[i][l] = '~';
                }
                continue;
            }
            if (diagonalPin) {
                storeH = -1;
                storeD = -1;
                if (colorV == 1) {
                    travelPosH = wkingH;
                    travelPosD = wkingD;
                    dis2k1 = wkingH - i;
                    dis2k2 = wkingD - l;
                    for (int k = 0; k < abs(dis2k1) - 1; k++) {
                        travelPosD += (dis2k2 < 0) ? 1 : -1;
                        travelPosH += (dis2k1 < 0) ? 1 : -1;

                        if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] < 10) {
                            if (storeH == -1) {
                                storeH = travelPosH;
                                storeD = travelPosD;
                            }
                            else {
                                storeH = -1;
                                break;
                            }
                        }
                        else if (pos[travelPosH][travelPosD] > 10) {
                            storeH = -1;
                            break;
                        }
                    }
                    if (storeH != -1) {
                        if (pos[storeH][storeD] == 1) {
                            flags[storeH][storeD] = '9';
                        }
                        else if (pos[storeH][storeD] == 2) {
                            flags[storeH][storeD] = 'J';
                        }
                        else if (pos[storeH][storeD] == 3) {
                            flags[storeH][storeD] = 'b';
                        }
                        else if (pos[storeH][storeD] == 4) {
                            flags[storeH][storeD] = 'c';
                        }
                        else if (pos[storeH][storeD] == 5) {
                            flags[storeH][storeD] = 'H';
                        }
                    }
                }
                else {
                    travelPosH = bkingH;
                    travelPosD = bkingD;
                    dis2k1 = bkingH - i;
                    dis2k2 = bkingD - l;
                    for (int k = 0; k < abs(dis2k1) - 1; k++) {
                        travelPosD += (dis2k2 < 0) ? 1 : -1;
                        travelPosH += (dis2k1 < 0) ? 1 : -1;

                        if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] > 10) {
                            if (storeH == -1) {
                                storeH = travelPosH;
                                storeD = travelPosD;
                            }
                            else {
                                storeH = -1;
                                break;
                            }
                        }
                        else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] < 10) {
                            storeH = -1;
                            break;
                        }
                    }
                    if (storeH != -1) {
                        if (pos[storeH][storeD] == 1) {
                            flags[storeH][storeD] = 'p';
                        }
                        else if (pos[storeH][storeD] == 2) {
                            flags[storeH][storeD] = 'N';
                        }
                        else if (pos[storeH][storeD] == 3) {
                            flags[storeH][storeD] = 'r';
                        }
                        else if (pos[storeH][storeD] == 4) {
                            flags[storeH][storeD] = 's';
                        }
                        else if (pos[storeH][storeD] == 5) {
                            flags[storeH][storeD] = 'L';
                        }
                    }
                }
            }
            if (directionalPin) {
                storeH = -1;
                storeD = -1;
                if (colorV == 1) {
                    travelPosH = wkingH;
                    travelPosD = wkingD;
                    dis2k1 = (i == wkingH) ? wkingD - l : wkingH - i;
                    for (int k = 0; k < abs(dis2k1) - 1; k++) {
                        if (wkingH - i == 0) {
                            travelPosD += (dis2k1 < 0) ? 1 : -1;
                        }
                        else {
                            travelPosH += (dis2k1 < 0) ? 1 : -1;
                        }
                        if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] < 10) {
                            if (storeH == -1) {
                                storeH = travelPosH;
                                storeD = travelPosD;
                            }
                            else {
                                storeH = -1;
                                break;
                            }
                        }
                        else if (pos[travelPosH][travelPosD] > 10) {
                            storeH = -1;
                            break;
                        }
                    }
                    if (storeH != -1) {
                        if (pos[storeH][storeD] == 1) {
                            flags[storeH][storeD] = '8';
                        }
                        else if (pos[storeH][storeD] == 2) {
                            flags[storeH][storeD] = 'd';
                        }
                        else if (pos[storeH][storeD] == 3) {
                            flags[storeH][storeD] = 'a';
                        }
                        else if (pos[storeH][storeD] == 4) {
                            flags[storeH][storeD] = 'I';
                        }
                        else if (pos[storeH][storeD] == 5) {
                            flags[storeH][storeD] = 'G';
                        }
                    }
                }
                else {
                    travelPosH = bkingH;
                    travelPosD = bkingD;
                    dis2k1 = (i == bkingH) ? bkingD - l : bkingH - i;
                    for (int k = 0; k < abs(dis2k1) - 1; k++) {
                        if (bkingH - i == 0) {
                            travelPosD += (dis2k1 < 0) ? 1 : -1;
                        }
                        else {
                            travelPosH += (dis2k1 < 0) ? 1 : -1;
                        }

                        if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] > 10) {
                            if (storeH == -1) {
                                storeH = travelPosH;
                                storeD = travelPosD;
                            }
                            else {
                                storeH = -1;
                                break;
                            }
                        }
                        else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] < 10) {
                            storeH = -1;
                            break;
                        }
                    }
                    if (storeH != -1) {
                        if (pos[storeH][storeD] == 1) {
                            flags[storeH][storeD] = 'o';
                        }
                        else if (pos[storeH][storeD] == 2) {
                            flags[storeH][storeD] = 't';
                        }
                        else if (pos[storeH][storeD] == 3) {
                            flags[storeH][storeD] = 'q';
                        }
                        else if (pos[storeH][storeD] == 4) {
                            flags[storeH][storeD] = 'M';
                        }
                        else if (pos[storeH][storeD] == 5) {
                            flags[storeH][storeD] = 'K';
                        }
                    }
                }
            }
        }
    }
}


void Chess::UpdateKingMatrix(bool color, short checkCase) {
    bool check1, check2, check3, check4, check;
    short ckingH, ckingD, okingH, okingD, colorV, travelPosH, travelPosD, caseV, kingMatrix[3][3];

    ckingH = (color) ? wkingH : bkingH;
    ckingD = (color) ? wkingD : bkingD;

    okingH = (!color) ? wkingH : bkingH;
    okingD = (!color) ? wkingD : bkingD;

    caseV = (checkCase < 0) ? -1 : 1;

    if (color) {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                if (wkingMatrix[i][l] == NULL) {
                    wkingMatrix[i][l] = 0;
                }
                if (checkCase == 0) {
                    kingMatrix[i][l] = 0;
                }
                else {
                    kingMatrix[i][l] = wkingMatrix[i][l];
                }
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                if (bkingMatrix[i][l] == NULL) {
                    bkingMatrix[i][l] = 0;
                }
                if (checkCase == 0) {
                    kingMatrix[i][l] = 0;
                }
                else {
                    kingMatrix[i][l] = bkingMatrix[i][l];
                }
            }
        }
    }

    /*
    Cases:
    0: Do all cases
    1: Do knight case
    2: Do pawn case
    3: Do king case
    4: Do middle vertical check case
    5: Do right vertical check case
    6: Do left vertical check case
    7: Do middle horizontal check case
    8: Do upper horizontal check case
    9: Do lower horizontal check case
    10: Do middle positive slope check case
    11: Do H+ positive slope check case
    12: Do H+ D- positive slope check case
    13: Do H- positive slope check case
    14: Do H- D+ positive slope check case
    15: Do middle negative slope check case
    16: Do H+ negative slope check case
    17: Do H+ D+ negative slope check case
    18: Do H- negative slope check case
    19: Do H- D- negative slope check case
    -19 to -1: Subtract instead of add to king matrix
    */

    colorV = (color) ? 1 : 0;
    doubleCheck = false;

    //knight checks
    if (checkCase == 0 || abs(checkCase) == 1) {
        for (int i = 0; i < 8; i++) {
            for (int l = 0; l < 8; l++) {
                if (pos[i][l] == 3 + 10 * colorV) {
                    //cout << abs(i - ckingH) << " " << abs(l - ckingD) << " " << (abs(i - ckingH) != 3 || abs(l - ckingD) != 3) << endl;
                    if (!(abs(i - ckingH) < 4 && abs(l - ckingD) < 4 && (abs(i - ckingH) != 3 || abs(l - ckingD) != 3))) {
                        continue;
                    }
                    //cout << i << " " << l << endl;
                    if (abs(i - ckingH) == abs(l - ckingD)) {
                        if (abs(i - ckingH) == 1) {
                            kingMatrix[(i - ckingH > 0) ? 0 : 2][1] += caseV;
                            kingMatrix[1][(l - ckingD > 0) ? 0 : 2] += caseV;
                        }
                        else {
                            kingMatrix[(i - ckingH > 0) ? 2 : 0][1] += caseV;
                            kingMatrix[1][(l - ckingD > 0) ? 2 : 0] += caseV;
                        }
                    }
                    else if (abs(i - ckingH) > abs(l - ckingD)) {
                        if (abs(i - ckingH) == 3) {
                            if (l == ckingD) {
                                kingMatrix[(i - ckingH > 0) ? 2 : 0][0] += caseV;
                                kingMatrix[(i - ckingH > 0) ? 2 : 0][2] += caseV;
                            }
                            else {
                                kingMatrix[(i - ckingH > 0) ? 2 : 0][1 + ((l - ckingD > 0) ? 1 : -1) * ((abs(l - ckingD) == 2) ? 1 : 0)] += caseV;
                            }
                        }
                        else if (abs(i - ckingH) == 2) {
                            if (abs(l - ckingD) != 0) {
                                apH = i;
                                apD = l;
                                checkV = 0;
                                kingMatrix[0][0] += caseV;
                            }
                            else {
                                kingMatrix[1][0] += caseV;
                                kingMatrix[1][2] += caseV;
                            }
                        }
                        else {
                            kingMatrix[(i - ckingH > 0) ? 0 : 2][0] += caseV;
                            kingMatrix[(i - ckingH > 0) ? 0 : 2][2] += caseV;
                        }
                    }
                    else {
                        if (abs(l - ckingD) == 3) {
                            if (i == ckingH) {
                                kingMatrix[0][(l - ckingD > 0) ? 2 : 0] += caseV;
                                kingMatrix[2][(l - ckingD > 0) ? 2 : 0] += caseV;
                            }
                            else {
                                kingMatrix[1 + ((i - ckingH > 0) ? 1 : -1) * ((abs(i - ckingH) == 2) ? 1 : 0)][(l - ckingD > 0) ? 2 : 0] += caseV;
                            }
                        }
                        else if (abs(l - ckingD) == 2) {
                            if (abs(i - ckingH) != 0) {
                                apH = i;
                                apD = l;
                                checkV = 0;
                                kingMatrix[0][0] += caseV;
                            }
                            else {
                                kingMatrix[0][1] += caseV;
                                kingMatrix[2][1] += caseV;
                            }
                        }
                        else {
                            kingMatrix[0][(l - ckingD > 0) ? 0 : 2] += caseV;
                            kingMatrix[2][(l - ckingD > 0) ? 0 : 2] += caseV;
                        }
                    }
                }
            }
        }
    }


    //pawn checks
    if (checkCase == 0 || abs(checkCase) == 2) {
        for (int i = -2; i < 1; i++) {
            for (int l = -2; l < 3; l++) {
                if ((i == 0 && l == 0) || 8 + (2 * colorV - 1) * i <= ckingH || 8 - l <= ckingD || (2 * colorV - 1) * i > ckingH || -l > ckingD) {
                    continue;
                }
                if (l == 0 && pos[ckingH + (1 - 2 * colorV) * i][ckingD] == 1 + 10 * colorV) {
                    kingMatrix[(1 - 2 * colorV) * (i + 2)][2] += caseV;
                    kingMatrix[(1 - 2 * colorV) * (i + 2)][0] += caseV;
                }
                else if (l < 0 && pos[ckingH + (1 - 2 * colorV) * i][ckingD + l] == 1 + 10 * colorV) {
                    kingMatrix[(1 - 2 * colorV) * (i + 2) + 2 * colorV][l + 2] += caseV;
                    if ((1 - 2 * colorV) * (i + 2) + 2 * colorV == 1 && l + 2 == 1) {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + (1 - 2 * colorV) * i;
                            apD = ckingD + l;
                            checkV = 7 - 6 * colorV;
                        }
                    }
                }
                else if (l > 0 && pos[ckingH + (1 - 2 * colorV) * i][ckingD + l] == 1 + 10 * colorV) {
                    kingMatrix[(1 - 2 * colorV) * (i + 2) + 2 * colorV][l] += caseV;
                    if ((1 - 2 * colorV) * (i + 2) + 2 * colorV && l == 1) {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + (1 - 2 * colorV) * i;
                            apD = ckingD + l;
                            checkV = 5 - 2 * colorV;
                        }
                    }
                }
            }
        }
    }

    //king "checks"
    if (checkCase == 0 || abs(checkCase) == 3) {
        if ((abs(okingH - ckingH) == 2 && abs(okingD - ckingD) <= 2) || (abs(okingH - ckingH) <= 2 && abs(okingD - ckingD) == 2)) {
            if (abs(okingH - ckingH) == 2) {
                if (abs(okingD - ckingD) < 2) {
                    kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][1] += caseV;
                    if (abs(okingD - ckingD) < 1) {
                        kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][0] += caseV;
                        kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][2] += caseV;
                    }
                    else {
                        kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                    }
                }
                else {
                    kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                }
            }
            else {
                if (abs(okingH - ckingH) < 2) {
                    kingMatrix[1][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                    if (abs(okingH - ckingH) < 1) {
                        kingMatrix[0][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                        kingMatrix[2][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                    }
                    else {
                        kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                    }
                }
                else {
                    kingMatrix[1 + abs(okingH - ckingH) / (okingH - ckingH)][1 + abs(okingD - ckingD) / (okingD - ckingD)] += caseV;
                }
            }
        }
    }

    //vertical checks
    travelPosH = 0;
    travelPosD = 0;
    if (checkCase == 0 || abs(checkCase) == 4) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH++;
            if (8 - travelPosH <= ckingH) {
                check1 = true;
            }
            if (travelPosH > ckingH) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD] != 0 && pos[ckingH + travelPosH][ckingD] != 7 && pos[ckingH + travelPosH][ckingD] != 17) {
                if (!check3 && travelPosH == 1 && pos[ckingH + travelPosH][ckingD] != 4 + 10 * colorV && pos[ckingH + travelPosH][ckingD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD] == 4 + 10 * colorV || pos[ckingH + travelPosH][ckingD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][1] += caseV;
                    }
                    else if (travelPosH == 1) {
                        kingMatrix[0][1] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + travelPosH;
                            apD = ckingD;
                            checkV = 2;
                        }
                        kingMatrix[2][1] += caseV;
                        kingMatrix[0][1] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                }
            }
            if (!check2 && ckingH - travelPosH > -1 && pos[ckingH - travelPosH][ckingD] != 0 && pos[ckingH - travelPosH][ckingD] != 7 && pos[ckingH - travelPosH][ckingD] != 17) {
                if (!check4 && travelPosH == 1 && pos[ckingH - travelPosH][ckingD] != 4 + 10 * colorV && pos[ckingH - travelPosH][ckingD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD] == 4 + 10 * colorV || pos[ckingH - travelPosH][ckingD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][1] += caseV;
                    }
                    else if (travelPosH == 1) {
                        kingMatrix[2][1] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH - travelPosH;
                            apD = ckingD;
                            checkV = 6;
                        }
                        kingMatrix[2][1] += caseV;
                        kingMatrix[0][1] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 5) {
        check = false;
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        if (pos[ckingH][ckingD + 1] != 0 && pos[ckingH][ckingD + 1] != 7 && pos[ckingH][ckingD + 1] != 17) {
            check = true;

            if ((pos[ckingH][ckingD + 1] == 4 + 10 * colorV || pos[ckingH][ckingD + 1] == 5 + 10 * colorV)) {
                check1 = true;
                check2 = true;
                kingMatrix[0][2] += caseV;
                kingMatrix[2][2] += caseV;
            }
        }
        while (!check1 || !check2) {
            travelPosH++;
            if (ckingD == 7) {
                break;
            }
            if (8 - travelPosH <= ckingH) {
                check1 = true;
            }
            if (travelPosH > ckingH) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD + 1] != 0 && pos[ckingH + travelPosH][ckingD + 1] != 7 && pos[ckingH + travelPosH][ckingD + 1] != 17) {
                if (!check3 && travelPosH == 1 && pos[ckingH + travelPosH][ckingD + 1] != 4 + 10 * colorV && pos[ckingH + travelPosH][ckingD + 1] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD + 1] == 4 + 10 * colorV || pos[ckingH + travelPosH][ckingD + 1] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][2] += caseV;
                    }
                    else if (travelPosH == 1) {
                        if (!check) {
                            kingMatrix[0][2] += caseV;
                        }
                        kingMatrix[1][2] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[0][2] += caseV;
                        }
                        kingMatrix[1][2] += caseV;
                        kingMatrix[2][2] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD + 1] != 0 && pos[ckingH - travelPosH][ckingD + 1] != 7 && pos[ckingH - travelPosH][ckingD + 1] != 17) {
                if (!check4 && travelPosH == 1 && pos[ckingH - travelPosH][ckingD + 1] != 4 + 10 * colorV && pos[ckingH - travelPosH][ckingD + 1] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD + 1] == 4 + 10 * colorV || pos[ckingH - travelPosH][ckingD + 1] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][2] += caseV;
                    }
                    else if (travelPosH == 1) {
                        if (!check) {
                            kingMatrix[2][2] += caseV;
                        }
                        kingMatrix[1][2] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[2][2] += caseV;
                        }
                        kingMatrix[1][2] += caseV;
                        kingMatrix[0][2] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 6) {
        check = false;
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        if (pos[ckingH][ckingD - 1] != 0 && pos[ckingH][ckingD - 1] != 7 && pos[ckingH][ckingD - 1] != 17) {
            check = true;

            if ((pos[ckingH][ckingD - 1] == 4 + 10 * colorV || pos[ckingH][ckingD - 1] == 5 + 10 * colorV)) {
                check1 = true;
                check2 = true;
                kingMatrix[0][0] += caseV;
                kingMatrix[2][0] += caseV;
            }
        }
        while (!check1 || !check2) {
            travelPosH++;
            if (ckingD == 0) {
                break;
            }
            if (8 - travelPosH <= ckingH) {
                check1 = true;
            }
            if (travelPosH > ckingH) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD - 1] != 0 && pos[ckingH + travelPosH][ckingD - 1] != 7 && pos[ckingH + travelPosH][ckingD - 1] != 17) {
                if (!check3 && travelPosH == 1 && pos[ckingH + travelPosH][ckingD - 1] != 4 + 10 * colorV && pos[ckingH + travelPosH][ckingD - 1] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD - 1] == 4 + 10 * colorV || pos[ckingH + travelPosH][ckingD - 1] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][0] += caseV;
                    }
                    else if (travelPosH == 1) {
                        if (!check) {
                            kingMatrix[0][0] += caseV;
                        }
                        kingMatrix[1][0] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[0][0] += caseV;
                        }
                        kingMatrix[1][0] += caseV;
                        kingMatrix[2][0] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD - 1] != 0 && pos[ckingH - travelPosH][ckingD - 1] != 7 && pos[ckingH - travelPosH][ckingD - 1] != 17) {
                if (!check4 && travelPosH == 1 && pos[ckingH - travelPosH][ckingD - 1] != 4 + 10 * colorV && pos[ckingH - travelPosH][ckingD - 1] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD - 1] == 4 + 10 * colorV || pos[ckingH - travelPosH][ckingD - 1] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][0] += caseV;
                    }
                    else if (travelPosH == 1) {
                        if (!check) {
                            kingMatrix[2][0] += caseV;
                        }
                        kingMatrix[1][0] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[2][0] += caseV;
                        }
                        kingMatrix[1][0] += caseV;
                        kingMatrix[0][0] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
    }

    //horizontal checks
    if (checkCase == 0 || abs(checkCase) == 7) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosD++;
            if (8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH][ckingD + travelPosD] != 0 && pos[ckingH][ckingD + travelPosD] != 7 && pos[ckingH][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH][ckingD + travelPosD] != 4 + 10 * colorV && pos[ckingH][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }
                if (pos[ckingH][ckingD + travelPosD] == 4 + 10 * colorV || pos[ckingH][ckingD + travelPosD] == 5 + 10 * colorV) {
                    if (check3) {
                        kingMatrix[1][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][0] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH;
                            apD = ckingD + travelPosD;
                            checkV = 4;
                        }
                        kingMatrix[1][2] += caseV;
                        kingMatrix[1][0] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH][ckingD - travelPosD] != 0 && pos[ckingH][ckingD - travelPosD] != 7 && pos[ckingH][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH][ckingD - travelPosD] != 4 + 10 * colorV && pos[ckingH][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (pos[ckingH][ckingD - travelPosD] == 4 + 10 * colorV || pos[ckingH][ckingD - travelPosD] == 5 + 10 * colorV) {
                    if (check4) {
                        kingMatrix[1][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][2] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH;
                            apD = ckingD - travelPosD;
                            checkV = 8;
                        }
                        kingMatrix[1][2] += caseV;
                        kingMatrix[1][0] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                }
            }
        }
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 8) {
        check = false;
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        if (pos[ckingH + 1][ckingD] != 0 && pos[ckingH + 1][ckingD] != 7 && pos[ckingH + 1][ckingD] != 17) {
            check = true;

            if (pos[ckingH + 1][ckingD] == 4 + 10 * colorV || pos[ckingH + 1][ckingD] == 5 + 10 * colorV) {
                check1 = true;
                check2 = true;
                kingMatrix[2][0] += caseV;
                kingMatrix[2][2] += caseV;
            }
        }
        while (!check1 || !check2) {
            travelPosD++;
            if (ckingH == 7) {
                break;
            }
            if (8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + 1][ckingD + travelPosD] != 0 && pos[ckingH + 1][ckingD + travelPosD] != 7 && pos[ckingH + 1][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH + 1][ckingD + travelPosD] != 4 + 10 * colorV && pos[ckingH + 1][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + 1][ckingD + travelPosD] == 4 + 10 * colorV || pos[ckingH + 1][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        if (!check) {
                            kingMatrix[2][0] += caseV;
                        }
                        kingMatrix[2][1] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[2][0] += caseV;
                        }
                        kingMatrix[2][2] += caseV;
                        kingMatrix[2][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH + 1][ckingD - travelPosD] != 0 && pos[ckingH + 1][ckingD - travelPosD] != 7 && pos[ckingH + 1][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH + 1][ckingD - travelPosD] != 4 + 10 * colorV && pos[ckingH + 1][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH + 1][ckingD - travelPosD] == 4 + 10 * colorV || pos[ckingH + 1][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[2][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        if (!check) {
                            kingMatrix[2][2] += caseV;
                        }
                        kingMatrix[2][1] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[2][2] += caseV;
                        }
                        kingMatrix[2][0] += caseV;
                        kingMatrix[2][1] += caseV;
                    }
                }
            }
        }
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 9) {
        check = false;
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        if (pos[ckingH - 1][ckingD] != 0 && pos[ckingH - 1][ckingD] != 7 && pos[ckingH - 1][ckingD] != 17) {
            check = true;

            if ((pos[ckingH - 1][ckingD] == 4 + 10 * colorV || pos[ckingH - 1][ckingD] == 5 + 10 * colorV)) {
                check1 = true;
                check2 = true;
                kingMatrix[0][0] += caseV;
                kingMatrix[0][2] += caseV;
            }
        }
        while (!check1 || !check2) {
            travelPosD++;
            if (ckingH == 0) {
                break;
            }
            if (8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH - 1][ckingD + travelPosD] != 0 && pos[ckingH - 1][ckingD + travelPosD] != 7 && pos[ckingH - 1][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH - 1][ckingD + travelPosD] != 4 + 10 * colorV && pos[ckingH - 1][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH - 1][ckingD + travelPosD] == 4 + 10 * colorV || pos[ckingH - 1][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[0][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        if (!check) {
                            kingMatrix[0][0] += caseV;
                        }
                        kingMatrix[0][1] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[0][0] += caseV;
                        }
                        kingMatrix[0][2] += caseV;
                        kingMatrix[0][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - 1][ckingD - travelPosD] != 0 && pos[ckingH - 1][ckingD - travelPosD] != 7 && pos[ckingH - 1][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH - 1][ckingD - travelPosD] != 4 + 10 * colorV && pos[ckingH - 1][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - 1][ckingD - travelPosD] == 4 + 10 * colorV || pos[ckingH - 1][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        if (!check) {
                            kingMatrix[0][2] += caseV;
                        }
                        kingMatrix[0][1] += caseV;
                    }
                    else {
                        if (!check) {
                            kingMatrix[0][2] += caseV;
                        }
                        kingMatrix[0][1] += caseV;
                        kingMatrix[0][0] += caseV;
                    }
                }
            }
        }
        travelPosD = 0;
    }

    //positive slope diagonals
    if (checkCase == 0 || abs(checkCase) == 10) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH++;
            travelPosD++;
            if (8 - travelPosH <= ckingH || 8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (travelPosH > ckingH || travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD + travelPosD] != 0 && pos[ckingH + travelPosH][ckingD + travelPosD] != 7 && pos[ckingH + travelPosH][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && (pos[ckingH + travelPosH][ckingD + travelPosD] != 2 + 10 * colorV && pos[ckingH + travelPosH][ckingD + travelPosD] != 5 + 10 * colorV)) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD + travelPosD] == 2 + 10 * colorV || pos[ckingH + travelPosH][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[0][0] += caseV;
                        kingMatrix[1][1] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + travelPosH;
                            apD = ckingD + travelPosD;
                            checkV = 3;
                        }
                        kingMatrix[0][0] += caseV;
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][2] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD - travelPosD] != 0 && pos[ckingH - travelPosH][ckingD - travelPosD] != 7 && pos[ckingH - travelPosH][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && (pos[ckingH - travelPosH][ckingD - travelPosD] != 2 + 10 * colorV && pos[ckingH - travelPosH][ckingD - travelPosD] != 5 + 10 * colorV)) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD - travelPosD] == 2 + 10 * colorV || pos[ckingH - travelPosH][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][2] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH - travelPosH;
                            apD = ckingD - travelPosD;
                            checkV = 7;
                        }
                        kingMatrix[0][0] += caseV;
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][2] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 11) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH++;
            travelPosD++;
            if (8 - travelPosH <= ckingH || 8 - travelPosD <= ckingD - 1) {
                check1 = true;
            }
            if (travelPosH > ckingH + 1 || travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 0 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 7 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 2 + 10 * colorV && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD - 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH + travelPosH][ckingD - 1 + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][1] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][0] += caseV;
                    }
                    else {
                        kingMatrix[1][0] += caseV;
                        kingMatrix[2][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH + 1 - travelPosH][ckingD - travelPosD] != 0 && pos[ckingH + 1 - travelPosH][ckingD - travelPosD] != 7 && pos[ckingH + 1 - travelPosH][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH + 1 - travelPosH][ckingD - travelPosD] != 2 + 10 * colorV && pos[ckingH + 1 - travelPosH][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH + 1 - travelPosH][ckingD - travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 - travelPosH][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[1][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[2][1] += caseV;
                    }
                    else {
                        kingMatrix[1][0] += caseV;
                        kingMatrix[2][1] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 12) {
        check1 = false;
        check2 = false;
        /*if (pos[ckingH + 1][ckingD - 1] == 2 + 10 * colorV || pos[ckingH + 1][ckingD - 1] == 5 + 10 * colorV) {
            check1 = true;
            check2 = true;
        }*/
        while (!check1 || !check2) {
            travelPosH++;
            travelPosD++;
            if (8 - travelPosH <= ckingH + 1 || 8 - travelPosD <= ckingD - 1) {
                check1 = true;
            }
            if (travelPosH > ckingH + 1 || travelPosD > ckingD - 1) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + 1 + travelPosH][ckingD - 1 + travelPosD] != 0 && pos[ckingH + 1 + travelPosH][ckingD - 1 + travelPosD] != 7 && pos[ckingH + 1 + travelPosH][ckingD - 1 + travelPosD] != 17) {
                check1 = true;

                if (check1 && (pos[ckingH + 1 + travelPosH][ckingD - 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 + travelPosH][ckingD - 1 + travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[2][0] += caseV;
                }
            }
            if (!check2 && pos[ckingH + 1 - travelPosH][ckingD - 1 - travelPosD] != 0 && pos[ckingH + 1 - travelPosH][ckingD - 1 - travelPosD] != 7 && pos[ckingH + 1 - travelPosH][ckingD - 1 - travelPosD] != 17) {
                check2 = true;

                if (check2 && (pos[ckingH + 1 - travelPosH][ckingD - 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 - travelPosH][ckingD - 1 - travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[2][0] += caseV;
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 13) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH++;
            travelPosD++;
            if (8 - travelPosH <= ckingH || 8 - travelPosD <= ckingD + 1) {
                check1 = true;
            }
            if (travelPosH > ckingH - 1 || travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH - 1 + travelPosH][ckingD + travelPosD] != 0 && pos[ckingH - 1 + travelPosH][ckingD + travelPosD] != 7 && pos[ckingH - 1 + travelPosH][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH - 1 + travelPosH][ckingD + travelPosD] != 2 + 10 * colorV && pos[ckingH - 1 + travelPosH][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH - 1 + travelPosH][ckingD + travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 + travelPosH][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[1][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[0][1] += caseV;
                    }
                    else {
                        kingMatrix[1][2] += caseV;
                        kingMatrix[0][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 0 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 7 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 2 + 10 * colorV && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD + 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH - travelPosH][ckingD + 1 - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[0][1] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][2] += caseV;
                    }
                    else {
                        kingMatrix[1][2] += caseV;
                        kingMatrix[0][1] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 14) {
        check1 = false;
        check2 = false;
        /*(if (pos[ckingH - 1][ckingD + 1] == 2 + 10 * colorV || pos[ckingH - 1][ckingD + 1] == 5 + 10 * colorV) {
            check1 = true;
            check2 = true;
        }*/
        while (!check1 || !check2) {
            travelPosH++;
            travelPosD++;
            if (8 - travelPosH <= ckingH - 1 || 8 - travelPosD <= ckingD + 1) {
                check1 = true;
            }
            if (travelPosH > ckingH - 1 || travelPosD > ckingD + 1) {
                check2 = true;
            }
            if (!check1 && pos[ckingH - 1 + travelPosH][ckingD + 1 + travelPosD] != 0 && pos[ckingH - 1 + travelPosH][ckingD + 1 + travelPosD] != 7 && pos[ckingH - 1 + travelPosH][ckingD + 1 + travelPosD] != 17) {
                check1 = true;

                if (check1 && (pos[ckingH - 1 + travelPosH][ckingD + 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 + travelPosH][ckingD + 1 + travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[0][2] += caseV;
                }
            }
            if (!check2 && pos[ckingH - 1 - travelPosH][ckingD + 1 - travelPosD] != 0 && pos[ckingH - 1 - travelPosH][ckingD + 1 - travelPosD] != 7 && pos[ckingH - 1 - travelPosH][ckingD + 1 - travelPosD] != 17) {
                check2 = true;

                if (check2 && (pos[ckingH - 1 - travelPosH][ckingD + 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 - travelPosH][ckingD + 1 - travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[0][2] += caseV;
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    //negative slope diagonals
    if (checkCase == 0 || abs(checkCase) == 15) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH--;
            travelPosD++;
            if (-travelPosH > ckingH || 8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (8 + travelPosH <= ckingH || travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD + travelPosD] != 0 && pos[ckingH + travelPosH][ckingD + travelPosD] != 7 && pos[ckingH + travelPosH][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH + travelPosH][ckingD + travelPosD] != 2 + 10 * colorV && pos[ckingH + travelPosH][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD + travelPosD] == 2 + 10 * colorV || pos[ckingH + travelPosH][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[0][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][0] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + travelPosH;
                            apD = ckingD + travelPosD;
                            checkV = 5;
                        }
                        kingMatrix[0][2] += caseV;
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][0] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD - travelPosD] != 0 && pos[ckingH - travelPosH][ckingD - travelPosD] != 7 && pos[ckingH - travelPosH][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH - travelPosH][ckingD - travelPosD] != 2 + 10 * colorV && pos[ckingH - travelPosH][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD - travelPosD] == 2 + 10 * colorV || pos[ckingH - travelPosH][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[2][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][1] += caseV;
                        kingMatrix[0][2] += caseV;
                    }
                    else {
                        if (kingMatrix[1][1] > 0) {
                            doubleCheck = true;
                        }
                        else {
                            apH = ckingH + travelPosH;
                            apD = ckingD + travelPosD;
                            checkV = 1;
                        }
                        kingMatrix[0][2] += caseV;
                        kingMatrix[1][1] += caseV;
                        kingMatrix[2][0] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 16) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH--;
            travelPosD++;
            if (-travelPosH > ckingH + 1 || 8 - travelPosD <= ckingD) {
                check1 = true;
            }
            if (8 + travelPosH <= ckingH || travelPosD > ckingD + 1) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + 1 + travelPosH][ckingD + travelPosD] != 0 && pos[ckingH + 1 + travelPosH][ckingD + travelPosD] != 7 && pos[ckingH + 1 + travelPosH][ckingD + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH + 1 + travelPosH][ckingD + travelPosD] != 2 + 10 * colorV && pos[ckingH + 1 + travelPosH][ckingD + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }
                if (check1 && (pos[ckingH + 1 + travelPosH][ckingD + travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 + travelPosH][ckingD + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[1][2] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[2][1] += caseV;
                    }
                    else {
                        kingMatrix[2][1] += caseV;
                        kingMatrix[1][2] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 0 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 7 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 2 + 10 * colorV && pos[ckingH - travelPosH][ckingD + 1 - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - travelPosH][ckingD + 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH - travelPosH][ckingD + 1 - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[2][1] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][2] += caseV;
                    }
                    else {
                        kingMatrix[1][2] += caseV;
                        kingMatrix[2][1] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 17) {
        check1 = false;
        check2 = false;
        /*if (pos[ckingH + 1][ckingD + 1] == 2 + 10 * colorV || pos[ckingH + 1][ckingD + 1] == 5 + 10 * colorV) {
            check1 = true;
            check2 = true;
        }*/
        while (!check1 || !check2) {
            travelPosH--;
            travelPosD++;
            if (-travelPosH > ckingH + 1 || 8 - travelPosD <= ckingD + 1) {
                check1 = true;
            }
            if (8 + travelPosH <= ckingH + 1 || travelPosD > ckingD + 1) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + 1 + travelPosH][ckingD + 1 + travelPosD] != 0 && pos[ckingH + 1 + travelPosH][ckingD + 1 + travelPosD] != 7 && pos[ckingH + 1 + travelPosH][ckingD + 1 + travelPosD] != 17) {
                check1 = true;

                if (check1 && (pos[ckingH + 1 + travelPosH][ckingD + 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 + travelPosH][ckingD + 1 + travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[2][2] += caseV;
                }
            }
            if (!check2 && pos[ckingH + 1 - travelPosH][ckingD + 1 - travelPosD] != 0 && pos[ckingH + 1 - travelPosH][ckingD + 1 - travelPosD] != 7 && pos[ckingH + 1 - travelPosH][ckingD + 1 - travelPosD] != 17) {
                check2 = true;

                if (check2 && (pos[ckingH + 1 - travelPosH][ckingD + 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH + 1 - travelPosH][ckingD + 1 - travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[2][2] += caseV;
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 18) {
        check1 = false;
        check2 = false;
        check3 = false;
        check4 = false;
        while (!check1 || !check2) {
            travelPosH--;
            travelPosD++;
            if (-travelPosH > ckingH || 8 - travelPosD <= ckingD - 1) {
                check1 = true;
            }
            if (8 + travelPosH <= ckingH - 1 || travelPosD > ckingD) {
                check2 = true;
            }
            if (!check1 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 0 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 7 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 17) {
                if (!check3 && travelPosD == 1 && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 2 + 10 * colorV && pos[ckingH + travelPosH][ckingD - 1 + travelPosD] != 5 + 10 * colorV) {
                    check3 = true;
                }
                else {
                    check1 = true;
                }

                if (check1 && (pos[ckingH + travelPosH][ckingD - 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH + travelPosH][ckingD - 1 + travelPosD] == 5 + 10 * colorV)) {
                    if (check3) {
                        kingMatrix[0][1] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[1][0] += caseV;
                    }
                    else {
                        kingMatrix[1][0] += caseV;
                        kingMatrix[0][1] += caseV;
                    }
                }
            }
            if (!check2 && pos[ckingH - 1 - travelPosH][ckingD - travelPosD] != 0 && pos[ckingH - 1 - travelPosH][ckingD - travelPosD] != 7 && pos[ckingH - 1 - travelPosH][ckingD - travelPosD] != 17) {
                if (!check4 && travelPosD == 1 && pos[ckingH - 1 - travelPosH][ckingD - travelPosD] != 2 + 10 * colorV && pos[ckingH - 1 - travelPosH][ckingD - travelPosD] != 5 + 10 * colorV) {
                    check4 = true;
                }
                else {
                    check2 = true;
                }

                if (check2 && (pos[ckingH - 1 - travelPosH][ckingD - travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 - travelPosH][ckingD - travelPosD] == 5 + 10 * colorV)) {
                    if (check4) {
                        kingMatrix[1][0] += caseV;
                    }
                    else if (travelPosD == 1) {
                        kingMatrix[0][1] += caseV;
                    }
                    else {
                        kingMatrix[0][1] += caseV;
                        kingMatrix[1][0] += caseV;
                    }
                }
            }
        }
        travelPosH = 0;
        travelPosD = 0;
    }

    if (checkCase == 0 || abs(checkCase) == 19) {
        check1 = false;
        check2 = false;
        /*if (pos[ckingH - 1][ckingD - 1] == 2 + 10 * colorV || pos[ckingH - 1][ckingD - 1] == 5 + 10 * colorV) {
            check1 = true;
            check2 = true;
        }*/
        while (!check1 || !check2) {
            travelPosH--;
            travelPosD++;
            if (-travelPosH > ckingH - 1 || 8 - travelPosD <= ckingD - 1) {
                check1 = true;
            }
            if (8 + travelPosH <= ckingH - 1 || travelPosD > ckingD - 1) {
                check2 = true;
            }
            if (!check1 && pos[ckingH - 1 + travelPosH][ckingD - 1 + travelPosD] != 0 && pos[ckingH - 1 + travelPosH][ckingD - 1 + travelPosD] != 7 && pos[ckingH - 1 + travelPosH][ckingD - 1 + travelPosD] != 17) {
                check1 = true;

                if (check1 && (pos[ckingH - 1 + travelPosH][ckingD - 1 + travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 + travelPosH][ckingD - 1 + travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[0][0] += caseV;
                }
            }
            if (!check2 && pos[ckingH - 1 - travelPosH][ckingD - 1 - travelPosD] != 0 && pos[ckingH - 1 - travelPosH][ckingD - 1 - travelPosD] != 7 && pos[ckingH - 1 - travelPosH][ckingD - 1 - travelPosD] != 17) {
                check2 = true;

                if (check2 && (pos[ckingH - 1 - travelPosH][ckingD - 1 - travelPosD] == 2 + 10 * colorV || pos[ckingH - 1 - travelPosH][ckingD - 1 - travelPosD] == 5 + 10 * colorV)) {
                    kingMatrix[0][0] += caseV;
                }
            }
        }
    }
    if (color) {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                wkingMatrix[i][l] = kingMatrix[i][l];
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                bkingMatrix[i][l] = kingMatrix[i][l];
            }
        }
    }
}


void Chess::GenerateMoveList() {
    moveList.clear();
    bool check1, check2, kingMatrix[3][3], CastleMatrix[8], runCastleMatrix;
    short ckingH, ckingD, colorV, travelPosH, travelPosD;

    ckingH = (turn) ? wkingH : bkingH;
    ckingD = (turn) ? wkingD : bkingD;

    if (turn) {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                if (wkingMatrix[i][l] == NULL) {
                    wkingMatrix[i][l] = 0;
                }
                kingMatrix[i][l] = wkingMatrix[i][l];
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            for (int l = 0; l < 3; l++) {
                if (bkingMatrix[i][l] == NULL) {
                    bkingMatrix[i][l] = 0;
                }
                kingMatrix[i][l] = bkingMatrix[i][l];
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int l = 0; l < 8; l++) {
            colorV = (pos[i][l] > 10) ? 1 : 0;
            if ((colorV == 1 && turn) || (colorV == 0 && !turn) || (pos[i][l] == 0) || (pos[i][l] == 7) || (pos[i][l] == 17)) {
                continue;
            }
            if (pos[i][l] == 1 + 10 * colorV && !doubleCheck) {
                if (flags[i][l] == '~') {
                    if (pos[i + 1 - 2 * colorV][l] == 0 && (!inCheck || (checkV = 4 && i + 1 - 2 * colorV == apH && l > ckingD && l < apD) || (checkV = 8 && i + 1 - 2 * colorV == apH && l < ckingD && l > apD))) {
                        if (i == 6 - 5 * colorV) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1 - 2 * colorV) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1 - 2 * colorV) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1 - 2 * colorV) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1 - 2 * colorV) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1 - 2 * colorV));
                        }
                    }

                    if (i == 1 + 5 * colorV && pos[i + 1 - 2 * colorV][l] == 0 && pos[i + 2 - 4 * colorV][l] == 0 && (!inCheck || (checkV = 4 && i + 2 - 4 * colorV == apH && l > ckingD && l < apD) || (checkV = 8 && i + 2 - 4 * colorV == apH && l < ckingD && l > apD))) {
                        moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 2 - 4 * colorV));
                    }

                    if (l > 0 && pos[i + 1 - 2 * colorV][l - 1] != 0 && (1 - 2 * colorV) * (pos[i + 1 - 2 * colorV][l - 1]) < (1 - 2 * colorV) * 10 && (!inCheck || (l - 1 == apD && i + 1 - 2 * colorV == apH))) {
                        if (i == 6 - 5 * colorV) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV) + "=B");
                        }
                        else {
                            if (pos[i + 1 - 2 * colorV][l - 1] == 7 + 10 * colorV && i == ckingH) {
                                pos[i][l] = 0;
                                pos[i][l - 1] = 0;
                                if (!isInCheck(turn, 0)) {
                                    moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV));
                                }
                                pos[i][l] = 1 + 10 * colorV;
                                pos[i][l - 1] = 11 - 10 * colorV;
                            }
                            else {
                                moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1 - 2 * colorV));
                            }
                        }
                    }

                    if (l < 7 && pos[i + 1 - 2 * colorV][l + 1] != 0 && (1 - 2 * colorV) * (pos[i + 1 - 2 * colorV][l + 1]) >(1 - 2 * colorV) * 10 && (!inCheck || (l + 1 == apD && i + 1 - 2 * colorV == apH))) {
                        if (i == 6 - 5 * colorV) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV) + "=B");
                        }
                        else {
                            if (pos[i + 1 - 2 * colorV][l + 1] == 7 + 10 * colorV && i == ckingH) {
                                pos[i][l] = 0;
                                pos[i][l + 1] = 0;
                                if (!isInCheck(turn, 0)) {
                                    moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV));
                                }
                                pos[i][l] = 1 + 10 * colorV;
                                pos[i][l + 1] = 11 - 10 * colorV;
                            }
                            else {
                                moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1 - 2 * colorV));
                            }
                        }
                    }
                }
                else if (flags[i][l] == '8') {
                    if (wkingD == l && pos[i + 1][l] == 0 && (!inCheck || (checkV = 4 && i + 1 == apH && l > ckingD && l < apD) || (checkV = 8 && i + 1 == apH && l < ckingD && l > apD))) {
                        if (i == 6) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 1));
                        }
                    }

                    if (i == 1 && pos[i + 1][l] == 0 && pos[i + 2][l] == 0 && (!inCheck || (checkV = 4 && i + 2 == apH && l > ckingD && l < apD) || (checkV = 8 && i + 2 == apH && l < ckingD && l > apD))) {
                        moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i + 2));
                    }
                }
                else if (flags[i][l] == '9') {
                    if (l > 0 && pos[i + 1][l - 1] != 0 && wkingH - i == l - wkingD && (pos[i + 1][l - 1]) > 10 && (!inCheck || (l - 1 == apD && i + 1 == apH))) {
                        if (i == 6) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 1));
                        }
                    }
                    if (l < 7 && pos[i + 1][l + 1] != 0 && wkingH - i == wkingD - l && (pos[i + 1][l + 1]) > 10 && (!inCheck || (l + 1 == apD && i + 1 == apH))) {
                        if (i == 6) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 1));
                        }
                    }
                }
                else if (flags[i][l] == 'o') {
                    if (bkingD == 6 && pos[i - 1][l] == 0 && (!inCheck || (checkV = 4 && i - 1 == apH && l > ckingD && l < apD) || (checkV = 8 && i - 1 == apH && l < ckingD && l > apD))) {
                        if (i == 1) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 1));
                        }
                    }

                    if (i == 6 && pos[i - 1][l] == 0 && pos[i - 2][l] == 0 && (!inCheck || (checkV = 4 && i - 2 == apH && l > ckingD && l < apD) || (checkV = 8 && i - 2 == apH && l < ckingD && l > apD))) {
                        moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l) + to_string(i - 2));
                    }
                }
                else if (flags[i][l] == 'p') {
                    if (l > 0 && pos[i - 1][l - 1] != 0 && bkingH - i == l - bkingD && (pos[i - 1][l - 1]) < 10 && (!inCheck || (l - 1 == apD && i - 1 == apH))) {
                        if (i == 1) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 1));
                        }
                    }
                    if (l < 7 && pos[i + 1][l + 1] != 0 && bkingH - i == bkingD - l && (pos[i - 1][l + 1]) < 10 && (!inCheck || (l + 1 == apD && i - 1 == apH))) {
                        if (i == 1) {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 1) + "=Q");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 1) + "=R");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 1) + "=N");
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 1) + "=B");
                        }
                        else {
                            moveList.push_back("P" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 1));
                        }
                    }
                }
            }
            else if (pos[i][l] == 2 + 10 * colorV && (flags[i][l] == 'F' - 3 * colorV || flags[i][l] == 'm' - 55 * colorV || flags[i][l] == 'n' - 55 * colorV || flags[i][l] == 'J' + 4 * colorV) && !doubleCheck) {
                travelPosH = i;
                travelPosD = l;
                while (travelPosH < 7 && travelPosD < 7 && (flags[i][l] != 'J' + 4 * colorV || ckingH - i == ckingD - l)) {
                    travelPosH++;
                    travelPosD++;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH < 7 && travelPosD > 0 && (flags[i][l] != 'J' + 4 * colorV || ckingH - i == l - ckingD)) {
                    travelPosH++;
                    travelPosD--;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH > 0 && travelPosD < 7 && (flags[i][l] != 'J' + 4 * colorV || ckingH - i == l - ckingD)) {
                    travelPosH--;
                    travelPosD++;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH > 0 && travelPosD > 0 && (flags[i][l] != 'J' + 4 * colorV || ckingH - i == ckingD - l)) {
                    travelPosH--;
                    travelPosD--;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("B" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }
            }
            else if (pos[i][l] == 3 + 10 * colorV && flags[i][l] == '~' && !doubleCheck) {
                if (l < 7 && i < 6) {
                    if (pos[i + 2][l + 1] == 0 && (!inCheck || (checkV % 4 == 1 && i + 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 2 == ckingH) || (checkV % 4 == 2 && l + 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 2));
                    }
                    else if (pos[i + 2][l + 1] == 7 || pos[i + 2][l + 1] == 17 && (!inCheck || (checkV % 4 == 1 && i + 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 2 == ckingH) || (checkV % 4 == 2 && l + 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 2));
                    }
                    else if ((1 - 2 * colorV) * (pos[i + 2][l + 1]) > (1 - 2 * colorV) * 10 && (!inCheck || (l + 1 == apD && i + 2 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i + 2));
                    }
                }

                if (l > 0 && i < 6) {
                    if (pos[i + 2][l - 1] == 0 && (!inCheck || (checkV % 4 == 1 && i + 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 2 == ckingH) || (checkV % 4 == 2 && l - 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 2));
                    }
                    else if (pos[i + 2][l - 1] == 7 || pos[i + 2][l - 1] == 17 && (!inCheck || (checkV % 4 == 1 && i + 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 2 == ckingH) || (checkV % 4 == 2 && l - 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 2));
                    }
                    else if ((1 - 2 * colorV) * (pos[i + 2][l - 1]) > (1 - 2 * colorV) * 10 && (!inCheck || (l - 1 == apD && i + 2 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i + 2));
                    }
                }

                if (l > 1 && i < 7) {
                    if (pos[i + 1][l - 2] == 0 && (!inCheck || (checkV % 4 == 1 && i + 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 1 == ckingH) || (checkV % 4 == 2 && l - 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i + 1));
                    }
                    else if (pos[i + 1][l - 2] == 7 || pos[i + 1][l - 2] == 17 && (!inCheck || (checkV % 4 == 1 && i + 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 1 == ckingH) || (checkV % 4 == 2 && l - 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i + 1));
                    }
                    else if ((1 - 2 * colorV) * (pos[i + 1][l - 2]) > (1 - 2 * colorV) * 10 && (!inCheck || (l - 2 == apD && i + 1 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i + 1));
                    }
                }

                if (l > 1 && i > 0) {
                    if (pos[i - 1][l - 2] == 0 && (!inCheck || (checkV % 4 == 1 && i + 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 1 == ckingH) || (checkV % 4 == 2 && l - 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i - 1));
                    }
                    else if (pos[i - 1][l - 2] == 7 || pos[i - 1][l - 2] == 17 && (!inCheck || (checkV % 4 == 1 && i + 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i + 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 1 == ckingH) || (checkV % 4 == 2 && l - 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i - 1));
                    }
                    else if ((1 - 2 * colorV) * (pos[i - 1][l - 2]) > (1 - 2 * colorV) * 10 && (!inCheck || (l - 2 == apD && i - 1 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 2) + to_string(i - 1));
                    }
                }

                if (l > 0 && i > 1) {
                    if (pos[i - 2][l - 1] == 0 && (!inCheck || (checkV % 4 == 1 && i - 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 2 == ckingH) || (checkV % 4 == 2 && l - 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 2));
                    }
                    else if (pos[i - 2][l - 1] == 7 || pos[i - 2][l - 1] == 17 && (!inCheck || (checkV % 4 == 1 && i - 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 2 == ckingH) || (checkV % 4 == 2 && l - 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 2));
                    }
                    else if ((1 - 2 * colorV) * (pos[i - 2][l - 1]) > (1 - 2 * colorV) * 10 && (!inCheck || (l - 1 == apD && i - 2 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l - 1) + to_string(i - 2));
                    }
                }

                if (l < 7 && i > 1) {
                    if (pos[i - 2][l + 1] == 0 && (!inCheck || (checkV % 4 == 1 && i - 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 2 == ckingH) || (checkV % 4 == 2 && l + 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 2));
                    }
                    else if (pos[i - 2][l + 1] == 7 || pos[i - 2][l + 1] == 17 && (!inCheck || (checkV % 4 == 1 && i - 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 2 == ckingH) || (checkV % 4 == 2 && l + 1 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 2));
                    }
                    else if ((1 - 2 * colorV) * (pos[i - 2][l + 1]) > (1 - 2 * colorV) * 10 && (!inCheck || (l + 1 == apD && i - 2 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 1) + to_string(i - 2));
                    }
                }

                if (l < 6 && i < 7) {
                    if (pos[i + 1][l + 2] == 0 && (!inCheck || (checkV % 4 == 1 && i - 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 1 == ckingH) || (checkV % 4 == 2 && l + 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i + 1));
                    }
                    else if (pos[i + 1][l + 2] == 7 || pos[i + 1][l + 2] == 17 && (!inCheck || (checkV % 4 == 1 && i - 1 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 1 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i + 1 == ckingH) || (checkV % 4 == 2 && l + 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i + 1));
                    }
                    else if ((1 - 2 * colorV) * (pos[i + 1][l + 2]) > (1 - 2 * colorV) * 10 && (!inCheck || (l + 2 == apD && i + 1 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i + 1));
                    }
                }

                if (l < 6 && i > 0) {
                    if (pos[i - 1][l + 2] == 0 && (!inCheck || (checkV % 4 == 1 && i - 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 1 == ckingH) || (checkV % 4 == 2 && l + 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i - 1));
                    }
                    else if (pos[i - 1][l + 2] == 7 || pos[i - 1][l + 2] == 17 && (!inCheck || (checkV % 4 == 1 && i - 3 - l == ckingD - ckingH) || (checkV % 4 == 3 && i - 3 - l == ckingH - ckingD) || (checkV % 4 == 0 && checkV != 0 && i - 1 == ckingH) || (checkV % 4 == 2 && l + 2 == ckingD))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i - 1));
                    }
                    else if ((1 - 2 * colorV) * (pos[i - 1][l + 2]) > (1 - 2 * colorV) * 10 && (!inCheck || (l + 2 == apD && i - 1 == apH))) {
                        moveList.push_back("N" + to_string(l) + to_string(i) + to_string(l + 2) + to_string(i - 1));
                    }
                }
            }
            //ROOK MOVES
            //cout << (pos[i][l] == 4 + 10 * colorV && (flags[i][l] == 'B' + 3 * colorV || flags[i][l] == 'k' - 55 * colorV || flags[i][l] == 'l' - 55 * colorV || flags[i][l] == 'M' - 4 * colorV)) << endl;
            else if (pos[i][l] == 4 + 10 * colorV && (flags[i][l] == 'E' - 3 * colorV || flags[i][l] == 'k' - 55 * colorV || flags[i][l] == 'l' - 55 * colorV || flags[i][l] == 'I' + 4 * colorV) && !doubleCheck) {
                travelPosH = i;
                travelPosD = l;
                //cout << "hi\n";
                while (travelPosH < 7 && (flags[i][l] != 'I' + 4 * colorV || l == ckingD)) {
                    travelPosH++;
                    if (pos[travelPosH][l] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][l]) > (1 - 2 * colorV) * 10 && pos[travelPosH][l] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][l] == 7 || pos[travelPosH][l] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                while (travelPosH > 0 && (flags[i][l] != 'I' + 4 * colorV || l == ckingD)) {
                    travelPosH--;
                    if (pos[travelPosH][l] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][l]) > (1 - 2 * colorV) * 10 && pos[travelPosH][l] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][l] == 7 || pos[travelPosH][l] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                while (travelPosD < 7 && (flags[i][l] != 'I' + 4 * colorV || i == ckingH)) {
                    travelPosD++;
                    if (pos[i][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else if ((1 - 2 * colorV) * (pos[i][travelPosD]) > (1 - 2 * colorV) * 10 && pos[i][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                        break;
                    }
                    else if (pos[i][travelPosD] == 7 || pos[i][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else {
                        break;
                    }
                }

                travelPosD = l;
                while (travelPosD > 0 && (flags[i][l] != 'I' + 4 * colorV || i == ckingH)) {
                    travelPosD--;
                    if (pos[i][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else if ((1 - 2 * colorV) * (pos[i][travelPosD]) > (1 - 2 * colorV) * 10 && pos[i][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                        break;
                    }
                    else if (pos[i][travelPosD] == 7 || pos[i][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("R" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else {
                        break;
                    }
                }
            }
            //QUEEN MOVES
            else if (pos[i][l] == 5 + 10 * colorV && (flags[i][l] == 'G' + 4 * colorV || flags[i][l] == 'H' + 4 * colorV || flags[i][l] == 'D' - 3 * colorV) && !doubleCheck) {
                travelPosH = i;
                travelPosD = l;
                while (travelPosH < 7 && travelPosD < 7 && (flags[i][l] != 'H' + 4 * colorV || ckingH - i == ckingD - l)) {
                    travelPosH++;
                    travelPosD++;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH < 7 && travelPosD > 0 && (flags[i][l] != 'H' + 4 * colorV || ckingH - i == l - ckingD)) {
                    travelPosH++;
                    travelPosD--;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH > 0 && travelPosD < 7 && (flags[i][l] != 'H' + 4 * colorV || ckingH - i == l - ckingD)) {
                    travelPosH--;
                    travelPosD++;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH > 0 && travelPosD > 0 && (flags[i][l] != 'H' + 4 * colorV || ckingH - i == ckingD - l)) {
                    travelPosH--;
                    travelPosD--;
                    if (pos[travelPosH][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][travelPosD]) > (1 - 2 * colorV) * 10 && pos[travelPosH][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] == 7 || pos[travelPosH][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                travelPosD = l;
                while (travelPosH < 7 && (flags[i][l] != 'G' + 4 * colorV || l == ckingD)) {
                    travelPosH++;
                    if (pos[travelPosH][l] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][l]) > (1 - 2 * colorV) * 10 && pos[travelPosH][l] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][l] == 7 || pos[travelPosH][l] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                while (travelPosH > 0 && (flags[i][l] != 'G' + 4 * colorV || l == ckingD)) {
                    travelPosH--;
                    if (pos[travelPosH][l] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else if ((1 - 2 * colorV) * (pos[travelPosH][l]) > (1 - 2 * colorV) * 10 && pos[travelPosH][l] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                        break;
                    }
                    else if (pos[travelPosH][l] == 7 || pos[travelPosH][l] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(l) + to_string(travelPosH));
                    }
                    else {
                        break;
                    }
                }

                travelPosH = i;
                while (travelPosD < 7 && (flags[i][l] != 'G' + 4 * colorV || i == ckingH)) {
                    travelPosD++;
                    if (pos[i][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else if ((1 - 2 * colorV) * (pos[i][travelPosD]) > (1 - 2 * colorV) * 10 && pos[i][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                        break;
                    }
                    else if (pos[i][travelPosD] == 7 || pos[i][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else {
                        break;
                    }
                }

                travelPosD = l;
                while (travelPosD > 0 && (flags[i][l] != 'G' + 4 * colorV || i == ckingH)) {
                    travelPosD--;
                    if (pos[i][travelPosD] == 0 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else if ((1 - 2 * colorV) * (pos[i][travelPosD]) > (1 - 2 * colorV) * 10 && pos[i][travelPosD] != 17 - colorV && (!inCheck || (travelPosD == apD && travelPosH == apH))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                        break;
                    }
                    else if (pos[i][travelPosD] == 7 || pos[i][travelPosD] == 17 && (!inCheck || (checkV % 4 == 1 && travelPosD - travelPosH == ckingH - ckingD) || (checkV % 4 == 3 && travelPosD - travelPosH == ckingD - ckingH) || (checkV % 4 == 0 && checkV != 0 && travelPosH == ckingH) || (checkV % 4 == 2 && travelPosD == ckingD))) {
                        moveList.push_back("Q" + to_string(l) + to_string(i) + to_string(travelPosD) + to_string(i));
                    }
                    else {
                        break;
                    }
                }
            }
            //KING MOVES
            else if (pos[i][l] == 6 + 10 * colorV) {
                for (int j = -1; j < 2; j++) {
                    for (int k = -1; k < 2; k++) {
                        if (j == 0 && k == 0) {
                            continue;
                        }
                        if (l + k > -1 && l + k < 8 && i + j > -1 && i + j < 8 && kingMatrix[j + 1][k + 1] == 0 && (pos[i + j][l + k] == 0 || pos[i + j][l + k] == 17 - 10 * colorV || (1 - 2 * colorV) * pos[i + j][l + k] > (1 - 2 * colorV) * 10)) {
                            moveList.push_back("K" + to_string(l) + to_string(i) + to_string(l + k) + to_string(i + j));
                        }
                    }
                }
                if (!inCheck && turn && wleftRookD != -1) {
                    if (canCastleShortWhite) {
                        check1 = false;
                        check2 = false;
                        for (int j = (ckingD < 4) ? ckingD : 4; j < wrightRookD; j++) {
                            if (j == wrightRookD && pos[0][j] != 4) {
                                check1 = true;
                                break;
                            }
                            else if ((pos[0][j] == 0) || (pos[0][j] == 6)) {
                                continue;
                            }
                            else {
                                check1 = true;
                                break;
                            }
                        }
                        if (!check1) {
                            travelPosD = ckingD + 1;
                            if (kingMatrix[1][2] > 0) {
                                check2 = true;
                            }
                            while (travelPosD < 6) {
                                travelPosD++;
                                if (isInCheck(true, travelPosD - ckingD)) {
                                    check2 = true;
                                }
                            }
                        }
                        else {
                            check2 = true;
                        }
                        if (!check2) {
                            moveList.push_back("O-O");
                        }
                    }
                    if (canCastleLongWhite) {
                        check1 = false;
                        check2 = false;
                        for (int j = (ckingD > 4) ? ckingD : 4; j > (wleftRookD > 2) ? 2 : wleftRookD - 1; j--) {
                            if (j == wleftRookD && pos[0][j] != 4) {
                                check1 = true;
                                break;
                            }
                            else if ((pos[0][j] == 0) || (pos[0][j] == 6)) {
                                continue;
                            }
                            else {
                                check1 = true;
                                break;
                            }
                        }
                        if (!check1) {
                            if (ckingD == 1) {
                                if (isInCheck(true, 2 - ckingD)) {
                                    check2 = true;
                                }
                            }
                            else {
                                travelPosD = ckingD - 1;
                                if (kingMatrix[1][0] > 0) {
                                    check2 = true;
                                }
                                while (travelPosD > 2) {
                                    travelPosD--;
                                    if (isInCheck(true, travelPosD - ckingD)) {
                                        check2 = true;
                                    }
                                }
                            }
                        }
                        else {
                            check2 = true;
                        }
                        if (!check2) {
                            moveList.push_back("O-O-O");
                        }
                    }
                }
                else if (!inCheck && !turn && bleftRookD != -1) {
                    if (canCastleShortBlack) {
                        check1 = false;
                        check2 = false;
                        for (int j = (ckingD < 4) ? ckingD : 4; j < brightRookD; j++) {
                            if (j == brightRookD && pos[7][j] != 14) {
                                check1 = true;
                                break;
                            }
                            else if ((pos[7][j] == 0) || (pos[7][j] == 16)) {
                                continue;
                            }
                            else {
                                check1 = true;
                                break;
                            }
                        }
                        if (!check1) {
                            travelPosD = ckingD + 1;
                            if (kingMatrix[1][2] > 0) {
                                check2 = true;
                            }
                            while (travelPosD < 6) {
                                travelPosD++;
                                if (isInCheck(true, travelPosD - ckingD)) {
                                    check2 = true;
                                }
                            }
                        }
                        else {
                            check2 = true;
                        }
                        if (!check2) {
                            moveList.push_back("O-O");
                        }
                    }
                    if (canCastleLongBlack) {
                        check1 = false;
                        check2 = false;
                        for (int j = (ckingD > 4) ? ckingD : 4; j > (bleftRookD > 2) ? 2 : bleftRookD - 1; j--) {
                            if (j == bleftRookD && pos[7][j] != 14) {
                                check1 = true;
                                break;
                            }
                            else if ((pos[7][j] == 0) || (pos[7][j] == 16)) {
                                continue;
                            }
                            else {
                                check1 = true;
                                break;
                            }
                        }
                        if (!check1) {
                            if (ckingD == 1) {
                                if (isInCheck(true, 2 - ckingD)) {
                                    check2 = true;
                                }
                            }
                            else {
                                travelPosD = ckingD - 1;
                                if (kingMatrix[1][0] > 0) {
                                    check2 = true;
                                }
                                while (travelPosD > 2) {
                                    travelPosD--;
                                    if (isInCheck(true, travelPosD - ckingD)) {
                                        check2 = true;
                                    }
                                }
                            }
                        }
                        else {
                            check2 = true;
                        }
                        if (!check2) {
                            moveList.push_back("O-O-O");
                        }
                    }
                }
            }
        }
    }
}


void Chess::NextTurn(string move1) {
    bool diagonalPin1 = false;
    bool directionalPin1 = false;
    bool diagonalPin2 = false;
    bool directionalPin2 = false;
    bool directionalPin3 = false;
    bool diagonalPin3 = false;
    short posSave = 0;
    short storeH, storeD, piece, travelPosD, travelPosH, dis2k1, dis2k2;

    short colorV = (turn) ? 0 : 1;

    short ckingH = (turn) ? wkingH : bkingH;
    short ckingD = (turn) ? wkingD : bkingD;

    short okingH = (!turn) ? wkingH : bkingH;
    short okingD = (!turn) ? wkingD : bkingD;

    string move = move1;

    if (::tolower(move[0]) == 'p') {
        piece = 1 + 10 * colorV;
    }
    else if (::tolower(move[0]) == 'b') {
        piece = 2 + 10 * colorV;
    }
    else if (::tolower(move[0]) == 'n') {
        piece = 3 + 10 * colorV;
    }
    else if (::tolower(move[0]) == 'r') {
        piece = 4 + 10 * colorV;
    }
    else if (::tolower(move[0]) == 'q') {
        piece = 5 + 10 * colorV;
    }
    else if (::tolower(move[0]) == 'k') {
        piece = 6 + 10 * colorV;
    }
    else {
        move = 'P' + move;
        piece = 1 + 10 * colorV;
    }
    if (::tolower(move[0]) == move[0]) {
        move[0] = toupper(move[0]);
    }

    /*if (!strictpieceinput) {
        for (int i = 0; i < sizeof(move); i++) {
            if (move[0] == 'O') {
                if (sizeof(move) == 3) {
                    move = "O-O";
                }
                else if (sizeof(move) == 5) {
                    move = "O-O-O";
                }
                else {
                    cout << "Error: invalid move\n";
                }
                break;
            }
            else if (i == 0) {
                continue;
            }

            if (move[i] > 47 && move[i] < 56) {
                if (sizeof(move) - i == 2) {
                    if (pos1D == NULL) {
                        for (int j = 0; j < 8; j++) {
                            for (int k = 0; k < 8; k++) {
                                if (pos[j][k] == piece) {
                                    if (piece % 10 == 1) {

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }*/

    for (int i = 0; i < moveList.size(); i++) {
        if (move.compare(moveList[i]) == 0) {
            break;
        }
        else if (moveList.size() - 1 == i) {
            cout << "Error: move not in move list; illegal move\n";
            return;
        }
    }

    if (move[0] == 'O') {
        Chess::UpdateKingMatrix(!turn, -3);
        if (sizeof(move) == 3 && turn) {
            pos[0][wkingD] = 0;
            pos[0][wrightRookD] = 0;
            pos[0][6] = 6;
            wkingD = 6;
            pos[0][5] = 4;
            wleftRookD = -1;
            canCastleShortWhite = false;
            canCastleLongWhite = false;
            flags[0][wrightRookD] = '~';
        }
        else if (sizeof(move) == 3 && !turn) {
            pos[0][wkingD] = 0;
            pos[0][wleftRookD] = 0;
            pos[0][2] = 6;
            wkingD = 2;
            pos[0][3] = 4;
            wleftRookD = -1;
            canCastleShortWhite = false;
            canCastleLongWhite = false;
            flags[0][wleftRookD] = '~';
        }
        else if (turn) {
            pos[7][bkingD] = 0;
            pos[7][brightRookD] = 0;
            pos[7][6] = 16;
            bkingD = 6;
            pos[7][5] = 14;
            bleftRookD = -1;
            canCastleShortBlack = false;
            canCastleLongBlack = false;
            flags[7][wrightRookD] = '~';
        }
        else {
            pos[7][bkingD] = 0;
            pos[7][bleftRookD] = 0;
            pos[7][2] = 16;
            bkingD = 2;
            pos[7][3] = 14;
            bleftRookD = -1;
            canCastleShortBlack = false;
            canCastleLongBlack = false;
            flags[7][wleftRookD] = '~';
        }
        Chess::UpdateFlagList(colorV + 1);
        Chess::UpdateKingMatrix(turn, 0);
        Chess::UpdateKingMatrix(!turn, 3);
    }
    else {
        //UPDATE PAWN FLAGS
        if (piece % 10 == 1) {
            if (abs(move[4] - move[2]) == 2) {
                pos[move[2] - 48 + 1 - 2 * colorV][move[1] - 48] = 7 + 10 * colorV;
                if (turn) {
                    winvisP = move[1] - 48;
                }
                else {
                    binvisP = move[1] - 48;
                }
            }

            if (inCheck) {
                if (okingH == move[4] - 48) {
                    flags[move[4] - 48][move[3] - 48] = 'o' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'p' - 55 * colorV;
                }
            }

            if (move[3] - okingD < 3 && okingD - move[3] < 3 && move[4] - okingH < 2 * colorV && okingH - move[4] < 2 - 2 * colorV) {
                Chess::UpdateKingMatrix(!turn, -2);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 2);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
        }
        //UPDATES BISHOP FLAGS
        else if (piece % 10 == 2) {
            if (inCheck) {
                if (move[4] - 48 == ckingH || move[3] - 48 == ckingD) {
                    flags[move[4] - 48][move[3] - 48] = 't' - 16 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'N' - 4 * colorV;
                }
                diagonalPin1 = true;
                diagonalPin2 = true;
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'F' - 3 * colorV || flags[move[2] - 48][move[1] - 48] == 't' - 16 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'm' - 55 * colorV;
                    diagonalPin1 = true;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'n' - 55 * colorV;
                    diagonalPin1 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'F' - 3 * colorV;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'm' - 55 * colorV) {
                if ((move[3] - move[1]) * (move[4] - move[2]) > 0) {
                    flags[move[4] - 48][move[3] - 48] = 'm' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'F' - 3 * colorV;
                    diagonalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'N' - 4 * colorV) {
                diagonalPin1 = true;
                diagonalPin2 = true;
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'n' - 55 * colorV) {
                if ((move[3] - move[1]) * (move[4] - move[2]) < 0) {
                    flags[move[4] - 48][move[3] - 48] = 'n' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'F' - 3 * colorV;
                    diagonalPin2 = true;
                }
            }
            /*
            if (abs(okingH - okingD - move[4] + move[3]) < 3) {
                if (okingH - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[4] - move[3] + 96) < 3) {
                if (okingH - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - okingD - move[2] + move[1]) < 3) {
                if (okingH - move[2] == okingD - move[1] && okingH - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] == okingD - move[1] && okingH + 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] == okingD - move[1] && okingH + 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] == okingD - move[1] && okingH - 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] == okingD - move[1] && okingH - 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[2] - move[1] + 96) < 3) {
                if (okingH - move[2] + 96 == move[1] - okingD && okingH - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] + 96 == move[1] - okingD && okingH + 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] + 96 == move[1] - okingD && okingH + 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] + 96 == move[1] - okingD && okingH - 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] + 96 == move[1] - okingD && okingH - 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            */
            flags[move[2] - 48][move[1] - 48] = '~';
        }
        //UPDATE KNIGHT FLAGS
        else if (piece % 10 == 3) {
            if (inCheck) {
                if (okingH == move[4] - 48 || okingD == move[3] - 48) {
                    flags[move[4] - 48][move[3] - 48] = 'q' - 16 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'r' - 16 * colorV;
                }
            }
            Chess::UpdateKingMatrix(!turn, -1);
            pos[move[2] - 48][move[1] - 48] = 0;
            posSave = pos[move[4] - 48][move[3] - 48];
            pos[move[4] - 48][move[3] - 48] = piece;
            Chess::UpdateKingMatrix(!turn, 1);
            pos[move[4] - 48][move[3] - 48] = posSave;
            pos[move[2] - 48][move[1] - 48] = piece;
        }
        //UPDATE ROOK FLAGS
        else if (piece % 10 == 4) {
            cout << colorV << endl;
            if (inCheck) {
                if (move[4] - 48 == ckingH || move[3] - 48 == ckingD) {
                    flags[move[4] - 48][move[3] - 48] = 'I' + 4 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 's' - 16 * colorV;
                }
                directionalPin1 = true;
                directionalPin2 = true;
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'B' + 3 * colorV || flags[move[2] - 48][move[1] - 48] == 's' - 16 * colorV) {
                if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'k' - 55 * colorV;
                    directionalPin1 = true;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'l' - 55 * colorV;
                    directionalPin1 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'B' + 3 * colorV;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'k' - 55 * colorV) {
                if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'k' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'E' - 3 * colorV;
                    directionalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'M' - 4 * colorV) {
                directionalPin1 = true;
                directionalPin2 = true;
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'l' - 55 * colorV) {
                if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'l' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'E' - 3 * colorV;
                    directionalPin2 = true;
                }
            }
            /*
            if (abs(okingD - move[3] + 48) < 2) {
                if (okingD == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[4] + 48) < 2) {
                if (okingH == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingD - move[1] + 48) < 2) {
                if (okingD == move[1] - 48 && okingD != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[1] - 48 && okingD + 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[1] - 48 && okingD - 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[2] + 48) < 2) {
                if (okingH == move[2] - 48 && okingH != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[2] - 48 && okingH + 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[2] - 48 && okingH - 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            */
            flags[move[2] - 48][move[1] - 48] = '~';
        }
        //UPDATE QUEEN FLAGS
        else if (piece % 10 == 5) {
            if (inCheck) {
                if (move[4] - 48 == ckingH || move[3] - 48 == ckingD) {
                    flags[move[4] - 48][move[3] - 48] = 'K' - 4 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'L' - 4 * colorV;
                }
                diagonalPin1 = true;
                directionalPin1 = true;
                diagonalPin2 = true;
                directionalPin2 = true;
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'D' - 3 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'i' - 55 * colorV;
                    diagonalPin1 = true;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'j' - 55 * colorV;
                    diagonalPin1 = true;
                }
                else if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'g' - 55 * colorV;
                    directionalPin1 = true;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'h' - 55 * colorV;
                    directionalPin1 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'D' - 3 * colorV;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'i' - 55 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'i' - 55 * colorV;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'j' - 55 * colorV;
                    diagonalPin1 = true;
                    diagonalPin2 = true;
                }
                else if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'g' - 55 * colorV;
                    directionalPin1 = true;
                    diagonalPin2 = true;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'h' - 55 * colorV;
                    directionalPin1 = true;
                    diagonalPin2 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'D' - 3 * colorV;
                    diagonalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'j' - 55 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'i' - 55 * colorV;
                    diagonalPin1 = true;
                    diagonalPin2 = true;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'j' - 55 * colorV;
                }
                else if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'g' - 55 * colorV;
                    directionalPin1 = true;
                    diagonalPin2 = true;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'h' - 55 * colorV;
                    directionalPin1 = true;
                    diagonalPin2 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'D' - 3 * colorV;
                    diagonalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'h' - 55 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'i' - 55 * colorV;
                    diagonalPin1 = true;
                    directionalPin2 = true;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'j' - 55 * colorV;
                    diagonalPin1 = true;
                    directionalPin2 = true;
                }
                else if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'g' - 55 * colorV;
                    directionalPin1 = true;
                    directionalPin2 = true;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'h' - 55 * colorV;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'D' - 3 * colorV;
                    directionalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'g' - 55 * colorV) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    flags[move[4] - 48][move[3] - 48] = 'i' - 55 * colorV;
                    diagonalPin1 = true;
                    directionalPin2 = true;
                }
                else if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'j' - 55 * colorV;
                    diagonalPin1 = true;
                    directionalPin2 = true;
                }
                else if (move[4] - 48 == okingH) {
                    flags[move[4] - 48][move[3] - 48] = 'g' - 55 * colorV;
                }
                else if (move[3] - 48 == okingD) {
                    flags[move[4] - 48][move[3] - 48] = 'h' - 55 * colorV;
                    directionalPin1 = true;
                    directionalPin2 = true;
                }
                else {
                    flags[move[4] - 48][move[3] - 48] = 'D' - 3 * colorV;
                    directionalPin2 = true;
                }
            }
            else if (flags[move[2] - 48][move[1] - 48] == 'K' - 4 * colorV || flags[move[2] - 48][move[1] - 48] == 'L' - 4 * colorV) {
                directionalPin1 = true;
                directionalPin2 = true;
                diagonalPin1 = true;
                diagonalPin2 = true;
            }
            /*
            if (abs(okingH - okingD - move[4] + move[3]) < 3) {
                if (okingH - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[4] - move[3] + 96) < 3) {
                if (okingH - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingD - move[3] + 48) < 2) {
                if (okingD == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[4] + 48) < 2) {
                if (okingH == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - okingD - move[2] + move[1]) < 3) {
                if (okingH - move[2] == okingD - move[1] && okingH - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] == okingD - move[1] && okingH + 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] == okingD - move[1] && okingH + 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] == okingD - move[1] && okingH - 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] == okingD - move[1] && okingH - 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[2] - move[1] + 96) < 3) {
                if (okingH - move[2] + 96 == move[1] - okingD && okingH - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] + 96 == move[1] - okingD && okingH + 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] + 96 == move[1] - okingD && okingH + 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] + 96 == move[1] - okingD && okingH - 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] + 96 == move[1] - okingD && okingH - 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingD - move[1] + 48) < 2) {
                if (okingD == move[1] - 48 && okingD != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[1] - 48 && okingD + 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[1] - 48 && okingD - 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[2] + 48) < 2) {
                if (okingH == move[2] - 48 && okingH != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[2] - 48 && okingH + 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[2] - 48 && okingH - 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            */
            flags[move[2] - 48][move[1] - 48] = '~';
        }
        //UPDATE KING FLAGS & MATRIX
        if (piece % 10 == 6) {
            Chess::UpdateKingMatrix(!turn, -3);
            if (turn) {
                wkingH = move[4] - 48;
                wkingD = move[3] - 48;
            }
            else {
                bkingH = move[4] - 48;
                bkingD = move[3] - 48;
            }
            pos[move[2] - 48][move[1] - 48] = 0;
            pos[move[4] - 48][move[3] - 48] = piece;
            Chess::UpdateFlagList(colorV + 1);
            Chess::UpdateKingMatrix(turn, 0);
            Chess::UpdateKingMatrix(!turn, 3);
        }
        else {
            if (ckingH - move[4] + 48 == ckingD - move[3] + 48 || ckingH - move[4] + 48 == move[3] - 48 - ckingD) {
                diagonalPin3 = true;
            }
            else if (move[4] - 48 == ckingH || move[3] - 48 == ckingD) {
                directionalPin3 = true;
            }
            if (okingH - move[4] + 48 == okingD - move[3] + 48 || okingH - move[4] + 48 == move[3] - 48 - okingD) {
                diagonalPin1 = true;
            }
            else if (move[4] - 48 == okingH || move[3] - 48 == okingD) {
                directionalPin1 = true;
            }
            /*
            if (okingH - move[4] + 48 == okingD - move[3] + 48 || okingH - move[2] + 48 == okingD - move[1] + 48) {
                if (okingH - move[4] + 48 == okingD - move[3] + 48) {
                    diagonalPin3 = true;
                }
                Chess::UpdateKingMatrix(!turn, -10);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 10);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH - move[4] + 48 == move[3] - 48 - okingD || okingH - move[2] + 48 == move[1] - 48 - okingD) {
                if (okingH - move[4] + 48 == move[3] - 48 - okingD) {
                    diagonalPin3 = true;
                }
                Chess::UpdateKingMatrix(!turn, -15);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 15);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[4] - 48 == okingH || move[2] - 48 == okingH) {
                if (move[4] - 48 == okingH) {
                    directionalPin3 = true;
                }
                Chess::UpdateKingMatrix(!turn, -7);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 7);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[3] - 48 == okingD || move[1] - 48 == okingD) {
                if (move[3] - 48 == okingD) {
                    directionalPin3 = true;
                }
                Chess::UpdateKingMatrix(!turn, -4);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 4);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[3] - 48 == okingD + 1 || move[1] - 48 == okingD + 1) {
                Chess::UpdateKingMatrix(!turn, -5);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 5);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[3] - 48 == okingD - 1 || move[1] - 48 == okingD - 1) {
                Chess::UpdateKingMatrix(!turn, -6);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 6);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[4] - 48 == okingH + 1 || move[2] - 48 == okingH + 1) {
                Chess::UpdateKingMatrix(!turn, -8);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 8);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (move[4] - 48 == okingH - 1 || move[2] - 48 == okingH - 1) {
                Chess::UpdateKingMatrix(!turn, -9);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 9);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH + 1 - move[4] + 48 == okingD - move[3] + 48 || okingH + 1 - move[2] + 48 == okingD - move[1] + 48) {
                Chess::UpdateKingMatrix(!turn, -11);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 11);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH + 1 - move[4] + 48 == okingD - 1 - move[3] + 48 || okingH + 1 - move[2] + 48 == okingD - 1 - move[1] + 48) {
                Chess::UpdateKingMatrix(!turn, -12);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 12);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH - 1 - move[4] + 48 == okingD - move[3] + 48 || okingH - 1 - move[2] + 48 == okingD - move[1] + 48) {
                Chess::UpdateKingMatrix(!turn, -13);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 13);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH - 1 - move[4] + 48 == okingD + 1 - move[3] + 48 || okingH - 1 - move[2] + 48 == okingD + 1 - move[1] + 48) {
                Chess::UpdateKingMatrix(!turn, -14);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 14);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH + 1 - move[4] + 48 == move[3] - 48 - okingD || okingH + 1 - move[2] + 48 == move[1] - 48 - okingD) {
                Chess::UpdateKingMatrix(!turn, -16);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 16);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH + 1 - move[4] + 48 == move[3] - 48 - okingD - 1 || okingH + 1 - move[2] + 48 == move[1] - 48 - okingD - 1) {
                Chess::UpdateKingMatrix(!turn, -17);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 17);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH - 1 - move[4] + 48 == move[3] - 48 - okingD || okingH - 1 - move[2] + 48 == move[1] - 48 - okingD) {
                Chess::UpdateKingMatrix(!turn, -18);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 18);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            else if (okingH - 1 - move[4] + 48 == move[3] - 48 - okingD + 1 || okingH - 1 - move[2] + 48 == move[1] - 48 - okingD + 1) {
                Chess::UpdateKingMatrix(!turn, -19);
                pos[move[2] - 48][move[1] - 48] = 0;
                posSave = pos[move[4] - 48][move[3] - 48];
                pos[move[4] - 48][move[3] - 48] = piece;
                Chess::UpdateKingMatrix(!turn, 19);
                pos[move[4] - 48][move[3] - 48] = posSave;
                pos[move[2] - 48][move[1] - 48] = piece;
            }
            */
            if (abs(okingH - okingD - move[4] + move[3]) < 3) {
                if (okingH - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] == okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[4] - move[3] + 96) < 3) {
                if (okingH - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[4] + 96 == move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingD - move[3] + 48) < 2) {
                if (okingD == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[4] + 48) < 2) {
                if (okingH == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - okingD - move[2] + move[1]) < 3) {
                if (okingH - move[2] == okingD - move[1] && okingH - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -10);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 10);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] == okingD - move[1] && okingH + 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -11);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 11);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] == okingD - move[1] && okingH + 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -12);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 12);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] == okingD - move[1] && okingH - 1 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -13);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 13);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] == okingD - move[1] && okingH - 2 - move[4] != okingD - move[3]) {
                    Chess::UpdateKingMatrix(!turn, -14);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 14);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH + okingD - move[2] - move[1] + 96) < 3) {
                if (okingH - move[2] + 96 == move[1] - okingD && okingH - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -15);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 15);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 - move[2] + 96 == move[1] - okingD && okingH + 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -16);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 16);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 2 - move[2] + 96 == move[1] - okingD && okingH + 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -17);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 17);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 - move[2] + 96 == move[1] - okingD && okingH - 1 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -18);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 18);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 2 - move[2] + 96 == move[1] - okingD && okingH - 2 - move[4] + 96 != move[3] - okingD) {
                    Chess::UpdateKingMatrix(!turn, -19);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 19);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingD - move[1] + 48) < 2) {
                if (okingD == move[1] - 48 && okingD != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -4);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 4);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD + 1 == move[1] - 48 && okingD + 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -5);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 5);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingD - 1 == move[1] - 48 && okingD - 1 != move[3] - 48) {
                    Chess::UpdateKingMatrix(!turn, -6);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 6);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
            if (abs(okingH - move[2] + 48) < 2) {
                if (okingH == move[2] - 48 && okingH != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -7);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 7);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH + 1 == move[2] - 48 && okingH + 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -8);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 8);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
                else if (okingH - 1 == move[2] - 48 && okingH - 1 != move[4] - 48) {
                    Chess::UpdateKingMatrix(!turn, -9);
                    pos[move[2] - 48][move[1] - 48] = 0;
                    posSave = pos[move[4] - 48][move[3] - 48];
                    pos[move[4] - 48][move[3] - 48] = piece;
                    Chess::UpdateKingMatrix(!turn, 9);
                    pos[move[4] - 48][move[3] - 48] = posSave;
                    pos[move[2] - 48][move[1] - 48] = piece;
                }
            }
        }
        pos[move[2] - 48][move[1] - 48] = 0;
        pos[move[4] - 48][move[3] - 48] = piece;
    }
    //
    travelPosH = move[4] - 48;
    travelPosD = move[3] - 48;
    if (diagonalPin1) {
        dis2k1 = okingH - move[4] + 48;
        dis2k2 = okingD - move[3] + 48;
        for (int i = 0; i < abs(dis2k1); i++) {
            travelPosD += (dis2k2 > 0) ? 1 : -1;
            travelPosH += (dis2k1 > 0) ? 1 : -1;
            if (pos[travelPosH][travelPosD] == 11 - 10 * colorV) {
                flags[travelPosH][travelPosD] = 'p' - 16 * colorV;
            }
            else if (pos[travelPosH][travelPosD] == 12 - 10 * colorV) {
                flags[travelPosH][travelPosD] = 'N' - 4 * colorV;
            }
            else if (pos[travelPosH][travelPosD] == 13 - 10 * colorV) {
                flags[travelPosH][travelPosD] = 'r' - 16 * colorV;
            }
            else if (pos[travelPosH][travelPosD] == 14 - 10 * colorV) {
                flags[travelPosH][travelPosD] = 's' - 16 * colorV;
            }
            else if (pos[travelPosH][travelPosD] == 15 - 10 * colorV) {
                flags[travelPosH][travelPosD] = 'L' - 4 * colorV;
            }
        }
    }
    if (directionalPin1) {
        // cout << "hi1\n";
        storeH = -1;
        storeD = -1;
        dis2k1 = (move[4] - 48 == okingH) ? okingD - move[3] + 48 : okingH - move[4] + 48;
        travelPosH = okingH;
        travelPosD = okingD;
        if (okingH - move[4] + 48 == 0) {
            travelPosD += (dis2k1 > 0) ? -1 : 1;
        }
        else {
            travelPosH += (dis2k1 > 0) ? -1 : 1;
        }
        while (pos[travelPosH][travelPosD] != 4 + 10 * colorV && pos[travelPosH][travelPosD] != 5 + 10 * colorV) {
            cout << travelPosH << " " << travelPosD << endl;
            if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] > (1 - 2 * colorV) * 10) {
                cout << travelPosH << " " << travelPosD << endl;
                if (storeH == -1) {
                    storeH = travelPosH;
                    storeD = travelPosD;
                }
                else {
                    travelPosD = -1;
                    break;
                }
            }
            else if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] < (1 - 2 * colorV) * 10) {
                travelPosD = -1;
                break;
            }
            if (okingH - move[4] + 48 == 0) {
                travelPosD += (dis2k1 > 0) ? -1 : 1;
            }
            else {
                travelPosH += (dis2k1 > 0) ? -1 : 1;
            }
        }
        if (travelPosD != -1) {
            if (pos[storeH][storeD] == 11 - 10 * colorV) {
                flags[storeH][storeD] = '8' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 12 - 10 * colorV) {
                flags[storeH][storeD] = 'd' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 13 - 10 * colorV) {
                flags[storeH][storeD] = 'a' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 14 - 10 * colorV) {
                flags[storeH][storeD] = 'I' + 4 * colorV;
            }
            else if (pos[storeH][storeD] == 15 - 10 * colorV) {
                flags[storeH][storeD] = 'G' + 4 * colorV;
            }
        }
        else {
            // cout << "hello\n" << storeH << " " << storeD << endl;
            if (pos[storeH][storeD] == 12 - 10 * colorV && flags[storeH][storeD] == 't' - 16 * colorV) {
                flags[storeH][storeD] = 'C' + 3 * colorV;
            }
            else if (pos[storeH][storeD] == 14 - 10 * colorV && flags[storeH][storeD] == 'M' - 4 * colorV) {
                flags[storeH][storeD] = 'B' + 3 * colorV;
            }
            else if (pos[storeH][storeD] == 15 - 10 * colorV && flags[storeH][storeD] == 'K' - 4 * colorV) {
                flags[storeH][storeD] = 'A' + 3 * colorV;
            }
        }
    }
    travelPosH = ckingH;
    travelPosD = ckingD;
    if (diagonalPin3) {
        storeH = -1;
        storeD = -1;
        dis2k1 = move[4] - 48 - ckingH;
        dis2k2 = move[3] - 48 - ckingD;
        travelPosH = ckingH + (dis2k1 > 0) ? 1 : -1;
        travelPosD = ckingD + (dis2k2 > 0) ? 1 : -1;
        while (pos[travelPosH][travelPosD] != 2 + 10 * colorV && pos[travelPosH][travelPosD] != 5 + 10 * colorV) {
            if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] > (1 - 2 * colorV) * 10) {
                if (storeH == -1) {
                    storeH = travelPosH;
                    storeD = travelPosD;
                }
                else {
                    travelPosD = -1;
                    break;
                }
            }
            else if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] < (1 - 2 * colorV) * 10) {
                travelPosD = -1;
                break;
            }
            travelPosD += (dis2k2 > 0) ? 1 : -1;
            travelPosH += (dis2k1 > 0) ? 1 : -1;
        }
        if (travelPosD != -1) {
            if (pos[storeH][storeD] == 1 + 10 * colorV) {
                flags[storeH][storeD] = '9' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 2 + 10 * colorV) {
                flags[storeH][storeD] = 'J' + 4 * colorV;
            }
            else if (pos[storeH][storeD] == 3 + 10 * colorV) {
                flags[storeH][storeD] = 'b' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 4 + 10 * colorV) {
                flags[storeH][storeD] = 'c' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 5 + 10 * colorV) {
                flags[storeH][storeD] = 'H' + 4 * colorV;
            }
        }
        else {
            if (pos[storeH][storeD] == 12 - 10 * colorV && flags[storeH][storeD] == 'N' - 4 * colorV) {
                flags[storeH][storeD] = 'C' + 3 * colorV;
            }
            else if (pos[storeH][storeD] == 14 - 10 * colorV && flags[storeH][storeD] == 's' - 16 * colorV) {
                flags[storeH][storeD] = 'B' + 3 * colorV;
            }
            else if (pos[storeH][storeD] == 15 - 10 * colorV && flags[storeH][storeD] == 'L' - 4 * colorV) {
                flags[storeH][storeD] = 'A' + 3 * colorV;
            }
        }
    }
    if (directionalPin3) {
        // cout << "hi\n";
        storeH = -1;
        storeD = -1;
        dis2k1 = (move[4] - 48 == ckingH) ? ckingD - move[3] + 48 : ckingH - move[4] + 48;
        travelPosH = ckingH;
        travelPosD = ckingD;
        if (ckingH - move[4] + 48 == 0) {
            travelPosD += (dis2k1 > 0) ? -1 : 1;
        }
        else {
            travelPosH += (dis2k1 > 0) ? -1 : 1;
        }
        while (pos[travelPosH][travelPosD] != 14 - 10 * colorV && pos[travelPosH][travelPosD] != 15 - 10 * colorV) {
            if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] < (1 - 2 * colorV) * 10) {
                if (storeH == -1) {
                    storeH = travelPosH;
                    storeD = travelPosD;
                }
                else {
                    travelPosD = -1;
                    break;
                }
            }
            else if (pos[travelPosH][travelPosD] != 0 && (1 - 2 * colorV) * pos[travelPosH][travelPosD] > (1 - 2 * colorV) * 10) {
                travelPosD = -1;
                break;
            }
            if (ckingH - move[4] + 48 == 0) {
                travelPosD += (dis2k1 > 0) ? -1 : 1;
            }
            else {
                travelPosH += (dis2k1 > 0) ? -1 : 1;
            }
        }
        if (travelPosD != -1) {
            if (pos[storeH][storeD] == 1 + 10 * colorV) {
                flags[storeH][storeD] = '8' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 2 + 10 * colorV) {
                flags[storeH][storeD] = 'd' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 3 + 10 * colorV) {
                flags[storeH][storeD] = 'a' + 16 * colorV;
            }
            else if (pos[storeH][storeD] == 4 + 10 * colorV) {
                flags[storeH][storeD] = 'I' + 4 * colorV;
            }
            else if (pos[storeH][storeD] == 5 + 10 * colorV) {
                flags[storeH][storeD] = 'G' + 4 * colorV;
            }
        }
        else {
            // cout << "hi again xoxo\n";
            if (pos[storeH][storeD] == 12 - 10 * colorV && flags[storeH][storeD] == 'd' + 16 * colorV) {
                flags[storeH][storeD] = 'F' - 3 * colorV;
            }
            else if (pos[storeH][storeD] == 14 - 10 * colorV && flags[storeH][storeD] == 'I' + 4 * colorV) {
                flags[storeH][storeD] = 'E' - 3 * colorV;
            }
            else if (pos[storeH][storeD] == 15 - 10 * colorV && flags[storeH][storeD] == 'G' + 4 * colorV) {
                flags[storeH][storeD] = 'D' - 3 * colorV;
            }
        }
    }
    travelPosH = move[4] - 48;
    travelPosD = move[3] - 48;
    if (diagonalPin2) {
        travelPosD = okingD;
        travelPosH = okingH;
        apD = -1;
        apH = -1;
        if (okingD - move[1] + 48 > 0) {
            if (okingH - move[2] + 48 > 0) {
                while (travelPosD > 0 && travelPosH > 0) {
                    travelPosD--;
                    travelPosH--;
                    if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'i' - 55 * colorV && flags[travelPosH][travelPosD] != 'm' - 55 * colorV) {
                        if (apD == -1) {
                            apD = travelPosD;
                            apH = travelPosH;
                            diagonalPin2 = false;
                        }
                        else {
                            diagonalPin2 = true;
                            break;
                        }
                    }
                    else if (flags[travelPosH][travelPosD] == 'i' - 55 * colorV && flags[travelPosH][travelPosD] == 'm' - 55 * colorV) {
                        break;
                    }
                }
            }
            else {
                while (travelPosD > 0 && travelPosH < 7) {
                    travelPosD--;
                    travelPosH++;
                    if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'j' - 55 * colorV && flags[travelPosH][travelPosD] != 'n' - 55 * colorV) {
                        if (apD == -1) {
                            apD = travelPosD;
                            apH = travelPosH;
                            diagonalPin2 = false;
                        }
                        else {
                            diagonalPin2 = true;
                            break;
                        }
                    }
                    else if (flags[travelPosH][travelPosD] == 'j' - 55 * colorV && flags[travelPosH][travelPosD] == 'n' - 55 * colorV) {
                        break;
                    }
                }
            }
        }
        else {
            if (okingH - move[2] + 48 > 0) {
                while (travelPosD < 7 && travelPosH > 0) {
                    travelPosD++;
                    travelPosH--;
                    if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'j' - 55 * colorV && flags[travelPosH][travelPosD] != 'n' - 55 * colorV) {
                        if (apD == -1) {
                            apD = travelPosD;
                            apH = travelPosH;
                            diagonalPin2 = false;
                        }
                        else {
                            diagonalPin2 = true;
                            break;
                        }
                    }
                    else if (flags[travelPosH][travelPosD] == 'j' - 55 * colorV && flags[travelPosH][travelPosD] == 'n' - 55 * colorV) {
                        break;
                    }
                }
            }
            else {
                while (travelPosD < 7 && travelPosH < 7) {
                    travelPosD++;
                    travelPosH++;
                    if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                        break;
                    }
                    else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'i' - 55 * colorV && flags[travelPosH][travelPosD] != 'm' - 55 * colorV) {
                        if (apD == -1) {
                            apD = travelPosD;
                            apH = travelPosH;
                            diagonalPin2 = false;
                        }
                        else {
                            diagonalPin2 = true;
                            break;
                        }
                    }
                    else if (flags[travelPosH][travelPosD] == 'i' - 55 * colorV && flags[travelPosH][travelPosD] == 'm' - 55 * colorV) {
                        break;
                    }
                }
            }
        }
        if (!diagonalPin2) {
            flags[apH][apD] = '~';
        }
    }
    travelPosH = move[4] - 48;
    travelPosD = move[3] - 48;
    if (directionalPin2) {
        // cout << "gawrsh\n";
        travelPosD = okingD;
        travelPosH = okingH;
        apD = -1;
        apH = -1;
        if (okingD - move[1] + 48 > 0) {
            while (travelPosD > 0) {
                travelPosD--;
                if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                    break;
                }
                else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'g' - 55 * colorV && flags[travelPosH][travelPosD] != 'k' - 55 * colorV) {
                    if (apD == -1) {
                        apD = travelPosD;
                        apH = travelPosH;
                        directionalPin2 = false;
                    }
                    else {
                        directionalPin2 = true;
                        break;
                    }
                }
                else if (flags[travelPosH][travelPosD] == 'g' - 55 * colorV && flags[travelPosH][travelPosD] == 'k' - 55 * colorV) {
                    break;
                }
            }
        }
        else if (okingD - move[1] + 48 < 0) {
            while (travelPosD < 7) {
                travelPosD++;
                if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                    break;
                }
                else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'g' - 55 * colorV && flags[travelPosH][travelPosD] != 'k' - 55 * colorV) {
                    if (apD == -1) {
                        apD = travelPosD;
                        apH = travelPosH;
                        directionalPin2 = false;
                    }
                    else {
                        directionalPin2 = true;
                        break;
                    }
                }
                else if (flags[travelPosH][travelPosD] == 'g' - 55 * colorV && flags[travelPosH][travelPosD] == 'k' - 55 * colorV) {
                    break;
                }
            }
        }
        if (okingH - move[2] + 48 > 0) {
            while (travelPosH > 0) {
                travelPosH--;
                if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                    break;
                }
                else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'h' - 55 * colorV && flags[travelPosH][travelPosD] != 'l' - 55 * colorV) {
                    if (apD == -1) {
                        apD = travelPosD;
                        apH = travelPosH;
                        directionalPin2 = false;
                    }
                    else {
                        directionalPin2 = true;
                        break;
                    }
                }
                else if (flags[travelPosH][travelPosD] == 'h' - 55 * colorV && flags[travelPosH][travelPosD] == 'l' - 55 * colorV) {
                    break;
                }
            }
        }
        else if (okingH - move[2] + 48 < 0) {
            while (travelPosH < 7) {
                travelPosH++;
                if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] == '~') {
                    break;
                }
                else if (pos[travelPosH][travelPosD] != 0 && pos[travelPosH][travelPosD] != 7 && pos[travelPosH][travelPosD] != 17 && flags[travelPosH][travelPosD] != '~' && flags[travelPosH][travelPosD] != 'h' - 55 * colorV && flags[travelPosH][travelPosD] != 'l' - 55 * colorV) {
                    if (apD == -1) {
                        apD = travelPosD;
                        apH = travelPosH;
                        directionalPin2 = false;
                    }
                    else {
                        directionalPin2 = true;
                        break;
                    }
                }
                else if (flags[travelPosH][travelPosD] == 'h' - 55 * colorV && flags[travelPosH][travelPosD] == 'l' - 55 * colorV) {
                    break;
                }
            }
        }
        if (!directionalPin2) {
            flags[apH][apD] = '~';
        }
    }


    turn = !turn;
    doubleCheck = false;
    apD = -1;
    apH = -1;
    if (winvisP != -1 && turn && pos[2][winvisP] == 7) {
        pos[2][winvisP] = 0;
        winvisP = -1;
    }
    else if (binvisP != -1 && !turn && pos[5][binvisP] == 17) {
        pos[5][binvisP] = 0;
        binvisP = -1;
    }
    inCheck = isInCheck(turn, 0);
    GenerateMoveList();

    if (moveList.size() == 0) {
        if (inCheck) {
            cout << "Checkmate! " << ((turn) ? "Black wins" : "White wins");
            Chess::EndGame();
        }
        else {
            cout << "Draw by stalemate";
            Chess::EndGame();
        }
    }

    if (useText) {
        Chess::ShowBoard();
        if (running) {
            cout << ((turn) ? "White" : "Black") << " to move:\n";
        }
    }
    moves += .5;
}


void Chess::ShowPawnTrail() {
    showPawnTrail = true;
}

void Chess::StrictPieceInput() {
    strictpieceinput = true;
}

void Chess::ShowKingMatrix(bool color) {
    if (color) {
        for (int i = 2; i > -1; i--) {
            for (int l = 0; l < 3; l++) {
                cout << wkingMatrix[i][l] << "  ";
            }
            cout << "\n";
        }
    }
    else {
        for (int i = 2; i > -1; i--) {
            for (int l = 0; l < 3; l++) {
                cout << bkingMatrix[i][l] << "  ";
            }
            cout << "\n";
        }
    }
}

int main() {
    string input;
    Chess chess;
    chess.StartGame(true);
    // chess.StartGame(true, false, "2r5/5k2/4n3/8/2R5/8/2K5/8", false, false);
    chess.ShowPawnTrail();
    chess.StrictPieceInput();
    while (chess.running) {
        cout << "Possible moves:\n";
        for (int i = 0; i < chess.moveList.size(); i++) {
            cout << chess.moveList[i] << "\n";
        }

        // for (int i = 7; i > -1; i--) {
        //     for (int l = 0; l < 8; l++) {
        //         cout << chess.flags[i][l] << "  ";
        //     }
        //     cout << "\n";
        // }

        // for (int i = 2; i > -1; i--) {
        //     for (int l = 0; l < 3; l++) {
        //         cout << chess.wkingMatrix[i][l] << "  ";
        //     }
        //     cout << "   ";
        //     for (int l = 0; l < 3; l++) {
        //         cout << chess.bkingMatrix[i][l] << "  ";
        //     }
        //     cout << "\n";
        // }

        //cout << chess.bkingH << "  " << chess.bkingD << endl;

        cin >> input;
        chess.NextTurn(input);
    }
}