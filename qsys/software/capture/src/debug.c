#include "debug.h"

char MSG[100];
void debugMSG(){
	if (IS_DEBUG_MODE){
		printf("%s",MSG);	
	}
}
void debug(char str[]){
	if (IS_DEBUG_MODE){
		printf("%s",str);
	}
}
