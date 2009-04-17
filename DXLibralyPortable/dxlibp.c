/*
*	DX���C�u����Portable	�S�̊Ǘ���	Ver1.00
*	�����	�F�����J��
*
*	���l	�F�Ƃ��ɂȂ�
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pspkernel.h>
#include <psputility.h>
#include <psprtc.h>
#include "dxlibp.h"
#include "dxpstatic.h"

PSP_HEAP_SIZE_KB(-1);/*�q�[�v�����������E�܂Ŏg����悤�ɂ���B*/

/*���ϐ�*/

DXPDATA dxpdata = 
{
	10000,
	{0}
};
/*����*/
int GetNowCount()/*milis�P�ʂł̎���*/
/*TODO ���݂̎d�l����RTC���g���Ă��Ēx����������Ȃ��B��ւł�����̂�����Έڍs�B*/
/*1�񂠂���35�}�C�N���b�����������Ă��Ȃ��̂ł��̂܂܂ł����ł���*/
{
	u64 tick;
	if(sceRtcGetCurrentTick(&tick) != 0)return -1;
	return (int)(tick * 1000 / dxpdata.TPS);
}
u64	GetNowHiPerformanceCount()/*micros�P��*/
{
	//20090417 ���_�^�T���v���̃f�o�b�O�@�\�Ŏg���Ă��邪
	//�R�����g�̏ꏊ�������Ă���Ɗ֐��̖߂�l��0�ɂȂ���
	//���܂��̂Ŏb��΍�
	u64 tick;
	//float tmp;
	if(sceRtcGetCurrentTick(&tick) != 0)return -1;
	//tmp = tick;
	//tmp = tmp * 1000 * 1000 / dxpdata.TPS;
	//tick = tmp;
	return tick;
}
/*HOME�{�^���Ď�*/
static int exit_callback(int arg1, int arg2, void *common)
{

	sceKernelExitGame();
	return 0;
}

static int ExitCallbackThread(SceSize args, void *argp)
{
	sceKernelRegisterExitCallback(sceKernelCreateCallback("Exit Callback", exit_callback, NULL));	
	sceKernelSleepThreadCB();//�悭�킩��Ȃ�
	return 0;
}

static int SetExitCallback(void)
{
	int thid = 0;
	
	thid = sceKernelCreateThread("update_thread", ExitCallbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	atexit(sceKernelExitGame);
	return thid;
}
/*����*/
#ifndef DXP_NOUSE_MTRAND
static SceKernelUtilsMt19937Context RandData;	/*�����v�Z�̂��߂̃f�[�^*/
int SRand(int seed)
{
	return sceKernelUtilsMt19937Init(&RandData,seed) < 0 ? -1 : 0;
}

int GetRand(int maxnum)
{
	if(maxnum <= 0)return 0;
	return sceKernelUtilsMt19937UInt(&RandData) % maxnum;
}
#else
static unsigned int randdata;
int SRand(int seed)
{
	randdata = seed;
	return 0;
}

static void lfsr()
{
	randdata = (randdata >> 1) ^ (-(randdata & 1u) & 0x80200003u); /* taps 32 31 29 1 */
}

int GetRand(int maxnum)
{
	if(maxnum == 0)return 0;
	lfsr();
	//138
	//62
	//34
	unsigned int i = randdata % 34u;
	for(i += 1;i > 0;--i)lfsr();

	return (s32)(randdata % (u32)maxnum);
}
#endif

/*�K�{�֐�*/
/*������*/
int DxLib_IsInit()
{
	return dxpdata.flags[0] & DXPDATAFLAGS_0_INITIALIZED;
}
int DxLib_Init()
{
	if(DxLib_IsInit())return -1;
	dxpdata.flags[0] |= DXPDATAFLAGS_0_INITIALIZING;
	sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);//MP3��Jpeg�̃f�R�[�h�ɂ������W���[�������[�h����B

	SetExitCallback();/*HOME�{�^���R�[���o�b�N�֐��̃Z�b�g*/
	InitInput();/*���͂̏�����*/
	InitGUEngine();/*GPU����̏�����*/
	SRand(time(NULL));/*����������*/

	dxpdata.TPS = sceRtcGetTickResolution();

	dxpdata.flags[0] &= (~DXPDATAFLAGS_0_INITIALIZING);
	dxpdata.flags[0] |= DXPDATAFLAGS_0_INITIALIZED;
	return 0;
}
int DxLib_End()
{
//	StopMusic();
	dxpdata.flags[0] &= (~DXPDATAFLAGS_0_INITIALIZED);
	sceUtilityUnloadAvModule(PSP_AV_MODULE_AVCODEC);
	EndGUEngine();
	return 0;
}

int ProcessMessage()
{
	RenewInput();
//	ProcessAudio();
	return 0;
}



int WaitTimer(int time)
{
	int i = 0;
	int t0 = GetNowCount();
	while(GetNowCount() - t0 < time)
	{
		sceKernelDelayThread(1000);
		if(i % 30 == 0)ProcessMessage();
		++i;
	}
	return 0;
}
int Sleep(int time)
{
	sceKernelDelayThread(time * 1000);
	return 0;
}

int SetupSTREAMDATA(const char *FileName,STREAMDATA *DataPtr)
/*DX���C�u������STREAMDATA��ݒ肷��B���������Ȃ͎̂d�l*/
{
	if(DataPtr == NULL)return -1;
	if((int)(DataPtr->DataPoint = (void*)FileRead_open(FileName)) == -1)return -1;
	DataPtr->ReadShred.Close	= (int(*)(void*))FileRead_close;
	DataPtr->ReadShred.Eof		= (int(*)(void*))FileRead_eof;
	DataPtr->ReadShred.IdleCheck= (int(*)(void*))FileRead_idle_chk;
	DataPtr->ReadShred.Write	= (unsigned int(*)(void*,size_t,size_t,void*))FileRead_write;
	DataPtr->ReadShred.Read		= (unsigned int(*)(void*,size_t,size_t,void*))FileRead_stdread;
	DataPtr->ReadShred.Seek		= (int(*)(void*,long,int))FileRead_seek;
	DataPtr->ReadShred.Tell		= (long(*)(void*))FileRead_tell;
	//STSEEK(DataPtr,0,SEEK_SET);
	return 0;
}
