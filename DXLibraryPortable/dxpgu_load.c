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
	DXPGRAPHDATA *gptr = GenerateGraphHandle();
	if(gptr == NULL)return -1;
	gptr->tex = LoadPngImage(FileName);
	if(gptr->tex == NULL)gptr->tex = LoadJpegImage(FileName);
	if(gptr->tex == NULL)
	{
		GraphHandleFree(gptr->handle);
		return -1;
	}
	++gptr->tex->refcount;
	gptr->u0 = 0;
	gptr->v0 = 0;
	gptr->u1 = gptr->tex->umax;
	gptr->v1 = gptr->tex->vmax;
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
		SwizzleGraph(gptr->handle);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
		MoveGraphToVRAM(gptr->handle);
	return gptr->handle;
}

int DerivationGraph( int SrcX, int SrcY,int Width, int Height, int src )
{
	if(SrcX < 0 || SrcY < 0)return -1;
	DXPGRAPHDATA *texptr = GraphHandle2Ptr(src);
	if(texptr == NULL)return -1;
	if(texptr->tex == NULL)return -1;
	if(SrcX + Width > texptr->tex->umax || SrcY + Height > texptr->tex->vmax)return -1;	
	DXPGRAPHDATA *res = GenerateGraphHandle();
	if(res == NULL)return -1;
	res->u0 = SrcX;
	res->v0 = SrcY;
	res->u1 = SrcX + Width;
	res->v1 = SrcY + Height;
	res->tex = texptr->tex;
	++res->tex->refcount;
	return res->handle;
}
//#define	CODE090405hvhkrhIMnI
#ifdef	CODE090405hvhkrhIMnI
//�V���[�e�B���O��D�����̃R�[�h�B��K�͎d�l�ύX�̂��ߎg�p�s�\�ɂȂ�܂����B
u16	TextureGetColor16(DXPTEXTURE *texptr,int Sx,int Sy)
{
	if(texptr == NULL)return 0;
	u16* TEXRAM = texptr->vramflag ? sceGeEdramGetAddr() + texptr->pvram->flags : texptr->pmemory;
	return TEXRAM[texptr->pitch * Sy + Sx];
}

//32bit�A���t�@�L��t�H�[�}�b�g�Œ�
u32	TextureGetColor32(DXPTEXTURE *texptr,int Sx,int Sy)
{
	if(texptr == NULL)return 0;
	u32* TEXRAM = texptr->vramflag ? sceGeEdramGetAddr() + texptr->pvram->flags : texptr->pmemory;
	return TEXRAM[texptr->pitch * Sy + Sx];
}

//�w�肵���e�N�X�`���̔C�Ӎ��W�֎w�肵���F�����Z�b�g
void	TextureSetColor16(DXPTEXTURE *texptr,int Sx,int Sy,u16 color)
{
	if(texptr == NULL)return 0;
	u16* TEXRAM = texptr->vramflag ? sceGeEdramGetAddr() + texptr->pvram->flags : texptr->pmemory;
	TEXRAM[texptr->pitch * Sy + Sx] = color;
}
//32bit�A���t�@�L��t�H�[�}�b�g�Œ�
void	TextureSetColor32(DXPTEXTURE *texptr,int Sx,int Sy,u32 color)
{
	if(texptr == NULL)return 0;
	u32* TEXRAM = texptr->vramflag ? sceGeEdramGetAddr() + texptr->pvram->flags : texptr->pmemory;
	TEXRAM[texptr->pitch * Sy + Sx] = color;
}

