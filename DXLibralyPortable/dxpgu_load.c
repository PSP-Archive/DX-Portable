/*
*	DX���C�u����Portable	�摜�ǂݍ��݁A�ϊ���	Ver1.10
*	�����	�F�J��
*	���l	�F���������F�X�Ə���������܂��BSwizzle������VRAM�ւ̓]���͂����ōs���Ă��܂��B
*	
*/

#include <pspgu.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <png.h>
#include "dxlibp.h"
#include "dxpstatic.h"
#include <pspjpeg.h>
/*
�摜�̑傫��
���ۂɓǂݏo���̈�
�s�b�`�̌v�Z
�m��
�ǂݏo��
�o�^

*/
#define GU_PSM_5650		(0) /* Display, Texture, Palette */
#define GU_PSM_5551		(1) /* Display, Texture, Palette */
#define GU_PSM_4444		(2) /* Display, Texture, Palette */
#define GU_PSM_8888		(3) /* Display, Texture, Palette */

DXPTEXTURE2* LoadJpegImage(const char *FileName);
DXPTEXTURE2* LoadPngImage(const char *FileName);



int LoadGraph(const char *FileName)
{
	DXPGRAPHDATA *gptr = (DXPGRAPHDATA*)MALLOC(sizeof(DXPGRAPHDATA));
	if(gptr == NULL)return -1;
	gptr->tex = LoadPngImage(FileName);
	if(gptr->tex == NULL)gptr->tex = LoadJpegImage(FileName);
	if(gptr->tex == NULL)
	{
		FREE(gptr);
		return -1;
	}
	++gptr->tex->refcount;
	gptr->handle = GenerateGraphHandle();
	gptr->u0 = 0;
	gptr->v0 = 0;
	gptr->u1 = gptr->tex->umax;
	gptr->v1 = gptr->tex->vmax;
	if(gptr->handle == -1)
	{
		return -1;
	}
	GraphArray[gptr->handle] = gptr;
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
		SwizzleGraph(gptr->handle);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
		MoveGraphToVRAM(gptr->handle);
	return gptr->handle;
}

int DerivationGraph( int SrcX, int SrcY,int Width, int Height, int src )
{
	if(SrcX < 0 || SrcY < 0)return -1;
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(src);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	if(SrcX + Width > gptr->tex->umax || SrcY + Height > gptr->tex->vmax)return -1;	
	DXPGRAPHDATA *res = (DXPGRAPHDATA*)MALLOC(sizeof(DXPGRAPHDATA));
	if(res == NULL)return -1;
	res->u0 = SrcX;
	res->v0 = SrcY;
	res->u1 = SrcX + Width;
	res->v1 = SrcY + Height;
	res->tex = gptr->tex;
	++res->tex->refcount;
	res->handle = GenerateGraphHandle();
	GraphArray[res->handle] = res;
	return res->handle;
}

int LoadDivGraph( const char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf)
{
	DXPTEXTURE2 *tex = NULL;
	int i;
	if(HandleBuf == NULL)return -1;
	if(AllNum > XNum * YNum)AllNum = XNum * YNum;
	for(i = 0;i < AllNum;++i)HandleBuf[i] = -1;
	if(AllNum <= 0 || XNum <= 0 || YNum <= 0 || XSize <= 0 || YSize <= 0)return -1;
	if(tex == NULL)tex = LoadPngImage(FileName);
	if(tex == NULL)tex = LoadJpegImage(FileName);
	if(tex == NULL)return -1;
	for(i = 0;i < AllNum;++i)
	{
		int x,y;
		x = XSize * (i % XNum);
		y = YSize * (i / XNum);
		if(x > tex->umax || y > tex->vmax || x + XSize > tex->umax || y + YSize > tex->vmax)continue;
		DXPGRAPHDATA *gptr = (DXPGRAPHDATA*)MALLOC(sizeof(DXPGRAPHDATA));
		if(gptr == NULL)break;
		gptr->tex = tex;
		++tex->refcount;
		gptr->u0 = x;
		gptr->v0 = y;
		gptr->u1 = x + XSize;
		gptr->v1 = y + YSize;
		HandleBuf[i] = gptr->handle = GenerateGraphHandle();
		GraphArray[gptr->handle] = gptr;
	}
	if(i == 0)
	{
		DeleteTexture(tex);
		return -1;
	}
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
		SwizzleGraph(HandleBuf[0]);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
		MoveGraphToVRAM(HandleBuf[0]);
	return 0;
}

//int PSM2BYTEx2(int psm)
//{
//	switch(psm)
//	{
//	case GU_PSM_T4:
//		return 1;
//	case GU_PSM_T8:
//		return 2;
//	case GU_PSM_4444:
//	case GU_PSM_5551:
//	case GU_PSM_5650:
//		return 4;
//	case GU_PSM_8888:
//		return 8;
//	default:
//		return 0;
//	}
//}

