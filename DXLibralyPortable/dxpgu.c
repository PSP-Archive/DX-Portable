/*
*	DX���C�u����Portable	GPU���䕔	Ver1.20
*	�����	�F�J��
*	���l	�F�O���t�B�b�N�̓ǂݍ��݊֐��͕ʃt�@�C���ɂ���\��
*	300KByte����L���Ă܂��B�O���t�B�b�N���ǂ܂��ƈ�C�Ƀ�����������̂Œ���
*/

/*
����
pspDebugScreen�n�֐��͎g���Ƌ���������̂Ŏg�p�֎~�BVRAM�̐擪����o�C�g���g���Ȃ��Ȃ�܂��B
*/
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include "dxlibp.h"
#include "dxpstatic.h"
#include <malloc.h>
#include <string.h>
#include "zenkaku.h"
#include <fastmath.h>
/*�萔��`*/
#define GULIST_LEN	262144	/*(256 * 1024) 1MByte*/
#define GULIST_SIZE (GULIST_LEN << 2)
/*�}�N����`*/

/*���ϐ���`��*/
DXPGPUSETTING gusettings = 
{
	{{GU_PSM_8888,NULL,NULL,NULL,NULL,NULL,480,272,512,480,272,0x00000000,1,0,1,0,0},{GU_PSM_8888,NULL,NULL,NULL,NULL,NULL,480,272,512,480,272,0x00000000,1,0,1,0,0}},								/*�t�����g�o�b�t�@*/
	{GU_PSM_4444},									/*�[�x�o�b�t�@�̃|�C���^*/
	NULL,									/*�`���O���t�B�b�N*/
	NULL,										/*�Z�b�g����Ă���e�N�X�`��*/
//	GU_PSM_8888,							/*�f�B�X�v���[�o�b�t�@�̐F�ݒ�*/
//	GU_PSM_4444,							/*�[�x�o�b�t�@�̐F�ݒ�*/
	0,										/*�N���A�J���[*/
	0,										/*�N���A�[�x*/
	0,										/*�N���A�X�e���V��*/
	{GPUSETTINGFLAGS_0_CREATEVRAMGRAPH | GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH},/*�e��t���O*/
	0x00000000,								/*�J���[�L�[*/
	//0,										/*�e�N�X�`���̒��_�F�Ɏg���l*/
	DX_BLENDMODE_NOBLEND,					/*�u�����h���[�h*/
	255,									/*�u�����h�p�����[�^*/
	255,255,255,255,						/*RGBA*/
	0,										/*2D�`�掞�Ɏg��Z�l*/
	64,										/*���o�C�g��slice���邩*/
	0,										/*frontbuffer�̂ǂ��炪�\������Ă���̂�*/
	{0,0,480,272},								/*�V�U�[�̈�̐ݒ�l*/
	{1,-1,0,0,0,0,0,-1,0}						/*GPU�ݒ�l�̕ێ�*/
};
u32 __attribute__((aligned(16))) gulist[GULIST_LEN];/*GPU�ɑ��閽�߂𗭂ߍ��ނ��߂̃o�b�t�@�@�Ƃ肠����256KByte*/
//DXPTEXTURE texarray[TEXTURE_MAX];
DXPTEXTURE2		*texlist	= NULL;
DXPGRAPHDATA	*graphlist	= NULL;


/*�֐���`��*/
//static int ApplyBrightAndBlendMode();


void GuListSafety()
{
	if(sceGuCheckList() > GULIST_SIZE * 0.8f)
	{
		GUFINISH
		GUSTART
	}
}

void TextureList_PushFront(DXPTEXTURE2 *ptr)
{
	ptr->next = texlist;
	ptr->prev = NULL;
	if(texlist != NULL)texlist->prev = ptr;
	texlist = ptr;
}
void TextureList_Remove(DXPTEXTURE2 *ptr)
{
	if(ptr->next != NULL)ptr->next->prev = ptr->prev;
	if(ptr->prev != NULL)ptr->prev->next = ptr->next;
	else texlist = ptr->next;
	ptr->next = ptr->prev = NULL;
}
void GraphDataList_PushFront(DXPGRAPHDATA *ptr)
{
	ptr->next = graphlist;
	ptr->prev = NULL;
	if(graphlist != NULL)graphlist->prev = ptr;
	graphlist = ptr;
}
void GraphDataList_Remove(DXPGRAPHDATA *ptr)
{
	if(ptr->next != NULL)ptr->next->prev = ptr->prev;
	if(ptr->prev != NULL)ptr->prev->next = ptr->next;
	else graphlist = ptr->next;
	ptr->next = ptr->prev = NULL;
}

int GenerateGraphHandle()//�n���h���̔ԍ��𐶐�����B
{
	DXPGRAPHDATA *ptr = graphlist;
	int handle = 0;
	while(ptr != NULL)
	{
		if(handle <= ptr->handle)handle = ptr->handle + 1;
		ptr = ptr->next;
	}
	return handle;
}

DXPGRAPHDATA* GraphHandle2Ptr(int handle)
{
	DXPGRAPHDATA *ptr;
	for(ptr = graphlist;ptr != NULL;ptr = ptr->next)if(ptr->handle == handle)return ptr;
	return NULL;
}
DXPTEXTURE2* GraphHandle2TexPtr(int handle)
{
	DXPGRAPHDATA *ptr;
	if(handle == DXP_SCREEN_BACK)return &gusettings.displaybuffer[gusettings.backbuffer];
	for(ptr = graphlist;ptr != NULL;ptr = ptr->next)if(ptr->handle == handle)return ptr->tex;
	return NULL;
}

DXPTEXTURE2* MakeTexture(int x,int y,int format)
{
	int size;
	int height,width,pitch;
	DXPTEXTURE2 *ptr;
	if((ptr = (DXPTEXTURE2*)MALLOC(sizeof(DXPTEXTURE2))) == NULL)return NULL;
	//�e�N�X�`���̃T�C�Y�v�Z
	x = MIN(x,512);
	y = MIN(y,512);
	if(x <= 0 || y <= 0)return NULL;
	height = width = 1;
	while(height < y)height <<= 1;
//	height = (y + 15) & 0xfffffff0;
	while(width < x)width <<= 1;
	pitch = width;
	switch(format)
	{
	case GU_PSM_T4:
		if(pitch < 32)pitch = 32;
		ptr->ppalette = MEMALIGN(16,sizeof(DXPPALETTE));
		break;
	case GU_PSM_T8:
		if(pitch < 16)pitch = 16;
		ptr->ppalette = MEMALIGN(16,sizeof(DXPPALETTE));
		break;
	case GU_PSM_5650:
	case GU_PSM_5551:
	case GU_PSM_4444:
		if(pitch < 8)pitch = 8;
		ptr->ppalette = NULL;
		break;
	case GU_PSM_8888:
		if(pitch < 4)pitch = 4;
		ptr->ppalette = NULL;
		break;
	default:
		FREE(ptr);
		return NULL;
	}
	if((size = GraphSize2DataSize(pitch,y,format)) == -1)
	{
		FREE(ptr->ppalette);
		FREE(ptr);
		return NULL;
	}

	//�e�N�X�`���̃������m��
	if((ptr->pmemory = MEMALIGN(16,size)) == NULL)
	{
		FREE(ptr->ppalette);
		FREE(ptr);
		return NULL;
	}
	memset(ptr->pmemory,0x00,size);
	ptr->pvram		= NULL;
	ptr->height		= height;
	ptr->width		= width;
	ptr->pitch		= pitch;
	ptr->umax		= x;
	ptr->vmax		= y;
	ptr->psm		= format;
	ptr->vramflag	= 0;
	ptr->swizzledflag= 0;
	ptr->reloadflag	= 1;
	ptr->alphabit	= 0;
	ptr->colorkey	= gusettings.colorkey;
	ptr->refcount	= 0;
	TextureList_PushFront(ptr);
	return ptr;
	
}

