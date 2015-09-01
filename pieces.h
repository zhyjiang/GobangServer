#ifndef PIECES_H
#define PIECES_H

class Gobang;

class Pieces
{
private:
    int camp; //Show the piece belongs to which player.
    int lian[4]; //Show how many pieces around it(That can make a line).[0] / [1] \ [2] | [3] -
    int maxLian; //Show the longest pieces line.
    bool refresh;

public:
    Pieces();
    void clear();
    friend class Gobang;
};

#endif // PIECES_H
