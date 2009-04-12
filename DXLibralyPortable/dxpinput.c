/*
*	DX���C�u����Portable	���͊Ǘ���	Ver1.00
*	�����	�F�����J��
*
*	���l	�F
*	��L�������̖ڈ��͂��悻40Byte�ł��B
*/
#include <pspctrl.h>
#include "dxlibp.h"
#include <string.h>

int InitInput();		/*������*/
int RenewInput();		/*�X�V*/
int GetInputState();	/*�擾*/
int GetAnalogInput(int *XBuf,int *YBuf);/*�A�i���O�f�[�^�̎擾*/

static struct SceCtrlData CtrlData;		/*���̓f�[�^�@RenewInput�ōX�V�����*/


//���[�h����
int	PspCtrlMode		= 0;// 0) �}�C�N���\�t�g�T�C�h���C���_�[�p�b�h�������[�h
						// 1) �L�[�{�[�h���[�h
int PspAnalogMode	= 0;// 0) PSP�A�i���O�p�b�h���\���L�[�̌��ʂɍ�������
						//    �}�C�N���\�t�g�T�C�h���C���_�[�p�b�h��
						//    �A�i���O�@�\�����Ƃ��ĐU����
						// 1) PSP�A�i���O�p�b�h���\���L�[�̌��ʂɍ������Ȃ�
						//    �}�C�N���\�t�g�T�C�h���C���_�[�p�b�h��
						//    �A�i���O�@�\�L��Ƃ��ĐU����
						//��) PspCtrlMode�̃��[�h�Ɋ֌W�Ȃ��U����

#define PSPCTRLMODE_MSWP		0x00000001
#define PSPCTRLMODE_KB			0x00000002
#define PSPANALOGMODE_CKEY		0x00000010
#define PSPANALOGMODE_NOCKEY	0x00000020

//PSP�̃_���A�i���O�p�b�h(��)�̃u�����z�����邽�߂̐���
#define ANALOG_BOUNDARY_MIN  85
#define ANALOG_BOUNDARY_MAX 170

int InitInput()
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	return 0;
}
int RenewInput()
{
	return sceCtrlReadBufferPositive(&CtrlData,1) < 0 ? -1 : 0;
}
int GetInputState()
{
	return CtrlData.Buttons;
}
int GetAnalogInput(int *XBuf,int *YBuf)
{
	//�A�i���O���[�h����
	// 0) PSP�A�i���O�p�b�h���\���L�[�̌��ʂɍ������邽�ߐ�����Ԃ��Ȃ�(�f�t�H���g
	// 1) PSP�A�i���O�p�b�h���\���L�[�̌��ʂɍ������Ȃ����ߐ�����Ԃ�
	//    (�ʓ|�Ȃ̂�0�łȂ��Ƃ��͍������Ȃ������ɂ��Ă܂�
	if(PspAnalogMode == 0)
	{
		*XBuf = 0;
		*YBuf = 0;
	}
	else
	{
		*XBuf = CtrlData.Lx * 7.8125 - 1000;
		*YBuf = CtrlData.Ly * 7.8125 - 1000;
	}
	return 0;
}
int GetHitKeyStateAll( char *KeyStateBuf )
{
	if(KeyStateBuf == NULL)return -1;
	memset(KeyStateBuf,0x00,256);
	//PAD�G�~�����[�V�������͏������X�L�b�v
	if(PspCtrlMode == 0) return 0;
	KeyStateBuf[KEY_INPUT_UP] = CtrlData.Buttons & DXP_INPUT_UP ? 1 : 0;
	KeyStateBuf[KEY_INPUT_DOWN] = CtrlData.Buttons & DXP_INPUT_DOWN ? 1 : 0;
	KeyStateBuf[KEY_INPUT_LEFT] = CtrlData.Buttons & DXP_INPUT_LEFT ? 1 : 0;
	KeyStateBuf[KEY_INPUT_RIGHT] = CtrlData.Buttons & DXP_INPUT_RIGHT ? 1 : 0;
	KeyStateBuf[KEY_INPUT_Z] = CtrlData.Buttons & DXP_INPUT_CROSS ? 1 : 0;
	KeyStateBuf[KEY_INPUT_X] = CtrlData.Buttons & DXP_INPUT_CIRCLE ? 1 : 0;
	KeyStateBuf[KEY_INPUT_A] = CtrlData.Buttons & DXP_INPUT_SQUARE ? 1 : 0;
	KeyStateBuf[KEY_INPUT_S] = CtrlData.Buttons & DXP_INPUT_TRIANGLE ? 1 : 0;
	KeyStateBuf[KEY_INPUT_Q] = CtrlData.Buttons & DXP_INPUT_LTRIGGER ? 1 : 0;
	KeyStateBuf[KEY_INPUT_W] = CtrlData.Buttons & DXP_INPUT_RTRIGGER ? 1 : 0;
	KeyStateBuf[KEY_INPUT_ESCAPE] = CtrlData.Buttons & DXP_INPUT_START ? 1 : 0;
	KeyStateBuf[KEY_INPUT_SPACE] = CtrlData.Buttons & DXP_INPUT_SELECT ? 1 : 0;
	return 0;
}