int MakeGraph(int x,int y,int format)
{
	int size;
	int i = GenerateGraphHandle();
	int height,width,pitch;
	DXPGRAPHDATA *ptr;
	if((ptr = (DXPGRAPHDATA*)MALLOC(sizeof(DXPGRAPHDATA))) == NULL)return -1;
	if((ptr->tex = (DXPTEXTURE2*)MALLOC(sizeof(DXPTEXTURE2))) == NULL)
	{
		FREE(ptr);
		return -1;
	}
	//�e�N�X�`���̃T�C�Y�v�Z
	x = MIN(x,512);
	y = MIN(y,512);
	if(x <= 0 || y <= 0)return -1;
	height = width = 1;
	while(height < y)height <<= 1;
//	height = (y + 15) & 0xfffffff0;
	while(width < x)width <<= 1;
	pitch = width;
	switch(format)
	{
	case GU_PSM_T4:
		if(pitch < 32)pitch = 32;
		ptr->tex->ppalette = MEMALIGN(16,sizeof(DXPPALETTE));
		break;
	case GU_PSM_T8:
		if(pitch < 16)pitch = 16;
		ptr->tex->ppalette = MEMALIGN(16,sizeof(DXPPALETTE));
		break;
	case GU_PSM_5650:
	case GU_PSM_5551:
	case GU_PSM_4444:
		if(pitch < 8)pitch = 8;
		ptr->tex->ppalette = NULL;
		break;
	case GU_PSM_8888:
		if(pitch < 4)pitch = 4;
		ptr->tex->ppalette = NULL;
		break;
	default:
		FREE(ptr->tex);
		FREE(ptr);
		return -1;
	}
	if((size = GraphSize2DataSize(pitch,y,format)) == -1)
	{
		FREE(ptr->tex->ppalette);
		FREE(ptr->tex);
		FREE(ptr);
		return -1;
	}

	//�e�N�X�`���̃������m��
	if((ptr->tex->pmemory = MEMALIGN(16,size)) == NULL)
	{
		FREE(ptr->tex->ppalette);
		FREE(ptr->tex);
		FREE(ptr);
		return -1;
	}
	memset(ptr->tex->pmemory,0x00,size);
	ptr->tex->pvram		= NULL;
	ptr->tex->height		= height;
	ptr->tex->width		= width;
	ptr->tex->pitch		= pitch;
	ptr->tex->umax		= x;
	ptr->tex->vmax		= y;
	ptr->tex->psm		= format;
	ptr->u0			= 0;
	ptr->v0			= 0;
	ptr->u1			= x;
	ptr->v1			= y;
	ptr->tex->vramflag	= 0;
	ptr->tex->swizzledflag= 0;
	ptr->tex->reloadflag	= 1;
	ptr->tex->alphabit	= 0;
	ptr->tex->colorkey	= gusettings.colorkey;
	ptr->tex->refcount	= 1;
	ptr->handle		= i;
	TextureList_PushFront(ptr->tex);
	GraphDataList_PushFront(ptr);
	return i;
}

int GetColor(int Red,int Green,int Blue)/*���݂̃J���[�t�H�[�}�b�g�ŐF��Ԃ�*/
{
	int psm = gusettings.rendertarget != NULL ? gusettings.rendertarget->psm : GU_PSM_8888;
	switch(psm)
	{
		case GU_PSM_4444:
			return 0x0000f000 | (Blue & 0x000000f0) << 4 | (Green & 0x000000f0) | (Red & 0x000000f0) >> 4;
		case GU_PSM_5551:
			return 0x00008000 | (Blue & 0x000000f8) << 7 | (Green & 0x000000f8) << 2| (Red & 0x000000f8) >> 3;
		case GU_PSM_5650:
			return 0x00008000 | (Blue & 0x000000f8) << 8 | (Green & 0x000000f8) << 3| (Red & 0x000000f8) >> 3;
		case GU_PSM_8888:
			return 0xff000000 | (Blue & 0x000000ff) << 16 | (Green & 0x000000ff) << 8 | (Red & 0x000000ff);
		default:
			return 0;
	}
}

int GraphSize2DataSize(int width,int height,int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;
		case GU_PSM_T8:
			return width * height;
		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;
		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;
		default:
			return -1;
	}
}

int SetCreateSwizzledGraphFlag(int Flag)
{
	if(Flag != 0)
		gusettings.flags[0] |= GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH;
	else
		gusettings.flags[0] &= (~GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH);
	return 0;
}

int InitGUEngine()
{
//	AppLogAdd2("GPU�֘A�̏��������J�n���܂��B");
AppLogAdd2("");
	gusettings.rendertarget		= NULL;
	gusettings.texture			= NULL;
	//gusettings.displaypsm		= GU_PSM_8888;
	//gusettings.depthpsm			= GU_PSM_4444;
	gusettings.clearcolor		= GetColor(0,0,0);
	gusettings.cleardepth		= 0;
	gusettings.clearstencil		= 0;
	gusettings.flags[0]			= GPUSETTINGFLAGS_0_CREATEVRAMGRAPH | GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH;
//gusettings.flags[0]			= 0x00000000;
	gusettings.colorkey			= 0x00000000;
//	gusettings.texcolor			= 0;
	gusettings.blendmode		= DX_BLENDMODE_NOBLEND;
	gusettings.blendparam		= 255;
	gusettings.red				= 255;
	gusettings.green			= 255;
	gusettings.blue				= 255;
	gusettings.alpha			= 255;
	gusettings.z_2d				= 0x0000;
	gusettings.backbuffer		= 0;
	gusettings.bc.forceupdate	= 1;
	texlist = NULL;
	AppLogAdd2("�e�N�X�`���Ǘ��f�[�^�����������܂����B");

	InitVRAM();	/*VRAM�̈�̏�����*/
AppLogAdd2("");
	/*�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̎擾*/
	gusettings.displaybuffer[0].colorkey	= 0;
	gusettings.displaybuffer[0].height		= 272;
	gusettings.displaybuffer[0].next		= NULL;
	gusettings.displaybuffer[0].pitch		= 512;
	gusettings.displaybuffer[0].pmemory		= NULL;
	gusettings.displaybuffer[0].ppalette	= NULL;
	gusettings.displaybuffer[0].prev		= NULL;
//	gusettings.displaybuffer[0].psm			= GU_PSM_8888;
	gusettings.displaybuffer[0].pvram		= AllocVRAM(GraphSize2DataSize(512,272,gusettings.displaybuffer[0].psm),1);
	gusettings.displaybuffer[0].refcount	= 0xffffffff;
	gusettings.displaybuffer[0].reloadflag	= 1;
	gusettings.displaybuffer[0].swizzledflag= 1;
	gusettings.displaybuffer[0].umax		= 480;
	gusettings.displaybuffer[0].vmax		= 272;
	gusettings.displaybuffer[0].vramflag	= 1;
	gusettings.displaybuffer[0].width		= 480;
AppLogAdd2("");

	gusettings.displaybuffer[1].colorkey	= 0;
	gusettings.displaybuffer[1].height		= 272;
	gusettings.displaybuffer[1].next		= NULL;
	gusettings.displaybuffer[1].pitch		= 512;
	gusettings.displaybuffer[1].pmemory		= NULL;
	gusettings.displaybuffer[1].ppalette	= NULL;
	gusettings.displaybuffer[1].prev		= NULL;
//	gusettings.displaybuffer[1].psm			= GU_PSM_8888;
	gusettings.displaybuffer[1].pvram		= AllocVRAM(GraphSize2DataSize(512,272,gusettings.displaybuffer[1].psm),1);
	gusettings.displaybuffer[1].refcount	= 0xffffffff;
	gusettings.displaybuffer[1].reloadflag	= 1;
	gusettings.displaybuffer[1].swizzledflag= 1;
	gusettings.displaybuffer[1].umax		= 480;
	gusettings.displaybuffer[1].vmax		= 272;
	gusettings.displaybuffer[1].vramflag	= 1;
	gusettings.displaybuffer[1].width		= 480;
AppLogAdd2("");

	if(gusettings.displaybuffer[0].pvram == NULL)return -1;
	if(gusettings.displaybuffer[1].pvram == NULL)return -1;

AppLogAdd2("");

//	gusettings.frontbuffer[0] = AllocVRAM(GraphSize2DataSize(512,272,gusettings.displaypsm),1);
//	gusettings.frontbuffer[1] = AllocVRAM(GraphSize2DataSize(512,272,gusettings.displaypsm),1);
//	memset(sceGeEdramGetAddr() + gusettings.frontbuffer[1]->offset,0x88,557056);
//	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_DEPTHENABLE)
	{//DepthBuffer�̊m��
		memset(&gusettings.depthbuffer,0,sizeof(DXPTEXTURE2));
		gusettings.depthbuffer.height		= 272;
		gusettings.depthbuffer.pitch		= 512;
//		gusettings.depthbuffer.psm			= GU_PSM_4444;
		gusettings.depthbuffer.pvram		= gusettings.flags[0] & GPUSETTINGFLAGS_0_DEPTHENABLE ? AllocVRAM(GraphSize2DataSize(512,272,gusettings.depthbuffer.psm),1) : NULL;
		gusettings.depthbuffer.refcount		= 0xffffffff;
		gusettings.depthbuffer.umax			= 480;
		gusettings.depthbuffer.vmax			= 272;
		gusettings.depthbuffer.vramflag		= 1;
		gusettings.depthbuffer.width		= 480;
		if(gusettings.depthbuffer.pvram == NULL)gusettings.flags[0] &=~ GPUSETTINGFLAGS_0_DEPTHENABLE;
	}
	gusettings.rendertarget	= gusettings.displaybuffer;

	/*GPU�̏������J�n*/
	sceGuInit();
	GUSTART
	sceGuDrawBuffer(gusettings.displaybuffer[0].psm,gusettings.displaybuffer[0].pvram->address,512);
	sceGuDispBuffer(480,272,gusettings.displaybuffer[1].pvram->address,512);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_DEPTHENABLE)
	{
		sceGuDepthBuffer(gusettings.depthbuffer.pvram->address,512);
		sceGuEnable(GU_DEPTH_TEST);
	}else sceGuDisable(GU_DEPTH_TEST);

	sceGuOffset(2048 - (480/2),2048 - (272/2));
	sceGuViewport(2048,2048,480,272);
	sceGuScissor(
		gusettings.scissor[0],
		gusettings.scissor[1],
		gusettings.scissor[2],
		gusettings.scissor[3]
	);

	sceGuDepthRange(65535,0);
