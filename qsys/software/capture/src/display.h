/*
 * nervous_nios.h
 *
 *  Created on: 2016/6/7
 *      Author: ytl
 */

#ifndef NERVOUS_NIOS_H_
#define NERVOUS_NIOS_H_

void drawMsg(unsigned bank, char *str, int x, int y);

void clearMsg(unsigned bank, char *str, int x, int y);

void drawCursor(unsigned bank, int x, int y);

#endif /* NERVOUS_NIOS_H_ */
