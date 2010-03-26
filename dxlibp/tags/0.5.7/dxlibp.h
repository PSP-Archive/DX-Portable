/**
 * @file dxlibp.h
 * @mainpage

 * DX���C�u����Portable @n
 * Ver0.5.7@n
 * ����ҁF�J��@n
 * ���̃��C�u�������g���ۂɂ��̃��C�u�����̒��쌠�\�L�͗v��܂��񂪁A���̃��C�u�������ˑ����Ă��郉�C�u�����̒��쌠�\�L�͍s���K�v������܂��B@n
 * �܂��A���̃��C�u�����̃\�[�X�R�[�h�̈ꕔ�܂��͑S����]�ځA���p�A���J�Ȃǂ���ꍇ�͂��̃��C�u�����̒��쌠�\�L���K�v�ł��B@n
 * �ڂ�����readme.txt��ǂ�ł��������B@n
 * �����Ɂi�{�ƌ݊��j�Ƃ���֐���DX���C�u�����Ƌ������قړ����ł��̂ŁA�{�Ƃ̊֐����t�@�����X���Q�l�ɂ��Ă��������B
*/
#ifndef DXLIBP_H__
#define DXLIBP_H__

#include <pspuser.h>

/*DXP�̃R���p�C���I�v�V����*/
#define DXP_BUILDOPTION_FILEHANDLE_MAX		32		//FileRead�n�֐��œ����ɊJ����t�@�C���n���h���̐�
#define DXP_BUILDOPTION_FILENAMELENGTH_MAX	260		//FileRead�n�֐��Ŏg����t�@�C�����̒���
#define DXP_BUILDOPTION_TEXTURE_MAXNUM		512		//DXP�������Ŏ��e�N�X�`���̍ő喇��
#define DXP_BUILDOPTION_GHANDLE_MAXNUM		2048	//�O���t�B�b�N�X�n���h���̍ő吔
#define DXP_BUILDOPTION_MODEL_MAXNUM		16		//3D���f���̍ő吔
#define DXP_BUILDOPTION_FONTHANDLE_MAX		8

#define DXP_BUILDOPTION_NO3D						//3D�@�\�̖������B������R�����g�A�E�g�����ꍇ�A�����r����3D�֐����g�p�\�ɂȂ�܂����Alibpspgum�ւ̈ˑ����������܂��B

/*---�萔�ƃ}�N��---*/

#define RUNCHECK {printfDx("%s,%s,%s,%d\n",__TIME__,__FILE__,__func__,__LINE__);ScreenCopy();/*AppLogAdd("%s,%s,%s,%d\n",__TIME__,__FILE__,__func__,__LINE__);*/}
#define FILETRACE {AppLogAdd("%s,%s,%s,%d\n",__TIME__,__FILE__,__func__,__LINE__);}

#ifndef	TRUE
#define	TRUE	(1)
#endif
#ifndef	FALSE
#define	FALSE	(0)
#endif
#ifndef	LONGLONG
#define LONGLONG u64
#endif


/*Windows�̒�`�����������p*/
#define	BYTE	u8
#define	WORD	u16
#define	DWORD	u32

/* �����R�[�h��` */
#define DXP_CP_SJIS			(0x0d)
#define DXP_CP_UTF8			(0xff)

/* PSP�̃{�^����` */
#define DXP_INPUT_UP		(0x00000010)
#define DXP_INPUT_RIGHT		(0x00000020)
#define DXP_INPUT_DOWN		(0x00000040)
#define DXP_INPUT_LEFT		(0x00000080)
#define DXP_INPUT_TRIANGLE	(0x00001000)
#define DXP_INPUT_CIRCLE	(0x00002000)
#define DXP_INPUT_CROSS		(0x00004000)
#define DXP_INPUT_SQUARE	(0x00008000)
#define DXP_INPUT_LTRIGGER	(0x00000100)
#define DXP_INPUT_RTRIGGER	(0x00000200)
#define DXP_INPUT_START		(0x00000008)
#define DXP_INPUT_SELECT	(0x00000001)

/* �Q�[���p�b�h��` */
#define DX_INPUT_PAD1								(1)	/* PSP�̓��͂�PAD1���͂Ƃ��Ď擾���邱�Ƃ��ł��܂��B�ڂ����͖{�ƃ��C�u�����̃}�j���A�����Q�Ƃ��Ă��������B */
#define DX_INPUT_PAD2								(0)
#define DX_INPUT_PAD3								(0)
#define DX_INPUT_PAD4								(0)
#define DX_INPUT_PAD5								(0)
#define DX_INPUT_PAD6								(0)
#define DX_INPUT_PAD7								(0)
#define DX_INPUT_PAD8								(0)
#define DX_INPUT_KEY								(0)
#define DX_INPUT_KEY_PAD1							(1)

/* �Q�[���p�b�h�̃{�^����` */
#define PAD_INPUT_DOWN								(0x00000001)
#define PAD_INPUT_LEFT								(0x00000002)
#define PAD_INPUT_RIGHT								(0x00000004)
#define PAD_INPUT_UP								(0x00000008)
#define PAD_INPUT_A									(0x00000010)
#define PAD_INPUT_B									(0x00000020)
#define PAD_INPUT_C									(0x00000040)
#define PAD_INPUT_X									(0x00000080)
#define PAD_INPUT_Y									(0x00000100)
#define PAD_INPUT_Z									(0x00000200)
#define PAD_INPUT_L									(0x00000400)
#define PAD_INPUT_R									(0x00000800)
#define PAD_INPUT_START								(0x00001000)
#define PAD_INPUT_M									(0x00002000)
#define PAD_INPUT_D									(0x00004000)
#define PAD_INPUT_F									(0x00008000)
#define PAD_INPUT_G									(0x00010000)
#define PAD_INPUT_H									(0x00020000)
#define PAD_INPUT_I									(0x00040000)
#define PAD_INPUT_J									(0x00080000)
#define PAD_INPUT_K									(0x00100000)
#define PAD_INPUT_LL								(0x00200000)
#define PAD_INPUT_N									(0x00400000)
#define PAD_INPUT_O									(0x00800000)
#define PAD_INPUT_P									(0x01000000)
#define PAD_INPUT_RR								(0x02000000)
#define PAD_INPUT_S									(0x04000000)
#define PAD_INPUT_T									(0x08000000)
#define PAD_INPUT_U									(0x10000000)
#define PAD_INPUT_V									(0x20000000)
#define PAD_INPUT_W									(0x40000000)
#define PAD_INPUT_XX								(0x80000000)


/*
 * �L�[��`
 * PSP�̃{�^�����͂��L�[�{�[�h���͂Ƃ��Ĉ������Ƃ��ł��܂��B�ڂ����͖{�ƃ��C�u�����̃}�j���A�����Q�Ƃ��Ă��������B
*/
#define KEY_INPUT_BACK								(0x0E)
#define KEY_INPUT_TAB								(0x0F)
#define KEY_INPUT_RETURN							(0x1C)

#define KEY_INPUT_LSHIFT							(0x2A)
#define KEY_INPUT_RSHIFT							(0x36)
#define KEY_INPUT_LCONTROL							(0x1D)
#define KEY_INPUT_RCONTROL							(0x9D)
#define KEY_INPUT_ESCAPE							(0x01)
#define KEY_INPUT_SPACE								(0x39)
#define KEY_INPUT_PGUP								(0xC9)
#define KEY_INPUT_PGDN								(0xD1)
#define KEY_INPUT_END								(0xCF)
#define KEY_INPUT_HOME								(0xC7)
#define KEY_INPUT_LEFT								(0xCB)
#define KEY_INPUT_UP								(0xC8)
#define KEY_INPUT_RIGHT								(0xCD)
#define KEY_INPUT_DOWN								(0xD0)
#define KEY_INPUT_INSERT							(0xD2)
#define KEY_INPUT_DELETE							(0xD3)

#define KEY_INPUT_MINUS								(0x0C)
#define KEY_INPUT_YEN								(0x7D)
#define KEY_INPUT_PREVTRACK							(0x90)
#define KEY_INPUT_PERIOD							(0x34)
#define KEY_INPUT_SLASH								(0x35)
#define KEY_INPUT_LALT								(0x38)
#define KEY_INPUT_RALT								(0xB8)
#define KEY_INPUT_SCROLL							(0x46)
#define KEY_INPUT_SEMICOLON							(0x27)
#define KEY_INPUT_COLON								(0x92)
#define KEY_INPUT_LBRACKET							(0x1A)
#define KEY_INPUT_RBRACKET							(0x1B)
#define KEY_INPUT_AT								(0x91)
#define KEY_INPUT_BACKSLASH							(0x2B)
#define KEY_INPUT_COMMA								(0x33)
#define KEY_INPUT_CAPSLOCK							(0x3A)
#define KEY_INPUT_SYSRQ								(0xB7)
#define KEY_INPUT_PAUSE								(0xC5)

#define KEY_INPUT_NUMPAD0							(0x52)
#define KEY_INPUT_NUMPAD1							(0x4F)
#define KEY_INPUT_NUMPAD2							(0x50)
#define KEY_INPUT_NUMPAD3							(0x51)
#define KEY_INPUT_NUMPAD4							(0x4B)
#define KEY_INPUT_NUMPAD5							(0x4C)
#define KEY_INPUT_NUMPAD6							(0x4D)
#define KEY_INPUT_NUMPAD7							(0x47)
#define KEY_INPUT_NUMPAD8							(0x48)
#define KEY_INPUT_NUMPAD9							(0x49)
#define KEY_INPUT_MULTIPLY							(0x37)
#define KEY_INPUT_ADD								(0x4E)
#define KEY_INPUT_SUBTRACT							(0x4A)
#define KEY_INPUT_DECIMAL							(0x53)
#define KEY_INPUT_DIVIDE							(0xB5)
#define KEY_INPUT_NUMPADENTER						(0x9C)

#define KEY_INPUT_F1								(0x3B)
#define KEY_INPUT_F2								(0x3C)
#define KEY_INPUT_F3								(0x3D)
#define KEY_INPUT_F4								(0x3E)
#define KEY_INPUT_F5								(0x3F)
#define KEY_INPUT_F6								(0x40)
#define KEY_INPUT_F7								(0x41)
#define KEY_INPUT_F8								(0x42)
#define KEY_INPUT_F9								(0x43)
#define KEY_INPUT_F10								(0x44)
#define KEY_INPUT_F11								(0x57)
#define KEY_INPUT_F12								(0x58)

#define KEY_INPUT_A									(0x1E)
#define KEY_INPUT_B									(0x30)
#define KEY_INPUT_C									(0x2E)
#define KEY_INPUT_D									(0x20)
#define KEY_INPUT_E									(0x12)
#define KEY_INPUT_F									(0x21)
#define KEY_INPUT_G									(0x22)
#define KEY_INPUT_H									(0x23)
#define KEY_INPUT_I									(0x17)
#define KEY_INPUT_J									(0x24)
#define KEY_INPUT_K									(0x25)
#define KEY_INPUT_L									(0x26)
#define KEY_INPUT_M									(0x32)
#define KEY_INPUT_N									(0x31)
#define KEY_INPUT_O									(0x18)
#define KEY_INPUT_P									(0x19)
#define KEY_INPUT_Q									(0x10)
#define KEY_INPUT_R									(0x13)
#define KEY_INPUT_S									(0x1F)
#define KEY_INPUT_T									(0x14)
#define KEY_INPUT_U									(0x16)
#define KEY_INPUT_V									(0x2F)
#define KEY_INPUT_W									(0x11)
#define KEY_INPUT_X									(0x2D)
#define KEY_INPUT_Y									(0x15)
#define KEY_INPUT_Z									(0x2C)