//	sceGuScissor(0,0,480,272);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFrontFace(GU_CW);
	sceGuEnable(GU_TEXTURE_2D);
	//sceGuEnable(GU_FRAGMENT_2X);
	sceGuDepthMask(0xffff);
	sceGuShadeModel(GU_SMOOTH);
	sceGuTexScale(1.0f,1.0f);
	sceGuTexOffset(0.0f,0.0f);
/*	sceGuShadeModel(GU_SMOOTH);*/
//	ApplyBrightAndBlendMode();
	SetDrawMode(DX_DRAWMODE_NEAREST);
	GUFINISH
	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	ScreenFlip();
	return 0;
}

int EndGUEngine()
{
	GUFINISH;
	sceGuTerm();
	DXPTEXTURE2 *tptr;
	while(texlist != NULL)
	{
		FREE(texlist->pmemory);
		FREE(texlist->ppalette);
		FreeVRAM(texlist->pvram);
		tptr = texlist;
		texlist = texlist->next;
		FREE(tptr);
	}
	texlist = NULL;
	DXPGRAPHDATA *gptr;
	while(graphlist != NULL)
	{
		gptr = graphlist;
		graphlist = graphlist->next;
		FREE(gptr);
	}
	graphlist = NULL;
	FreeVRAM(gusettings.displaybuffer[0].pvram);
	FreeVRAM(gusettings.displaybuffer[1].pvram);
	InitVRAM();
	return 0;
}

int ScreenFlip()
{
	void *p;
	GUFINISH
	sceDisplayWaitVblankStart();//����������҂�
	//�X���b�v����Ƃ��Ƀf�o�b�O�p�̃X�N���[���̃I�t�Z�b�g���w�肷��B
	DrawDebugScreen();
	p = sceGuSwapBuffers();
	mh_displayoffset((u32)(p) & 0x00ffffff);
	gusettings.backbuffer ^= 1;
	if(gusettings.rendertarget == &gusettings.displaybuffer[0] || gusettings.rendertarget == &gusettings.displaybuffer[1])
		gusettings.rendertarget = &gusettings.displaybuffer[gusettings.backbuffer];
	return 0;
}

int ScreenCopy()
{
	if(ScreenFlip() != 0)return -1;
	void *src = sceGeEdramGetAddr(),*dst;
	GUFINISH
	dst = src;
	src += gusettings.displaybuffer[gusettings.backbuffer^1].pvram->offset;
	dst += gusettings.displaybuffer[gusettings.backbuffer].pvram->offset;
	memcpy(dst,src,GraphSize2DataSize(512,272,gusettings.displaybuffer[0].psm));
	sceKernelDcacheWritebackAll();
	return 0;
}

int ClearDrawScreen()
{
	GUSTART;
	sceGuClearColor(gusettings.clearcolor);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_DEPTHENABLE && gusettings.flags[0] & GPUSETTINGFLAGS_0_CLEARDEPTH)
		sceGuClearDepth(gusettings.cleardepth);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_STENCILENABLE && gusettings.flags[0] & GPUSETTINGFLAGS_0_CLEARSTENCIL)
		sceGuClearStencil(gusettings.clearstencil);
	sceGuClear(
		GU_COLOR_BUFFER_BIT |
		((gusettings.flags[0] & GPUSETTINGFLAGS_0_DEPTHENABLE && gusettings.flags[0] & GPUSETTINGFLAGS_0_CLEARDEPTH) ? GU_DEPTH_BUFFER_BIT : 0) |
		((gusettings.flags[0] & GPUSETTINGFLAGS_0_STENCILENABLE && gusettings.flags[0] & GPUSETTINGFLAGS_0_CLEARSTENCIL) ? GU_STENCIL_BUFFER_BIT : 0)
		);
	return 0;
}

