/****************************************************
*		DX���C�u����Portable	Ver0.4.14			*
*		�����	�F�����J��							*
*		�ŏI�X�V�F2009/04/22						*
*		�X�V�����̓t�@�C���̍Ō�̂ق��ɂ���܂��B	*
****************************************************/
/****************************************************
     ---CAUTION!----
���̃��C�u�����͊���̃��C�u���������Ă��܂��B
readme.txt�ɏڍׂ�����܂��̂ŁA���Q�Ƃ��������B
****************************************************/
/****************************************************
		���l��										

	���̃��C�u�����ɂ���
'DX���C�u����Portable'�i�ȍ~'DXP'�ƕ\�L�j��'DX���C�u����'�i�ȍ~'�{��'�ƕ\�L�j�̕s���S�݊����C�u�����Ƃ��Đ��삳��Ă��܂��B
PSP�̃v���O�������ȒP�ɍ���悤�Ȋ֐��S�̒񋟂��ړI�ł��B
���݂̐�����j�������������Ă����܂��B�����悻��̂��̂قǗD�揇�ʂ������ł��B
���{�ƂƂ̌݊������\�Ȍ���ǋ����邱��
���\�Ȍ��荂���ɓ��삷�邱��				�iC++�ł͂Ȃ�C�ŋL�q���Ă���̂͂��̂���
����݊��֐����\�Ȍ���{�ƂɎ����邱��	�iGetInputState�֐��͖{�Ƃ�GetJoypadInputState�֐������f���ł�
�����[�U�[���[�h�ň��S�ɓ�������			�i�v���O���}�̃~�X��PSP�{�̂��N���s�\�ɂȂ�����V�����ɂȂ�Ȃ��̂Łc

�Ȃ��A�{�ƂƊ��S�݊��̊֐��ɂ̓R�����g�����Ă��܂���B

****************************************************
****************************************************
		�p��W�Ƃ�									

	Swizzle�i�������[����j
Swizzle�Ƃ�PSP�̃Q�[���v���O���~���O�ɂ����Ė����Ă͂Ȃ�Ȃ��Z�p�ł��B
�v���O���~���O�̐��E�ł͂悭�Q�ƂȂǂƖ󂳂�܂����APSP�ł���Swizzle�Ƃ�
GPU�̃L���b�V�����ő���L�����p�ł���悤�Ƀe�N�X�`����ϊ����邱�Ƃ������܂��B
DX���C�u����Portable�ł͊�{�I�Ɉӎ����Ȃ��Ă�Swizzle�e�N�X�`�����g����悤�ɂȂ��Ă��܂��B
�������A�ꕔ�̊֐��ɂ����Ė�肪�������܂��B�e�N�X�`���ɒ��ڃA�N�Z�X���������ł��B
Swizzle�̓e�N�X�`����̃s�N�Z������בւ���悤�ȑ���Ȃ̂ŁA������l�����Ɏ��s����Ǝv��ʎ��Ԃ������܂��B
�����炭�v���O�������̂͐���ɓ��삷��ł��傤���A�v�������ʂɂ͂Ȃ�Ȃ��ł��傤�B
�ŁA�摜��ǂݍ��ލۂ�Swizzle�����s���邩�ǂ����̃t���O������ɐ݂��܂����B
�ݒ肷��ɂ�SetCreateSwizzledGraphFlag�֐����Ă�ł��������B0�ȊO�Ȃ�Swizzle����i����j�A0�Ȃ�Swizzle���Ȃ��Ƃ����Ӗ��ł��B

�Ƃ������Ƃ��āA�܂��O���t�B�b�N�f�[�^��������֐������ĂȂ��̂ł����i��
�Ȃ��A�e�N�X�`����Swizzle��Ԃƒʏ��Ԃɑ��݂ɕϊ�����֐������܂����B
SwizzleGraph�֐���UnswizzleGraph�֐��ł��B���x�̔�r�����Ăɂ�ɂ₵�Ă���������

	Slice
GPU�̃L���b�V�����ł��邾���L�����p���邽�߂ɉ摜���c�ɐ؂��ĕ`�悷�邱�Ƃł��BDX���C�u����Portable�ł͎����I�Ɏ��s����܂��B
��������32��64�o�C�g���Ƃɍs���ƍō��̃p�t�H�[�}���X�ƂȂ�̂ł����A�摜�ɂ���čœK�Ȓl���قȂ�悤�Ȃ̂Ő؂�ւ�����悤�ɂ��܂����B
SetSliceSize�֐��������p���������B�L���Ȉ�����16,32,64,128�̂����ꂩ�ł��B����ȊO�̓G���[��Ԃ��A�؂�ւ��͍s���܂���B
****************************************************/
#ifndef DXLIBP_H__
#define DXLIBP_H__
#ifdef	__cplusplus
#define	DXPDEFARG(ARG)	=(ARG)		/*C++�̃f�t�H���g�����̋@�\��C�ł̓G���[�ɂȂ�̂ő΍�*/
extern "C" {
#else
#define	DXPDEFARG(ARG)
#endif

#include <pspkernel.h>
//#include <stdio.h>

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

/*�萔��`��*/
	/*�����Z�b�g*/
	#define DXP_CHARSET_ASCII		0x00
	#define	DXP_CHARSET_US			0x01
	#define	DXP_CHARSET_LATIN_I		0x05
	#define	DXP_CHARSET_LATIN_II	0x13
	#define	DXP_CHARSET_RUSSIAN		0x0b
	#define DXP_CHARSET_SHIFTJIS	0x0d
	#define	DXP_CHARSET_GBK			0x0e
	#define	DXP_CHARSET_KOREAN		0x0f
	#define	DXP_CHARSET_BIG5		0x10
	#define	DXP_CHARSET_CYRILLIC	0x12
	#define	DXP_CHARSET_UTF8		0xff
	#define	DXP_CHARSET_DEFAULT		DXP_CHARSET_SHIFTJIS
	/*�t�@�C���̓ǂݍ���*/
	#define DXP_FILEREAD_DOS	0x00000000
	#define DXP_FILEREAD_UNIX	0x00000001
	/*�`��*/
	#define DXP_SCREEN_BACK		0xfffffffe

	/*�O���t�B�b�N�̃t�H�[�}�b�g��`*/
	#define DXP_FMT_5650		(0)	/* �e�N�X�`���A�p���b�g�A�`���*/
	#define DXP_FMT_5551		(1)	/* �e�N�X�`���A�p���b�g�A�`���*/
	#define DXP_FMT_4444		(2)	/* �e�N�X�`���A�p���b�g�A�`���*/
	#define DXP_FMT_8888		(3)	/* �e�N�X�`���A�p���b�g�A�`���*/
	#define DXP_FMT_T4			(4) /* �e�N�X�`���̂� */
	#define DXP_FMT_T8			(5) /* �e�N�X�`���̂� */
	#define DXP_FMT_DXT1		(8) /* �e�N�X�`���̂� */
	#define DXP_FMT_DXT3		(9) /* �e�N�X�`���̂� */
	#define DXP_FMT_DXT5		(10)/* �e�N�X�`���̂� */

	/*�`�惂�[�h��`*/
	#define DX_DRAWMODE_NEAREST	0
	#define DX_DRAWMODE_BILINEAR	1

	 /*�`��u�����h���[�h��`*/
	#define DX_BLENDMODE_NOBLEND						(0)				/* �m�[�u�����h*/
	#define DX_BLENDMODE_ALPHA							(1)				/* ���u�����h*/
	#define DX_BLENDMODE_ADD							(2)				/* ���Z�u�����h*/
	#define DX_BLENDMODE_SUB							(3)				/* ���Z�u�����h*/
	#define DX_BLENDMODE_MUL							(4)				/* ��Z�u�����h*/
	#define DX_BLENDMODE_DESTCOLOR						(8)				/* �J���[�͍X�V����Ȃ�*/
	#define DX_BLENDMODE_INVDESTCOLOR					(9)				/* �`���̐F�̔��]�l���|����*/
	#define DX_BLENDMODE_INVSRC						(10)			/* �`�挳�̐F�𔽓]����*/
	/*#define DX_BLENDMODE_MULA						(11)		*/
		/* �A���t�@�`�����l���l���t����Z�u�����h��PSP�̐����Ŏ����ł��܂���ł��������B*/

	/*����*/
		/*�{�^������*/
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
		/*�I���X�N���[���L�[�{�[�h����*/
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

	#define PSPCTRLMODE_MSWP		0x00000001
	#define PSPCTRLMODE_KB			0x00000002
	#define PSPANALOGMODE_CKEY		0x00000010
	#define PSPANALOGMODE_NOCKEY	0x00000020

	/*���y�Đ�*/
	#define DX_PLAYTYPE_NORMAL							(0)												// �m�[�}���Đ�
	#define DX_PLAYTYPE_BACK				  			(1)							// �o�b�N�O���E���h�Đ�
	#define DX_PLAYTYPE_LOOP							(3)	// ���[�v�Đ�

	#define DXP_FONT_ALIGN_LEFT		0x0000		//����
	#define DXP_FONT_ALIGN_CENTER	0x0200		//����
	#define DXP_FONT_ALIGN_RIGHT	0x0400		//�E��
	#define DXP_FONT_WIDTH_FIX		0x0800		//�����w��p
	#define DXP_FONT_ALIGN_DEFAULT	(DXP_FONT_ALIGN_LEFT)
	#define DXP_FONT_COLOR_NONE		0x00000000	//����
	#define DXP_FONT_COLOR_BLACK	0xff000000
	#define DXP_FONT_COLOR_RED 		0xff0000ff
	#define DXP_FONT_COLOR_GREEN	0xff00ff00
	#define DXP_FONT_COLOR_BLUE		0xffff0000
	#define DXP_FONT_COLOR_WHITE	0xffffffff
	#define DXP_FONT_COLOR_LITEGRAY	0xffbfbfbf
	#define DXP_FONT_COLOR_GRAY		0xff7f7f7f
	#define DXP_FONT_COLOR_DARKGRAY	0xff3f3f3f
	#define DXP_FONT_DEFAULT_SIZE	16			//1.0f�̎��̃h�b�g�T�C�Y

/*�\���̒�`��*/
	typedef struct tagVECTOR
	{
		float	x, y, z ;
	}VECTOR, *LPVECTOR ;

	typedef struct tagVERTEX_3D
	{
		VECTOR					pos ;
		unsigned char			b, g, r, a ;
		float					u, v ;
	}VERTEX_3D,*LPVERTEX_3D;
/*�֐���`��*/
	/*�K�{�֐�*/
		int DxLib_IsInit();
		int DxLib_Init();
		int DxLib_End();
		int ProcessMessage();
	/*�ėp�֐�*/
		/*�ݒ�n*/
		/*�擾�n*/
		/*�@�\�n*/
		int GetNowCount();
		u64 GetNowHiPerformanceCount();
		int	SRand(int RandSeed);
		int	GetRand(int RandMax);
		int Sleep(int time);							//����WindowsAPI�Ȃ�ł�����
		int WaitTimer(int time);
		void AppLogAdd(const char *Format,...);
		int sjis2unicode(u16 sjis,u16 *punicode);		//sjis�̕�����Unicode�ɂ��܂�
		int unicode2sjis(u16 unicode,u16 *psjis);		//Unicode�̕�����sjis�ɂ��܂�
		int unicodestr2sjisstr(const u16 *unis,char *sjiss);
		int sjisstr2unicodestr(const char *sjiss,u16 *unis);
#define AppLogAdd2(FMT,...)	AppLogAdd("%s,%s,%d,%s\t"FMT,__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);
	/*���͊֘A�֐�*/
		int InitInput();							/*�������B�ʏ�͌Ă΂Ȃ��Ă�����*/
		int RenewInput();							/*�X�V�BProcessMessage�̒��ŌĂ΂��̂Œʏ�͌Ă΂Ȃ��Ă�����*/
		int GetInputState();						/*�{�^�����͂̏��𓾂�*/
		int GetAnalogInput(int *XBuf,int *YBuf);	/*�A�i���O�p�b�h�̓��͂𓾂�B�͈͂́}1000�B�V�є��肪�K�v�ł��B*/
		int GetHitKeyStateAll( char *KeyStateBuf );	/*�S�Ă̓��̓o�b�t�@��0���i�[���A���߂�l�Ƃ���0��Ԃ��܂��B*/
		int GetJoypadInputState( int InputType );	/*PSP�̓��͂�JOYPAD���͂Ɍ����ĂĕԂ��܂��BDX���C�u�����Ɍ݊�����邽�߂̊֐��ł��B*/
		int GetTextOSK(char *buf,int buflen,int inputmode,const char *title DXPDEFARG(NULL),const char *init DXPDEFARG(NULL));
													/*�I���X�N���[���L�[�{�[�h���g���܂��B	*/
													/*buf		�F�擾����������̊i�[��	*/
													/*buflen	�F�ő啶����				*/
													/*inputmode	�FDXP_OSKMODE�̂ǂꂩ		*/
													/*title		�F�^�C�g��������			*/
													/*init		�F����������				*/
		int	SetPspCtrlMode(int Mode);
		int	SetPspAnalogMode(int Mode);

	/*�`��֘A�֐�*/
		/*�ݒ�n*/
		int SetDisplayFormat(int format);				/*��ʂ̃t�H�[�}�b�g��ύX���܂��BDXP_FMT_5650���w�肷��ƃ}�X�N�@�\���g���Ȃ��Ȃ�̂Œ��ӂ��Ă��������B*/
		int SetDrawScreen(int ghandle);
		int SetDrawMode(int mode);
		int SetDrawBlendMode(int BlendMode,int Param);
		int SetDrawBright(int red,int green,int blue);
		int SetCreateSwizzledGraphFlag(int Flag);		/*�摜�̍쐬�A�ǂݍ��ݎ��iMakeGraph�����j��Swizzle��Ԃɂ��邩�w��*/
		int SetSliceSize(int size);						/*�`�掞��slice���\�ȏꍇ�A���o�C�g���E�ōs�����w��*/
		int SetTransColor( int Red , int Green , int Blue );
		int SetDrawArea(int x1,int y1,int x2,int y2);
		/*�擾�n*/
		int GetDisplayFormat();
		int GetColor(int red,int green,int blue);
		int GraphSize2DataSize(int width,int height,int Format);				/*�c���̑傫���ƃt�H�[�}�b�g����f�[�^�T�C�Y���v�Z����B*/
		int AppLogAdd_GraphicData(int gh);										/*�O���t�B�b�N�n���h���̏��������o���B�f�o�b�O��p*/
		int GetGraphSize(int gh,int *px,int *py);
		/*�ǂݍ��݌n*/
		int MakeGraph(int x,int y,int format DXPDEFARG(DXP_FMT_8888));		/*�Ō�̈����ɉ摜�t�H�[�}�b�g���L�q����K�v������܂�*/
		int LoadRAWData(const char *FileName,int SizeX,int SizeY,int Format);/*RAW�t�@�C����ǂݍ��ނ̂Ɏg���܂�*/
		int LoadDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf);
		int DerivationGraph( int SrcX, int SrcY,int Width, int Height, int src );
		int LoadGraph(const char *FileName);
		int DeleteGraph(int GrHandle);
		int ConvertGraphFormat(int gh,int psm);/*�O���t�B�b�N�̃t�H�[�}�b�g��ύX����B�������̐ߖ�ƍ������ɂǂ����BDX_SCREEN_BACK�ɂ͓K�p�ł��܂���*/
		/*�`��n*/
		/*
				�`�قƂ�ǂ̕`��n�֐��ɑ΂��钍�Ӂ`
			PSP�̃n�[�h�E�F�A�̐����A�����I�Ɏw�肳�����W���}4000�t�߂𒴂���Ƃ������ȕ`��ɂȂ�܂��B
			���ɁADrawRotaGraph�ł͊g�嗦�̏グ�����ɒ��ӂ��Ă��������B���܂����Ɖ��������@�����ݍl�����ł��B
		*/
		int ScreenFlip();
		int ScreenCopy();
		int ClearDrawScreen();
		int	DrawPixel( int x, int y, int Color);
		int	DrawLine( int x1, int y1, int x2, int y2, int Color);
		int DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color,int fill);
		int DrawBox(int x1,int y1,int x2,int y2,int color,int fillflag);
		int	DrawCircle( int x, int y, int r, int Color,int fill);
		int	DrawGraph(int x,int y,int gh,int trans);
		int DrawExtendGraph(int x1,int y1,int x2,int y2,int gh,int trans);
		int	DrawModiGraph(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int gh,int trans);
		int	DrawTurnGraph(int x,int y,int gh,int trans);
		int	DrawRotaGraph(int x,int y,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int DrawRotaGraph2(int x,int y,int cx,int cy,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));

		int	DrawModiGraphF(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,int gh,int trans);
		int	DrawRotaGraphF(float x,float y,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int	DrawRotaGraph2F(float x,float y,float cx,float cy,double ExtRate,double Angle,int gh,int trans,int turn DXPDEFARG(0));
		int DrawPolygon3D(VERTEX_3D *Vertex,int PolygonNum,int GrHandle,int TransFlag);//�e�X�g���ł��c

		/*���̑�*/
		int SwizzleGraph(int gh);	/*�w�肳�ꂽ�O���t�B�b�N��Swizzle����B�������A���C���������̋󂫗e�ʂ�����Ȃ��i�O���t�B�b�N�̎��T�C�Y��菭�Ȃ��j�Ǝ��s���܂��B*/
		int UnswizzleGraph(int gh);	/*�w�肳�ꂽ�O���t�B�b�N��Unswizzle����B�������A���C���������̋󂫗e�ʂ�����Ȃ��i�O���t�B�b�N�̎��T�C�Y��菭�Ȃ��j�Ǝ��s���܂��B*/
		int MoveGraphToDDR(int gh);	/*�w�肳�ꂽ�O���t�B�b�N��VRAM��ɂ���ꍇ�̓��C���������Ɉړ�*/
		int MoveGraphToVRAM(int gh);/*�w�肳�ꂽ�O���t�B�b�N�����C���������ɂ���ꍇ��VRAM��Ɉړ�*/
		void WaitGPUSync();			/*GPU���`����I����܂ő҂�*/
	/*������`��֘A�֐�*/
		int InitString();			/*�͂��߂�DrawString�n�֐����Ă΂ꂽ�Ƃ��ɌĂяo����܂��B���炩���ߌĂяo���Ă����̂���ł��B*/
		int EndString();			/*�t�H���g�f�[�^��S�ă������ォ��J�����܂��B�i�f�t�H���g�t�H���g�܂ށj*/
		int DrawString( int x, int y, const char *String, int Color, int EdgeColor DXPDEFARG(0));/*�܂�����ɓ��삵�܂���I*/
		int DrawFormatString(int x,int y,int color,const char *String,...);
		int	DrawFormatStringToHandle( int x, int y, int Color, int FontHandle, const char *FormatString, ... );
		int	CreateFontToHandle( const char *FontName, int Size, int Thick, int FontType , int CharSet , int EdgeSize , int Italic  , int DataIndex , int ID );
		int SetFontSize( int FontSize );
		int SetFontSizeF( float FontSize );
		int SetFontBackgroundColor(int Color);
		int SetFontAlignment(int Position,int Width);

	/*���y�֘A�֐�*/
		/*�ݒ�n*/
		int	SetCreateSoundDataType( int SoundDataType );
		int	ChangeVolumeSoundMem( int VolumePal, int SoundHandle );
		int	SetLoopPosSoundMem( int LoopTime, int SoundHandle );
		/*�擾�n*/
		int	CheckSoundMem( int handle );
		/*�ǂݍ��݌n*/
		int LoadSoundMem(const char* filename);
		//20090415 Mp3�̓ǂݍ��݂̍������̂��ߎd�l�ύX
		//int LoadStreamSound(const char* filename);
		int LoadStreamSound(const char *filename,int SetPcmLen DXPDEFARG(-1),int* AnsPcmLen DXPDEFARG(NULL));
		int	DeleteSoundMem( int SoundHandle, int LogOutFlag );
		/*�Đ��n*/
		int PlaySoundMem(int SoundHandle,int PlayType/*,int TopPositionFlag DXPDEFARG(1)*/);
		int StopSoundMem(int handle);
		int PlayStreamSound(int SoundHandle,int PlayType/*,int TopPositionFlag DXPDEFARG(1)*/);
		int getRefCount(int sh);
		//20090415
		int PlaySoundFile(const char *FileName , int PlayType ,int SetPcmLen DXPDEFARG(-1),int* AnsPcmLen DXPDEFARG(NULL));
		int CheckSoundFile();
		int StopSoundFile();
		//20090425
		int	SetTopPosSoundMem(int handle );

	/*�t�@�C���ǂݍ��݊֐�*/
		int FileRead_SetMode(int Mode);		/*Windows�ō�����e�L�X�g�̉��s�R�[�h�΍�*/
		int FileRead_open(const char *FileName);
		int FileRead_close(int FileHandle);
		int FileRead_size(const char *FileName);
		int FileRead_tell(int FileHandle);
		int FileRead_seek(int FileHandle,int Offset,int Origin);
		int FileRead_write(void * Buffer,int Size,int Num,int FileHandle);
		int FileRead_stdread(void * Buffer,int Size,int Num,int FileHandle);
		int FileRead_read(void * Buffer,int Size,int FileHandle);
		int FileRead_eof(int FileHandle);
		int FileRead_gets(char *Buffer,int Num,int FileHandle);
		int FileRead_getc(int FileHandle);
		int FileRead_scanf(int FileHandle,const char* Format,...);
		int FileRead_idle_chk( int FileHandle );/*���ݏ��1��Ԃ��܂�*/
	/*�ȈՕ�����`��*/
		void printfDx(const char *format,...);	/*��񂠂��蔼�p80������22�񂠂�܂��B�����o�b�t�@��2048�����ŁA����ȏ�͕`�悳��܂���B����ȊO�͖{�ƃ��C�u�����Ɠ����ł��B*/
		void clsDx();
		void DrawString_Shinonome(int x,int y,const char *str,int color);/*printfDx�Ŏg���Ă���S�p�`�惉�C�u�������Ăяo���܂��B*/
	/*�}�C�i�[�n*/
		int SaveDrawScreen( int x1, int y1, int x2, int y2, char *FileName );
	/*�f�o�b�O�p*/
		void*	TsFileLoad(const char* filename,int* FileSize);
		int	TsFileSave(const char* filename,unsigned int FileSize,const char* writedata);
		//int GetCpuUsage();//���쒆

/*DX���C�u��������R�s�y*/
	//20090405
	//DX���C�u�����{�Ƃ��R�s�[
	#define PAD_INPUT_DOWN								(0x00000001)	// ���`�F�b�N�}�X�N
	#define PAD_INPUT_LEFT								(0x00000002)	// ���`�F�b�N�}�X�N
	#define PAD_INPUT_RIGHT								(0x00000004)	// ���`�F�b�N�}�X�N
	#define PAD_INPUT_UP								(0x00000008)	// ���`�F�b�N�}�X�N
	#define PAD_INPUT_A									(0x00000010)	// �`�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_B									(0x00000020)	// �a�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_C									(0x00000040)	// �b�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_X									(0x00000080)	// �w�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_Y									(0x00000100)	// �x�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_Z									(0x00000200)	// �y�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_L									(0x00000400)	// �k�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_R									(0x00000800)	// �q�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_START								(0x00001000)	// �r�s�`�q�s�{�^���`�F�b�N�}�X�N
	#define PAD_INPUT_M									(0x00002000)	// �l�{�^���`�F�b�N�}�X�N
	//20090405
	#define KEY_INPUT_ESCAPE	0x0E
	#define KEY_INPUT_LEFT		0xCB
	#define KEY_INPUT_UP		0xC8
	#define KEY_INPUT_RIGHT		0xCD
	#define KEY_INPUT_DOWN		0xD0
	#define KEY_INPUT_LSHIFT	0x2A
	#define KEY_INPUT_LCONTROL	0x1D
	#define KEY_INPUT_Z			0x2C
	#define KEY_INPUT_X			0x2D
	#define	KEY_INPUT_A			0x1e
	#define	KEY_INPUT_S			0x1f
	#define	KEY_INPUT_Q			0x10
	#define	KEY_INPUT_W			0x11
	#define	KEY_INPUT_SPACE		0x39
	//20090405
	// �p�b�h���͎擾�p�����[�^
	#define DX_INPUT_KEY_PAD1							(0x1001)		// �L�[���͂ƃp�b�h�P����
	#define DX_INPUT_PAD1								(0x0001)		// �p�b�h�P����
	#define DX_INPUT_PAD2								(0x0002)		// �p�b�h�Q����
	#define DX_INPUT_PAD3								(0x0003)		// �p�b�h�R����
	#define DX_INPUT_PAD4								(0x0004)		// �p�b�h�S����
	#define DX_INPUT_PAD5								(0x0005)		// �p�b�h�T����
	#define DX_INPUT_PAD6								(0x0006)		// �p�b�h�U����
	#define DX_INPUT_PAD7								(0x0007)		// �p�b�h�V����
	#define DX_INPUT_PAD8								(0x0008)		// �p�b�h�W����
	#define DX_INPUT_KEY								(0x1000)		// �L�[����

// �X�g���[���f�[�^����p�֐��|�C���^�\����
typedef struct
{
	long					(*Tell)( void *StreamDataPoint ) ;
	int						(*Seek)( void *StreamDataPoint, long SeekPoint, int SeekType ) ;
	size_t					(*Read)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
	size_t					(*Write)( void *Buffer, size_t BlockSize, size_t DataNum, void *StreamDataPoint ) ;
	int						(*Eof)( void *StreamDataPoint ) ;
	int						(*IdleCheck)( void *StreamDataPoint ) ;
	int						(*Close)( void *StreamDataPoint ) ;
} STREAMDATASHRED, *LPSTREAMDATASHRED ;

// �X�g���[���f�[�^����p�f�[�^�\����
typedef struct
{
	STREAMDATASHRED			ReadShred ;
	void					*DataPoint ;
} STREAMDATA ;
// �X�g���[���f�[�^����̃V�[�N�^�C�v��`
#define STREAM_SEEKTYPE_SET							(SEEK_SET)
#define STREAM_SEEKTYPE_END							(SEEK_END)
#define STREAM_SEEKTYPE_CUR							(SEEK_CUR)

#define STTELL( st )								((st)->ReadShred.Tell( (st)->DataPoint ))
#define STSEEK( st, pos, type )						((st)->ReadShred.Seek( (st)->DataPoint, (pos), (type) ))
#define STREAD( buf, length, num, st )				((st)->ReadShred.Read( (buf), (length), (num), (st)->DataPoint ))
#define STWRITE( buf, length, num, st )				((st)->ReadShred.Write( (buf), (length), (num), (st)->DataPoint ))
#define STEOF( st )									((st)->ReadShred.Eof( (st)->DataPoint ))
#define STCLOSE( st )								((st)->ReadShred.Close( (st)->DataPoint ))

int SetupSTREAMDATA(const char *FileName,STREAMDATA *DataPtr);	/*�t�@�C��������STREAMDATA�\���̂��Z�b�g�A�b�v�B*/



#ifdef	__cplusplus
};
#endif

