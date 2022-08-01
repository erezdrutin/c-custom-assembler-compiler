#include "dates.h"
#include <time.h>

void delay(double seconds)
{

    // Converting time into milli_seconds
    int milli_seconds = (int)(1000 * seconds);

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds) ;
}