#define KEY_INPUT_0 								(0x0B)
#define KEY_INPUT_1									(0x02)
#define KEY_INPUT_2									(0x03)
#define KEY_INPUT_3									(0x04)
#define KEY_INPUT_4									(0x05)
#define KEY_INPUT_5									(0x06)
#define KEY_INPUT_6									(0x07)
#define KEY_INPUT_7									(0x08)
#define KEY_INPUT_8									(0x09)
#define KEY_INPUT_9									(0x0A)


/*�I���X�N���[���L�[�{�[�h�̃��[�h�w��*/
#define	DXP_OSK_ALL			(0x00000000)	//�S��
#define	DXP_OSK_DIGIT		(0x00000001)	//���p����
#define	DXP_OSK_SYMBOL		(0x00000002)	//���p�L��
#define	DXP_OSK_SMALL		(0x00000004)	//���p������
#define	DXP_OSK_LARGE		(0x00000008)	//���p�啶��
#define	DXP_OSK_DIGIT2		(0x00000100)	//�S�p����
#define	DXP_OSK_SYMBOL2		(0x00000200)	//�S�p�L��
#define	DXP_OSK_SMALL2		(0x00000400)	//�S�p������
#define	DXP_OSK_LARGE2		(0x00000800)	//�S�p�啶��
#define	DXP_OSK_HIRAGANA	(0x00001000)	//�Ђ炪��
#define	DXP_OSK_KATAKANA_H	(0x00002000)	//���p�J�^�J�i
#define	DXP_OSK_KATAKANA	(0x00004000)	//�S�p�J�^�J�i
#define	DXP_OSK_KANJI		(0x00008000)	//����
#define	DXP_OSK_URL			(0x00080000)	//URL���͎x��


/* �����������@�̒�`�BSetRandMode�֐��ɓn���܂��B */
#define DXP_RANDMODE_MT	(0)	//�����Z���k�E�c�C�X�^�[�����̗�����������g���܂��B�����炭�A�{�ƂƂ̌݊���������܂��B
#define DXP_RANDMODE_HW	(1)	//PSP�̃n�[�h�E�F�A�̗�����������g���܂��B�����ł��B

/* ������Ƃ����F�̒萔 */
#define DXP_COLOR_NONE		0x00000000
#define DXP_COLOR_BLACK		0xff000000
#define DXP_COLOR_RED 		0xff0000ff
#define DXP_COLOR_GREEN		0xff00ff00
#define DXP_COLOR_BLUE		0xffff0000
#define DXP_COLOR_WHITE		0xffffffff
#define DXP_COLOR_LITEGRAY	0xffbfbfbf
#define DXP_COLOR_GRAY		0xff7f7f7f
#define DXP_COLOR_DARKGRAY	0xff3f3f3f

/* �t�H���g���� */
#define DXP_FONT_ALIGN_LEFT		0x0000		//����
#define DXP_FONT_ALIGN_CENTER	0x0200		//����
#define DXP_FONT_ALIGN_RIGHT	0x0400		//�E��
#define DXP_FONT_WIDTH_FIX		0x0800		//�����w��p
#define DXP_FONT_ALIGN_DEFAULT	(DXP_FONT_ALIGN_LEFT)
#define DXP_FONT_DEFAULT_SIZE	16

#define DX_FONTTYPE_NORMAL				0 
#define DX_FONTTYPE_EDGE				1 
#define DX_FONTTYPE_ANTIALIASING		2
#define DX_FONTTYPE_ANTIALIASING_EDGE	3

/*�`��*/
#define DX_SCREEN_BACK		0xfffffffe
/*�O���t�B�b�N�̃t�H�[�}�b�g��`*/
#define DXP_FMT_5650		(0)	/* �e�N�X�`���A�p���b�g�A�`���*/
#define DXP_FMT_5551		(1)	/* �e�N�X�`���A�p���b�g�A�`���*/
#define DXP_FMT_4444		(2)	/* �e�N�X�`���A�p���b�g�A�`���*/
#define DXP_FMT_8888		(3)	/* �e�N�X�`���A�p���b�g�A�`���*/
#define DXP_FMT_T4			(4) /* �e�N�X�`���̂� */
#define DXP_FMT_T8			(5) /* �e�N�X�`���̂� */
//#define DXP_FMT_DXT1		(8) /* �e�N�X�`���̂� */
//#define DXP_FMT_DXT3		(9) /* �e�N�X�`���̂� */
//#define DXP_FMT_DXT5		(10)/* �e�N�X�`���̂� */

/*�`�惂�[�h��`*/
#define DX_DRAWMODE_NEAREST		0
#define DX_DRAWMODE_BILINEAR	1

/* �`��u�����h���[�h��` */
#define DX_BLENDMODE_NOBLEND						(0)				/* �m�[�u�����h*/
#define DX_BLENDMODE_ALPHA							(1)				/* ���u�����h*/
#define DX_BLENDMODE_ADD							(2)				/* ���Z�u�����h*/
#define DX_BLENDMODE_SUB							(3)				/* ���Z�u�����h*/
#define DX_BLENDMODE_MUL							(4)				/* ��Z�u�����h*/
#define DX_BLENDMODE_DESTCOLOR						(8)				/* �J���[�͍X�V����Ȃ�*/
#define DX_BLENDMODE_INVDESTCOLOR					(9)				/* �`���̐F�̔��]�l���|����*/
#define DX_BLENDMODE_INVSRC							(10)			/* �`�挳�̐F�𔽓]����*/
/*#define DX_BLENDMODE_MULA							(11)		*/
	/* �A���t�@�`�����l���l���t����Z�u�����h��PSP�̐����Ŏ����ł��܂���ł��������B*/

/* SetGraphMode�̖߂�l */
#define DX_CHANGESCREEN_OK							(0)					// ��ʕύX�͐�������
#define DX_CHANGESCREEN_RETURN						(-1)				// ��ʂ̕ύX�͎��s���A���̉�ʃ��[�h�ɖ߂��ꂽ

/* �Đ��^�C�v */
#define DX_PLAYTYPE_NORMAL	0
#define DX_PLAYTYPE_BACK	1
#define DX_PLAYTYPE_LOOP	3

/* ���y */
#define DX_SOUNDDATATYPE_MEMNOPRESS					(0)
//#define DX_SOUNDDATATYPE_MEMPRESS					(1)
#define DX_SOUNDDATATYPE_FILE						(3)


/* �\���̒�` */
typedef struct DATEDATA__
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Sec;
}DATEDATA;


typedef ScePspFVector3 VECTOR;

typedef struct MATRIX
{
	union
	{
		float m[4][4];
		ScePspFMatrix4 pspm;
	};
}MATRIX;

typedef struct QUATERNION
{
	float x;
	float y;
	float z;
	float w;
}QUATERNION;

typedef struct COLOR_U8
{
	union
	{
		u32 code;
		struct
		{
			u8 r;
			u8 g;
			u8 b;
			u8 a;
		};
	};
}COLOR_U8;

typedef struct COLOR_F
{
	float r,g,b,a;
}COLOR_F;

typedef struct VERTEX3D
{
	// �e�N�X�`�����W
	float u, v ;

	// �f�B�t���[�Y�J���[
	COLOR_U8 dif ;

	// �@��
	VECTOR norm ;

	// ���W
	VECTOR pos ;
} VERTEX3D;



