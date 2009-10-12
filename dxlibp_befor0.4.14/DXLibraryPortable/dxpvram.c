/*
*	DX���C�u����Portable	VRAM�Ǘ���	Ver1.10
*	�����	�F�����J��
*
*	���l	�F�K��16�o�C�g���E�ɉ������f�[�^��Ԃ��悤�ɂȂ��Ă��܂��B
*	��L�������̖ڈ��͂��悻130KByte�ł��B
*	Ver2.00�������VRAM�̃f�t���O�ɑΉ�������\��ł��B
*/

#include <pspgu.h>
#include <pspdisplay.h>
#include "dxlibp.h"
#include "dxpstatic.h"

//#define HOGE

#define VRAMARRAYNUM	(2*1024)			/*�m�ۂł���VRAM�̈�̍ő吔�i���e�N�X�`���̍ő吔�j*/

static	u8			vramdata[2*1024*64 + 1];	/*VRAM�̎g���Ă���ꏊ���L������16byte�P�ʁB�Ō�͔̂ԕ�*/
static	DXPVRAMCTRL	vramarray[VRAMARRAYNUM + 1];/*�m�ۍς�VRAM�̏��ێ�*/
static	u32			vramfree;				/*���Ƃǂꂭ�炢�󂢂Ă���̂�*/

void InitVRAM();
DXPVRAMCTRL* AllocVRAM(int Size,int StaticFlag);
void FreeVRAM(DXPVRAMCTRL *VRamCtrl);
//static void DeflagVRAM(){}

