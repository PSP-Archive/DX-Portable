#ifndef	DXPSTATIC_H__
#define	DXPSTATIC_H__
/*
*	DX���C�u����Portable	���C�u���������p�w�b�_
*	�����	�F�J��
*/

#include <pspkernel.h>

/*DXP�̃R���p�C�����I�v�V����*/

#define DXP_NOUSE_LIBJPEG		/*�R�����g�A�E�g�����512x512�ȏ�̃T�C�Y��Jpeg�t�@�C����ǂݍ��߂�悤�ɂȂ�܂��B�������A���s�t�@�C���̃T�C�Y��80KB�قǑ傫���Ȃ�܂��B*/
#define DXP_NOUSE_MTRAND		/*�R�����g�A�E�g����Ɨ��������Ƀ����Z���k�E�c�C�X�^�[���g���܂��B�f�t�H���g�ł�LFSR�������ƂȂ�܂��B*/
/*#define DXP_NON_ZENKAKU*/		/*�S�p�����̃f�o�b�O�X�N���[�����g��Ȃ��ꍇ�̓R�����g�A�E�g���O���Ă��������B*/
//#define DXP_NOUSE_FLTVERTEX_WITH_ROTA	/*�R�����g�A�E�g�����DrawRotaGraph�ADrawRotaGraph2�̕`��ɕs�������_���^���_���g���悤�ɂȂ�܂��B*/
/*DXP�̃R���p�C�����I�v�V�����I���*/



/*�}�N��*/

//#define PTR2HND(PTR)	((int)(PTR) - 1)
//#define HND2PTR(HND)	((void*)((int)HND + 1))
#define PTR2HND(PTR)	((PTR) - 1)
#define HND2PTR(HND)	(HND + 1)

#define MIN(A,B)		((A) > (B) ? (B) : (A))
#define MAX(A,B)		((A) > (B) ? (A) : (B))

/*���C�u�����{�̂̐ݒ�Ƃ�*/
#define	DXPDATAFLAGS_0_INITIALIZED	0x00000001
#define	DXPDATAFLAGS_0_INITIALIZING	0x00000002

typedef	struct
{
	u32			TPS;		/*�e�B�b�N�@�p�[�@�Z�R���h�@RTC�i���A���^�C���N���b�N�j����ŕb�P�ʂɂȂ�̂����L�^�@PSP-2000�ł�1000000������*/
	u32			flags[1];
}DXPDATA;

/*VRAM��̃A�h���X���Ǘ����邽�߂̍\����*/
#define	VRAMCTRLFLAGS_EXIST		0x01
#define	VRAMCTRLFLAGS_STATIC	0x02
typedef struct tagDXPVRAMCTRL
{
	union
	{
		u32		offset;
		void*	address;
	};
	u32			size;
	u8			flags;
}DXPVRAMCTRL;

/*�p���b�g�f�[�^*/
typedef struct tagDXPPALETTE	/*PSM��GU_PSM_8888�ŌŒ�ł��B*/
{
	u32			data[256];
}DXPPALETTE;

/*�e�N�X�`���Ǘ��\����*/
//#define TEXTURE_MAX	(1024)		/*�����ɗ��p�\�ȃO���t�B�b�N�n���h���̍ő吔*/
//#define	TEXTUREFLAGS_VRAM		0x01
//#define	TEXTUREFLAGS_SWIZZLED	0x02
//#define	TEXTUREFLAGS_EXIST		0x04
//#define TEXTUREFLAGS_RELOAD		0x08	/*�e�N�X�`���̋��������[�h���K�v�ȏꍇ�ɐݒ肳���B��x���[�h���ꂽ��t���O�͓|�����B*/
//typedef struct tagDXPTEXTURE
//{
//	DXPPALETTE	*ppalette;				/*�p���b�g�f�[�^�ւ̃|�C���^*/
//	DXPVRAMCTRL *pvram;					/*VRAM�f�[�^�ւ̃|�C���^*/
//	void		*pmemory;				/*RAM��̃|�C���^*/
//	int			psm;					/*�e�N�X�`���t�H�[�}�b�g*/
//	u16			width;					/*�e�N�X�`���̉��T�C�Y*/
//	u16			height;					/*�e�N�X�`���̏c�T�C�Y*/
//	u16			u0,v0;					/*�e�N�X�`���̎��ۂɎg����̈�̃T�C�Y*/
//	u16			mu,mv;					/*�e�N�X�`���̎��ۂɎg����̈�̃T�C�Y*/
//	u16			pitch;					/*�e�N�X�`���̕��Bbyte�P�ʂ��Ǝv��*/
//	u32			colorkey;				/*�J���[�L�[*/
//	unsigned	vramflag:1;				/*VRAM��ɑ��݂��Ă��邩�ǂ����̃t���O�B�r�b�g�t�B�[���h�ł��B*/
//	unsigned	swizzledflag:1;			/*Swizzle����Ă��邩�ǂ���*/
//	unsigned	existflag:1;			/*���݂��Ă��邩�ǂ���*/
//	unsigned	reloadflag:1;			/*���������[�h���邩�ǂ���*/
//	struct tagDXPTEXTURE	*next,*prev;
//	int			handle;
//	int			refhandle;
//	int			refcount;
//}DXPTEXTURE;

