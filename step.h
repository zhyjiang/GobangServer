#ifndef STEP_H
#define STEP_H

class Gobang;
class NetworkServer;

class Step //The class save the data of the player's step.
{
private:
    int x;
    int y;
    int camp;
    friend class Gobang;
    friend class NetworkServer;
public:
    Step(int _x, int _y, int _camp);
    ~Step(){}
};

#endif // STEP_H