void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;
 
   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);
 
   unsigned int src_pitch = (width-16)/4;
   unsigned int src_row = width * 8;
 
   const u8* ysrc = in;
   u32* dst = (u32*)out;
 
   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      const u8* xsrc = ysrc;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         const u32* src = (u32*)xsrc;
         for (j = 0; j < 8; ++j)//16byte���ō���8�̏�����`�ɓ]��
         {
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            src += src_pitch;
         }
         xsrc += 16;
     }
     ysrc += src_row;
   }
}
void unswizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;
 
   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);
 
   unsigned int src_pitch = (width-16)/4;
   unsigned int src_row = width * 8;
 
   u8* ysrc = out;
   u32* dst = (u32*)in;
 
   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      u8* xsrc = ysrc;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         u32* src = (u32*)xsrc;
         for (j = 0; j < 8; ++j)
         {
            *(src++) = *(dst++);
            *(src++) = *(dst++);
            *(src++) = *(dst++);
            *(src++) = *(dst++);
            src += src_pitch;
         }
         xsrc += 16;
     }
     ysrc += src_row;
   }
}

int SwizzleGraph(int gh)	/*�w�肳�ꂽ�O���t�B�b�N��Swizzle����B*/
{
	GUFINISH
	void *buf;
	s32 size;
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	if(gptr->tex->swizzledflag){return 0;}
	if(gptr->tex == gusettings.rendertarget)return -1;
	if((size = GraphSize2DataSize(gptr->tex->pitch,gptr->tex->height,gptr->tex->psm)) <= 0){return -1;}
	if((buf = MEMALIGN(16,size)) == NULL){return -1;}
	if(gptr->tex->vramflag)
	{
		swizzle_fast(buf,sceGeEdramGetAddr() + gptr->tex->pvram->offset, PSM2BYTEx2(gptr->tex->psm) * gptr->tex->pitch / 2,gptr->tex->height);
		memcpy(sceGeEdramGetAddr() + gptr->tex->pvram->offset,buf,size);
		FREE(buf);
	}else
	{
		swizzle_fast(buf,gptr->tex->pmemory, PSM2BYTEx2(gptr->tex->psm) * gptr->tex->pitch >> 1,gptr->tex->height);
		FREE(gptr->tex->pmemory);
		gptr->tex->pmemory = buf;
	}
	gptr->tex->swizzledflag = 1;
	sceKernelDcacheWritebackAll();
	gptr->tex->reloadflag = 1;
	return 0;
}
int UnswizzleGraph(int gh)	/*�w�肳�ꂽ�O���t�B�b�N��Unswizzle����B*/
{
	GUFINISH
	void *buf;
	s32 size;
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	if(!gptr->tex->swizzledflag){return 0;}
	if((size = GraphSize2DataSize(gptr->tex->pitch,gptr->tex->height,gptr->tex->psm)) <= 0){return -1;}
	if((buf = MEMALIGN(16,size)) == NULL){return -1;}
	if(gptr->tex->vramflag)
	{
		unswizzle_fast(buf,sceGeEdramGetAddr() + gptr->tex->pvram->offset, PSM2BYTEx2(gptr->tex->psm) * gptr->tex->pitch / 2,gptr->tex->height);
		memcpy(sceGeEdramGetAddr() + gptr->tex->pvram->offset,buf,size);
		FREE(buf);
	}else
	{
		unswizzle_fast(buf,gptr->tex->pmemory, PSM2BYTEx2(gptr->tex->psm) * gptr->tex->pitch >> 1,gptr->tex->height);
		FREE(gptr->tex->pmemory);
		gptr->tex->pmemory = buf;
	}
	gptr->tex->swizzledflag = 0;
	sceKernelDcacheWritebackAll();
	gptr->tex->reloadflag = 1;
	return 0;
}

int MoveGraphToVRAM(int gh)	/*�O���t�B�b�N��VRAM�Ɉړ�����BVRAM�s���̏ꍇ�͂Ȃɂ����Ȃ��B*/
{
	GUFINISH
	s32 size;
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	if(gptr->tex->vramflag)return 0;
	if((size = GraphSize2DataSize(gptr->tex->pitch,gptr->tex->height,gptr->tex->psm)) <= 0)return -1;
	if((gptr->tex->pvram = AllocVRAM(size,0)) == NULL)return -1;
	memcpy(sceGeEdramGetAddr() + gptr->tex->pvram->offset,gptr->tex->pmemory,size);
	sceKernelDcacheWritebackAll();
	FREE(gptr->tex->pmemory);
	gptr->tex->pmemory = NULL;
	gptr->tex->vramflag = 1;
	gptr->tex->reloadflag = 1;
	return 0;
}