typedef struct tagDXPTEXTURE2
{
	int			psm;
	struct tagDXPTEXTURE2 *next,*prev;
	DXPPALETTE	*ppalette;
	DXPVRAMCTRL	*pvram;
	void*		pmemory;
	u16			width;					/*�e�N�X�`���̉��T�C�Y*/
	u16			height;					/*�e�N�X�`���̏c�T�C�Y*/
	u16			pitch;					/*�e�N�X�`���̉��T�C�Y*/
	u16			umax,vmax;				/*�L����UV���W�̍ő�l*/
	u32			colorkey;				/*�J���[�L�[*/
	unsigned	vramflag:1;				/*VRAM��ɑ��݂��Ă��邩�ǂ����̃t���O�B�r�b�g�t�B�[���h�ł��B*/
	unsigned	swizzledflag:1;
	unsigned	reloadflag:1;
	unsigned	alphabit:1;				/*�ǂݍ��񂾉摜�Ƀ���񂪑��݂������ǂ���*/
	int			refcount;
}DXPTEXTURE2;

typedef	struct	tagDXPGRAPHDATA
{
	struct tagDXPGRAPHDATA	*next,*prev;
	DXPTEXTURE2	*tex;
	u16			u0,v0,u1,v1;
	int			handle;
}DXPGRAPHDATA;
extern DXPGRAPHDATA	*graphlist;

/*�`��ݒ�\����*/
#define	GPUSETTINGFLAGS_0_GUSTART		0x00000001	/*GuStart�`GuFinish�̊Ԃł���*/
#define	GPUSETTINGFLAGS_0_DEPTHENABLE	0x00000002	/*�[�x�o�b�t�@���L���ɂȂ��Ă���*/
#define GPUSETTINGFLAGS_0_STENCILENABLE	0x00000004	/*�X�e���V���o�b�t�@���L���ɂȂ��Ă���*/
#define	GPUSETTINGFLAGS_0_CLEARDEPTH	0x00000008	/*ClearDrawScreen�Ő[�x�o�b�t�@������������*/
#define GPUSETTINGFLAGS_0_CLEARSTENCIL	0x00000010	/*ClearDrawScreen�ŃX�e���V���o�b�t�@������������*/
#define GPUSETTINGFLAGS_0_BILINEAR		0x00000020	/*�o�C���j�A���[�h�Ńe�N�X�`�����g���B�����Ă��Ȃ���΍ŋߓ_���[�h*/
#define	GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH	0x00000040	/*Swizzle���Ă���O���t�B�b�N�����B*/
#define	GPUSETTINGFLAGS_0_CREATEVRAMGRAPH	0x00000080	/*VRAM��ɃO���t�B�b�N����邩*//*�f�t�H���g��ON*/
typedef struct tagDXPGPUSETTING
{
	DXPTEXTURE2	displaybuffer[2];
	DXPTEXTURE2	depthbuffer;
	DXPTEXTURE2	*rendertarget;
//	DXPVRAMCTRL	*frontbuffer[2];/*��ʃo�b�t�@*/
//	DXPVRAMCTRL	*depthbuffer;/*�[�x�o�b�t�@*/
	//int		rendertarget;/*�`���̃e�N�X�`���̃n���h���@�o�b�N�o�b�t�@�̏ꍇ��DXP_SCREEN_BACK*/
	DXPTEXTURE2	*texture;/*�Z�b�g����Ă���e�N�X�`���̃n���h���@�Z�b�g����Ă��Ȃ����-1*/
//	int			displaypsm;/*�f�B�X�v���[�̐F�ݒ�*/
//	int			depthpsm;/*�[�x�o�b�t�@�̐F�i�H�j�ݒ�*/
	u32			clearcolor;/*ClearDrawScreen���Ɏg���w�i�F*/
	u32			cleardepth;/*ClearDrawScreen���Ɏg���[�x*/
	u32			clearstencil;/*ClearDrawScreen���Ɏg���X�e���V���l*/
	u32			flags[1];	/*�e��t���O�f�[�^�B�ł��邾���������͐H�������Ȃ��̂�bit�Ǘ�*/
	u32			colorkey;	/*�J���[�L�[�̃f�t�H���g�l*/
//	int			texcolor;	/*�e�N�X�`���̒��_�F�Ƃ��Ďw�肷��B�ꕔ�̃u�����h���[�h��p*/
	int			blendmode;	/*SetDrawBlendMode�Ŏw�肳���l*/
	int			blendparam;
	u8			red,green,blue,alpha;/*SetDrawBright��SetDrawBlendMode�Ŏw�肳�ꂽ�l���犄��o����鐔�l*/
	u16			z_2d;		/*2D�`�掞�ɗp����Z�l�B*/
	u8			slice;		/*���o�C�g��slice���邩*/
	u8			backbuffer;/*displaybuffer�̂ǂ��炪����ʂƂ���Ă���̂�*/
	int			scissor[4];	/*sceGuScissor�̐ݒ�l��ۑ��BDrawString�n�֐��̂��߂Ɏg�p����B*/
	struct BLENDANDCOLOR
	{
		u8	forceupdate;//�����ݒ�X�V
		u32	color;
		int op;
		int src;
		int dest;
		u32 srcfix;
		u32 destfix;
		int tfx,tcc;
	}bc;
}DXPGPUSETTING;
extern DXPGPUSETTING gusettings;

