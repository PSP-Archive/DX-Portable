//#include <intrafont.h>


#include <valloc.h>
#include <malloc.h>
#include <string.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include"../graphics.h"
u32 __attribute__((aligned(16))) dxpGuList[GULIST_LEN];

u8 dxpPsm2SliceSize[11][2]
= {
	{64,64},//16
	{64,64},
	{64,64},
	{64,32},//32
	{128,128},//4
	{128,64},//8
	{64,64},//16
	{64,32},//32
	{64,32},//32
	{64,32},//32
	{64,32},//32
};

DXPGRAPHICSDATA dxpGraphicsData = 
{
	0,
	GU_PSM_8888,
	{},
	NULL,
	{},
	NULL,
	NULL,

	NULL,

	0,
	0,
	0,

	0,
	0,
	0,
	0,
	1,
	1,
	0,
	0,
	1,

	0,
	DX_BLENDMODE_NOBLEND,
	DXP_DRAWSTATE_EITHER,
	0,
	1,
	0,
	0xffffffff,

};

int dxpGraphicsInit()
{
	u32 i;
	//構造体初期化
	if(dxpGraphicsData.init)return 0;

	{//深度バッファの初期化
		dxpGraphicsData.depthbuffer.psm = GU_PSM_4444;
		dxpGraphicsData.depthbuffer.ppalette = NULL;
		dxpGraphicsData.depthbuffer.texdata = NULL;
		dxpGraphicsData.depthbuffer.width = 480;
		dxpGraphicsData.depthbuffer.height = 272;
		dxpGraphicsData.depthbuffer.pitch = 512;
		dxpGraphicsData.depthbuffer.umax = 480;
		dxpGraphicsData.depthbuffer.vmax = 272;
		dxpGraphicsData.depthbuffer.colorkey = 0;
		dxpGraphicsData.depthbuffer.size2_nflag = 0;
		dxpGraphicsData.depthbuffer.swizzledflag = 1;
		dxpGraphicsData.depthbuffer.reloadflag = 1;
		dxpGraphicsData.depthbuffer.alphabit = 0;
		dxpGraphicsData.depthbuffer.refcount = 0;
		dxpGraphicsData.depthbuffer.thisptrptr = NULL;
		if(dxpGraphicsData.usedepth || dxpGraphicsData.usedepth3d)
		{
			dxpGraphicsData.depthbuffer.texvram = valloc(vgetMemorySize(512,272,GU_PSM_4444));
			if(!dxpGraphicsData.depthbuffer.texvram)return -1;
		}else dxpGraphicsData.depthbuffer.texvram = NULL;
	}

	for(i = 0;i < 2;++i)
	{//フレームバッファの初期化
		dxpGraphicsData.displaybuffer[i].psm = dxpGraphicsData.display_psm;
		dxpGraphicsData.displaybuffer[i].ppalette = NULL;
		dxpGraphicsData.displaybuffer[i].texdata = NULL;
		dxpGraphicsData.displaybuffer[i].texvram = valloc(vgetMemorySize(512,272,dxpGraphicsData.display_psm));
		dxpGraphicsData.displaybuffer[i].width = 480;
		dxpGraphicsData.displaybuffer[i].height = 272;
		dxpGraphicsData.displaybuffer[i].pitch = 512;
//		dxpGraphicsData.displaybuffer[i].pitch = 480;//試したら見事に駄目だったｗ
		dxpGraphicsData.displaybuffer[i].umax = 480;
		dxpGraphicsData.displaybuffer[i].vmax = 272;
		dxpGraphicsData.displaybuffer[i].colorkey = 0;
		dxpGraphicsData.displaybuffer[i].size2_nflag = 0;
		dxpGraphicsData.displaybuffer[i].swizzledflag = 0;
		dxpGraphicsData.displaybuffer[i].reloadflag = 1;
		dxpGraphicsData.displaybuffer[i].alphabit = 0;
		dxpGraphicsData.displaybuffer[i].refcount = 0;
	}
	if(!dxpGraphicsData.displaybuffer[0].texvram || !dxpGraphicsData.displaybuffer[1].texvram)
	{
		vfree(dxpGraphicsData.displaybuffer[0].texvram);
		vfree(dxpGraphicsData.displaybuffer[1].texvram);
		vfree(dxpGraphicsData.depthbuffer.texvram);
		return -1;
	}
	dxpGraphicsData.displaybuffer_back = &dxpGraphicsData.displaybuffer[0];
	dxpGraphicsData.rendertarget = dxpGraphicsData.displaybuffer_back;
	dxpGraphicsData.texture = NULL;
	dxpGraphicsData.clear_color_value = 0;
	dxpGraphicsData.clear_depth_value = 0;
	dxpGraphicsData.clear_stencil_value = 0;
	dxpGraphicsData.gustarted = 0;
	dxpGraphicsData.clear_depth = 0;
	dxpGraphicsData.clear_stencil = 0;
	dxpGraphicsData.bilinear_enable = 0;
	dxpGraphicsData.create_vram_graph = 1;
	dxpGraphicsData.create_swizzled_graph = 1;
	dxpGraphicsData.colorkey = 0;

	dxpGraphicsData.blendmode = DX_BLENDMODE_NOBLEND;
	dxpGraphicsData.drawstate = DXP_DRAWSTATE_EITHER;
	dxpGraphicsData.flag = 0xffffffff;
	dxpGraphicsData.forceupdate = 1;
	dxpGraphicsData.z_2d = 0;
	dxpGraphicsData.color = 0xffffffff;
	//dxpGraphicsData.usedepth = 0;
	//dxpGraphicsData.usedepth3d = 0;
	dxpGraphicsData.writedepth = 0;
	dxpGraphicsData.writedepth3d = 0;

	dxpGraphicsData.fog_color = 0xffffffff;
	dxpGraphicsData.fog_near = 10.0f;
	dxpGraphicsData.fog_far = 100.0f;

	dxpGraphicsData.intrafont_scissor[0] = 0;
	dxpGraphicsData.intrafont_scissor[1] = 0;
	dxpGraphicsData.intrafont_scissor[2] = 480;
	dxpGraphicsData.intrafont_scissor[3] = 272;

	for(i = 0;i < DXP_BUILDOPTION_TEXTURE_MAXNUM;++i)dxpGraphicsData.texarray[i] = NULL;
	for(i = 0;i < DXP_BUILDOPTION_GHANDLE_MAXNUM;++i)dxpGraphicsData.grapharray[i] = NULL;
	//GEのセットアップ
	sceGuInit();
	GUSTART
		
	sceGuDrawBuffer(dxpGraphicsData.display_psm,vGuPointer(dxpGraphicsData.displaybuffer[0].texvram),dxpGraphicsData.displaybuffer[0].pitch);
	sceGuDispBuffer(480,272,vGuPointer(dxpGraphicsData.displaybuffer[1].texvram),dxpGraphicsData.displaybuffer[1].pitch);

	sceGuOffset(2048 - (480/2),2048 - (272/2));
	sceGuViewport(2048,2048,480,272);
	sceGuScissor(
		dxpGraphicsData.intrafont_scissor[0],
		dxpGraphicsData.intrafont_scissor[1],
		dxpGraphicsData.intrafont_scissor[2],
		dxpGraphicsData.intrafont_scissor[3]
	);

	sceGuDepthRange(65535,0);
	sceGuDepthFunc(GU_LESS);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_FLAT);
	sceGuTexScale(1.0f,1.0f);
	sceGuTexOffset(0.0f,0.0f);
	sceGuTexFilter(GU_NEAREST,GU_NEAREST);
	GUFINISH;
	sceDisplayWaitVblankStart();
	GUSTART;
	sceGuDisplay(GU_TRUE);
	dxpGraphicsData.init = 1;
	return 0;
}