int MoveGraphToDDR(int gh)	/*�O���t�B�b�N�����C���������Ɉړ�����*/
{
	GUFINISH
	s32 size;
	DXPGRAPHDATA *gptr = GraphHandle2Ptr(gh);
	if(gptr == NULL)return -1;
	if(gptr->tex == NULL)return -1;
	if(!gptr->tex->vramflag)return 0;
	if(gptr->tex == gusettings.rendertarget)return -1;
	if((size = GraphSize2DataSize(gptr->tex->pitch,gptr->tex->height,gptr->tex->psm)) <= 0)return -1;
	if((gptr->tex->pmemory = MEMALIGN(16,size)) == NULL)return -1;
	
	memcpy(gptr->tex->pmemory,sceGeEdramGetAddr() + gptr->tex->pvram->offset,size);
	sceKernelDcacheWritebackAll();
	FreeVRAM(gptr->tex->pvram);
	gptr->tex->pvram = NULL;
	gptr->tex->vramflag = 0;
	gptr->tex->reloadflag = 1;
	return 0;
}

//int LoadRAWData(const char *FileName,int SizeX,int SizeY,int Format)
//{
//	int i;
//	int fh;
//	int gh;
//	int depth;
//	if((fh = FileRead_open(FileName)) == -1)return -1;
//	if((gh = MakeGraph(SizeX,SizeY,Format)) == -1)
//	{
//		FileRead_close(fh);
//		return -1;
//	}
//	DXPTEXTURE *texptr = GraphHandle2Ptr(gh);
//	if(texptr == NULL)
//	{
//		DeleteGraph(gh);
//		FileRead_close(fh);
//		return -1;
//	}
//	depth = Format == GU_PSM_8888 ? 4 : 2;
//	for(i = 0;i < texptr->mv;++i)
//	{
//		FileRead_seek(fh,i * SizeX * depth,SEEK_SET);
//		FileRead_read(texptr->pmemory + i * texptr->pitch * depth,i * texptr->pitch * depth,fh);
//	}
////	FileRead_read(texarray[gh].pmemory,texarray[gh].mu * texarray[gh].mv * depth,fh);
//	FileRead_close(fh);
//	sceKernelDcacheWritebackAll();
////	sceKernelDcacheWritebackRange(texarray[gh].pmemory,texarray[gh].pitch * texarray[gh].height * depth);
//
//	/*�K�v������A���\�Ȃ��swizzle����*/
//	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
//	{
//		//swizzle����
//		SwizzleGraph(gh);
//	}
//	/*�K�v������A���\�Ȃ��VRAM�ɓ]������*/
//	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
//	{
//		MoveGraphToVRAM(gh);
//	}
//	return gh;
//}

int ConvertGraphFormat(int gh,int psm)
/*
DDR���Swizzle�Ȃ��O���t�B�b�N�ɂ���B
����
VRAM�O���t�B�b�N��������Swizzle�O���t�B�b�N��������Ȃ猳�ɖ߂��B

�X�[�p�[�r�b�g���Z�^�C���B�f�o�b�O����C�������܂���
*/
{
	int x,y;
	char swizzleflag = 0;
	char vramflag = 0;
	int size;
	void* buf;
	//GraphicHandleCheck(gh);
	DXPTEXTURE2 *texptr = GraphHandle2TexPtr(gh);
	if(texptr == NULL)return -1;
	if(psm == texptr->psm)return 0;
	if(psm < 0 || psm > 3 || texptr->psm < 0 || texptr->psm > 3)return -1;	/*�ϊ����ƕϊ���̂ǂ��炩��4444,5551,5650,8888�ȊO�̃t�H�[�}�b�g�Ȃ玸�s*/
	size = GraphSize2DataSize(texptr->pitch,texptr->height,psm);
	if((buf = MEMALIGN(16,size)) == NULL)return -1;
	if(texptr->vramflag)
	{
		MoveGraphToDDR(gh);
		vramflag = 1;
	}
	if(texptr->swizzledflag)
	{
		UnswizzleGraph(gh);
		swizzleflag = 1;
	}
	switch(texptr->psm)
	{
	case GU_PSM_4444:
		goto SRC4444;
	case GU_PSM_5551:
		goto SRC5551;
	case GU_PSM_5650:
		goto SRC5650;
	case GU_PSM_8888:
		goto SRC8888;
	default:
		return -1;
	}
SRC4444:
	switch(psm)
	{
	case GU_PSM_5551:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = (color & 0x8000) | ((color & 0x0f00) << 3) | ((color & 0x00f0) << 2) | ((color & 0x000f) << 1)
					| ((color & 0x0800) >> 1) | ((color & 0x0080) >> 2) | ((color & 0x0008) >> 3);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	case GU_PSM_5650:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x0f00) << 4) | ((color & 0x00f0) << 3) | ((color & 0x000f) << 1)
					| (color & 0x0800) | ((color & 0x00c0) >> 1) | ((color & 0x0008) << 1);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		texptr->alphabit = 0;
		goto ENDPROCESS;
	case GU_PSM_8888:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x0000f000) << 16) | ((color & 0x0000f000) << 12)
					| ((color & 0x00000f00) << 12) | ((color & 0x00000f00) << 8)
					| ((color & 0x000000f0) << 8) | ((color & 0x000000f0) << 4)
					| ((color & 0x0000000f) << 4) | (color & 0x0000000f);
				*(((u32*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	}
SRC5551:
	switch(psm)
	{
	case GU_PSM_4444:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = (color & 0x8000) | ((color & 0x8000) >> 1) | ((color & 0x8000) >> 2) | ((color & 0x8000) >> 3)
					| ((color & 0x7800) >> 3) | ((color & 0x03c0) >> 2) | ((color & 0x001e) >> 1);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	case GU_PSM_5650:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u32*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x7ef0) << 1) | (color & 0x01f)
					| ((color & 0x0200) >> 4);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		texptr->alphabit = 0;
		goto ENDPROCESS;
	case GU_PSM_8888:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x8000) << 16) | ((color & 0x8000) << 15) | ((color & 0x8000) << 14) | ((color & 0x8000) << 13)
					 | ((color & 0x8000) << 12) | ((color & 0x8000) << 11) | ((color & 0x8000) << 10) | ((color & 0x8000) << 9)
					 | ((color & 0x7c00) << 9) | ((color & 0x03e0) << 6) | ((color & 0x001f) << 3)
					 | ((color & 0x7000) << 4) | ((color & 0x0380) << 1) | ((color & 0x001c) >> 2);
				*(((u32*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	}
SRC5650:
	switch(psm)
	{
	case GU_PSM_4444:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = 0xf000 | ((color & 0xf000) >> 4) | ((color & 0x0780) >> 3) | ((color & 0x001e) >> 1);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	case GU_PSM_5551:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u16 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = 0x8000 | ((color & 0xffc0) >> 1) | (color & 0x001f);
				*(((u16*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	case GU_PSM_8888:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u16*)texptr->pmemory) + y * texptr->pitch + x);
				color = 0xff000000 | ((color & 0x00008f00) << 8) | ((color & 0x07e0) << 5) | ((color & 0x0000001f) << 3)
					| ((color & 0x0000e000) << 3) | ((color & 0x00000600) >> 1) | ((color & 0x0000001c) >> 2);
				*(((u32*)buf) + y * texptr->pitch + x) = color;
			}
		}
		goto ENDPROCESS;
	}