int GetJoypadInputState( int InputType )
{
	//�L�[�{�[�h�G�~�����[�V�������͏������X�L�b�v
	if(PspCtrlMode == 1) return 0;
	int ret = 0;
	//PSP�A�i���O�p�b�h���\���L�[�̌��ʂɍ������郂�[�h�̏���
	if (PspAnalogMode == 0) {
		if (CtrlData.Lx <= ANALOG_BOUNDARY_MIN) CtrlData.Buttons |= PSP_CTRL_LEFT;
		if (CtrlData.Lx >= ANALOG_BOUNDARY_MAX) CtrlData.Buttons |= PSP_CTRL_RIGHT;
		if (CtrlData.Ly <= ANALOG_BOUNDARY_MIN) CtrlData.Buttons |= PSP_CTRL_UP;
		if (CtrlData.Ly >= ANALOG_BOUNDARY_MAX) CtrlData.Buttons |= PSP_CTRL_DOWN;
	}
	if(CtrlData.Buttons & PSP_CTRL_UP)		 ret |= PAD_INPUT_UP;
	if(CtrlData.Buttons & PSP_CTRL_DOWN)	 ret |= PAD_INPUT_DOWN;
	if(CtrlData.Buttons & PSP_CTRL_LEFT)	 ret |= PAD_INPUT_LEFT;
	if(CtrlData.Buttons & PSP_CTRL_RIGHT)	 ret |= PAD_INPUT_RIGHT;
	if(CtrlData.Buttons & PSP_CTRL_CROSS)	 ret |= PAD_INPUT_A;
	if(CtrlData.Buttons & PSP_CTRL_CIRCLE)	 ret |= PAD_INPUT_B;
	//if(CtrlData.Buttons & PSP_CTRL_)		 ret |= PAD_INPUT_C;
	if(CtrlData.Buttons & PSP_CTRL_SQUARE)	 ret |= PAD_INPUT_X;
	if(CtrlData.Buttons & PSP_CTRL_TRIANGLE) ret |= PAD_INPUT_Y;
	//if(CtrlData.Buttons & PSP_CTRL_)		 ret |= PAD_INPUT_Z;
	if(CtrlData.Buttons & PSP_CTRL_LTRIGGER) ret |= PAD_INPUT_L;
	if(CtrlData.Buttons & PSP_CTRL_RTRIGGER) ret |= PAD_INPUT_R;
	if(CtrlData.Buttons & PSP_CTRL_START)	 ret |= PAD_INPUT_START;
	if(CtrlData.Buttons & PSP_CTRL_SELECT)	 ret |= PAD_INPUT_M;
	return ret;
}

int GetJoypadNum()
{
	return 1;
}
int GetJoypadAnalogInput(int *px,int *py,int inputtype)
{
	return GetAnalogInput(px,py);
}
int GetJoypadAnalogInputRight(int *px,int *py,int inputtype)
{
	if(px != NULL)*px = 0;
	if(py != NULL)*py = 0;
	return 0;
}
//SetJoypadInputToKeyInput �@�W���C�p�b�h�̓��͂ɑΉ������L�[�{�[�h�̓��͂�ݒ肷�� 
int StartJoypadVibration(int a,int b,int c){return 0;}// �@�W���C�p�b�h�̐U�����J�n���� 
int StopJoypadVibration(int a){return 0;}

//�ȉ���PSP�̓Ǝ��֐�
//���ʂ͔���J��
//�v�]������������J
int	SetPspCtrlMode(int Mode)
{
	if(Mode & PSPCTRLMODE_MSWP)
	{
		PspCtrlMode = 0;
	}
	else
	{
		PspCtrlMode = 1;
	}
	return 0;
}

int	SetPspAnalogMode(int Mode)
{
	if(Mode & PSPANALOGMODE_CKEY)
	{
		PspAnalogMode = 0;
	}
	else
	{
		PspAnalogMode = 1;
	}
	return 0;
}
