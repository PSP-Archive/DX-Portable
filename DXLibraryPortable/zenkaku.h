/*
	mediumgauge���쐬�̑S�p�����\�����C�u�����̂c�w���C�u�����K����
	�ڍׂ�zenkaku.c�̓����ɂ���܂��B
*/

#ifndef	ZENKAKU_H__
#define	ZENKAKU_H__
#include "dxlibp.h"

#define BUF_WIDTH	(512)	/*�ꃉ�C��������̃s�N�Z����*/
#define SCR_WIDTH	(480)	/*��ʉ��s�N�Z����*/
#define SCR_HEIGHT	(272)	/*��ʏc�s�N�Z����*/
#define	FONTWIDTH	6
#define	FONTHEIGHT	12

void	mh_print(int x,int y,const char *str,int col);
void	mh_displayoffset(int offset);
#endif