SRC8888:
	switch(psm)
	{
	case GU_PSM_4444:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u32*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0xf0000000) >> 16) | ((color & 0x00f00000) >> 12) | ((color & 0x0000f000) >> 8) | ((color & 0x000000f0) >> 4);
				*(((u16*)buf) + y * texptr->pitch + x) = (u16)(color & 0x0000ffff);
			}
		}
		goto ENDPROCESS;
	case GU_PSM_5551:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u32*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x80000000) >> 16) | ((color & 0x00f80000) >> 9) | ((color & 0x0000f800) >> 6) | ((color & 0x000000f8) >> 3);
				*(((u16*)buf) + y * texptr->pitch + x) = (u16)(color & 0x0000ffff);
			}
		}
		goto ENDPROCESS;
	case GU_PSM_5650:
		for(y = 0;y < texptr->height;++y)
		{
			for(x = 0;x < texptr->pitch;++x)
			{
				u32 color = *(((u32*)texptr->pmemory) + y * texptr->pitch + x);
				color = ((color & 0x00f80000) >> 8) | ((color & 0x0000fc00) >> 5) | ((color & 0x000000f8) >> 3);
				*(((u16*)buf) + y * texptr->pitch + x) = (u16)(color & 0x0000ffff);
			}
		}
		texptr->alphabit = 0;
		goto ENDPROCESS;
	}
ENDPROCESS:
	FREE(texptr->pmemory);
	texptr->pmemory = buf;
	texptr->psm = psm;

	if(swizzleflag)
	{
		SwizzleGraph(gh);
	}
	if(vramflag)
	{
		MoveGraphToVRAM(gh);
	}
	return 0;
}

//#define	DXP_NOUSE_LIBJPEG
#ifndef DXP_NOUSE_LIBJPEG

// (�w�� jdatasrc.c �̗��p)
//#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"

// �i�o�d�f�ǂ݂��݃G���[���������[�`���p�\����
typedef struct my_error_mgr {
	struct jpeg_error_mgr pub;	// �W���G���[�f�[�^

	jmp_buf setjmp_buffer;		// �����O�W�����v�p�\����
} *my_error_ptr ;

// �G���[���ɌĂ΂��֐�
void my_error_exit( j_common_ptr cinfo )
{
	// cinfo->err�������W���G���[�f�[�^�̐擪�A�h���X��my_error_mgr�\���̂̐擪�A�h���X�ɕϊ�
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	// �����܂���悭�킩��܂���A�Ƃ肠�����G���[���b�Z�[�W�W���֐��H
	(*cinfo->err->output_message) (cinfo);
	// ���[�U�[�����߂��G���[�����R�[�h�̈ʒu�܂Ŕ��
	longjmp( myerr->setjmp_buffer, 1 ) ;
}


