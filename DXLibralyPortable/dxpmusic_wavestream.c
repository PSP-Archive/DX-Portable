#include "dxpmusic2.h"
//
//int preread_wave(DXP_MUSICDECODECONTEXT *context,STREAMDATA *src)
//{
//	if(context == NULL || src == NULL)return -1;
//}
//
int decodeprepare_wave(DXP_MUSICDECODECONTEXT *context)
{
	u8 headerbuf[16];
	int csize;
	STSEEK(context->src,0,SEEK_SET);
	if(STREAD(headerbuf,1,8,context->src) != 8)return -1;
	if(strncmp((char*)headerbuf,"RIFF",4))return -1;
	if(STREAD(headerbuf,1,4,context->src) != 4)return -1;
	if(strncmp((char*)headerbuf,"WAVE",4))return -1;
	u8 fmt = 0;//fmtチャンクを取得したかどうかのフラグ
	context->wavestream.flag = 0;
	while(1)
	{
		if(STREAD(headerbuf,1,8,context->src) != 8)return -1;
		if(!strncmp((char*)headerbuf,"fmt ",4))
		{
			fmt = 1;
			u16 buf2;
			u32 buf4;
			if(STREAD(&buf2,2,1,context->src) != 1)return -1;	//フォーマットタグを取得
			if(buf2 != 1)return -1;		//フォーマットタグが1以外の場合は何かしらの圧縮がなされているので再生できない
			if(STREAD(&buf2,2,1,context->src) != 1)return -1;	//チャンネル数を取得
			if(buf2 == 1 || buf2 == 2)context->wavestream.channel_num = buf2;
			else return -1;
			if(STREAD(&buf4,4,1,context->src) != 1)return -1;	//サンプリングレートを取得
			if(buf4 == 44100 || buf4 == 48000)context->wavestream.samplerate = buf4;
			else return -1;
			STSEEK(context->src,6,SEEK_CUR);
			if(STREAD(&buf2,2,1,context->src) != 1)return -1;	//１サンプル数あたりのビット数を取得
			if(buf2 == 8)context->wavestream.flag |= MUSICFLAG_WAVE_8BIT;
			else if(buf2 != 16)return -1;
			csize = headerbuf[4];
			csize = (csize << 8) | headerbuf[5];
			csize = (csize << 8) | headerbuf[6];
			csize = (csize << 8) | headerbuf[7];
			STSEEK(context->src,csize - 16,SEEK_CUR);
		}
		else if(!strncmp((char*)headerbuf,"data",4))
		{
			context->filetype = DXPMFT_WAVE;
			return fmt - 1;
		}
		else
		{
			csize = headerbuf[4];
			csize = (csize << 8) | headerbuf[5];
			csize = (csize << 8) | headerbuf[6];
			csize = (csize << 8) | headerbuf[7];
			STSEEK(context->src,csize,SEEK_CUR);
		}
	}
	return -1;
}

int decode_wave(DXP_MUSICDECODECONTEXT *context)
{
	if(context == NULL || context->src == NULL)return -1;
//	if(context->nextframe != -1)waveseek(context);
	return 0;
}
//
//int readframe_wave(int handle,u16 *buf,int frame)
//{
//	if(handle < 0 || handle > MUSICHANDLE_MAX)return -1;
//	if(musicarray[handle].src == NULL)return -1;
//	if(frame != -1)waveseek(handle,frame);
//	STREAD(musicarray[handle].playbuf[musicarray[handle].playingbuf],(musicarray[handle].flag & MUSICFLAG_WAVE_8BIT ? 1 : 2) * musicfile_settings[DXPMFT_WAVE].framesize,1,musicarray[handle].src);
//	return 0;
//}
//
//int decodefinish_wave(int handle)
//{
//	if(handle < 0 || handle > MUSICHANDLE_MAX)return -1;
//	if(musicarray[handle].src == NULL)return -1;
//	STCLOSE(musicarray[handle].src);
//	musicarray[handle].src = NULL;
//	return 0;
//}
//
//int waveseek(int handle,int frame)
//{
//	if(handle < 0 || handle > MUSICHANDLE_MAX)return -1;
//	if(musicarray[handle].src == NULL)return -1;
//	STSEEK(musicarray[handle].src,12,SEEK_SET);
//	char tagbuf[4];
//	u32 buf4;
//	while(1)
//	{
//		if(STREAD(tagbuf,1,4,musicarray[handle].src) != 4)return -1;
//		if(STREAD(&buf4,4,1,musicarray[handle].src) != 1)return -1;
//		if(!strncmp(tagbuf,"data",4))break;
//		STSEEK(musicarray[handle].src,buf4,SEEK_CUR);
//	}
//	STSEEK(musicarray[handle].src,(musicarray[handle].flag & MUSICFLAG_WAVE_8BIT ? 1 : 2) * musicfile_settings[DXPMFT_WAVE].framesize * frame,SEEK_CUR);
//	return 0;
//}