int SetTexture(int handle,int TransFlag)//�e�N�X�`�����g���`��֐��ŌĂԁB�e�N�X�`���g��Ȃ��`��֐��ł�SetBaseColor���g�����B
{
	GUSTART;
	if(handle == -1)
	{
		/*�e�N�X�`������������B*/
		gusettings.texture = NULL;
		sceGuDisable(GU_TEXTURE_2D);
		return 0;
	}
	DXPTEXTURE2 *texptr = GraphHandle2TexPtr(handle);
	if(texptr == NULL)
	{
		gusettings.texture = NULL;
		return -1;
	}
	sceGuEnable(GU_TEXTURE_2D);
	if(gusettings.texture != texptr || texptr->reloadflag )/*�����e�N�X�`�����Z�b�g����Ă���ꍇ�͂Ȃɂ����Ȃ��B�p�t�H�[�}���X�������Ⴄ���炗*/
	{
		texptr->reloadflag = 0;
		/*�e�N�X�`���̃t�H�[�}�b�g��R�����A�K�v�Ȃ�p���b�g��ݒ肷��*/
		switch(texptr->psm)
		{
		case GU_PSM_4444:/*�ʏ�̃e�N�X�`��*/
		case GU_PSM_5551:
		case GU_PSM_5650:
		case GU_PSM_8888:
		case GU_PSM_DXT1:/*���k���ꂽ�e�N�X�`��*/
		case GU_PSM_DXT3:
		case GU_PSM_DXT5:
			break;
		case GU_PSM_T4:/*���Ԃ�p���b�g���Ǝv���񂾂�ˁc�悭�킩��Ȃ��P�h*/
		case GU_PSM_T8:
	//	case GU_PSM_T16:
	//	case GU_PSM_T32:
			if(texptr->ppalette == NULL)return -1;
			sceGuClutMode(GU_PSM_8888,0,0xff,0);
			sceGuClutLoad(256 / 8,texptr->ppalette->data);
			break;
		default:
			return -1;
		}

		sceGuTexMode(texptr->psm,0,0,texptr->swizzledflag ? GU_TRUE : GU_FALSE);
		/*�e�N�X�`�����Z�b�g*/
		if(texptr->vramflag)
		{
			sceGuTexImage(0,texptr->width,texptr->height,texptr->pitch,sceGeEdramGetAddr() + texptr->pvram->offset);
		}
		else
		{
			sceGuTexImage(0,texptr->width,texptr->height,texptr->pitch,texptr->pmemory);
		}
		gusettings.texture = texptr;
	}

//��������F�ݒ��u�����h���[�h�ݒ蓙���s���B
//�Ȃɂ�ݒ肷�邩����
	u8	ColorKey	= 0;
	u8	AlphaChannel= 0;
	if(TransFlag)
	{
		if(texptr->alphabit)
		{
			AlphaChannel = 1;
		}
		else
		{
			ColorKey = 1;
		}
	}

//�J���[�L�[��ݒ肷��
	if(ColorKey)
	{
		sceGuEnable(GU_COLOR_TEST);
		sceGuColorFunc(GU_NOTEQUAL,texptr->colorkey,0x00fefefe);
	}
	else
	{
		sceGuDisable(GU_COLOR_TEST);
	}
//�u�����f�B���O�̕��@��ݒ肷��
	int op;
	int src,dest;
	unsigned int srcfix;
	unsigned int destfix;	 
	switch(gusettings.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
		op = GU_ADD;
		src = GU_FIX;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0;
		if(!AlphaChannel)break;
	case DX_BLENDMODE_ALPHA:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_ONE_MINUS_SRC_ALPHA;
		srcfix = 0;
		destfix = 0;
		break;
	case DX_BLENDMODE_ADD:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_SUB:
		op = GU_REVERSE_SUBTRACT;
		src = GU_SRC_ALPHA;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_MUL:
		op = GU_ADD;
		src = GU_DST_COLOR;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_DESTCOLOR:
		op = GU_ADD;
		src = GU_FIX;
		dest = GU_FIX;
		srcfix = 0;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_INVDESTCOLOR:
		op = GU_ADD;
		src = GU_ONE_MINUS_DST_COLOR;
		dest = GU_FIX;
		srcfix = 0;
		destfix = 0;
		break;
	case DX_BLENDMODE_INVSRC:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_ONE_MINUS_SRC_ALPHA;
		srcfix = 0;
		destfix = 0;
		break;
	default:
		return -1;
	}

	if(gusettings.blendmode == DX_BLENDMODE_NOBLEND && !AlphaChannel)
	{
		if(sceGuGetStatus(GU_BLEND))sceGuDisable(GU_BLEND);
	}
	else
	{
		if(!sceGuGetStatus(GU_BLEND))sceGuEnable(GU_BLEND);
		if(gusettings.bc.forceupdate
			|| gusettings.bc.op != op
			|| gusettings.bc.src != src
			|| gusettings.bc.dest != dest
			|| gusettings.bc.srcfix != srcfix
			|| gusettings.bc.destfix != destfix
		){
			sceGuBlendFunc(op,src,dest,srcfix,destfix);
			gusettings.bc.op = op;
			gusettings.bc.src = src;
			gusettings.bc.dest = dest;
			gusettings.bc.srcfix = srcfix;
			gusettings.bc.destfix = destfix;
		}
	}
//�F��ݒ�
	unsigned int color;
	int tfx,tcc;
	color = (u32)(gusettings.blendmode == DX_BLENDMODE_NOBLEND ? 255 : gusettings.alpha) << 24 | (u32)(gusettings.blue) << 16 | (u32)(gusettings.green) << 8 | (u32)(gusettings.red);
	if(gusettings.bc.color != color || gusettings.bc.forceupdate)
	{
		sceGuColor(color);
		gusettings.bc.color = color;
	}

	switch(gusettings.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
	case DX_BLENDMODE_MUL:
	case DX_BLENDMODE_DESTCOLOR:
		tcc = GU_TCC_RGB;
		tfx = GU_TFX_MODULATE;
		if(AlphaChannel)tcc = GU_TCC_RGBA;
	break;
	case DX_BLENDMODE_ALPHA:
	case DX_BLENDMODE_ADD:
	case DX_BLENDMODE_SUB:
	case DX_BLENDMODE_INVDESTCOLOR:
		tcc = GU_TCC_RGBA;
		tfx = GU_TFX_MODULATE;
		break;
	case DX_BLENDMODE_INVSRC:
		sceGuTexEnvColor(0x00000000);
		tcc = GU_TCC_RGBA;
		tfx = GU_TFX_BLEND;
		break;
	default:
		return -1;
	}
	if(gusettings.bc.forceupdate || gusettings.bc.tfx != tfx || gusettings.bc.tcc)
	{
		sceGuTexFunc(tfx,tcc);
		gusettings.bc.tfx = tfx;
		gusettings.bc.tcc = tcc;
	}
	gusettings.bc.forceupdate = 0;
	return 0;
}

/*static*/ int SetBaseColor(u32 color)
/* DrawLine���̃e�N�X�`����g�p�֐��ŌĂԁB�F�̃Z�b�g*/
{
	GUSTART;
	gusettings.texture = NULL;
	sceGuDisable(GU_TEXTURE_2D);
	switch(gusettings.blendmode)
	{
	case DX_BLENDMODE_INVSRC:
		color = (color & 0xff000000) | ~(color & 0x00ffffff);
	case DX_BLENDMODE_NOBLEND:
	case DX_BLENDMODE_ALPHA:
	case DX_BLENDMODE_ADD:
	case DX_BLENDMODE_SUB:
	case DX_BLENDMODE_MUL:
	case DX_BLENDMODE_DESTCOLOR:
	case DX_BLENDMODE_INVDESTCOLOR:
	break;
	default:
		return -1;
	}
	u32 r,g,b,a;
	r = color & 0x000000ff;
	r *= gusettings.red;
	r /= 255;
	r &= 0x000000ff;
	g = (color & 0x0000ff00) >> 8;
	g *= gusettings.green;
	g /= 255;
	g &= 0x000000ff;
	b = (color & 0x00ff0000) >> 16;
	b *= gusettings.blue;
	b /= 255;
	b &= 0x000000ff;
	a = (color & 0xff000000) >> 24;
	a *= gusettings.alpha;
	a /= 255;
	a &= 0x000000ff;
	color = (a << 24) | (b << 16) | (g << 8) | r;
	if(gusettings.bc.color != color || gusettings.bc.forceupdate)
	{
		sceGuColor(color);
		gusettings.bc.color = color;
	}
//�u�����f�B���O�̕��@��ݒ肷��
	int op;
	int src,dest;
	unsigned int srcfix;
	unsigned int destfix;	 
	switch(gusettings.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
		break;
	case DX_BLENDMODE_ALPHA:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_ONE_MINUS_SRC_ALPHA;
		srcfix = 0;
		destfix = 0;
		break;
	case DX_BLENDMODE_ADD:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_SUB:
		op = GU_REVERSE_SUBTRACT;
		src = GU_SRC_ALPHA;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_MUL:
		op = GU_ADD;
		src = GU_DST_COLOR;
		dest = GU_FIX;
		srcfix = 0;
		destfix = 0;
		break;
	case DX_BLENDMODE_DESTCOLOR:
		op = GU_ADD;
		src = GU_FIX;
		dest = GU_FIX;
		srcfix = 0;
		destfix = 0xffffffff;
		break;
	case DX_BLENDMODE_INVDESTCOLOR:
		op = GU_ADD;
		src = GU_ONE_MINUS_DST_COLOR;
		dest = GU_FIX;
		srcfix = 0;
		destfix = 0;
		break;
	case DX_BLENDMODE_INVSRC:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_ONE_MINUS_SRC_ALPHA;
		srcfix = 0;
		destfix = 0;
		break;
	default:
		return -1;
	}
	if(gusettings.blendmode == DX_BLENDMODE_NOBLEND)
	{
		if(sceGuGetStatus(GU_BLEND))sceGuDisable(GU_BLEND);
	}
	else
	{
		if(!sceGuGetStatus(GU_BLEND))sceGuEnable(GU_BLEND);
		if(gusettings.bc.forceupdate
			|| gusettings.bc.op != op
			|| gusettings.bc.src != src
			|| gusettings.bc.dest != dest
			|| gusettings.bc.srcfix != srcfix
			|| gusettings.bc.destfix != destfix
		){
			sceGuBlendFunc(op,src,dest,srcfix,destfix);
			gusettings.bc.op = op;
			gusettings.bc.src = src;
			gusettings.bc.dest = dest;
			gusettings.bc.srcfix = srcfix;
			gusettings.bc.destfix = destfix;
		}
	}
	gusettings.bc.forceupdate = 0;
	return 0;
}