// �ėp�f�[�^�`���̓]���p�\����
typedef struct
{
	struct jpeg_source_mgr pub;		/* public fields */

	JOCTET *buffer;					/* start of buffer */
	boolean start_of_file;			/* have we gotten any data yet? */

	STREAMDATA *Data ;				// �ėp�f�[�^�`���ǂݍ��ݏ����p�\����
	int DataSize ;					// �f�[�^�̃T�C�Y
} my_source_general_mgr;

typedef my_source_general_mgr	*my_src_general_ptr;

#define INPUT_BUF_SIZE		(4096)	// ��Ɨp�o�b�t�@�̃T�C�Y

// �ǂݍ��ރ\�[�X������������֐�
METHODDEF(void)
init_source_general( j_decompress_ptr cinfo )
{
	my_src_general_ptr src = (my_src_general_ptr) cinfo->src;

	// �t�@�C���ǂݍ��݊J�n�̃t���O�𗧂Ă�
	src->start_of_file = TRUE;
}

// �f�[�^�o�b�t�@�Ƀf�[�^��]������
METHODDEF(boolean)
fill_input_buffer_general (j_decompress_ptr cinfo)
{
	my_src_general_ptr src = (my_src_general_ptr) cinfo->src;
	size_t nbytes;

	// �]������f�[�^�̗ʂ��R�s�[����
	nbytes = ( src->DataSize - STTELL( src->Data ) < INPUT_BUF_SIZE ) ?
			 src->DataSize - STTELL( src->Data ) : INPUT_BUF_SIZE ;
	if( nbytes != 0 )
	{
		STREAD( src->buffer, nbytes, 1, src->Data ) ;
	}

	// �ǂݍ��݂Ɏ��s������G���[
	if( nbytes <= 0 )
	{
		if (src->start_of_file)	/* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);

		/* Insert a fake EOI marker */
	    src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	// ���̑��̏���
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;

	return TRUE;
}

// �w�肳�ꂽ�T�C�Y�̃f�[�^���X�L�b�v���鏈��
METHODDEF(void)
skip_input_data_general( j_decompress_ptr cinfo, long num_bytes)
{
	my_src_general_ptr src = (my_src_general_ptr) cinfo->src;

	// �f�[�^�X�L�b�v����
	if( num_bytes > 0 )
	{
		while( num_bytes > (long) src->pub.bytes_in_buffer )
		{
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer_general( cinfo ) ;
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

// �f�[�^�����Ƃ��̏���
METHODDEF(void)
term_source_general( j_decompress_ptr cinfo )
{
  /* no work necessary here */
}

// �ėp�f�[�^�ǂݍ��ݏ�������f�[�^��ǂݍ��ނ悤�ɂ�����ۂ̐ݒ���s���֐�
GLOBAL(void)
jpeg_general_src (j_decompress_ptr cinfo, STREAMDATA *Data )
{
	my_src_general_ptr src;

	// �܂��i�o�d�f�f�[�^���ꎞ�I�Ɋi�[����o�b�t�@���m�ۂ��Ă��Ȃ�������m�ۂ���
	if (cinfo->src == NULL)
	{
		/* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
						(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
						sizeof(my_source_general_mgr));
		src = (my_src_general_ptr) cinfo->src;
		src->buffer = (JOCTET *)
					(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
					INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	// �֐��|�C���^�Ȃǂ��Z�b�g����
	src = (my_src_general_ptr) cinfo->src;
	src->pub.init_source			= init_source_general ;
	src->pub.fill_input_buffer		= fill_input_buffer_general ;
	src->pub.skip_input_data		= skip_input_data_general ;
	src->pub.resync_to_restart		= jpeg_resync_to_restart ; /* use default method */
	src->pub.term_source			= term_source_general ;

	src->Data = Data ;

	// ���݂̃t�@�C���|�C���^����I�[�܂ł̃T�C�Y���擾����
	{
		long pos ;
		pos = STTELL( src->Data ) ;
		STSEEK( src->Data, 0, STREAM_SEEKTYPE_END ) ;
		src->DataSize = STTELL( src->Data ) - pos ;
		STSEEK( src->Data, pos, STREAM_SEEKTYPE_SET ) ;
	}

	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
}

#ifdef DX_TEST	// �e�X�g�p
extern LONGLONG time2;
#endif




extern int LoadJpegImage(const char *FileName)
{
	STREAMDATA src;
	STREAMDATA *Src = &src;
	if(SetupSTREAMDATA(FileName,Src) == -1)
	{
		return -1;
	}
	struct jpeg_decompress_struct cinfo ;
	struct my_error_mgr jerr ;
	JSAMPARRAY buffer ;
	int InPitch ;
	int i ;

	// �ʏ�i�o�d�f�G���[���[�`���̃Z�b�g�A�b�v
	memset( &cinfo, 0, sizeof( cinfo ) );
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if( setjmp( jerr.setjmp_buffer ) )
	{
		jpeg_destroy_decompress( &cinfo );
		STCLOSE(Src);
		return -1;
	}

#ifdef DX_TEST	// �e�X�g�p
	time2 = ST_GetNowHiPerformanceCount();
#endif
	// cinfo���̃A���P�[�g�Ə��������s��
	jpeg_create_decompress(&cinfo);

	// �ėp�f�[�^�ǂݍ��ݏ�������f�[�^��ǂݍ��ސݒ���s��
	jpeg_general_src( &cinfo, Src ) ;

	// �i�o�d�f�t�@�C���̃p�����[�^���̓ǂ݂���
	(void)jpeg_read_header(&cinfo, TRUE);

	// �i�o�d�f�t�@�C���̉𓀂̊J�n
	(void)jpeg_start_decompress(&cinfo);

	if(cinfo.output_components != 1 && cinfo.out_color_components != 3)
	{
		jpeg_destroy_decompress( &cinfo );
		STCLOSE(Src);
		return -1 ;
	}
	/*�O���t�B�b�N�n���h�������B*/
	int gh = MakeGraph(cinfo.output_width,cinfo.output_height,GU_PSM_8888);
	if(gh == -1)
	{
		//�G���[����
		jpeg_destroy_decompress( &cinfo );
		STCLOSE(Src);
		return -1 ;
	}

	// �P���C��������̃f�[�^�o�C�g�����v�Z
	InPitch = cinfo.output_width * cinfo.output_components ;

	// �f�[�^�o�b�t�@�̊m��
	buffer = (*cinfo.mem->alloc_sarray)
				((j_common_ptr) &cinfo, JPOOL_IMAGE, InPitch, 1 );
	
	// �摜�f�[�^�̓ǂ݂���
	int row = 0;
	while( cinfo.output_scanline < cinfo.output_height && row < texarray[gh].mv)
	{
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		// �f�[�^���o�̓f�[�^�ɕϊ����āA�܂��͂��̂܂ܓ]��
		for(i = 0;i < texarray[gh].mu;++i)
		{
			if(cinfo.output_components == 1)
			{/*�O���[�X�P�[��*/
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 0) = 0xff;
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 1) = *(buffer[0] + i);
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 2) = *(buffer[0] + i);
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 3) = *(buffer[0] + i);
			}
			else
			{/*ABGR*/
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 3) = 0xff;					//A
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 2) = *(buffer[0] + i * 3 + 2);//B
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 1) = *(buffer[0] + i * 3 + 1);//G
				*((unsigned char*)(texarray[gh].pmemory) + texarray[gh].pitch * row * 4 + i * 4 + 0) = *(buffer[0] + i * 3 + 0);//R
			}
		}
		++row;
	}

	// �𓀏����̏I��
	(void) jpeg_finish_decompress(&cinfo);
	// cinfo�\���̂̉��
	jpeg_destroy_decompress(&cinfo);