void InitVRAM()
{
	AppLogAdd2("VRAM�̊Ǘ��f�[�^�����������܂��B");

	int i;
	for(i = 0;i < 2 * 1024 * 64;++i)vramdata[i] = 0x00;
//	for(i = 0;i < 34816;++i)vramdata[i] = 0x01;

//	vramdata[0] = 0x01;
	vramdata[2 * 1024 * 64] = 0x01;
	for(i = 0;i < VRAMARRAYNUM;++i)
	{
		vramarray[i].address= 0;
		vramarray[i].flags	= 0;
		vramarray[i].size	= 0;
	}
	vramarray[VRAMARRAYNUM].flags = VRAMCTRLFLAGS_EXIST;
	vramfree = 2*1024*64;
	AppLogAdd2("VRAM�̊Ǘ��f�[�^�����������܂����B");
}
#ifdef	HOGE
DXPVRAMCTRL* AllocVRAM(int Size,int StaticFlag)
/*�ÓI�t���O�������Ă���ꍇ�͋t�������猟������B�኱�̉��ǁi�H�j*/
{
	int i,j,k,df;
	if(Size > vramfree << 4)return NULL;
	if(Size <= 0)return NULL;
	if(Size % 16)Size += 16 - Size % 16;/*�m�ۂ���T�C�Y��16�Ŋ���؂�Ȃ��ꍇ�͕␳*/
	df = 0;

	if(!StaticFlag)
	{
		i = 0;
FREEAREASERCH:
		while(vramdata[i])++i;	/*�g���Ă��Ȃ��ꏊ������*/
		if(i >= 2 * 1024 * 64)return NULL;/*�ԕ��ɂԂ��������A�݂��ȗ͂����������̂ǂ��炩*/
		for(j = 0;i + j < 2 * 1024 * 64 && j < (Size >> 4);++j);/*�A�����ċ󂢂Ă���̂��m�F*/
		if(i + j >= 2 * 1024 * 64)return NULL;
		if(j < (Size >> 4))goto FREEAREASERCH;/*�A�����ĂȂ������猟�����Ȃ���*/
	}
	else
	{
		i = 2 * 1024 * 64;
FREEAREASERCH_STATIC:
		while(vramdata[i])
		{
			--i;	/*�g���Ă��Ȃ��ꏊ������*/
			if(i < 0)
			{
				return NULL;
			}
		}
		for(j = 0;i - j >= 0 && j < (Size >> 4);++j);/*�A�����ċ󂢂Ă���̂��m�F*/
		if(i - j < 0)return NULL;
		if(j < (Size >> 4))goto FREEAREASERCH_STATIC;/*�A�����ĂȂ������猟�����Ȃ���*/
		i = i + 1 - j;	/*i���ꏊ�̐擪�ɂ����Ă���*/
	}
/*�����܂łŋ󂫗̈挟���I���B�f�[�^�����ɓ���܂�*/
	j = 0;
	while(vramarray[j].flags & VRAMCTRLFLAGS_EXIST)++j;
	if(j >= VRAMARRAYNUM)return NULL;
/*�����I����*/
	for(k = 0;k < (Size >> 4);++k)vramdata[i + k] = 0x01;
	vramarray[j].offset = i * 16;/*16�o�C�g���E�O��Ȃ̂ł�*/
	vramarray[j].size	= Size;
	vramarray[j].flags	= VRAMCTRLFLAGS_EXIST |
						(StaticFlag ? VRAMCTRLFLAGS_STATIC : 0)
						;
	if(StaticFlag)
	{
		AppLogAdd2("�ÓI��VRAM�̈悪�m�ۂ���܂����B");
		AppLogAdd2("�I�t�Z�b�g�F%#x\t�T�C�Y�F%d",i * 16,Size);
	}
	return vramarray + j;
}
#else
DXPVRAMCTRL* AllocVRAM(int Size,int StaticFlag)
/*�ÓI�t���O�������Ă���ꍇ�͋t�������猟������B�኱�̉��ǁi�H�j*/
{
	int i,j,k,df;
	if(Size > vramfree << 4)return NULL;
	if(Size <= 0)return NULL;
	if(Size % 16)Size += 16 - Size % 16;/*�m�ۂ���T�C�Y��16�Ŋ���؂�Ȃ��ꍇ�͕␳*/
	df = 0;

	{
		i = 0;
FREEAREASERCH:
		while(vramdata[i])++i;	/*�g���Ă��Ȃ��ꏊ������*/
		if(i >= 2 * 1024 * 64)return NULL;/*�ԕ��ɂԂ��������A�݂��ȗ͂����������̂ǂ��炩*/
		for(j = 0;i + j < 2 * 1024 * 64 && j < (Size >> 4);++j);/*�A�����ċ󂢂Ă���̂��m�F*/
		if(i + j >= 2 * 1024 * 64)return NULL;
		if(j < (Size >> 4))goto FREEAREASERCH;/*�A�����ĂȂ������猟�����Ȃ���*/
	}
/*�����܂łŋ󂫗̈挟���I���B�f�[�^�����ɓ���܂�*/
	j = 0;
	while(vramarray[j].flags & VRAMCTRLFLAGS_EXIST)++j;
	if(j >= VRAMARRAYNUM)return NULL;
/*�����I����*/
	for(k = 0;k < (Size >> 4);++k)vramdata[i + k] = 0x01;
	vramarray[j].offset = i * 16;/*16�o�C�g���E�O��Ȃ̂ł�*/
	vramarray[j].size	= Size;
	vramarray[j].flags	= VRAMCTRLFLAGS_EXIST |
						(StaticFlag ? VRAMCTRLFLAGS_STATIC : 0)
						;
	if(StaticFlag)
	{
		AppLogAdd2("�ÓI��VRAM�̈悪�m�ۂ���܂����B");
		AppLogAdd2("�I�t�Z�b�g�F%d\t�T�C�Y�F%d",i * 16,Size);
	}
	return vramarray + j;
}
#endif
void FreeVRAM(DXPVRAMCTRL *ptr)
{
	int i;
	if(ptr == NULL)return;
	if(!(ptr->flags & VRAMCTRLFLAGS_EXIST))return;
	if(ptr < vramarray || ptr >= vramarray + VRAMARRAYNUM)return;
	for(i = (ptr->offset >> 4);i < (ptr->size >> 4);++i)vramdata[i] = 0x00;
	ptr->address	= 0;
	ptr->flags		= 0;
	ptr->size		= 0;
}