int SetDrawMode(int Mode)
{
	GUSTART;
	switch(Mode)
	{
	case DX_DRAWMODE_NEAREST:
		sceGuTexFilter(GU_NEAREST,GU_NEAREST);
		gusettings.flags[0] &= (~GPUSETTINGFLAGS_0_BILINEAR);
		return 0;
	case DX_DRAWMODE_BILINEAR:
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		gusettings.flags[0] |= GPUSETTINGFLAGS_0_BILINEAR;
		return 0;
	default:
		return -1;
	}
}

//static int ApplyBrightAndBlendMode();
int SetDrawBright(int Red,int Green,int Blue)
{
	gusettings.red		= Red & 0x000000ff;
	gusettings.green	= Green & 0x000000ff;
	gusettings.blue		= Blue & 0x000000ff;
	return 0;
}

int SetDrawBlendMode(int BlendMode,int Param)
{
	gusettings.blendmode = BlendMode;
	gusettings.blendparam= Param;
	gusettings.alpha = Param & 0x000000ff;
	return 0;
}



int DrawGraph(int x,int y,int gh,int trans)
{
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	return DrawExtendGraph(x,y,x + gptr->u1 - gptr->u0,y + gptr->v1 - gptr->v0,gh,trans);
}
int DrawTurnGraph(int x,int y,int gh,int trans)
{
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	return DrawExtendGraph(x + gptr->u1 - gptr->u0,y,x,y + gptr->v1 - gptr->v0,gh,trans);	
}

#define USE_OPTIMIZED_VERTEXBUFFER090410	//�R�����g�A�E�g�Œ��_���œK�����Ȃ��`��ɂ���B
int DrawExtendGraph(int x1,int y1,int x2,int y2,int gh,int trans)
{
	if((x2 < x1 || y2 < y1) && !(x2 < x1 && y2 < y1))return DrawModiGraph(x1,y1,x2,y1,x2,y2,x1,y2,gh,trans);//X�������]�摜�����܂����삵�Ȃ��̂ŁiGU_SPRITES���g�����̂������Ǝv����j�����ɉ��
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	GUSTART;
	if(SetTexture(gh,trans) == -1)return -1;
	int sw = gusettings.slice * 2 / PSM2BYTEx2(gptr->tex->psm);	/*���s�N�Z�����Ƃ�slice���邩*/
	int u[2];
	u[0] = gptr->u0;
#ifdef USE_OPTIMIZED_VERTEXBUFFER090410
	int count = (gptr->u1 - gptr->u0 + sw - 1) / sw;
	DXPVERTEX_2DTEX *vtxbuf = (DXPVERTEX_2DTEX*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX) * 2 * count);
	int i = 0;
	while(u[0] < gptr->u1)
	{
		u[1] = MIN(u[0] + sw,gptr->u1);
		if(vtxbuf == NULL)return -1;
		vtxbuf[i<<1].u = u[0];
		vtxbuf[i<<1].v = gptr->v0;
		vtxbuf[i<<1].x = x1 + (float)(x2 - x1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[i<<1].y = y1;
		vtxbuf[i<<1].z = gusettings.z_2d;
		vtxbuf[(i<<1)+1].u = u[1];
		vtxbuf[(i<<1)+1].v = gptr->v1;
		vtxbuf[(i<<1)+1].x = x1 + (float)(x2 - x1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+1].y = y2;
		vtxbuf[(i<<1)+1].z = gusettings.z_2d;
		u[0] += sw;
		++i;
	}
	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX) * 2 * count);
	sceGuDrawArray(GU_SPRITES,DXP_VTYPE_2DTEX | GU_TRANSFORM_2D,2 * count,NULL,vtxbuf);
#else
	while(u[0] < gptr->u1)
	{
		u[1] = MIN(u[0] + sw,gptr->u1);
		DXPVERTEX_2DTEX *vtxbuf = (DXPVERTEX_2DTEX*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX) * 2);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].u = u[0];
		vtxbuf[0].v = gptr->v0;
		vtxbuf[0].x = x1 + (float)(x2 - x1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[0].y = y1;
		vtxbuf[0].z = gusettings.z_2d;
		vtxbuf[1].u = u[1];
		vtxbuf[1].v = gptr->v1;
		vtxbuf[1].x = x1 + (float)(x2 - x1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[1].y = y2;
		vtxbuf[1].z = gusettings.z_2d;
		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX) * 2);
		sceGuDrawArray(GU_SPRITES,DXP_VTYPE_2DTEX | GU_TRANSFORM_2D,2,NULL,vtxbuf);
		u[0] += sw;
	}