#ifdef DX_TEST	// �e�X�g�p
	time2 = ST_GetNowHiPerformanceCount() - time2 ;
#endif

	// �I��
//	GraphicNormalize(texi);/*�O���t�B�b�N�𐳋K���i���Ă����́H�j*/
	STCLOSE(Src);
	return gh ;
}


#else
// �i�o�d�f�摜�̓ǂ݂��� PSP�����̃��C�u�������g���o�[�W����
DXPTEXTURE2* LoadJpegImage(const char *FileName)
{
	if(sceJpegInitMJpeg() < 0)return NULL;
	if(sceJpegCreateMJpeg(512,512) < 0)
	{
		sceJpegFinishMJpeg();
		return NULL;
	}
	int size = FileRead_size(FileName);
	u8* rgbabuf = (u8*)malloc(4*512*512);
	u8* databuf = (u8*)malloc(size);
	if(databuf == NULL || rgbabuf == NULL)
	{
		free(databuf);
		free(rgbabuf);
		sceJpegDeleteMJpeg();
		sceJpegFinishMJpeg();
		return NULL;
	}
	int fh = FileRead_open(FileName);
	if(fh == -1)
	{
		free(databuf);
		free(rgbabuf);
		sceJpegDeleteMJpeg();
		sceJpegFinishMJpeg();
		return NULL;
	}
	FileRead_read(databuf,size,fh);
	FileRead_close(fh);
	int res = sceJpegDecodeMJpeg(databuf,size,rgbabuf,0);
	free(databuf);
	sceJpegDeleteMJpeg();
	sceJpegFinishMJpeg();
	if(res < 0)
	{
		free(rgbabuf);
		return NULL;
	}
	int height = res >> 16;
	int width = res & 0x0000ffff;
	DXPTEXTURE2 *texptr = MakeTexture(width,height,DXP_FMT_8888);
	if(texptr == NULL)
	{
		free(rgbabuf);
		return NULL;
	}
//	memcpy(texptr->pmemory,rgbabuf,4 * height * width);//����1�s�Ȃ����݂���̂�����s�\�B�����Y�ꂩ��������ɉe���Ȃ���΍폜�B
	int i;
	for(i = 0;i < height;++i)
		memcpy(((u32*)texptr->pmemory) + texptr->pitch * i,((u32*)rgbabuf) + 512 * i,width * 4);
	free(rgbabuf);
	return texptr;
}
#endif


