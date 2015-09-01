#include "pieces.h"

Pieces::Pieces():camp(0), maxLian(0), refresh(false)
{
    for(int i = 0; i < 4; i++)
        lian[i] = 0;
}

void Pieces::clear()
{
    for(int i = 0; i < 4; ++i)
        lian[i] = 0;
    maxLian = 0;
    camp = 0;
}