#endif
	GuListSafety();
	return 0;
}
int DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int gh, int TransFlag )
{
	//TRIANGLESTRIP�ňꊇ�`�悷��悤�ɂ���Β��_�y�ѕ`��R�}���h�̓]�����啝�Ɍ��点��B���ɑ傫�ȃO���t�B�b�N��\������Ƃ��L�p���ȁH
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	GUSTART;
	if(SetTexture(gh,TransFlag) == -1)return -1;
	int sw = gusettings.slice * 2 / PSM2BYTEx2(gptr->tex->psm);	/*���s�N�Z�����Ƃ�slice���邩*/
#ifdef USE_OPTIMIZED_VERTEXBUFFER090410

//�V�����R�[�h��������----
	int count = (gptr->u1 - gptr->u0 + sw - 1) / sw;
	int u = gptr->u0,i = 1;
	DXPVERTEX_2DTEX *vtxbuf = (DXPVERTEX_2DTEX*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX) * 2 * (count + 1));
	if(vtxbuf == NULL)return -1;

	vtxbuf[0].u = u;
	vtxbuf[0].v = gptr->v0;
	vtxbuf[0].x = x1 + (float)(x2 - x1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[0].y = y1 + (float)(y2 - y1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[0].z = gusettings.z_2d;
	vtxbuf[1].u = u;
	vtxbuf[1].v = gptr->v1;
	vtxbuf[1].x = x4 + (float)(x3 - x4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[1].y = y4 + (float)(y3 - y4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[1].z = gusettings.z_2d;
	while(u < gptr->u1)
	{
		u = MIN(u + sw,gptr->u1);
		vtxbuf[(i<<1)+0].u = u;
		vtxbuf[(i<<1)+0].v = gptr->v0;
		vtxbuf[(i<<1)+0].x = x1 + (float)(x2 - x1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+0].y = y1 + (float)(y2 - y1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+0].z = gusettings.z_2d;
		vtxbuf[(i<<1)+1].u = u;
		vtxbuf[(i<<1)+1].v = gptr->v1;
		vtxbuf[(i<<1)+1].x = x4 + (float)(x3 - x4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+1].y = y4 + (float)(y3 - y4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+1].z = gusettings.z_2d;
		++i;
	}
	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX) * 2 * (count + 1));
	sceGuDrawArray(GU_TRIANGLE_STRIP,DXP_VTYPE_2DTEX | GU_TRANSFORM_2D,2 * (count + 1),NULL,vtxbuf);
//�����܂�----
#else
	int u[2];
	u[0] = gptr->u0;
	while(u[0] < gptr->u1)
	{
		u[1] = MIN(u[0] + sw,gptr->u1);
		DXPVERTEX_2DTEX *vtxbuf = (DXPVERTEX_2DTEX*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX) * 4);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].u = u[0];
		vtxbuf[0].v = gptr->v0;
		vtxbuf[0].x = x1 + (float)(x2 - x1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[0].y = y1 + (float)(y2 - y1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[0].z = gusettings.z_2d;

		vtxbuf[1].u = u[1];
		vtxbuf[1].v = gptr->v0;
		vtxbuf[1].x = x1 + (float)(x2 - x1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[1].y = y1 + (float)(y2 - y1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[1].z = gusettings.z_2d;

		vtxbuf[2].u = u[1];
		vtxbuf[2].v = gptr->v1;
		vtxbuf[2].x = x4 + (float)(x3 - x4) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[2].y = y4 + (float)(y3 - y4) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[2].z = gusettings.z_2d;

		vtxbuf[3].u = u[0];
		vtxbuf[3].v = gptr->v1;
		vtxbuf[3].x = x4 + (float)(x3 - x4) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[3].y = y4 + (float)(y3 - y4) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[3].z = gusettings.z_2d;

		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX) * 4);
		sceGuDrawArray(GU_TRIANGLE_FAN,DXP_VTYPE_2DTEX | GU_TRANSFORM_2D,4,NULL,vtxbuf);
		u[0] += sw;
	}
#endif	
	GuListSafety();
	return 0;
}
int DrawModiGraphF( float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4, int gh, int TransFlag )
{
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	GUSTART;
	if(SetTexture(gh,TransFlag) == -1)return -1;
	int sw = gusettings.slice * 2 / PSM2BYTEx2(gptr->tex->psm);	/*���s�N�Z�����Ƃ�slice���邩*/
#ifdef USE_OPTIMIZED_VERTEXBUFFER090410
//�V�����R�[�h��������----
	int count = (gptr->u1 - gptr->u0 + sw - 1) / sw;
	int u = gptr->u0,i = 1;
	DXPVERTEX_2DTEX_F *vtxbuf = (DXPVERTEX_2DTEX_F*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX_F) * 2 * (count + 1));
	if(vtxbuf == NULL)return -1;

	vtxbuf[0].u = u;
	vtxbuf[0].v = gptr->v0;
	vtxbuf[0].x = x1 + (float)(x2 - x1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[0].y = y1 + (float)(y2 - y1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[0].z = gusettings.z_2d;
	vtxbuf[1].u = u;
	vtxbuf[1].v = gptr->v1;
	vtxbuf[1].x = x4 + (float)(x3 - x4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[1].y = y4 + (float)(y3 - y4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
	vtxbuf[1].z = gusettings.z_2d;
	while(u < gptr->u1)
	{
		u = MIN(u + sw,gptr->u1);
		vtxbuf[(i<<1)+0].u = u;
		vtxbuf[(i<<1)+0].v = gptr->v0;
		vtxbuf[(i<<1)+0].x = x1 + (float)(x2 - x1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+0].y = y1 + (float)(y2 - y1) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+0].z = gusettings.z_2d;
		vtxbuf[(i<<1)+1].u = u;
		vtxbuf[(i<<1)+1].v = gptr->v1;
		vtxbuf[(i<<1)+1].x = x4 + (float)(x3 - x4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+1].y = y4 + (float)(y3 - y4) * (u - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[(i<<1)+1].z = gusettings.z_2d;
		++i;
	}
	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX_F) * 2 * (count + 1));
	sceGuDrawArray(GU_TRIANGLE_STRIP,DXP_VTYPE_2DTEX_F | GU_TRANSFORM_2D,2 * (count + 1),NULL,vtxbuf);
//�����܂�----
#else
	int u[2];
	u[0] = gptr->u0;
	while(u[0] < gptr->u1)
	{
		u[1] = MIN(u[0] + sw,gptr->u1);
		DXPVERTEX_2DTEX_F *vtxbuf = (DXPVERTEX_2DTEX_F*)sceGuGetMemory(sizeof(DXPVERTEX_2DTEX_F) * 4);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].u = u[0];
		vtxbuf[0].v = gptr->v0;
		vtxbuf[0].x = x1 + (float)(x2 - x1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[0].y = y1 + (float)(y2 - y1) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[0].z = gusettings.z_2d;

		vtxbuf[1].u = u[1];
		vtxbuf[1].v = gptr->v0;
		vtxbuf[1].x = x1 + (float)(x2 - x1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[1].y = y1 + (float)(y2 - y1) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[1].z = gusettings.z_2d;

		vtxbuf[2].u = u[1];
		vtxbuf[2].v = gptr->v1;
		vtxbuf[2].x = x4 + (float)(x3 - x4) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[2].y = y4 + (float)(y3 - y4) * (u[1] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[2].z = gusettings.z_2d;

		vtxbuf[3].u = u[0];
		vtxbuf[3].v = gptr->v1;
		vtxbuf[3].x = x4 + (float)(x3 - x4) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[3].y = y4 + (float)(y3 - y4) * (u[0] - gptr->u0) / (gptr->u1 - gptr->u0);
		vtxbuf[3].z = gusettings.z_2d;

		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2DTEX_F) * 4);
		sceGuDrawArray(GU_TRIANGLE_FAN,DXP_VTYPE_2DTEX_F | GU_TRANSFORM_2D,4,NULL,vtxbuf);
		u[0] += sw;
	}
#endif
	GuListSafety();
	return 0;
}
int	DrawRotaGraph(int x,int y,double ExtRate,double Angle,int gh,int trans,int turn)
{
	return DrawRotaGraphF(x,y,ExtRate,Angle,gh,trans,turn);
}
int	DrawRotaGraphF(float x,float y,double ExtRate,double Angle,int gh,int trans,int turn)
{
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	float x1,x2,x3,x4,y1,y2,y3,y4;
	float x1_,x2_,x3_,x4_,y1_,y2_,y3_,y4_;
	x2 = x3 = (gptr->u1 - gptr->u0) / 2;
	x1 = x4 = -x3;
	y3 = y4 = (gptr->v1 - gptr->v0) / 2;
	y1 = y2 = -y3;
	x1 *= ExtRate;
	x2 *= ExtRate;
	x3 *= ExtRate;
	x4 *= ExtRate;
	y1 *= ExtRate;
	y2 *= ExtRate;
	y3 *= ExtRate;
	y4 *= ExtRate;
	float s,c;
	s = sinf(Angle);
	c = cosf(Angle);

#define XROT(VARNUM)	\
	{	\
	x##VARNUM##_ = x##VARNUM * c - y##VARNUM * s + x;	\
	y##VARNUM##_ = x##VARNUM * s + y##VARNUM * c + y;	\
	}

	XROT(1)
	XROT(2)
	XROT(3)
	XROT(4)
#undef XROT
	if(turn)return DrawModiGraphF(x2_,y2_,x1_,y1_,x4_,y4_,x3_,y3_,gh,trans);
	return DrawModiGraphF(x1_,y1_,x2_,y2_,x3_,y3_,x4_,y4_,gh,trans);
//���f����cos��-ysin��
//���f����sin��+ycos��
}
int DrawRotaGraph2(int x,int y,int cx,int cy,double ExtRate,double Angle,int gh,int trans,int turn)
{
	return DrawRotaGraph2F(x,y,cx,cy,ExtRate,Angle,gh,trans,turn);
}
int	DrawRotaGraph2F(float x,float y,float cx,float cy,double ExtRate,double Angle,int gh,int trans,int turn)//���e�X�g
{
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	float x1,x2,x3,x4,y1,y2,y3,y4;
	float x1_,x2_,x3_,x4_,y1_,y2_,y3_,y4_;
	x2 = x3 = (gptr->u1 - gptr->u0) - cx;
	x1 = x4 = -cx;
	y3 = y4 = (gptr->v1 - gptr->v0) - cy;
	y1 = y2 = -cy;
	x1 *= ExtRate;
	x2 *= ExtRate;
	x3 *= ExtRate;
	x4 *= ExtRate;
	y1 *= ExtRate;
	y2 *= ExtRate;
	y3 *= ExtRate;
	y4 *= ExtRate;
	float s,c;
	s = sinf(Angle);
	c = cosf(Angle);

#define XROT(VARNUM)	\
	{	\
	x##VARNUM##_ = x##VARNUM * c - y##VARNUM * s + x;	\
	y##VARNUM##_ = x##VARNUM * s + y##VARNUM * c + y;	\
	}

	XROT(1)
	XROT(2)
	XROT(3)
	XROT(4)
#undef XROT
	if(turn)return DrawModiGraphF(x2_,y2_,x1_,y1_,x4_,y4_,x3_,y3_,gh,trans);
	return DrawModiGraphF(x1_,y1_,x2_,y2_,x3_,y3_,x4_,y4_,gh,trans);
//���f����cos��-ysin��
//���f����sin��+ycos��
}
//int	DrawRotaGraph2(int x,int y,int cx,int cy,double ExtRate,double Angle,int gh,int trans,int turn);
/*
�e�N�X�`���֘A�̊֐��T��
sceGuTexEnvColor	�e�N�X�`���u�����f�B���O�̒萔��ݒ�
sceGuTexFunc		�e�N�X�`���u�����h���@��ݒ�
sceGuTexFilter		�e�N�X�`���t�B���^�����O�̐ݒ�@�l�A���X�g�ƃo�C���j�A�����ł������Ƃ�

sceGuTexFlush		GPU�����̃e�N�X�`���L���b�V�����΂�
sceGuTexMapMode		�e�N�X�`���̒�����̎w��

sceGuTexMode		�e�N�X�`���t�H�[�}�b�g�Ƃ��̎w��
sceGuTexImage		�e�N�X�`����ݒ�

sceGuTexWrap		�e�N�X�`����UV���W�����E�˔j�����Ƃ��ǂ����邩�ݒ�

sceGuTexOffset		�e�N�X�`����UV���W�ɉ��Z�����l�B2D�ł͎g���Ȃ�
sceGuTexProjMapMode	�e�N�X�`����UV���W�n�ɉ����g����	�����Ăяo�����Ƃ͖���
sceGuTexScale		�e�N�X�`����UV���W�ɏ�Z�����l�B2D�ł͎g���Ȃ�
sceGuTexSlope		�s��
sceGuTexSync		sceGuCopyImage()�̏I����҂�
sceGuTexLevelMode	�݂��Ճ}�b�v�̐ݒ�@�����g��Ȃ�����


�X�e�[�^�X�̈ꗗ
#define GU_ALPHA_TEST		(0)		�A���t�@�e�X�g
#define GU_DEPTH_TEST		(1)		�[�x�e�X�g
#define GU_SCISSOR_TEST		(2)		�V�U�[�e�X�g�i�`���`���Ƀs�N�Z�������܂��Ă��邩�̔���j
#define GU_STENCIL_TEST		(3)		�X�e���V���e�X�g	�o�b�t�@�̐ݒ���@���悭�킩��Ȃ� �f�B�X�v���C�o�b�t�@�̃��������g���炵��
#define GU_BLEND		(4)			�u�����h���g�p����B
#define GU_CULL_FACE		(5)		�J�����O	2D�ł͎g��Ȃ�
#define GU_DITHER		(6)			���F		�悭�킩��Ȃ�
#define GU_FOG			(7)			�t�H�O		2D�ł͂��Ԃ�����Ȃ�
#define GU_CLIP_PLANES		(8)		�s��		�T���v���ł�Enable�ɂȂ��Ă�
#define GU_TEXTURE_2D		(9)		�e�N�X�`���𒣂邩�ǂ���
#define GU_LIGHTING		(10)		���C�e�B���O	�g��Ȃ�
#define GU_LIGHT0		(11)
#define GU_LIGHT1		(12)
#define GU_LIGHT2		(13)
#define GU_LIGHT3		(14)
#define GU_LINE_SMOOTH		(15)	�΂߂̐����̕`�悪�Y��ɂȂ�B
#define GU_PATCH_CULL_FACE	(16)	�s��
#define GU_COLOR_TEST		(17)	�J���[�e�X�g�@�J���[�L�[�݂����Ȃ̂��ł���H
#define GU_COLOR_LOGIC_OP	(18)	�����J���[�e�X�g�Ŏg�����̂���
#define GU_FACE_NORMAL_REVERSE	(19)�����@���𔽓]���Ĉ����̂ł͂Ȃ����Ɓc
#define GU_PATCH_FACE		(20)	�����Ȗʃp�b�`���g���B2D�ł͎g��Ȃ���c
#define GU_FRAGMENT_2X		(21)	�t���O�����g�Ƃ̓s�N�Z���̂��Ƃ炵���B����Łc�H

*/

int	DrawLine( int x1, int y1, int x2, int y2, int Color)
{
	GUSTART;
	SetTexture(-1,0);
	SetBaseColor(Color);
	DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 2);
	if(vtxbuf == NULL)return -1;
	vtxbuf[0].x = x1;
	vtxbuf[0].y = y1;
	vtxbuf[0].z = gusettings.z_2d;
	vtxbuf[1].x = x2;
	vtxbuf[1].y = y2;
	vtxbuf[1].z = gusettings.z_2d;
	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 2);
	sceGuDrawArray(GU_LINES,DXP_VTYPE_2D | GU_TRANSFORM_2D,2,0,vtxbuf);
	return 0;
}