#ifndef DX_NON_PNGREAD

// �ėp�f�[�^�ǂݍ��ݏ�������̓ǂݍ��݂����邽�߂̃f�[�^�^
typedef struct tagPNGGENERAL
{
	STREAMDATA *Data ;
	unsigned int DataSize ;
} PNGGENERAL ;

// �ėp�f�[�^�ǂݍ��ݏ�������f�[�^��ǂݍ��ރR�[���o�b�N�֐�
static void png_general_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	PNGGENERAL *PGen ;

	PGen = (PNGGENERAL *)CVT_PTR(png_ptr->io_ptr) ;

	// �c��̃T�C�Y������Ȃ�������G���[
	if( PGen->DataSize - STTELL( PGen->Data ) < length )
	{
		png_error(png_ptr, "Read Error");
	}
	else
	{
		STREAD( data, length, 1, PGen->Data ) ;
	}
}

// �ėp�f�[�^�ǂݍ��ݏ�������̓ǂݍ��݂�ݒ肷��֐�
int png_general_read_set( png_structp png_ptr, PNGGENERAL *PGen, STREAMDATA *Data )
{
	PGen->Data = Data ;

	// ���݂̃t�@�C���|�C���^����I�[�܂ł̃T�C�Y���擾����
	{
		long pos ;
		pos = STTELL( PGen->Data ) ;
		STSEEK( PGen->Data, 0, STREAM_SEEKTYPE_END ) ;
		PGen->DataSize = STTELL( PGen->Data ) - pos ;
		STSEEK( PGen->Data, pos, STREAM_SEEKTYPE_SET ) ;
	}

	// �R�[���o�b�N�֐��̃Z�b�g
	png_set_read_fn( png_ptr, PGen, png_general_read_data ) ;

	// �I��
	return 0 ;
}

// �o�m�f�摜�̓ǂ݂���
DXPTEXTURE2* LoadPngImage(const char *FileName)
{
//	BASEIMAGE *Image ;
	STREAMDATA src;
	STREAMDATA *Src = &src;
	if(SetupSTREAMDATA(FileName,Src) == -1)
	{
		STCLOSE(Src);
		return NULL;
	}
	png_bytep *row_pointers;
	unsigned int row, rowbytes ;
	u8 Expand ;
	PNGGENERAL PGen ;
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	int i ;
	png_bytep BufPoint ;

	Expand = 0;

	// �o�m�f�Ǘ����̍쐬
	if( ( png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ) ) == NULL )
	{
		STCLOSE(Src);
		return NULL ;
	}
	// �摜���\���̂̍쐬
	if( ( info_ptr = png_create_info_struct( png_ptr ) ) == NULL ) 
	{
		STCLOSE(Src);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return NULL ;
	}

	// �G���[���̏����Z�b�g�A�b�v����я�����
	if( setjmp( png_jmpbuf( png_ptr ) ) )
	{
		STCLOSE(Src);
		png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
		return NULL ;
	}

	// �ėp�f�[�^�ǂݍ��ݏ�������ǂݍ��ޏꍇ�̐ݒ���s��
	png_general_read_set( png_ptr, &PGen, Src ) ;

	// �ݒ菈���S
	png_set_sig_bytes(		png_ptr, sig_read ) ;												// �悭�킩�疳������(��)
	png_read_info(			png_ptr, info_ptr );												// �摜���𓾂�
	png_get_IHDR(			png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,		// �摜�̊�{�X�e�[�^�X���擾����
							&interlace_type, NULL, NULL ) ;
	png_set_strip_16(		png_ptr ) ;															// �P�U�r�b�g�̉摜�ł��W�r�b�g�Ŏ��[����悤�ɐݒ�
//	if( BmpFlag == TRUE )										png_set_strip_alpha( png_ptr ) ;// �A���t�@�`�����l������
	png_set_packing(		png_ptr ) ;															// �P�o�C�g�ȉ��̃p���b�g�摜���o�C�g�P�ʂœW�J����悤�w��
	png_set_packswap(		png_ptr ) ;															// �悭�킩�疳������

//	if( color_type == PNG_COLOR_TYPE_PALETTE )					png_set_expand( png_ptr ) ;		// �p���b�g�g�p�摜�f�[�^�̎����W�J�w��

	// tRNS(���̃J���[�L�[)�t���摜���W�r�b�g�ȉ��̃O���[�X�P�[���摜�̏ꍇ��
	// �o�͉摜�̃s�N�Z���t�H�[�}�b�g���t���J���[�ɂ���
	//�������A�p���b�g�摜��tRNS�̏ꍇ�̓p���b�g����tRNS������������B
	if( ( color_type == PNG_COLOR_TYPE_GRAY && bit_depth <= 8 ) ||
		(color_type != PNG_COLOR_TYPE_PALETTE && png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) ))
	{
		png_set_expand( png_ptr );		
		Expand = 1 ;
	}
	png_set_gray_to_rgb(png_ptr);
	png_set_add_alpha(png_ptr,0xff,PNG_FILLER_AFTER);			/*�A���t�@��񂪖����ꍇ�͕t��*/