int dxpGraphicsEnd()
{
	if(!dxpGraphicsData.init)return 0;
	GUSYNC;
	sceGuTerm();
	vfree(dxpGraphicsData.displaybuffer[0].texvram);
	vfree(dxpGraphicsData.displaybuffer[1].texvram);
	vfree(dxpGraphicsData.depthbuffer.texvram);
	vfree(dxpGraphicsData.depthbuffer.texvram);
	InitGraph();
	return 0;
}

int DeleteGraph(int gh)
{
	if(gh < 0 || gh >= DXP_BUILDOPTION_GHANDLE_MAXNUM)return -1;
	dxpGraphicsData.forceupdate = 1;
	return dxpGraphicsReleseGraphicHandle(dxpGraphicsData.grapharray[gh]);
}

int InitGraph()
{
	int i;
	GUINITCHECK;
	for(i = 0;i < DXP_BUILDOPTION_GHANDLE_MAXNUM;++i)
		if(dxpGraphicsData.grapharray[i] != NULL)
			dxpGraphicsReleseGraphicHandle(dxpGraphicsData.grapharray[i]);
	dxpGraphicsData.forceupdate = 1;
	return 0;
}



int dxpGraphicsSetup2DTex(DXPTEXTURE3 *texptr,int flag)
{
	if(!texptr)return -1;
	//設定が同じなら変更処理をしない
	if(dxpGraphicsData.drawstate == DXP_DRAWSTATE_TEX2D
		&& flag == dxpGraphicsData.flag
		&& texptr == dxpGraphicsData.texture
		&& !texptr->reloadflag
		&& !dxpGraphicsData.forceupdate
		)return 0;
	GUSTART;
	if(dxpGraphicsData.drawstate != DXP_DRAWSTATE_TEX2D)
	{//2Dテクスチャ用の描画設定を行う。
		dxpGraphicsData.drawstate = DXP_DRAWSTATE_TEX2D;
		dxpGraphicsData.forceupdate = 1;
		GUENABLE(GU_TEXTURE_2D);
		if(dxpGraphicsData.usedepth)
		{
			GUENABLE(GU_DEPTH_TEST);
			sceGuDepthBuffer(dxpGraphicsData.depthbuffer.texvram,512);	//深度バッファを有効にするときでいい
			if(dxpGraphicsData.writedepth)sceGuDepthMask(0xffff);
			else sceGuDepthMask(0);
		}
		else GUDISABLE(GU_DEPTH_TEST);
	}
	//テクスチャの設定開始
	if(texptr != dxpGraphicsData.texture || texptr->reloadflag)
	{//GUにテクスチャの指定をする
		if(texptr->ppalette && (texptr->psm == GU_PSM_T8 || texptr->psm == GU_PSM_T4))
		{//パレットの設定
			sceGuClutMode(GU_PSM_8888,0,0xff,0);
			sceGuClutLoad(texptr->psm == GU_PSM_T4 ? 2 : 32,texptr->ppalette);
		}
		sceGuTexMode(texptr->psm,0,0,texptr->swizzledflag);
		sceGuTexImage(0,texptr->width,texptr->height,texptr->pitch,texptr->texvram ? texptr->texvram : texptr->texdata);
		dxpGraphicsData.texture = texptr;
		if(texptr != &dxpGraphicsData.displaybuffer[0] && texptr != &dxpGraphicsData.displaybuffer[1])texptr->reloadflag = 0;
	}

	u8 colorKey = 0,alphaEnable = 0;
	
	if(flag & 1)
	{
		if(texptr->alphabit)
			alphaEnable = 1;
		else colorKey = 1;
	}
	if(colorKey)
	{
		GUENABLE(GU_COLOR_TEST);
		sceGuColorFunc(GU_NOTEQUAL,texptr->colorkey,0x00fefefe);
	}
	else
	{
		GUDISABLE(GU_COLOR_TEST);
	}
	int op;
	int src,dest;
	unsigned int srcfix;
	unsigned int destfix;	 
	switch(dxpGraphicsData.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
		op = GU_ADD;
		src = GU_FIX;
		dest = GU_FIX;
		srcfix = 0xffffffff;
		destfix = 0;
		if(!alphaEnable)break;
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

	if(dxpGraphicsData.blendmode == DX_BLENDMODE_NOBLEND && !alphaEnable)
	{
		GUDISABLE(GU_BLEND);
	}
	else
	{
		GUENABLE(GU_BLEND);
		if(dxpGraphicsData.forceupdate
			|| dxpGraphicsData.op != op
			|| dxpGraphicsData.src != src
			|| dxpGraphicsData.dest != dest
			|| dxpGraphicsData.srcfix != srcfix
			|| dxpGraphicsData.destfix != destfix
		){
			sceGuBlendFunc(op,src,dest,srcfix,destfix);
			dxpGraphicsData.op = op;
			dxpGraphicsData.src = src;
			dxpGraphicsData.dest = dest;
			dxpGraphicsData.srcfix = srcfix;
			dxpGraphicsData.destfix = destfix;
		}
	}

	if(dxpGraphicsData.forceupdate || dxpGraphicsData.color != dxpGraphicsData.gucolor)
	{//色を設定
		sceGuColor(dxpGraphicsData.color);
		dxpGraphicsData.gucolor = dxpGraphicsData.color;
	}

	int tfx,tcc;
	switch(dxpGraphicsData.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
	case DX_BLENDMODE_MUL:
	case DX_BLENDMODE_DESTCOLOR:
		tcc = GU_TCC_RGB;
		tfx = GU_TFX_MODULATE;
		if(alphaEnable)tcc = GU_TCC_RGBA;
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
	if(dxpGraphicsData.forceupdate || dxpGraphicsData.tfx != tfx || dxpGraphicsData.tcc != tcc)
	{
		sceGuTexFunc(tfx,tcc);
		dxpGraphicsData.tfx = tfx;
		dxpGraphicsData.tcc = tcc;
	}
	dxpGraphicsData.forceupdate = 0;
	dxpGraphicsData.flag = flag;
	return 0;
}

int dxpGraphicsSetup2D(u32 color)
{
	GUSTART;
	if(dxpGraphicsData.drawstate != DXP_DRAWSTATE_NONTEX2D)
	{
		GUDISABLE(GU_TEXTURE_2D);
		dxpGraphicsData.texture = NULL;
		dxpGraphicsData.drawstate = DXP_DRAWSTATE_NONTEX2D;
		dxpGraphicsData.forceupdate = 1;
	}

	register u32 r,g,b,a,t;
	r = color & 0x000000ff;
	t = dxpGraphicsData.color & 0x000000ff;
	r *= t;
	r /= 255;
	g = (color & 0x0000ff00) >> 8;
	t = (dxpGraphicsData.color & 0x0000ff00) >> 8;
	g *= t;
	g /= 255;
	b = (color & 0x00ff0000) >> 16;
	t = (dxpGraphicsData.color & 0x00ff0000) >> 16;
	b *= t;
	b /= 255;
	a = (color & 0xff000000) >> 24;
	t = (dxpGraphicsData.color & 0xff000000) >> 24;
	a *= t;
	a /= 255;
	color = (a << 24) | (b << 16) | (g << 8) | r;

	int op;
	int src,dest;
	unsigned int srcfix;
	unsigned int destfix;	 

	switch(dxpGraphicsData.blendmode)
	{
	case DX_BLENDMODE_NOBLEND:
		GUDISABLE(GU_BLEND);
		sceGuColor(color);
		dxpGraphicsData.forceupdate = 0;
		return 0;
	case DX_BLENDMODE_INVSRC:
		op = GU_ADD;
		src = GU_SRC_ALPHA;
		dest = GU_ONE_MINUS_SRC_ALPHA;
		srcfix = 0;
		destfix = 0;
		color = (color & 0xff000000) | ~(color & 0x00ffffff);
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
	default:
		return -1;
	}
	sceGuColor(color);

	GUENABLE(GU_BLEND);
	if(dxpGraphicsData.forceupdate
		|| dxpGraphicsData.op != op
		|| dxpGraphicsData.src != src
		|| dxpGraphicsData.dest != dest
		|| dxpGraphicsData.srcfix != srcfix
		|| dxpGraphicsData.destfix != destfix
	){
		sceGuBlendFunc(op,src,dest,srcfix,destfix);
		dxpGraphicsData.op = op;
		dxpGraphicsData.src = src;
		dxpGraphicsData.dest = dest;
		dxpGraphicsData.srcfix = srcfix;
		dxpGraphicsData.destfix = destfix;
	}

	dxpGraphicsData.forceupdate = 0;
	return 0;
}




int GetColor(int Red,int Green,int Blue)
{
	return 0xff000000 | ((Blue & 0xff) << 16) | ((Green & 0xff) << 8) | (Red & 0xff);
}


void *GetFramebufferAddress()
{
	return dxpGraphicsData.displaybuffer_back->texvram;
}

void *GetDisplaybufferAddress()
{
	return dxpGraphicsData.displaybuffer_back == dxpGraphicsData.displaybuffer ? dxpGraphicsData.displaybuffer[1].texvram : dxpGraphicsData.displaybuffer[0].texvram;
}


void* dxpGuGetMemory(u32 size)
{
	int p = sceGuCheckList();
	if(p > GULIST_LEN){return NULL;}//致命的なエラー。バッファオーバーランが発生している
	if(p > GULIST_LIM){GUSYNC;GUSTART;}//コマンドリストが残り少ないので一度同期をとる
	return sceGuGetMemory(size);
}

void dxpGraphicsWaitVSync()
{
	if(!dxpGraphicsData.waitvsinc)return;
	sceDisplayWaitVblank();
}