int DrawBox(int x1,int y1,int x2,int y2,int color,int fillflag)
{
	GUSTART
	SetTexture(-1,0);
	SetBaseColor(color);
	if(fillflag)
	{
		DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 2);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].x = x1;
		vtxbuf[0].y = y1;
		vtxbuf[0].z = gusettings.z_2d;
		vtxbuf[1].x = x2;
		vtxbuf[1].y = y2;
		vtxbuf[1].z = gusettings.z_2d;
		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 2);
		sceGuDrawArray(GU_SPRITES,DXP_VTYPE_2D | GU_TRANSFORM_2D,2,0,vtxbuf);
	}
	else
	{
		DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 5);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].x = x1;
		vtxbuf[0].y = y1;
		vtxbuf[0].z = gusettings.z_2d;
		vtxbuf[1].x = x2;
		vtxbuf[1].y = y1;
		vtxbuf[1].z = gusettings.z_2d;
		vtxbuf[2].x = x2;
		vtxbuf[2].y = y2;
		vtxbuf[2].z = gusettings.z_2d;
		vtxbuf[3].x = x1;
		vtxbuf[3].y = y2;
		vtxbuf[3].z = gusettings.z_2d;
		vtxbuf[4].x = x1;
		vtxbuf[4].y = y1;
		vtxbuf[4].z = gusettings.z_2d;
		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 5);
		sceGuDrawArray(GU_LINE_STRIP,DXP_VTYPE_2D | GU_TRANSFORM_2D,5,0,vtxbuf);
	}
	return 0;
}