/*���_�\����*/
#define	DXP_VTYPE_2D	GU_VERTEX_16BIT
typedef struct
{
	s16			x,y,z;
}DXPVERTEX_2D;

#define	DXP_VTYPE_2DTEX	(GU_VERTEX_16BIT | GU_TEXTURE_16BIT)
typedef struct
{
	u16			u,v;
	s16			x,y,z;
}DXPVERTEX_2DTEX;

#define	DXP_VTYPE_2DTEX_F	(GU_VERTEX_32BITF | GU_TEXTURE_16BIT)
typedef	struct
{
	u16			u,v;
	float		x,y,z;
}DXPVERTEX_2DTEX_F;

#define DXP_VTYPE_3DTEX_F	(GU_VERTEX_32BITF | GU_TEXTURE_16BIT | GU_COLOR_8888)
typedef	struct
{
	u16			u,v;
	u32			color;
	float		x,y,z;
}DXPVERTEX_3DTEX_F;


/*VRAM�̈�̎擾�A�J��*/
void InitVRAM();
DXPVRAMCTRL* AllocVRAM(int Size,int StaticFlag);
void FreeVRAM(DXPVRAMCTRL *VRamCtrl);
extern u32 gulist[];
/*GPU�n*/
#define GUSTART												\
{															\
	if(!(gusettings.flags[0] & GPUSETTINGFLAGS_0_GUSTART))	\
	{														\
		sceGuStart(GU_DIRECT,gulist);						\
		gusettings.flags[0] |= GPUSETTINGFLAGS_0_GUSTART;	\
	}														\
}
#define GUFINISH											\
{															\
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_GUSTART)		\
	{														\
		sceGuFinish();										\
		gusettings.flags[0] &= (~GPUSETTINGFLAGS_0_GUSTART);\
		sceGuSync(0,0);										\
	}														\
}

int InitGUEngine();
int EndGUEngine();
int SetTexture(int GrHandle,int TransFlag);
/*�f�o�b�O�X�N���[���p*/
void InitDebugScreen();
void DrawDebugScreen();

int LoadModule_AV_MP3();/*MP3�̃R�[�f�b�N�����[�h����B*/
int ProcessAudio();
DXPGRAPHDATA* GraphHandle2Ptr(int handle);
DXPTEXTURE2* GraphHandle2TexPtr(int handle);
void TextureList_PushFront(DXPTEXTURE2 *ptr);
void TextureList_Remove(DXPTEXTURE2 *ptr);
void GraphDataList_PushFront(DXPGRAPHDATA *ptr);
DXPTEXTURE2* MakeTexture(int x,int y,int format);
int GenerateGraphHandle();//�n���h���̔ԍ��𐶐�����B
int PSM2BYTEx2(int psm);
#define MALLOC		malloc
#define	MEMALIGN	memalign
#define	FREE		free


#define LINETRACE 	ClearDrawScreen();printfDx("%s::%d \n",__FILE__,__LINE__);ScreenFlip();

#endif