//	png_set_bgr(			png_ptr ) ;															// �J���[�z����q�f�a����a�f�q�ɔ��]
	
	// �ύX�����ݒ�𔽉f������
	png_read_update_info(	png_ptr, info_ptr ) ;

	// �P���C��������ɕK�v�ȃf�[�^�ʂ𓾂�
	rowbytes = png_get_rowbytes( png_ptr, info_ptr ) ;

	// �O���t�B�b�N�f�[�^���i�[���郁�����̈���쐬����
	{
		png_bytep BufP ;

		row_pointers = ( png_bytep * )malloc( height * sizeof( png_bytep * ) ) ;
		if( ( BufPoint = ( png_bytep )png_malloc( png_ptr, rowbytes * height ) ) == NULL )	return NULL ;
		BufP = BufPoint ;
		for (row = 0; row < height; row++, BufP += rowbytes )
			row_pointers[row] = BufP ;
	}

	// �摜�f�[�^�̓ǂݍ���
	png_read_image( png_ptr, row_pointers );


	DXPTEXTURE2 *texptr;
	// �����Z�b�g����
	{
		// �J���[�����Z�b�g����
		if( color_type == PNG_COLOR_TYPE_PALETTE && Expand == 0)
		{
			//�p���b�g�J���[�ł���
			if(bit_depth == 8)
texptr = MakeTexture(width,height,GU_PSM_T8);
			else
texptr = MakeTexture(width,height,GU_PSM_T4);
			if(texptr == NULL)
			{
				//���s����
				free( row_pointers ) ;
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				STCLOSE(Src);
				return NULL;
			}

			//�摜�����ۂɎg����悤�ɂ���
			int bn = 8 / bit_depth;
			for(i = 0;i < texptr->vmax;++i)
				memcpy(((u8*)texptr->pmemory) + texptr->pitch * i / bn,row_pointers[i],texptr->umax / bn);
			png_colorp SrcPalette ;
			int PaletteNum ;
			// �p���b�g���擾
			png_get_PLTE( png_ptr, info_ptr, &SrcPalette, &PaletteNum ) ;

			// �p���b�g�̐����Q�T�U�ȏゾ�����ꍇ�͂Q�T�U�ɕ␳
			if( PaletteNum < 256 ) PaletteNum = 256 ;

			for(i = 0;i < PaletteNum;++i)
			{
				texptr->ppalette->data[i] = 0xff000000 |
					(((u32)SrcPalette[i].blue & 0x000000ff) << 16) |
					(((u32)SrcPalette[i].green & 0x000000ff) << 8) |
					(((u32)SrcPalette[i].red & 0x000000ff) << 0);
			}

			//tRNS���擾
			if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
			{
				png_bytep trans;
				int num_trans;
				png_color_16p trans_values;
				png_get_tRNS(png_ptr,info_ptr,&trans,&num_trans,&trans_values);
				if(num_trans > 256)num_trans = 256;
				for(i = 0;i < num_trans;++i)
				{
					texptr->ppalette->data[i] &= (((u32)trans[i]) << 24) | 0x00ffffff;
				}
			}
		}
		else
		if( color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
		{
			texptr = MakeTexture(width,height,GU_PSM_8888);
			if(texptr == NULL)
			{
				//���s����
				free( row_pointers ) ;
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				STCLOSE(Src);
				return NULL;
			}
			//�摜�����ۂɎg����悤�ɂ���
			int i;
			int bn = 8 / bit_depth;
			for(i = 0;i < texptr->vmax;++i)
				memcpy(((u8*)texptr->pmemory) + texptr->pitch * 4 * i / bn,row_pointers[i],texptr->umax  * 4 / bn);
		}
		else
		{
			//�t���J���[�ł���
			texptr = MakeTexture(width,height,GU_PSM_8888);
			if(texptr == NULL)
			{
				//���s����
				free( row_pointers ) ;
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				STCLOSE(Src);
				return NULL;
			}
			//�摜�����ۂɎg����悤�ɂ���
			int i;
			int bn = 8 / bit_depth;
			for(i = 0;i < texptr->vmax;++i)
				memcpy(((u8*)texptr->pmemory) + texptr->pitch * 4 * i / bn,row_pointers[i],texptr->umax * 4 / bn);
		}
	}

	// �������̉��
	png_free( png_ptr, BufPoint ) ;
	free( row_pointers ) ;

	// �ǂݍ��ݏ����̏I��
	png_read_end(png_ptr, info_ptr);

	// �ǂݍ��ݏ����p�\���̂̔j��
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	STCLOSE(Src);
	// �I��
	sceKernelDcacheWritebackAll();//�S���������񂶂Ⴆ
	texptr->alphabit = 1;
	return texptr;
}
#else
int LoadPngImage(const char *FileName)
{
	return -1;
}

#endif // DX_NON_PNGREAD