int	DrawPixel( int x, int y, int Color)
{
	GUSTART
	SetTexture(-1,0);
	SetBaseColor(Color);
	DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 2);
	if(vtxbuf == NULL)return -1;
	vtxbuf[0].x = x;
	vtxbuf[0].y = y;
	vtxbuf[0].z = gusettings.z_2d;
	vtxbuf[1].x = x + 1;
	vtxbuf[1].y = y + 1;
	vtxbuf[1].z = gusettings.z_2d;
	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 2);
	sceGuDrawArray(GU_LINES,DXP_VTYPE_2D | GU_TRANSFORM_2D,2,0,vtxbuf);
	return 0;

}

#define DXPOVAL_DIV	128
int	DrawCircle( int x, int y, int r, int Color,int fill)
{
	GUSTART;
	SetTexture(-1,0);
	SetBaseColor(Color);
	DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * (DXPOVAL_DIV + 2));
	if(vtxbuf == NULL)return -1;
	int i;
	vtxbuf[0].x = x;
	vtxbuf[0].y = y;
	vtxbuf[0].z = gusettings.z_2d;

	for(i = 1;i <= DXPOVAL_DIV + 1;++i)
	{
		vtxbuf[i].x = x + r * cos(M_PI * 2 / DXPOVAL_DIV * i);
		vtxbuf[i].y = y + r * sin(M_PI * 2 / DXPOVAL_DIV * i);
		vtxbuf[i].z = gusettings.z_2d;
	}

	sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * (2 + DXPOVAL_DIV));
	if(fill)
		sceGuDrawArray(GU_TRIANGLE_FAN,DXP_VTYPE_2D | GU_TRANSFORM_2D,DXPOVAL_DIV + 2,0,vtxbuf);
	else
		sceGuDrawArray(GU_LINE_STRIP,DXP_VTYPE_2D | GU_TRANSFORM_2D,DXPOVAL_DIV + 1,0,vtxbuf + 1);
	return 0;
}


int DeleteGraph(int gh)
{
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex != NULL)
	{
		--gptr->tex->refcount;
		if(gptr->tex->refcount <= 0)
		{
			FREE(gptr->tex->pmemory);
			FREE(gptr->tex->ppalette);
			FreeVRAM(gptr->tex->pvram);
			TextureList_Remove(gptr->tex);
			FREE(gptr->tex);
		}
	}
	GraphDataList_Remove(gptr);
	FREE(gptr);
	return 0;
}

int SetSliceSize(int size)
{
	switch(size)
	{
	case 16:
	case 32:
	case 64:
	case 128:
		break;
	default:
		return -1;
	}
	gusettings.slice = size;
	return 0;
}

int GetGraphSize(int gh,int *px,int *py)
{
	DXPGRAPHDATA* gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(px != NULL)*px = gptr->u1 - gptr->u0;
	if(py != NULL)*py = gptr->v1 - gptr->v0;
	return 0;
}

int SetTransColor(int red,int green,int blue)
{
	gusettings.colorkey = ((blue & 0x000000ff) << 16) | ((green & 0x000000ff) << 8) | (red & 0x000000ff);
	return 0;
}

void WaitGPUSync()
{
	GUFINISH
}

int SetDrawArea(int x1,int y1,int x2,int y2)
{
	GUSTART
	gusettings.scissor[0] = x1;
	gusettings.scissor[1] = y1;
	gusettings.scissor[2] = x2;
	gusettings.scissor[3] = y2;
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(gusettings.scissor[0],gusettings.scissor[1],gusettings.scissor[2],gusettings.scissor[3]);
	return 0;
}

int DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color,int fill)
{
	GUSTART
	SetTexture(-1,0);
	SetBaseColor(color);
	if(fill)
	{
		DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 3);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].x = x1;
		vtxbuf[0].y = y1;
		vtxbuf[0].z = gusettings.z_2d;
		vtxbuf[1].x = x2;
		vtxbuf[1].y = y2;
		vtxbuf[1].z = gusettings.z_2d;
		vtxbuf[2].x = x3;
		vtxbuf[2].y = y3;
		vtxbuf[2].z = gusettings.z_2d;
		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 3);
		sceGuDrawArray(GU_TRIANGLES,DXP_VTYPE_2D | GU_TRANSFORM_2D,3,0,vtxbuf);
	}
	else
	{
		DXPVERTEX_2D *vtxbuf = sceGuGetMemory(sizeof(DXPVERTEX_2D) * 4);
		if(vtxbuf == NULL)return -1;
		vtxbuf[0].x = x1;
		vtxbuf[0].y = y1;
		vtxbuf[0].z = gusettings.z_2d;
		vtxbuf[1].x = x2;
		vtxbuf[1].y = y2;
		vtxbuf[1].z = gusettings.z_2d;
		vtxbuf[2].x = x3;
		vtxbuf[2].y = y3;
		vtxbuf[2].z = gusettings.z_2d;
		vtxbuf[3].x = x1;
		vtxbuf[3].y = y1;
		vtxbuf[3].z = gusettings.z_2d;
		sceKernelDcacheWritebackRange(vtxbuf,sizeof(DXPVERTEX_2D) * 4);
		sceGuDrawArray(GU_LINE_STRIP,DXP_VTYPE_2D | GU_TRANSFORM_2D,4,0,vtxbuf);
	}
	return 0;
}

int SetDrawScreen(int ghandle)
{
	GUSTART
	if(ghandle == -1)return -1;
	if(ghandle == DXP_SCREEN_BACK)
	{
		gusettings.rendertarget = &gusettings.displaybuffer[gusettings.backbuffer];
	}
	else
	{
		DXPTEXTURE2 *tmp = GraphHandle2TexPtr(ghandle);
		if(tmp != NULL)
		{
			if(tmp->vramflag && !tmp->swizzledflag)gusettings.rendertarget = tmp;
		}
	}
	if(gusettings.rendertarget == NULL)gusettings.rendertarget = &gusettings.displaybuffer[gusettings.backbuffer];
	sceGuTexFlush();
	sceGuDrawBufferList(gusettings.rendertarget->psm,gusettings.rendertarget->pvram->address,gusettings.rendertarget->pitch);
	sceGuOffset(2048 - (gusettings.rendertarget->width/2),2048 - (gusettings.rendertarget->height/2));
	sceGuViewport(2048,2048,gusettings.rendertarget->width,gusettings.rendertarget->height);

	return 0;
}

int SaveDrawScreen( int x1, int y1, int x2, int y2, char *FileName )
{
	return 0;
}

int SetDisplayFormat(int format)
{
	switch(format)
	{
	case GU_PSM_4444:
	case GU_PSM_5551:
	case GU_PSM_5650:
	case GU_PSM_8888:
		break;
	default:
		return -1;
	}
	if(format == gusettings.displaybuffer[0].psm)return 0;
	if(gusettings.displaybuffer[0].pvram == NULL)
	{
		gusettings.displaybuffer[0].psm = gusettings.displaybuffer[1].psm = format;
		return 0;
	}
	int size = GraphSize2DataSize(512,272,format);
	int pformat = gusettings.displaybuffer[0].psm;
	FreeVRAM(gusettings.displaybuffer[0].pvram);
	FreeVRAM(gusettings.displaybuffer[1].pvram);
	gusettings.displaybuffer[0].pvram = AllocVRAM(size,1);
	gusettings.displaybuffer[1].pvram = AllocVRAM(size,1);
	if(gusettings.displaybuffer[0].pvram == NULL || gusettings.displaybuffer[1].pvram == NULL)
	{
		FreeVRAM(gusettings.displaybuffer[0].pvram);
		FreeVRAM(gusettings.displaybuffer[1].pvram);
		size = GraphSize2DataSize(512,272,pformat);
		gusettings.displaybuffer[0].pvram = AllocVRAM(size,1);
		gusettings.displaybuffer[1].pvram = AllocVRAM(size,1);
		return -1;
	}
	gusettings.displaybuffer[0].psm = format;
	gusettings.displaybuffer[1].psm = format;
	return 0;
}

int GetDisplayFormat()
{
	return gusettings.displaybuffer[0].psm;
}