//�͈̓R�s�[�p Swizzle����Ă��Ȃ����ƑO��
int	TextureCopy16(int TextureNo1,int TextureNo2,int No1Sx,int No1Sy,int No2Sx,int No2Sy, int width, int height)
{
	//if (texarray[TextureNo1].flags != TEXTUREFLAGS_XXXX ) return -1;
	//if (texarray[TextureNo2].flags != TEXTUREFLAGS_XXXX ) return -2;
	DXPTEXTURE *texptr[2];
	texptr[0] = GraphHandle2Ptr(TextureNo1);
	texptr[1] = GraphHandle2Ptr(TextureNo2);
	if(texptr[0] == NULL)return -1;
	if(texptr[1] == NULL)return -1;
	int i;
	int j;
	for(i=0;i<height;i++) {
		for(j=0;j<width;j++) {
			TextureSetColor16(texptr[0],No1Sx + j,No1Sy + i,TextureGetColor16(texptr[1],No2Sx + j,No2Sy + i));
		}
	}
	return 0;
}
int	TextureCopy32(int TextureNo1,int TextureNo2,int No1Sx,int No1Sy,int No2Sx,int No2Sy, int width, int height)
{
	//if (texarray[TextureNo1].flags != TEXTUREFLAGS_XXXX ) return -1;
	//if (texarray[TextureNo2].flags != TEXTUREFLAGS_XXXX ) return -2;
	DXPTEXTURE *texptr[2];
	texptr[0] = GraphHandle2Ptr(TextureNo1);
	texptr[1] = GraphHandle2Ptr(TextureNo2);
	if(texptr[0] == NULL)return -1;
	if(texptr[1] == NULL)return -1;
	int i;
	int j;
	for(i=0;i<height;i++) {
		for(j=0;j<width;j++) {
			TextureSetColor32(texptr[0],No1Sx + j,No1Sy + i,TextureGetColor32(texptr[1],No2Sx + j,No2Sy + i));
		}
	}
	return 0;
}

int LoadDivGraph(const char *FileName , int AllNum , int XNum , int YNum , int XSize , int YSize , int *HandleBuf )
{
	int res = -1;

	if(res == -1)res = LoadPngImage(FileName);
	if(res == -1)res = LoadJpegImage(FileName);
	//PSP�̃������̏��Ȃ�����l����Ɛ��Div��̗̈�iXSize��YSize���J���[�t�H�[�}�b�g��AllNum�j
	//���m�ۂ����ق����������ɂȂ�ɂ���
	//�ł��Ƃ肠�����͓ǂݍ��܂Ȃ��Ɖ摜�̃t�H�[�}�b�g���킩��Ȃ�
	//�Ë��ĂƂ��Ă�
	//�i���x���������j
	//LoadPngImage��������LoadJpegImage���[�h��������
	//Div��̗̈���m�ۂ���Div�����㌳�̉摜��j��
	//�i���x���x�����j
	//LoadPngImage��������LoadJpegImage���[�h��������
	//�J���[�t�H�[�}�b�g�����擾�������U�摜��j��
	//Div��̗̈���m�ۂ��čēx�摜��ǂݍ���div���Ă���j��

	if(res == -1)return -1;
	//int Wpsm = texarray[res].psm;
	int i;
	int j;
	//for(i=0;i<AllNum;i++)
	//{
	//	HandleBuf[i] = MakeGraph(XSize,YSize,GU_PSM_8888);
	//���̊֐��łׂ���Ɋm�ۂ��Ă�悤�Ȃ̂ŏc���T�C�Y�����̂܂ܓn���Ă�
	//}
	//���S��͂Ƃ肠�����ȗ��i���������Ȃ�������E�E�E
	//�܂�PSP���PC�p�f�ނ����̂܂܎g���Ƃ����̂�������������
	int k;
	int wres;
	for(i=0;i<YNum;i++)
	{
		for(j=0;j<XNum;j++)
		{
			k = i*XNum + j;
			wres = MakeGraph(XSize,YSize,GU_PSM_8888);
			TextureCopy32(wres,res,0,0,j*XSize,i*YSize,XSize,YSize);
			if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
				SwizzleGraph(wres);
			if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
				MoveGraphToVRAM(wres);
			HandleBuf[k] = wres;
		}
	}
	DeleteGraph(res);
	sceKernelDcacheWritebackAll();
	return res == -1 ? -1 : 0;
}

#else
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
		DXPGRAPHDATA *gptr = GenerateGraphHandle();
		if(gptr == NULL)break;
		gptr->tex = tex;
		++tex->refcount;
		gptr->u0 = x;
		gptr->v0 = y;
		gptr->u1 = x + XSize;
		gptr->v1 = y + YSize;
		HandleBuf[i] = gptr->handle;
	}
	if(i == 0)
	{
		TextureList_Remove(tex);
		FREE(tex->pmemory);
		FREE(tex->ppalette);
		FreeVRAM(tex->pvram);
		FREE(tex);
		return -1;
	}
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATESWIZZLEDGRAPH)
		SwizzleGraph(HandleBuf[0]);
	if(gusettings.flags[0] & GPUSETTINGFLAGS_0_CREATEVRAMGRAPH)
		MoveGraphToVRAM(HandleBuf[0]);
	return 0;
}
#endif




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
