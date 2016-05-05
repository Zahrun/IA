/*
 * globals.cpp
 *
 *  Created on: 5 mai 2016
 *      Author: aroun
 */

#include "globals.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
