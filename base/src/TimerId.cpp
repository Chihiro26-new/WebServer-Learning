#include "TimerId.h"

bool TimerId::valid()const
{
    return !timer_.expired();
}