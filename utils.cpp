#include "utils.h"

bool CheckForProbability(int probability)
{
    if(probability < 0 || probability > 100)
    {
        return false;
    }
    int value = GetRandomValue(0, 100);

    if(value <= probability)
    {
        return true;
    }
    else
    {
        return false;
    }
}