#ifdef	__cplusplus
#define	DXPDEFARG(ARG)	=(ARG)		/* C++����̃f�t�H���g�������w�肷�邽�߂̃}�N�� */
extern "C" {
#else
#define	DXPDEFARG(ARG)
#endif

/** @defgroup �g�p�K�{�֐�*/
/*@{*/
/**
 * ���C�u�����̏�����(�{�ƌ݊�)
 * 
 * DXP���g�p�\�ȏ�Ԃɏ��������܂��BDXP���g�p����\�t�g�E�F�A�͂܂����� �֐����Ăяo���K�v������܂��B���̊֐����g���Ă���-1�i�G���[�j���Ԃ��Ă����ꍇ�͒����� �\�t�g���I�����Ȃ���΂Ȃ�܂���B
 * @retval 0 ����
 * @retval -1 ���s
 */
int DxLib_Init(void);
/**
 * ���C�u�����g�p�̏I���֐�(�{�ƌ݊�)
 * 
 * DXP�̎g�p���I������֐��ł��BDXP���g�p���Ă���\�t�g�͂��ׂă\�t�g�� �I��������O�ɂ��̊֐����ĂԕK�v������܂��B
 * @retval 0 ����
 * @retval -1 ���s
 */
int DxLib_End(void);
/**
 * ���C�u�������������ς݂��ǂ������ׂ�(�{�ƌ݊�)
 * 
 * DXP���������ς݂��ǂ������擾���܂��B
 * @retval 1 �������ς�
 * @retval 0 ��������
*/
int DxLib_Isinit(void);
/**
 * ���t���[���K�v�ȏ������s��(�{�ƌ݊�)
 * 
 * �{�^�����͏󋵂̍X�V�����s���܂��B���[�U�[��HOME�{�^���ɂ��I�����s�����Ƃ����A�Q�[�����I�����ׂ�����-1���Ԃ���܂��B
 * @retval 0 ����
 * @retval -1 ���s
*/
int ProcessMessage(void);
/*@}*/

/*@defgroup �����R�[�h�֘A*/
/*@{*/
/**
 * ���C�u�������g�������R�[�h��ύX����
 * 
 * �v���O���~���O�̊��ɂ���ĕ����R�[�h���قȂ�̂ŁA���̐ݒ�����܂��BOSK�╶����`�擙�Ŏg���܂��B
 * �����ݒ��SJIS�ł��BUTF-8���̕���DXP_CP_UTF8���w�肵�Ă��������B
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetCodepoint(int codepoint);
/*@}*/

/** @defgroup ���Ԋ֌W*/
/*@{*/
/**
 * �~���b�P�ʂ̐��x�����J�E���^�̌��ݒl�𓾂�(�{�ƌ݊�)
 * 
 * RTC(�{�̓����̎��v)�̒l���~���b�P�ʂł���킵���l���Ԃ��Ă��܂��B
 * @return RTC�̒l���~���b�P�ʂł���킵���l
*/
int GetNowCount(void);
/**
 * �}�C�N���b�P�ʂ̐��x�����J�E���^�̌��ݒl�𓾂�(�{�ƌ݊�)
 * 
 * RTC(�{�̓����̎��v)�̒l��Ԃ��܂��B���̊֐��̌Ăяo���ɏ��Ȃ��Ƃ�30�}�C�N���b�قǂ�����悤�ł��B
 * @return RTC�̒l
*/
u64 GetNowHiPerformanceCount(void);
/**
 * ���ݎ������擾����(�{�ƌ݊�)
 * 
 * DX���C�u�����Ǝ���DATEDATA�\���̂Ɍ��ݎ������i�[���܂��B
 * @param datebuf : DX���C�u�����Ǝ���DATEDATA�\���̂ւ̃|�C���^
 * @retval 0 ���� 
 * @retval -1 ���s
*/
int GetDateTime(DATEDATA *datebuf);
/**
 * �w�莞�Ԃ����������~�߂�
 * 
 * Win32API��Sleep�֐��Ɠ����ł��B
 * @param ms �~���b�P�ʂŏ������~�߂鎞�Ԃ��w�肵�܂��B
*/
void Sleep(int ms);
/*@}*/

/** @defgroup �����擾�֐�*/
/*@{*/
int SRand(int seed);							/**<�{�ƌ݊��֐�*/
/**
 * �������擾����(�{�ƌ݊�)
 * 
 * �����𓾂܂��B���̊֐���0����max�Ŏw�肵�����l�̂ǂꂩ�̐��l��Ԃ��܂��B
 * @param max �擾���闐���̍ő�l�B
 * @return 0����max�Ŏw�肵�����l�̂ǂꂩ�̐��l�B
**/
int GetRand(int max);
/**
 * �����������ύX����֐�(DXP�Ǝ�)
 * 
 * GetRand�����s�����Ƃ��Ɏg���闐����������w�肵�܂��B�G���[�����������ꍇ�ADXP_RANDMODE_HW���ݒ肳��܂��B�f�t�H���g�̐ݒ��DXP_RANDMODE_MT�ł��B
 * @par �w��\�Ȉ���:
 * - DXP_RANDMODE_MT - �����Z���k�E�c�C�X�^�[�����̗�����������g���܂��B�����炭�A�{�ƂƂ̌݊���������܂��B�ᑬ�ł��B
 * - DXP_RANDMODE_HW - PSP�̃n�[�h�E�F�A�̗�����������g���܂��B�����ł��B
 * @param mode - ������������w�肵�܂��B
 * @retval 0 ����
 * @retval -1 ���s
**/
int ChangeRandMode(int mode);
/*@}*/
/** @defgroup PSP�{�̐���֘A*/
/*@{*/
/**
 * PSP��CPU�N���b�N��ύX����
 * 
 * CPU�̃N���b�N��ύX���܂��B�o�X�N���b�N�͐ݒ肳�ꂽ���l�̔����ɐݒ肳��܂��B
 * 1�`333�ȊO�̒l���w�肳�ꂽ�ꍇ�͒P��CPU�N���b�N��Ԃ������̓���ɂȂ�܂��B
 * @param freq �ݒ肷��N���b�N
 * @return �֐����Ă΂ꂽ���_�ł�CPU�N���b�N
*/
int ChangeCpuSpeed(int freq);
/*@}*/

/** @defgroup �Z�p�֘A*/
/*@{*/

static inline VECTOR VGet(float x,float y,float z){VECTOR res = {x,y,z};return res;}
/**
 * ��̃x�N�g���𑫂��Z���܂��B
 *
 * @param v1 �������x�N�g��
 * @param v2 �����x�N�g��
 * @return �x�N�g�����m�𑫂�������
*/
static inline VECTOR VAdd(VECTOR v1,VECTOR v2){__asm__ volatile ("ulv.q C000,%0\n	ulv.q C010,%1\n	vadd.t C000,C000,C010\n	sv.s S000,0 + %0\n	sv.s S001,4 + %0\n	sv.s S002,8 + %0\n":"+m"(v1):"m"(v2));return v1;}
/**
 * ��̃x�N�g���������Z���܂��B
 *
 * @param v1 �������x�N�g��
 * @param v2 �����x�N�g��
 * @return �x�N�g�����m�������Z��������
*/
static inline VECTOR VSub(VECTOR v1,VECTOR v2){__asm__ volatile ("ulv.q C000,%0\n	ulv.q C010,%1\n	vsub.t C000,C000,C010\n	sv.s S000,0 + %0\n	sv.s S001,4 + %0\n	sv.s S002,8 + %0\n":"+m"(v1):"m"(v2));return v1;}
/**
 * ��̃x�N�g���̓��ς��v�Z���܂�
 *
 * @param v1 �x�N�g��
 * @param v2 �x�N�g��
 * @return ��̃x�N�g���̓���
*/
static inline float VDot(VECTOR v1,VECTOR v2){register float res;__asm__ volatile ("ulv.q C000,%1\n	ulv.q C010,%2\n	vdot.t S020,C000,C010\nmfv %0,S020\n":"=r"(res):"m"(v1),"m"(v2));return res;}
/**
 * ��̃x�N�g���̊O�ς��v�Z���܂�
 *
 * @param v1 �x�N�g��
 * @param v2 �x�N�g��
 * @return ��̃x�N�g���̊O��
*/
static inline VECTOR VCross(VECTOR v1,VECTOR v2){__asm__ volatile ("ulv.q C000,%0\nulv.q C010,%1\nvcrsp.t C000,C000,C010\nsv.s S000,0 + %0\nsv.s S001,4 + %0\nsv.s S002,8 + %0\n":"+m"(v1):"m"(v2));return v1;}
/**
 * �x�N�g�����X�J���[�{���܂�
 *
 * @param v �x�N�g��
 * @param scale �X�P�[�����O�̒l
 * @return ����
*/
static inline VECTOR VScale(VECTOR v,float scale){__asm__ volatile ("ulv.q C000,%0\nmtv %1,S010\nvscl.t C000,C000,S010\nsv.s S000,0 + %0\nsv.s S001,4 + %0\nsv.s S002,8 + %0\n":"+m"(v):"r"(scale));return v;}
/**
 * �x�N�g���̑傫�����v�Z���܂�
 *
 * @param v �x�N�g��
 * @return �x�N�g���̑傫��
*/
static inline float VSize(VECTOR v){float res;__asm__ volatile ("ulv.q C000,%1\nvdot.t S010,C000,C000\nvsqrt.s S010,S010\nsv.s %0,S010\n":"=r"(res):"m"(v));return res;}
/**
 * �x�N�g���̑傫���̓����v�Z���܂�
 *
 * @param v �x�N�g��
 * @return �x�N�g���̑傫���̓��
*/
static inline float VSquareSize(VECTOR v){register float res;__asm__ volatile("ulv.q C000,%1\nvdot.t S010,C000,C000\nmfv %0,S010\n":"=r"(res):"m"(v));return res;}
/**
 * ���K�x�N�g��(������1�ƂȂ�悤�ȃx�N�g��)�����߂܂�
 *
 * @param v �x�N�g��
 * @return ���K�x�N�g��
*/
static inline VECTOR VNorm(VECTOR v){__asm__ volatile("ulv.q C000,%0\nvdot.t S010,C000,C000\nvrsq.s S010,S010\nvscl.t C000,C000,S010\nusv.s S000,0 + %0\nusv.s S001,4 + %0\nusv.s		S002,	8 + %0\n":"+m"(v));return v;}
/**
 * �x�N�g�����s��ō��W�ϊ��ɂ����܂��B
 *
 * @param v �x�N�g��
 * @param m �ϊ��s��
 * @return �ϊ�����
*/
static inline VECTOR VTransform(VECTOR v,MATRIX m){__asm__ volatile("ulv.q C000,%0\nvone.s S003\nulv.q R100,0  + %1\nulv.q R101,16 + %1\nulv.q R102,32 + %1\nulv.q R103,48 + %1\nvtfm4.q C010,M100,C000\nsv.s S010,0 + %0\nsv.s S011,4 + %0\nsv.s S012,8 + %0\n":"+m"(v):"m"(m));return v;}
/**
 * �x�N�g�����s��ō��W�ϊ��ɂ����܂��B�������A���s�ړ����s���܂���
 *
 * @param v �x�N�g��
 * @param m �ϊ��s��
 * @return �ϊ�����
*/
static inline VECTOR VTransformSR(VECTOR v,MATRIX m){__asm__ volatile("ulv.q C000,%0\nulv.q R100,0  + %1\nulv.q R101,16 + %1\nulv.q R102,32 + %1\nulv.q R103,48 + %1\nvtfm3.t C010,M100,C000\nsv.s S010,0 + %0\nsv.s S011,4 + %0\nsv.s S012,8 + %0\n":"+m"(v):"m"(m));return v;}
/**
 * �P�ʍs����擾���܂�
 *
 * @return �P�ʍs��
*/
static inline MATRIX MGetIdent(){MATRIX m;__asm__ volatile ("vmidt.q M000\nusv.q R000,0 + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m));return m;}
/**
 * �g��ϊ��s����擾���܂�
 *
 * @param v XYZ���ꂼ��̎������ɑ΂���g�嗦��\���x�N�g��
 * @return �g��ϊ��s��
*/
static inline MATRIX MGetScale(VECTOR v){MATRIX m;__asm__ volatile ("ulv.q R100,%1\nvmov.q R000,R100[x,0,0,0]\nvmov.q R001,R100[0,y,0,0]\nvmov.q R002,R100[0,0,z,0]\nvmov.q R003,R100[0,0,0,1]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"m"(v));return m;}
/**
 * ���s�ړ��s����擾���܂�
 *
 * @param v XYZ���ꂼ��̎������ɑ΂���ړ�������\���x�N�g��
 * @return ���s�ړ��s��
*/
static inline MATRIX MGetTranslate(VECTOR v){MATRIX m;__asm__ volatile("ulv.q R100,%1\nvmidt.q M000\nvmov.q R003,R100[x,y,z,1]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"m"(v));return m;}
/**
 * X����]�s����擾���܂�
 *
 * @param angle X���ɑ΂��Ẳ�]�p�x
 * @return ��]�s��
*/
static inline MATRIX MGetRotX(float angle){MATRIX m;__asm__ volatile("mtv %1,S100\nvcst.s S110,VFPU_2_PI\nvmul.s S100,S100,S110\nvmidt.q M000\nvrot.q R001,S100,[0,c,s,0]\nvrot.q R002,S100,[0,-s,c,0]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"r"(angle));return m;}
/**
 * Y����]�s����擾���܂�
 *
 * @param angle Y���ɑ΂��Ẳ�]�p�x
 * @return ��]�s��
*/
static inline MATRIX MGetRotY(float angle){MATRIX m;__asm__ volatile("mtv %1,S100\nvcst.s S110,VFPU_2_PI\nvmul.s S100,S100,S110\nvmidt.q M000\nvrot.q R000,S100,[c,0,-s,0]\nvrot.q R002,S100,[s,0,c,0]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"r"(angle));return m;}
/**
 * Z����]�s����擾���܂�
 *
 * @param angle Z���ɑ΂��Ẳ�]�p�x
 * @return ��]�s��
*/
static inline MATRIX MGetRotZ(float angle){MATRIX m;__asm__ volatile("mtv %1,S100\nvcst.s S110,VFPU_2_PI\nvmul.s S100,S100,S110\nvmidt.q M000\nvrot.q R000,S100,[c,s,0,0]\nvrot.q R001,S100,[-s,c,0,0]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"r"(angle));return m;}
/**
 * �C�ӎ���]�s����擾���܂�
 *
 * @param v ��]���̕������w�肷��x�N�g��
 * @param angle �w�肵�����ɑ΂��Ẳ�]�p�x
 * @return ��]�s��
*/
static inline MATRIX MGetRotAxis(VECTOR v,float angle){MATRIX m;__asm__ volatile("ulv.q R100,%2\nvdot.t S103,R100,R100\nvrsq.s S103,S103\nvscl.t R100,R100,S103\nmtv %1,S130\nvcst.s S103,VFPU_2_PI\nvmul.s S130,S130,S103\nvrot.p R101,S130,[s,c]\nvocp.s S121,S111\nvscl.t R102,R100,S121\nvmov.q R000,R100[x,y,z,0]\nvmov.q R001,R100[x,y,z,0]\nvmov.q R002,R100[x,y,z,0]\nvmov.q R003,R100[0,0,0,1]\nvscl.t R000,R000,S102\nvscl.t R001,R001,S112\nvscl.t R002,R002,S122\nvmul.t R102,R101[y,x,-x],R100[1,z,y]\nvadd.t R000,R000,R102\nvmul.t R102,R101[-x,y,x],R100[z,1,x]\nvadd.t R001,R001,R102\nvmul.t R102,R101[x,-x,y],R100[y,x,1]\nvadd.t R002,R002,R102\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"r"(angle),"m"(v));return m;}
/**
 * ����x�N�g���̕�������ʂ̃x�N�g���̕����ɉ�]�����]�s����擾���܂�
 *
 * �x�N�g���̂Ȃ��p��90�x�𒴂��Ă���Ɛ������l���擾�ł��Ȃ��̂Œ��ӂ��Ă��������B
 * @param v1 �ϊ��������̃x�N�g��
 * @param v2 �ϊ�������̃x�N�g��
 * @return ��]�s��
*/
static inline MATRIX MGetRotVec2(VECTOR v1,VECTOR v2){MATRIX m;__asm__ volatile ("ulv.q R000,%1\nulv.q R001,%2\nvdot.t S002,R000,R000\nvdot.t S012,R001,R001\nvrsq.p R002,R002\nvscl.t R000,R000,S002\nvscl.t R001,R001,S012\nvdot.t S002,R000,R001\nvmul.s S012,S002,S002\nvocp.p R022,R002\nvsqrt.s S012,S032\nvcrsp.t R003,R000,R001\nvdot.t S032,R003,R003\nvrsq.s S032,S032\nvscl.t R003,R003,S032\nvscl.t R000,R003,S022\nvmov.q R100,R003[x,y,z,0]\nvmov.q R101,R003[x,y,z,0]\nvmov.q R102,R003[x,y,z,0]\nvmov.q R103,R003[0,0,0,1]\nvscl.t R100,R100,S000\nvscl.t R101,R101,S010\nvscl.t R102,R102,S020\nvmul.t R000,R002[x,y,-y],R003[1,z,y]\nvadd.t R100,R100,R000\nvmul.t R000,R002[-y,x,y],R003[z,1,x]\nvadd.t R101,R101,R000\nvmul.t R000,R002[y,-y,x],R003[y,x,1]\nvadd.t R102,R102,R000\nusv.q R100,0  + %0\nusv.q R101,16 + %0\nusv.q R102,32 + %0\nusv.q R103,48 + %0\n":"=m"(m):"m"(v1),"m"(v2));return m;}
/**
 * �w��̍��W�n�̃x�N�g������{���W�n��̃x�N�g���ɒ����ϊ��s����擾���܂�
 *
 * @param xAxis �ϊ������W�n��X��������\���x�N�g��
 * @param yAxis �ϊ������W�n��Y��������\���x�N�g��
 * @param zAxis �ϊ������W�n��Z��������\���x�N�g��
 * @param pos �ϊ������W�n�̌��_�̊�{���W�n�ł̈ʒu
 * @return �ϊ��s��
*/
static inline MATRIX MGetAxis1(VECTOR xAxis,VECTOR yAxis,VECTOR zAxis,VECTOR pos){MATRIX m;m.m[0][0] = xAxis.x;m.m[0][1] = xAxis.y;m.m[0][2] = xAxis.z;m.m[0][3] = 0.0f;m.m[1][0] = yAxis.x;m.m[1][1] = yAxis.y;m.m[1][2] = yAxis.z;m.m[1][3] = 0.0f;m.m[2][0] = zAxis.x;m.m[2][1] = zAxis.y;m.m[2][2] = zAxis.z;m.m[2][3] = 0.0f;m.m[3][0] = pos.x;m.m[3][1] = pos.y;m.m[3][2] = pos.z;m.m[3][3] = 1.0f;return m;}
/**
 * ��{���W�n�̃x�N�g�����w��̍��W�n��̃x�N�g���ɒ����ϊ��s����擾���܂�
 *
 * @param xAxis �ϊ�����W�n��X��������\���x�N�g��
 * @param yAxis �ϊ�����W�n��Y��������\���x�N�g��
 * @param zAxis �ϊ�����W�n��Z��������\���x�N�g��
 * @param pos �ϊ�����W�n�̌��_�̊�{���W�n�ł̈ʒu
 * @return �ϊ��s��
*/
static inline MATRIX MGetAxis2(VECTOR xAxis,VECTOR yAxis,VECTOR zAxis,VECTOR pos){MATRIX m;__asm__ volatile ("ulv.q C010,%1\nulv.q C020,%2\nulv.q C030,%3\nulv.q C040,%4\nvdot.t S003,C010,C040[-x,-y,-z]\nvdot.t S013,C020,C040[-x,-y,-z]\nvdot.t S023,C030,C040[-x,-y,-z]\nvmov.q C040,C040[0,0,0,1]\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"m"(xAxis),"m"(yAxis),"m"(zAxis),"m"(pos));return m;}
/**
 * �s�񓯎m�𑫂��Z���܂�
 *
 * @param m1 �������x�N�g��
 * @param m2 �����x�N�g��
 * @return ���ʂ̍s��
*/
static inline MATRIX MAdd(MATRIX m1,MATRIX m2){MATRIX m;__asm__ volatile ("ulv.q R000,0  + %1\nulv.q R001,16 + %1\nulv.q R002,32 + %1\nulv.q R003,48 + %1\nulv.q R100,0  + %2\nulv.q R101,16 + %2\nulv.q R102,32 + %2\nulv.q R103,48 + %2\nvadd.q R200,R000,R100\nvadd.q R201,R001,R101\nvadd.q R202,R002,R102\nvadd.q R203,R003,R103\nusv.q R200,0  + %0\nusv.q R201,16 + %0\nusv.q R202,32 + %0\nusv.q R203,48 + %0\n":"=m"(m):"m"(m1),"m"(m2));return m;}
/**
 * �s�񓯎m����Z���܂�
 *
 * @param m1 �|������x�N�g��
 * @param m2 �|����x�N�g��
 * @return ���ʂ̍s��
*/
static inline MATRIX MMult(MATRIX m1,MATRIX m2){MATRIX m;__asm__ volatile ("ulv.q R000,0  + %1\nulv.q R001,16 + %1\nulv.q R002,32 + %1\nulv.q R003,48 + %1\nulv.q R100,0  + %2\nulv.q R101,16 + %2\nulv.q R102,32 + %2\nulv.q R103,48 + %2\nvmmul.q M200,M000,M100\nusv.q R200,0  + %0\nusv.q R201,16 + %0\nusv.q R202,32 + %0\nusv.q R203,48 + %0\n":"=m"(m):"m"(m1),"m"(m2));return m;}
/**
 * �s����g�債�܂�
 *
 * @param m �ϊ����s��
 * @param scale �g�嗦
 * @return ���ʂ̍s��
*/

static inline MATRIX MScale(MATRIX m,float scale){__asm__ volatile ("ulv.q R000,0 + %0\nulv.q R001,16 + %0\nulv.q R002,32 + %0\nulv.q R003,48 + %0\nmtv %1,S100\nvmscl.q M000,M000,S100\nusv.q R000,0 + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m):"r"(scale));return m;}

/**
 * �]�u�s����擾���܂�
 *
 * @param m �]�u�������s��
 * @return �]�u�s��
*/
static inline MATRIX MTranspose(MATRIX m){__asm__ volatile ("ulv.q R000,0  + %0\nulv.q R001,16 + %0\nulv.q R002,32 + %0\nulv.q R003,48 + %0\nusv.q C000,0  + %0\nusv.q C010,16 + %0\nusv.q C020,32 + %0\nusv.q C030,48 + %0\n":"=m"(m));return m;}
/**
 * �t�s����擾���܂�
 *
 * �s�񎮂�0�ƂȂ�悤�ȍs��ɂ͋t�s�񂪑��݂��Ȃ��̂ŁA�P�ʍs���Ԃ��܂�
 * @param m �t�ɂ������s��
 * @return �t�s��
*/
static inline MATRIX MInverse(MATRIX m){register float d;__asm__ volatile ("ulv.q R200,0  + %1\nulv.q R201,16 + %1\nulv.q R202,32 + %1\nulv.q R203,48 + %1\nvmul.q R100,R201[y,x,x,x],R202[z,z,y,y]\nvmul.q R100,R203[w,w,w,z],R100\nvmul.q R101,R201[z,z,y,y],R202[w,w,w,z]\nvmul.q R101,R203[y,x,x,x],R101\nvadd.q R100,R100,R101\nvmul.q R101,R201[w,w,w,z],R202[y,x,x,x]\nvmul.q R101,R203[z,z,y,y],R101\nvadd.q R100,R100,R101\nvmul.q R101,R203[y,x,x,x],R202[z,z,y,y]\nvmul.q R101,R201[w,w,w,z],R101\nvsub.q R100,R100,R101\nvmul.q R101,R203[z,z,y,y],R202[w,w,w,z]\nvmul.q R101,R201[y,x,x,x],R101\nvsub.q R100,R100,R101\nvmul.q R101,R203[w,w,w,z],R202[y,x,x,x]\nvmul.q R101,R201[z,z,y,y],R101\nvsub.q R100,R100,R101\nvdot.q S101,R100,R200[x,-y,z,-w]\nvabs.s S102,S101\nmfv %0,S102\n":"=r"(d):"m"(m));if(d < 0.00001f){__asm__ volatile("vmidt.q M000\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m));return m;}__asm__ volatile("vrcp.s S101,S101\nvscl.q C000,R100[x,-y,z,-w],S101\nvmul.q R102,R200[y,x,x,x],R203[w,w,w,z]\nvmul.q R103,R203[y,x,x,x],R200[w,w,w,z]\nvsub.q R102,R102,R103\nvmul.q C010,R102,R202[z,z,y,y]\nvmul.q R102,R200[z,z,y,y],R203[y,x,x,x]\nvmul.q R103,R203[z,z,y,y],R200[y,x,x,x]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R202[w,w,w,z]\nvadd.q C010,C010,R102\nvmul.q R102,R200[w,w,w,z],R203[z,z,y,y]\nvmul.q R103,R203[w,w,w,z],R200[z,z,y,y]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R202[y,x,x,x]\nvadd.q C010,C010,R102\nvscl.q C010,C010[-x,y,-z,w],S101\nvmul.q R102,R200[y,x,x,x],R203[w,w,w,z]\nvmul.q R103,R203[y,x,x,x],R200[w,w,w,z]\nvsub.q R102,R102,R103\nvmul.q C020,R102,R201[z,z,y,y]\nvmul.q R102,R200[z,z,y,y],R203[y,x,x,x]\nvmul.q R103,R203[z,z,y,y],R200[y,x,x,x]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R201[w,w,w,z]\nvadd.q C020,C020,R102\nvmul.q R102,R200[w,w,w,z],R203[z,z,y,y]\nvmul.q R103,R203[w,w,w,z],R200[z,z,y,y]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R201[y,x,x,x]\nvadd.q C020,C020,R102\nvscl.q C020,C020[x,-y,z,-w],S101\nvmul.q R102,R200[y,x,x,x],R202[w,w,w,z]\nvmul.q R103,R202[y,x,x,x],R200[w,w,w,z]\nvsub.q R102,R102,R103\nvmul.q C030,R102,R201[z,z,y,y]\nvmul.q R102,R200[z,z,y,y],R202[y,x,x,x]\nvmul.q R103,R202[z,z,y,y],R200[y,x,x,x]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R201[w,w,w,z]\nvadd.q C030,C030,R102\nvmul.q R102,R200[w,w,w,z],R202[z,z,y,y]\nvmul.q R103,R202[w,w,w,z],R200[z,z,y,y]\nvsub.q R102,R102,R103\nvmul.q R102,R102,R201[y,x,x,x]\nvadd.q C030,C030,R102\nvscl.q C030,C030[-x,y,-z,w],S101\nusv.q R000,0  + %0\nusv.q R001,16 + %0\nusv.q R002,32 + %0\nusv.q R003,48 + %0\n":"=m"(m));return m;}

/*@}*/

/** @defgroup ���͊֘A*/
/*@{*/
/**
 * PSP�̃{�^�����͏��𒼐ڎ󂯎��
 * 
 * @code
 * if(GetInputState() & DXP_INPUT_CIRCLE)printfDx("���{�^����������܂���");
 * @endcode
 * @return �{�^���̓��͏��
*/
int GetInputState(void);
/**
 * �W���C�p�b�h���ڑ�����Ă��鐔���擾����B(�{�ƌ݊�)
 * 
 * DXP�ł͂��̊֐��͕K��1��Ԃ��܂��BPSP�̃{�^�����͂�GetJoypadInputState�ɂ��APC�̃W���C�p�b�h���͂Ƃ��Ď󂯎�邱�Ƃ��ł��܂��B�ڍׂ͖{�Ƃ̃w���v���Q�Ƃ��Ă��������B
*/
int GetJoypadNum(void);
/**
 * �W���C�p�b�h�̓��͏󋵂𓾂�B(�{�ƌ݊�)
 * 
 * DX_INPUT_PAD1���w�肳��Ă����PSP�̃{�^�����͂��W���C�p�b�h���͂Ƃ��ĕϊ����ꂽ���ʂ��Ԃ���܂��B�����łȂ����0���Ԃ���܂��B
 * @param inputtype ���͏�Ԃ��擾����p�b�h�̎��ʎq
 * @return �W���C�p�b�h�`���ɕϊ����ꂽ���͏�
*/
int GetJoypadInputState(int inputtype) ;
/**
 * �W���C�p�b�h�̃A�i���O�I�ȃ��o�[���͏��𓾂�(�{�ƌ݊�)
 * 
 * DX_INPUT_KEY1���w�肳��Ă����-1000�`+1000�͈̔͂ŃA�i���O���͂��Ԃ���܂��B�����łȂ���Ό��ʂ�0�ƂȂ�܂��B
 * @param xbuf �p�b�h���o�[�̍��E�̓��͏�Ԃ��i�[���� int �^�ϐ��̃A�h���X
 * @param ybuf �p�b�h���o�[�̏㉺�̓��͏�Ԃ��i�[���� int �^�ϐ��̃A�h���X
 * @param inputtype ���͏�Ԃ��擾����p�b�h�̎��ʎq�B
 * @retval 0 ����
 * @retval -1 ���s
*/
int GetJoypadAnalogInput(int *xbuf,int *ybuf,int inputtype);
/**
 * �W���C�p�b�h�̃A�i���O�I�ȃ��o�[���͏��𓾂�i�E�X�e�B�b�N�p�j(�{�ƌ݊�)
 * 
 * PSP�ɂ͉E�X�e�B�b�N�����݂��Ȃ��̂ŁA�Ăяo���Ă��Ӗ�������܂���B
*/
int GetJoypadAnalogInputRight(int *xbuf,int* ybuf,int inputtype);
/**
 * �W���C�p�b�h�̓��͂ɑΉ������L�[�{�[�h�̓��͂�ݒ肷��B(�{�ƌ݊�)
 * 
 * �W���C�p�b�h�̓��͂��L�[�{�[�h�̓��͂ɕϊ�����ׂ̑Ή������܂��B
 *
*/
int SetJoypadInputToKeyInput(int inputtype,int padinput,int keyinput1,int keyinput2,int keyinput3,int keyinput4);
/**
 * �W���C�p�b�h�̐U�����J�n����(�{�ƌ݊�)
 * 
 * PSP�ɂ͐U���@�\�Ȃ�Ė����̂œ����܂���B���0���Ԃ���܂�
*/
int StartJoypadVibration(int inputtype,int power,int time);
/**
 * �W���C�p�b�h�̐U�����~����(�{�ƌ݊�)
 * 
 * PSP�ɂ͐U���@�\�Ȃ�Ė����̂œ����܂���B���0���Ԃ���܂�
*/
int StopJoypadVibration(int inputtype);
/**
 * PSP�̃{�^�����͂ɑΉ������L�[�{�[�h�̓��͂�ݒ肷��B
 * 
 * �L�[�{�[�h�̃L�[�P��PSP�̃{�^���i�����j�����蓖�Ă�BSetJoypadInputToKeyInput�֐��Ɏ����Ă���B
 * @param key - KEY_INPUT_�@����n�܂�萔���ЂƂ����B
 * @param pspbutton - PSP_INPUT_�@����n�܂�萔���ЂƂA���͕�����|���Z�q�őg�ݍ��킹������
 * @retval 0 ����
 * @retval -1 ���s
 * @par �T���v���R�[�h
 * ���{�^�����͂�X�L�[�Ƃ��Ĉ��킹��B
 * @code
 * SetKeyInputToPspInput(KEY_INPUT_X,DXP_INPUT_CIRCLE);
 * @endcode
**/
int SetKeyInputToPspInput(int key,int pspbutton);
/**
 * ���ׂẴL�[�̉�����Ԃ��擾����(�L�[�̓���͂ł��Ȃ�)(�{�ƌ݊�)
 * 
 * @retval 1 �ǂꂩ�̃L�[�������ꂽ
 * @retval 0 �ǂ̃L�[��������Ă��Ȃ�
*/
int CheckHitKeyAll(void);												/**<�{�ƌ݊��֐�*/
/**
 * ����̃L�[�̓��͏�Ԃ𓾂�(�{�ƌ݊�)
 * 
 * @param keycode ���͏�Ԃ��擾����L�[�R�[�h(KEY_INPUT�Ŏn�܂�萔)
 * @retval 1 �L�[�������ꂽ
 * @retval 0 �L�[��������Ă��Ȃ�
*/
int CheckHitKey(int keycode);										/**<�{�ƌ݊��֐�*/
/**
 * �L�[�{�[�h�̂��ׂẴL�[�̉�����Ԃ��擾����(�{�ƌ݊�)
 * 
 * �{�Ƃ̃w���v���Q�Ƃ��Ă��������B
 * @param keystatebuf ���ׂẴL�[�̉�����Ԃ��i�[����o�b�t�@�̃|�C���^(256�v�f��char�^�z��)
 * @retval 0 ����
 * @retval -1 ���s
*/
int GetHitKeyStateAll(char* keystatebuf);							/**<�{�ƌ݊��֐�*/
/**
 * PSP�̃I���X�N���[���L�[�{�[�h�iOSK�j�𗘗p���ĕ�������͂��󂯕t����
 * 
 * OSK�𗘗p���ĕ�������͂��󂯕t���܂��B���[�U�[�����͂��I���Ă��̊֐����I������܂ő��̏����͈�؂ł��Ȃ��̂Œ��ӂ��Ă��������B
 * mode�ɂ͈ȉ��̒l���g���܂��B|���Z�q�ɂ���ĕK�v�Ȃ��̂�g�ݍ��킹�邩�ADXP_OSK_ALL�łǂ�ȓ��͂��󂯕t������悤�ɂ��Ă��������B
 * - DXP_OSK_ALL		�S��
 * - DXP_OSK_DIGIT		���p����
 * - DXP_OSK_SYMBOL		���p�L��
 * - DXP_OSK_SMALL		���p������
 * - DXP_OSK_LARGE		���p�啶��
 * - DXP_OSK_DIGIT2		�S�p����
 * - DXP_OSK_SYMBOL2	�S�p�L��
 * - DXP_OSK_SMALL2		�S�p������
 * - DXP_OSK_LARGE2		�S�p�啶��
 * - DXP_OSK_HIRAGANA	�Ђ炪��
 * - DXP_OSK_KATAKANA_H	���p�J�^�J�i
 * - DXP_OSK_KATAKANA	�S�p�J�^�J�i
 * - DXP_OSK_KANJI		����
 * - DXP_OSK_URL		URL���͎x��
 * 
 * @param buf ���[�U�[�����͂�����������i�[���邽�߂̃o�b�t�@
 * @param buflen �o�b�t�@�̃o�C�g��
 * @param mode �ǂ̂悤�ȕ������󂯕t���邩�w�肷��
 * @param title OSK�̉E���ɕ\������镶����
 * @param init OSK�̓��̓G���A�ɍŏ�������͂���Ă��镶����
 * @retval 0 ����
 * @retval -1 ���s
*/
int GetTextOSK(char *buf,int buflen,int mode,const char *title,const char *init);
/*@}*/

/** @defgroup �t�@�C���ǂݍ��݊֘A*/
/*@{*/
int	FileRead_open(const char *filename,int async DXPDEFARG(0));		/**<�{�ƌ݊��֐��@�������A���݃A�[�J�C�u�ɂ͖��Ή�*/
int	FileRead_size(const char *filename);							/**<�{�ƌ݊��֐�*/
int	FileRead_close(int filehandle);									/**<�{�ƌ݊��֐�*/
int	FileRead_tell(int filehandle);									/**<�{�ƌ݊��֐�*/
int	FileRead_seek(int filehandle,int offset,int origin);			/**<�{�ƌ݊��֐�*/
int	FileRead_read(void *buffer,int readsize,int filehandle);		/**<�{�ƌ݊��֐�*/
int	FileRead_idle_chk( int filehandle);								/**<�{�ƌ݊��֐�*/
int	FileRead_eof(int filehandle);									/**<�{�ƌ݊��֐�*/
int	FileRead_gets(char *buffer,int buffersize,int filehandle);		/**<�{�ƌ݊��֐�*/
int	FileRead_getc(int filehandle);									/**<�{�ƌ݊��֐�*/
//int	FileRead_scanf(int filehandle,const char *format, ...);	//���̊֐��̑Ή��\��͂���܂���B
/*@}*/


/** @defgroup �O���t�B�b�N�X�֘A */
/*@{*/
/**
 * ������`�悵�܂�(�{�ƌ݊�)
 * 
 * @param x1 �����̎n�_��X���W
 * @param y1 �����̎n�_��Y���W
 * @param x2 �����̏I�_��X���W
 * @param y2 �����̏I�_��Y���W
 * @param color �����̐F
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawLine(int x1,int y1,int x2,int y2,int color);
/**
 * ��`��`�悵�܂�(�{�ƌ݊�)
 * 
 * @param x1,y1 ��`�̍���̍��W
 * @param x2,y2 ��`�̉E���̍��W
 * @param color ��`�̐F
 * @param fillflag �h��Ԃ��t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawBox(int x1,int y1,int x2,int y2,int color,int fillflag);
/** 
 * �~��`�悵�܂�(�{�ƌ݊�)
 * 
 * @param x,y �~�̒��S���W
 * @param r �~�̔��a
 * @param color �~�̐F
 * @param fillflag �h��Ԃ��t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawCircle(int x,int y,int r,int color,int fillflag);
/** 
 * �ȉ~��`�悵�܂�(�{�ƌ݊�)
 * 
 * @param x,y �ȉ~�̒��S���W
 * @param rx �ȉ~��X�������a
 * @param ry �ȉ~��Y�������a
 * @param color �ȉ~�̐F
 * @param fillflag �h��Ԃ��t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawOval(int x,int y,int rx,int ry,int color,int fillflag);
/**
 * �O�p�`��`�悵�܂�(�{�ƌ݊�)
 * 
 * @param x1,y1 ���_���W1
 * @param x2,y2 ���_���W2
 * @param x3,y3 ���_���W3
 * @param color �O�p�`�̐F
 * @param fillflag �h��Ԃ��t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color,int fillflag);
/**
 * �_��`�悷��(�{�ƌ݊�)
 * 
 * @param x,y �_�̍��W
 * @param color �_�̐F
 * @retval 0 ����
 * @retval -1 ���s
 * 
*/
int DrawPixel(int x,int y,int color);
/**
 * �w��_�̐F�R�[�h���擾����(�{�ƌ݊�)
 * 
 * @param x,y �_�̍��W
 * @return �_�̐F�R�[�h
*/
int GetPixel(int x,int y);
/**
 * �摜��ǂݍ���ŉ�ʂɕ\������(�{�ƌ݊�)
 * 
 * @param x ���[�h�����摜��`�悷���`�̍��㒸�_��X���W
 * @param y ���[�h�����摜��`�悷���`�̍��㒸�_��Y���W
 * @param filename ���[�h����摜�p�X�̕����񂪂���|�C���^
 * @param trans ���ߐF�����邩�A�̃t���O�BTRUE�œ��ߐF�L���ɂȂ�
 * @retval 0 ����
 * @retval -1 ���s
*/
int LoadGraphScreen(int x,int y,const char* filename,int trans);
/**
 * �摜��ǂݍ��݂܂�(�قږ{�ƌ݊�)
 * 
 * �T�C�Y��512x512�܂ł�PNG�摜��ǂݍ��ނ��Ƃ��o���܂�
 * �߂�l��DrawGraph���̊֐��ŉ摜��`�悷��ەK�v�ɂȂ�̂ŁA�ϐ��ɕۑ����Ă�������
 * @param FileName �t�@�C����
 * @return �O���t�B�b�N�X�n���h��
 * @retval -1 ���s
*/
int LoadGraph(const char *FileName);
/**
 * �ꖇ�̉摜���畡���̃O���t�B�b�N�X�n���h�������܂�(�قږ{�ƌ݊�)
 * 
 * �摜�S�̂̃T�C�Y��512x512�܂łł��BPNG�摜�����ǂݍ��߂Ȃ��̂�LoadGraph�Ɠ��l�ł��B
 * @param filename �����ǂݍ��݂���摜�t�@�C��������̃|�C���^
 * @param allnum �摜�̕�������
 * @param xnum �摜�̉������ɑ΂��镪����
 * @param ynum �摜�̏c�����ɑ΂��镪����
 * @param xsize �������ꂽ�摜��̉���
 * @param ysize �������ꂽ�摜��̏c��
 * @param handlebuf �����ǂݍ��݂��ē����O���t�B�b�N�n���h����ۑ�����int�^�̔z��ւ̃|�C���^
*/
int LoadDivGraph(const char *filename,int allnum,int xnum,int ynum,int xsize,int ysize,int *handlebuf);
/**
 * ��̃O���t�B�b�N�X�n���h���𐶐�����(�{�ƌ݊�)
 * 
 * 512x512���傫�ȃO���t�B�b�N�͍��Ȃ��̂Œ��ӂ��Ă�������
 * @param xsize �쐬����O���t�B�b�N�X�n���h���̕�
 * @param ysize �쐬����O���t�B�b�N�X�n���h���̍���
 * @return �O���t�B�b�N�X�n���h��
 * @retval -1 ���s
*/
int MakeGraph(int xsize,int ysize);
/**
 * �摜�̕`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x �`���_��X���W
 * @param y �`���_��Y���W
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawGraph(int x,int y,int gh,int trans);
/**
 * �摜�̍��E���]�`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x �`���_��X���W
 * @param y �`���_��Y���W
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawTurnGraph(int x,int y,int gh,int trans);
/**
 * �摜�̊g��k���`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x1 �����X���W
 * @param y1 �����Y���W
 * @param x2 �E����X���W
 * @param y2 �E����Y���W
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawExtendGraph(int x1,int y1,int x2,int y2,int gh,int trans);
/**
 * �摜�̉�]�`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x �摜���S�ʒu��X���W
 * @param y �摜���S�ʒu��Y���W
 * @param scale �g�嗦�@1.0f�œ��{
 * @param angle ��]�p�x
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @param turn �摜���]�t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawRotaGraph(int x,int y,float scale,float angle,int gh,int trans,int turn DXPDEFARG(0));
/**
 * �摜�̉�]�`����s���܂��B(�{�ƌ݊�)
 * 
 * float�^�ō��W�w��ł���DrawRotaGraph�ł��B
 * @param x �摜���S�ʒu��X���W
 * @param y �摜���S�ʒu��Y���W
 * @param scale �g�嗦�@1.0f�œ��{
 * @param angle ��]�p�x
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @param turn �摜���]�t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawRotaGraphF(float x,float y,float scale,float angle,int gh,int trans,int turn DXPDEFARG(0));
/**
 * ��]���S���W�w��\�ȉ摜�̉�]�`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x,y �X�N���[����̉�]���S�ʒu�̍��W
 * @param cx,cy �摜��̉�]���S�ʒu�̍��W
 * @param scale �g�嗦�@1.0f�œ��{
 * @param angle ��]�p�x
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @param turn �摜���]�t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawRotaGraph2(int x,int y,int cx,int cy,float scale,float angle,int gh,int trans,int turn DXPDEFARG(0));
/**
 * ��]���S���W�w��\�ȉ摜�̉�]�`����s���܂��B(�{�ƌ݊�)
 * 
 * float�^�ō��W�w��ł���DrawRotaGraph2�ł��B
 * @param x,y �X�N���[����̉�]���S�ʒu�̍��W
 * @param cx,cy �摜��̉�]���S�ʒu�̍��W
 * @param scale �g�嗦�@1.0f�œ��{
 * @param angle ��]�p�x
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @param turn �摜���]�t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawRotaGraph2F(float x,float y,float cx,float cy,float scale,float angle,int gh,int trans,int turn DXPDEFARG(0));
/**
 * �摜�̎��R�ό`�`����s���܂��B(�{�ƌ݊�)
 * 
 * @param x1 �����X���W
 * @param y1 �����Y���W
 * @param x2 �E���X���W
 * @param y2 �E���Y���W
 * @param x3 �E����X���W
 * @param y3 �E����Y���W
 * @param x4 ������X���W
 * @param y4 ������Y���W
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawModiGraph(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int gh,int trans);
/**
 * �摜�̎��R�ό`�`����s���܂��B(�{�ƌ݊�)
 * float�^�ō��W�w��ł���DrawModiGraph�ł��B
 * 
 * @param x1 �����X���W
 * @param y1 �����Y���W
 * @param x2 �E���X���W
 * @param y2 �E���Y���W
 * @param x3 �E����X���W
 * @param y3 �E����Y���W
 * @param x4 ������X���W
 * @param y4 ������Y���W
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans ���߃t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawModiGraphF(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4, int gh, int trans );
/**
 * �摜�̎w���`�����̕`����s���܂��B(�{�ƌ݊�)
 * 
 * @param destx �`���̕`���_X���W
 * @param desty �`���̕`���_Y���W
 * @param srcx �`�挳�̕`���_X���W
 * @param srcy �`�挳�̕`���_Y���W
 * @param width �`�悷�镔����X�����̑傫��
 * @param height �`�悷�镔����Y�����̑傫��
 * @param gh �`�悷��O���t�B�b�N�X�̃n���h��
 * @param trans �摜���߃t���O
 * @param turn �摜���]�t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawRectGraph(int destx,int desty,int srcx,int srcy,int width,int height,int gh,int trans,int turn);
/**
 * �w��̃O���t�B�b�N�X�n���h���̎w��̕���������͈͂Ƃ���O���t�B�b�N�X�n���h�������(�{�ƌ݊�)
 * 
 * @param srcx,srcy �O���t�B�b�N���̔����o��������`�̍�����W
 * @param width,height �����o���O���t�B�b�N�̃T�C�Y
 * @param gh �����o�������O���t�B�b�N�̃n���h��
 * @return �O���t�B�b�N�X�n���h��
 * @retval -1 ���s
*/
int DerivationGraph(int srcx,int srcy,int width,int height,int gh);
/**
 * �`���ɐݒ肳��Ă���O���t�B�b�N����w��̈��ʂ̃O���t�B�b�N�֓ǂ݂���(�{�ƌ݊�)
 * 
 * �Ǎ���̃O���t�B�b�N��VRAM��ɂ���ASwizzle����Ă��炸�A�p���b�g�摜�łȂ��K�v������܂��B
 * �Ȃ��A���̊֐��œǂݍ��񂾃O���t�B�b�N�͓��ߐF�̋@�\������ɓ����Ȃ��̂Œ��ӂ��Ă��������B
 * @param x1 �擾����O���t�B�b�N�̈�i��`�j�̍��㒸�_��X���W
 * @param y1 �擾����O���t�B�b�N�̈�i��`�j�̍��㒸�_��Y���W
 * @param x2 �擾����O���t�B�b�N�̈�̉E�����_�{�P��X���W
 * @param y2 �擾����O���t�B�b�N�̈�̉E�����_�{�P��Y���W
 * @param gh �Ǎ���̃O���t�B�b�N�X�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int GetDrawScreenGraph(int x1,int y1,int x2,int y2,int gh);
/**
 * �O���t�B�b�N�̑傫���𓾂�(�{�ƌ݊�)
 * 
 * @param gh �O���t�B�b�N�X�n���h��
 * @param xbuf �O���t�B�b�N�̉������i�[����int�^�ϐ��ւ̃|�C���^
 * @param ybuf �O���t�B�b�N�̏c�����i�[����int�^�ϐ��ւ̃|�C���^
 * @retval 0 ����
 * @retval -1 ���s
 */
int GetGraphSize(int gh,int *xbuf,int *ybuf);
/**
 * �S�ẴO���t�B�b�N�X�n���h�����폜���܂�(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int InitGraph(void);
/**
 * �O���t�B�b�N�X�n���h�����폜���܂�(�{�ƌ݊�)
 * 
 * @param gh �O���t�B�b�N�X�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int DeleteGraph(int gh);
/**
 * �`�惂�[�h���Z�b�g����(�{�ƌ݊�)
 *
 * - DX_DRAWMODE_NEAREST �l�A���X�g�l�C�o�[�@�ŕ`�悷��(����l)
 * - DX_DRAWMODE_BILINEAR �o�C���j�A�@�ŕ`�悷��
 *
 * @param mode �`�惂�[�h
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetDrawMode(int mode);
/**
 * �u�����h���[�h���Z�b�g����(�{�ƌ݊�)
 *
 * �`���̉摜�ɑ΂��Ăǂ̂悤�ɐF�������邩�w��ł��܂��B
 * �ڂ����͖{�Ƃ̃��t�@�����X����ǂ��Ă��������B
 * �Ȃ��A�ꕔ�̃u�����h���[�h��PSP�̃n�[�h�E�F�A�̐���Ŏ����ł��Ă��܂���B�ȉ��̂ǂꂩ���g���Ă��������B
 * - DX_BLENDMODE_NOBLEND
 * - DX_BLENDMODE_ALPHA
 * - DX_BLENDMODE_ADD
 * - DX_BLENDMODE_SUB
 * - DX_BLENDMODE_MUL
 * - DX_BLENDMODE_DESTCOLOR
 * - DX_BLENDMODE_INVDESTCOLOR
 * - DX_BLENDMODE_INVSRC
 * @param blendmode �u�����h���[�h
 * @param param �p�����[�^
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetDrawBlendMode(int blendmode,int param);
/**
 * �`��P�x���Z�b�g����(�{�ƌ݊�)
 *
 * 0-255�͈̔͂Œl���w�肵�Ă�������
 * @param red �Ԃ̕`��P�x
 * @param green �΂̕`��P�x
 * @param blue �̕`��P�x
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetDrawBright(int red,int green,int blue);
/**
 * �摜�ɐݒ肷�铧�ߐF��ݒ肵�܂�(�{�ƌ݊�)
 * 
 * �摜�̓ǂݍ��ݎ��ɉ摜�ɐݒ肳��铧�ߐF��ݒ肵�܂��B�摜���A���t�@�����������A���߃t���O��ON�̏ꍇ���̒l�ɋ߂��F�̃G���A�͕`�悳��܂���B
 * @param color ���ߐF
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetTransColor(int color);
/**
 * ��ʃ��[�h��ݒ肵�܂�(�{�ƌ݊�)
 * 
 * ��ʃT�C�Y��480x272�ȊO�w��ł��܂���B����ȊO���w�肷��ƃG���[�ɂȂ�܂��B
 * bpp�ɂ�1�s�N�Z��������̃r�b�g�����w�肵�܂��B�w��\�Ȓl�͈ȉ��̒ʂ�ł��B
 * - 16 DXP_FMT_5551�Ɠ����ł�
 * - 32 DXP_FMT_8888�Ɠ����ł�
 * - DXP_FMT_4444 �e�F��4bit�����蓖�Ă��܂�
 * - DXP_FMT_5650 �ԂƐ�5bit�A�΂�6bit�����蓖�Ă��܂�
 * - DXP_FMT_5551 �e�F��5bit�����蓖�Ă��܂�
 * - DXP_FMT_8888 �e�F��8bit�����蓖�Ă��܂�
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetGraphMode(int xsize,int ysize,int bpp);
/**
 * ���݂̉�ʂ̑傫���ƃJ���[�r�b�g���𓾂�(�{�ƌ݊�)
 * 
 * @param xsize,ysize ��ʂ̕��ƍ������i�[����int�^�ϐ��ւ̃|�C���^
 * @param bpp �P�s�N�Z�������艽�r�b�g�g���Ă��邩���i�[����int�^�ϐ��ւ̃|�C���^
 * @retval 0 ����
 * @retval -1 ���s
*/
int GetScreenState(int *xsize,int *ysize,int *bpp);
/**
 * �`��\�̈�̃Z�b�g(�{�ƌ݊�)
 * 
 * @param x1,y1 �`��\�̈��������`�̍���̍��W
 * @param x2,y2 �`��\�̈��������`�̉E���̍��W
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetDrawArea(int x1,int y1,int x2,int y2);
/**
 * �`���̃O���t�B�b�N�X���N���A���܂��B(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int ClearDrawScreen(void);
/**
 * �F�R�[�h��Ԃ��܂�(�{�ƌ݊�)
 * 
 * @param Red �Ԑ���
 * @param Green �ΐ���
 * @param Blue ����
 * @return �F�R�[�h
*/
int GetColor(int Red,int Green,int Blue);
/**
 * �`����ύX���܂�
 * 
 * VRAM��ɂ���Swizzle����Ă��Ȃ��O���t�B�b�N�X�n���h����n���ƁA�`���Ƃ��Đݒ肳��܂�
 * LoadDivGraph��DerivationGraph�֐����g���č�����悤�ȁA�ꖇ�̃e�N�X�`���𕡐��̃O���t�B�b�N�X�����L����悤��
 * �O���t�B�b�N�X�n���h�����w�肷��Ɨ\�����Ȃ����ʂɂȂ�\��������܂�
 * @param ghandle �`���ɂ���O���t�B�b�N�X�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetDrawScreen(int ghandle);
/**
 * ����ʂƕ\��ʂ��������܂�(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int ScreenFlip(void);
/**
 * ����ʂ̓��e��\��ʂɃR�s�[���AScreenFlip�����s���܂��B(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int ScreenCopy(void);
/**
 * ScreenFlip�AScreenCopy�֐����s���ɐ��������҂������邩�̃t���O���Z�b�g(�{�ƌ݊�)
 * 
 * ����������҂ꍇ�A�Q�[���̕��ׂ��傫���Ƃ����Ȃ�60��30��20��60�̖񐔂�fps�ɐ؂�ւ���ėV�тɂ������Ƃ�����܂��B
 * ����������҂��Ȃ��ꍇ�A�Q�[���̕��ׂ��傫����fps���A���I�ɕϓ����܂��B�������A��ʂ��኱��������Ƃ�����܂��B
 * @param flag ���������҂��t���O
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetWaitVSyncFlag(int flag);
/**
 * �\�Ȃ�VRAM��Ƀe�N�X�`���̃R�s�[��u���A������g���悤�ɂ���
 * 
 * 0.5.0�ȍ~�ł̓��C���������[��̃e�N�X�`����ێ�����悤�ɂȂ�܂����B
 * @retval 0 ����
 * @retval -1 ���s
*/
int MoveGraphToVRAM(int gh);
/**
 * VRAM��Ƀe�N�X�`���̃f�[�^������΁A��������C���������[�̃e�N�X�`���ɃR�s�[���Ă���VRAM��̗̈���J������
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int MoveGraphToDDR(int gh);
/**
 * �\�Ȃ�O���t�B�b�N��Swizzle����
 * 
 * �󂫃�����������Ȃ��Ǝ��s���邱�Ƃ�����܂��B
 * @retval 0 ����
 * @retval -1 ���s
*/
int SwizzleGraph(int gh);
/**
 * �\�Ȃ�O���t�B�b�N��Swizzle����������
 * 
 * �󂫃�����������Ȃ��Ǝ��s���邱�Ƃ�����܂��B
 * @retval 0 ����
 * @retval -1 ���s
*/
int UnswizzleGraph(int gh);
/**
 * �O���t�B�b�N�̃t�H�[�}�b�g��ύX����B
 * 
 * �O���t�B�b�N�̃t�H�[�}�b�g��ύX�ł��܂��B
 * �ȉ��̃t�H�[�}�b�g�̂ݓ��o�͑Ή��ł��B�p���b�g�摜�ɂ͓K�p�ł��܂���B
 * - DXP_FMT_4444
 * - DXP_FMT_5650
 * - DXP_FMT_5551
 * - DXP_FMT_8888
 * @param gh �Ώۂ̃O���t�B�b�N�X�n���h��
 * @param psm �s�N�Z���t�H�[�}�b�g
 * @retval 0 ����
 * @retval -1 ���s
*/
int ConvertGraphFormat(int gh,int psm);
/**
 * �摜��ǂݍ��ލۂ�Swizzle�������s�����ǂ����ݒ肵�܂�
 * 
 * @param flag �摜�ǂݍ��ݎ�Swizzle�������s�t���O
 * @retval 0 ����
 * @retval -1 �G���[����
*/
int SetCreateSwizzledGraphFlag(int flag);
/**
 * GPU���������I����̂�҂��܂�
 * 
 * @retval 0 ����
 * @retval -1 �G���[����
*/
int WaitGPUSync(void);
/**
 * ��ʂ̃s�N�Z���t�H�[�}�b�g�𓾂�
 * 
 * �߂�l�͎��̂����ǂꂩ�ł�
 * - DXP_FMT_4444
 * - DXP_FMT_5551
 * - DXP_FMT_5650
 * - DXP_FMT_8888
 * @return �s�N�Z���t�H�[�}�b�g
 * @retval -1 ���s
*/
int GetDisplayFormat(void);
/**
 * ��ʂ̃s�N�Z���t�H�[�}�b�g��ύX����
 * 
 * �w��\�Ȓl�͎��̂����ǂꂩ�ł�
 * - DXP_FMT_4444
 * - DXP_FMT_5551
 * - DXP_FMT_5650
 * - DXP_FMT_8888
 * @return �s�N�Z���t�H�[�}�b�g
 * @retval -1 ���s
*/
int SetDisplayFormat(int psm);
/**
 * �t���[���o�b�t�@�i����ʁj�ւ̃|�C���^��Ԃ��܂�
 * 
 * @return �t���[���o�b�t�@�̃|�C���^
*/
void *GetFramebufferAddress(void);
/**
 * �f�B�X�v���C�o�b�t�@�i�\��ʁj�ւ̃|�C���^��Ԃ��܂�
 * 
 * @return �f�B�X�v���C�o�b�t�@�ւ̃|�C���^
*/
void *GetDisplaybufferAddress(void);
/*@}*/

/**@defgroup �T�E���h�֘A*/
/*@{*/

/**
 * ���y�f�[�^��ǂݍ��݂܂�(�{�ƌ݊�)
 * 
 * MP3�`���̉��y�t�@�C���̂ݓǂݍ��߂܂��B�Ȃ��A�ǂݍ��ݍ�Ƃ͕ʃX���b�h�ōs���܂��B
 * 
 * @param filename �t�@�C����
 * @return �T�E���h�n���h��
 * @retval -1 ���s
*/
int LoadSoundMem(const char *filename);
/**
 * �T�E���h�n���h������Đ����܂�(�{�ƌ݊�)
 * 
 * LoadSoundMem�œǂݍ��񂾉��y�t�@�C�����Đ����邱�Ƃ��ł��܂�
 * �w��\�ȍĐ��^�C�v�͈ȉ��̒ʂ�
 * - DX_PLAYTYPE_NORMAL �Đ����I���܂ő҂��܂�
 * - DX_PLAYTYPE_BACK �o�b�N�O���E���h�ōĐ����܂��B���y�̏I���܂ōĐ��������~���܂�
 * - DX_PLAYTYPE_LOOP �o�b�N�O���E���h�ōĐ����܂��B���[�v�Đ��ɂȂ�܂�
 * 
 * @param handle �T�E���h�n���h��
 * @param playtype �Đ��^�C�v
 * @param rewindflag �ŏ�����Đ������邩�ǂ���
 * @retval -1 ���s
*/
int PlaySoundMem(int handle,int playtype,int rewindflag DXPDEFARG(1));
/**
 * �T�E���h�n���h�����Đ������`�F�b�N���܂�(�{�ƌ݊�)
 * 
 * @param handle �T�E���h�n���h��
 * @retval 1 �Đ���
 * @retval 0 ��~
 * @retval -1 ���s
*/
int CheckSoundMem(int handle);
/**
 * �T�E���h�n���h���̍Đ����~�߂܂�(�{�ƌ݊�)
 * 
 * @param handle �T�E���h�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int StopSoundMem(int handle);
/**
 * �T�E���h�n���h�����폜���܂�(�{�ƌ݊�)
 * 
 * @param handle �T�E���h�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int DeleteSoundMem(int handle);
/**
 * �S�ẴT�E���h�n���h�����폜���܂��B(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int InitSoundMem(void);
/**
 * �T�E���h�n���h���Ƀp����ݒ肷��(�{�ƌ݊�)
 * 
 * ���̒l��ݒ肷��ƍ��̉��ʂ�������A���̒l��ݒ肷��ƉE�̉��ʂ�������܂��B0�Ȃ獶�E�̉��ʂ��������Ȃ�܂��B
 * @param handle �T�E���h�n���h��
 * @param pan �p���̒l�B-10000�`+10000�͈̔�
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetPanSoundMem(int pan,int handle);
/**
 * �T�E���h�n���h���̉��ʂ�ύX����(�{�ƌ݊�)
 * 
 * @param handle �T�E���h�n���h��
 * @param volume ���ʁB0�`255�͈̔�
 * @retval 0 ����
 * @retval -1 ���s
*/
int ChangeVolumeSoundMem(int volume,int handle);
/**
 * ���[�v�ʒu��ݒ肷��(�{�ƌ݊�)
 * 
 * ���[�v�Đ����ɁA�w�肵���ꏊ����Đ��ĊJ�ł���悤�ɂ��܂��B
 * @param looppos_s ���[�v�ĊJ�ʒu�i�~���b�P�ʁj
 * @param handle �T�E���h�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetLoopPosSoundMem(int looppos_s,int handle);
/**
 * ���[�v�ʒu��ݒ肷��(�{�ƌ݊�)
 * 
 * ���[�v�Đ����ɁA�w�肵���ꏊ����Đ��ĊJ�ł���悤�ɂ��܂��B�T���v�����Ŏw�肵�܂��B
 * @param looppos ���[�v�ĊJ�ʒu�i�T���v�����P�ʁj
 * @param handle �T�E���h�n���h��
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetLoopSamplePosSoundMem(int looppos,int handle);
/** 
 * LoadSoundMem�̋�����ύX���܂��B(�{�ƌ݊�)
 * 
 * LoadSoundMem�̓�������ɉ��y�f�[�^��W�J���A������Đ����܂��B
 * ���̊֐��ł��̎��̋�����ύX�ł��܂��B
 * �w��\�Ȓl�͈ȉ��̒ʂ�
 * - DX_SOUNDDATATYPE_MEMNOPRESS	��������Ƀf�[�^��W�J���A������Đ����܂��B
 * - DX_SOUNDDATATYPE_FILE �X�g���[�~���O�Đ������܂��B
 * 
 * �ȉ��̒l�͌��ݎg���܂���B
 * - DX_SOUNDDATATYPE_MEMPRESS
 * - DX_SOUNDDATATYPE_MEMPRESS_PLUS
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetCreateSoundDataType(int type);
/*@}*/

/**@defgroup ������`��֘A*/
/*@{*/
/**
 * �������`�悷��(�{�ƌ݊�)
 * 
 * @param x,y ������̍���̍��W
 * @param String �`�悷�镶����
 * @param color ������̐F
 * @param EdgeColor ������̃G�b�W�̐F
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawString(int x,int y,const char *String,int color, int EdgeColor DXPDEFARG(0));
/**
 * �����t���������`�悷��(�{�ƌ݊�)
 * 
 * @param x,y ������̍���̍��W
 * @param color ������̐F
 * @param String �����t��������
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawFormatString(int x,int y,int color,const char *String,...);
/**
 * �������`�悵�����̉������擾����(�{�ƌ݊�)
 * 
 * @param str ������
 * @param len ������̒���
 * @return �������`�悵�����̒���
 * @retval -1 ���s
*/
int GetDrawStringWidth(const char *str,int len);
/**
 * �����t���������`�悵�����̉������擾����(�{�ƌ݊�)
 * 
 * @param format �����t��������
 * @param ... �����t��������ɕt������f�[�^
 * @return �������`�悵�����̒���
 * @retval -1 ���s
*/
int GetDrawFormatStringWidth(const char *format, ... );
/**
 * �t�H���g�T�C�Y��ݒ肷��(�{�ƌ݊�)
 * 
 * @param size �t�H���g�T�C�Y
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetFontSize(int size);
/**
 * �t�H���g�̕����̑�����ݒ肷��
 * 
 * ���̊֐��͓����܂���B0��Ԃ������ł��B
 * @param thickness �t�H���g�̑����i��������܂��j
 * @retval 0 ����
 * @retval -1 ���s
*/
int SetFontThickness(int thickness);
/**
 * �t�H���g��ύX����(�{�ƌ݊�)
 * 
 * pgf�t�H���g�̂ݓǂݍ��߂܂��B
 * @param fontname �t�H���g��
 * @param charset �����R�[�h
 * @retval 0 ����
 * @retval -1 ���s
*/
int ChangeFont(const char *fontname,int charset);
/**
 * �t�H���g�^�C�v��ύX����(�{�ƌ݊�)
 * 
 * �ȉ��̒l���w��\�ł����A�A���`�G�C���A�V���O�̗L���͕`��ɔ��f����܂���B
 * - -1�iDX_FONTTYPE_NORMAL�Ƃ��Ĉ����܂��j
 * - DX_FONTTYPE_NORMAL
 * - DX_FONTTYPE_EDGE
 * - DX_FONTTYPE_ANTIALIASING�@
 * - DX_FONTTYPE_ANTIALIASING_EDGE
 * @param type �t�H���g�^�C�v
 * @retval 0 ����
 * @retval -1 ���s
*/
int ChangeFontType(int type);

/**
 * �t�H���g��ǂݍ��݁A�n���h�����쐬����
 * 
 * @param fontname �t�H���g��
 * @param size �t�H���g�T�C�Y
 * @param thick �t�H���g�̑����i��������܂��j
 * @param fonttype �t�H���g�^�C�v �ڂ�����SetFontType���Q�Ƃ��Ă��������B
 * @param charset �����Z�b�g�@DXP_CP_�����ɂ��Ă�����̂��w�肷��K�v������܂�
 * @return �t�H���g�n���h��
 * @retval -1 ���s
*/
int CreateFontToHandle(const char *fontname,int size,int thick,int fonttype,int charset DXPDEFARG(DXP_CP_SJIS));
/**
 * �t�H���g�n���h�����폜����(�{�ƌ݊�)
 *
 * @param handle �폜�������t�H���g�n���h��
 * @retval 0 ����
 * @retval -1 ���s
 *
*/
int DeleteFontToHandle(int handle);
/**
 * �t�H���g�n���h�����w�肵�ĕ������`�悷��(�{�ƌ݊�)
 * 
 * @param x,y ������̍���̍��W
 * @param str �`�悷�镶����
 * @param color ������̐F
 * @param handle �t�H���g�n���h��
 * @param edgecolor ������̃G�b�W�̐F
 * @retval 0 ����
 * @retval -1 ���s
*/
int DrawStringToHandle(int x,int y,const char *str,int color,int handle,int edgecolor DXPDEFARG(0));
/**
 * �t�H���g�n���h�����w�肵�ď����t���������`�悷��(�{�ƌ݊�)
 * 
 * @param x,y ������̍���̍��W
 * @param color ������̐F
 * @param handle �t�H���g�n���h��
 * @param format �����t��������
 * @retval 0 ����
 * @retval -1 ���s
*/
int	DrawFormatStringToHandle(int x,int y,int color,int handle,const char *format,...);
/**
 * �t�H���g�n���h�����w�肵�ĕ������`�悵�����̉������擾����(�{�ƌ݊�)
 * 
 * @param str ������
 * @param len ������̒���
 * @param handle �t�H���g�n���h��
 * @return �������`�悵�����̒���
 * @retval -1 ���s
*/
int GetDrawStringWidthToHandle(const char *str,int len,int handle);
/**
 * �t�H���g�n���h�����w�肵�ď����t���������`�悵�����̉������擾����(�{�ƌ݊�)
 * 
 * @param handle �t�H���g�n���h��
 * @param format �����t��������
 * @param ... �����t��������ɕt������f�[�^
 * @return �������`�悵�����̒���
 * @retval -1 ���s
*/
int GetDrawFormatStringWidthToHandle(int handle,const char *format, ... );
/**
 * �t�H���g�n���h����S�ĊJ������(�{�ƌ݊�)
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int InitFontToHandle(void);
/*@}*/

/**@defgroup �f�o�b�O�֘A*/
/*@{*/
/** 
 * ���O�t�@�C���ɏo�͂��܂�
 * printf�̂悤�Ɏg���ƃ��O�t�@�C���ɏ������܂�܂��B
*/
int AppLogAdd(const char *format,...);
/**
 * �ȈՕ�����o��
 * 
 * DOS���̂悤�ɕ�������o�͂��܂��B
 * ���ɒᑬ�ȏ�A���s�t�@�C���T�C�Y����C�ɖc���̂ŃQ�[���ɂ͕s�����ł��B
 * �Ȃ��AScreenFlip�֐���ScreenCopy�֐����Ă΂��܂ŕ`�悳��܂���̂Œ��ӂ��Ă��������B
 * @retval 0 ����
 * @retval -1 ���s
*/
int printfDx(const char *format,...);
/**
 * �ȈՕ�����o�̗͂������폜���܂�
 * 
 * @retval 0 ����
 * @retval -1 ���s
*/
int clsDx(void);
/*@}*/

int DrawLine3D(VECTOR pos1,VECTOR pos2,int color);
int DrawTriangle3D(VECTOR pos1,VECTOR pos2,VECTOR pos3,int color,int fillflag);
int DrawPolygon3D( VERTEX3D *Vertex, int PolygonNum, int GrHandle, int TransFlag );

int SetCameraPositionAndTargetAndUpVec( VECTOR Position, VECTOR Target, VECTOR Up );
int SetupCamera_ProjectionMatrix( MATRIX ProjectionMatrix );
int SetupCamera_Perspective( float Fov );
int SetCameraNearFar( float Near, float Far );
int SetCameraDotAspect(float Aspect);

int SetUseZBufferFlag(int flag);
int SetUseZBuffer3D(int flag);
int SetWriteZBufferFlag(int flag);
int SetWriteZBuffer3D(int flag);

int SetTransformToWorld( MATRIX *Matrix );

static inline COLOR_U8 GetColorU8(int red,int green,int blue,int alpha)
{
	COLOR_U8 c;
	c.a = alpha & 0xff;
	c.b = blue & 0xff;
	c.g = green & 0xff;
	c.r = red & 0xff;
	return c;
}

#ifdef __cplusplus
}
#endif
#endif//DXLIBP_H__