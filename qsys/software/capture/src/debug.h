#ifndef DEBUG_H
#define DEBUG_H

#include <system.h>
#include <io.h>
#include <stdio.h>

#define IS_DEBUG_MODE (IORD(NISO_TERMINAL_OUTPUT_ENABLE_BASE,0)==1)
extern char MSG[100];
void debugMSG();
void debug(char str[]);

#endif