#endif

/*
�X�V����

Ver0.0.0		���J�J�n
Ver0.0.1		�p���������o�O���C��
Ver0.0.2		���������o�O�C��
Ver0.1.0		Slice�̓����ADeleteGraph�֐������Y��Ă����̂Œǉ��ATransFlag���g����悤�ɂ����AConvertGraphFormat�֐���ǉ��A���̑��o�O�C��
Ver0.1.1		DrawRotaGraph�̌v�Z�ɒp���������~�X���������̂ŏC���B�S�Ẳ摜�`��֐���Slice�𗘗p����悤�ɕύX
Ver0.2.0		MP3�t�@�C����PlayMusic�ōĐ��ł���悤�ɂ��܂����BWAV�͍Đ��ł��Ȃ��̂Œ��ӂł�
Ver0.2.1		printfDx�n�֐���S�ʏ����������܂����BDrawString_Shinonome�֐���ǉ����܂����B
Ver0.3.0		GetTextOSK�֐���ǉ����܂����B
Ver0.3.1		�t�@�C���̎�舵���Ɋւ���v���I�ȃo�O���C��
Ver0.3.2		libjpeg��PSP������Jpeg�f�R�[�_�̂ǂ�����g������DXP�R���p�C�����ɑI���\�ɂ����B
Ver0.4.0		intraFont�����APSP�{�̂ɑg�ݍ��܂�Ă���t�H���g�𗘗p�\�ɂ����B���y�Đ��@�\�̍č\�z�������B
Ver0.4.1		DrawExtendGraph�ŕ\�����ʂ����]���鎞��DrawTurnGraph�g�p���ŕ`�挋�ʂ����������Ȃ�����C���B�i�O�L�ɊY������ꍇ�͓����I��DrawModiGraph���g�����ߎ኱�̑��x�ቺ���������܂��B
Ver0.4.2		GetAnalogInput���s���Ȓl��Ԃ������C���B
Ver0.4.3		LoadDivGraph�֐���DerivationGraph�֐��������B
Ver0.4.4		�����\���̈�V��SetDrawScreen�֐��̒ǉ��BSetDrawScreen�ɓn���O���t�B�b�N�n���h����VRAM��ɂ���K�v������܂��B
Ver0.4.5		���y�Đ��@�\�̃o�O�C���i�V���[�e�B���O��D�����ɂ��\�[�X�R�[�h���C�j�A�`�惋�[�`���̍������Ȃǂł��B
Ver0.4.6		���傤���Ȃ��~�X�^�C�v�ŕ`�挋�ʂ����������Ȃ��Ă����̂��C��
Ver0.4.7
Ver0.4.8		�o�O�C��
Ver0.4.9		�u�����h���[�h�̎d�l��{�Ƃɍ��킹���B
Ver0.4.10		���Z����������ɓ����Ȃ��o�O���C��
Ver0.4.11		�p���b�g�`����PNG�摜�̓p���b�g���g���ĕ`�悷��悤�ɏC��
Ver0.4.12		MP3�̍Đ������艻�A������`��̃o�O�C���A���̑����낢��
Ver0.4.14		�`��@�\���X�ɍ�����
*/

