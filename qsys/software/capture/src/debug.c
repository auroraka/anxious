#include "debug.h"

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
