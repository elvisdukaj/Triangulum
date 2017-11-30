#include "components/DeathSentence.h"

DeathSentence::DeathSentence()
    : timeToExecution(0.0)
    , haveBeenPrepared(false)
{
}

DeathSentence::DeathSentence(double timeToExecution)
    : timeToExecution(timeToExecution)
    , haveBeenPrepared(false)
{
}
