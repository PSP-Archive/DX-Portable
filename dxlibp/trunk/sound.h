#include "dxlibp.h"
#include <pspmp3.h>
#include <psputility.h>

#define DXP_BUILDOPTION_SOUNDHANDLE_MAX 32

#define DXP_SOUNDCMD_NONE 0
#define DXP_SOUNDCMD_STOP 1
#define DXP_SOUNDCMD_EXIT 3

#define DXP_SOUNDFMT_MP3 1

#define SHND2PTR(HNDLE,PTR) {if(!dxpSoundData.init)return -1; if(HNDLE < 0 || HNDLE >= DXP_BUILDOPTION_SOUNDHANDLE_MAX)return -1;PTR = dxpSoundArray + HNDLE;if(!PTR->used)return -1;}


typedef struct DXPAVCODEC_BUFFER
{
	u32 reserved0[6];
	u8* datIn;
	u32 frameSize0;
	u32* pcmOut;
	u32 decodeByte;//set 4608 (= 1152[sample per frame] * 2[byte per sample] * 2[channel])
	u32 frameSize1;
	u32 reserved1[54];
}DXPAVCODEC_BUFFER;

typedef struct DXPAVCONTEXT_MP3
{
	DXPAVCODEC_BUFFER *avBuf;
	int id3v1Pos;
	int id3v2Pos;
	u32 blockId;//�������m�ۂɎg���B
	u8 *mp3Buf;
	u32 mp3BufSize;
}DXPAVCONTEXT_MP3;

typedef struct DXPAVCONTEXT
{
	int fileHandle;//U�t�@�C���n���h��
	int fileSize;//U�t�@�C���T�C�Y

	u32 *pcmOut;//U�o�͐�

	int sampleRate;//D�T���v�����[�g
	int nextPos;//D�f�R�[�_�����Ƀf�R�[�h����T���v���ʒu
	int outSampleNum;//D�f�R�[�_���K�v�Ƃ���o�͐�o�b�t�@�T�C�Y�i�T���v�����j
	u8 format;//D�t�H�[�}�b�g
	union
	{
		DXPAVCONTEXT_MP3 mp3;
	};
}DXPAVCONTEXT;

typedef struct DXPSOUNDHANDLE
{
	//�n���h���X�e�[�^�X
	unsigned used : 1;
	int soundDataType;
	//���[�U�[����w�肷����
	unsigned cmd : 2;
	int loopResumePos;
	u8 volume;
	int pan;

	union
	{
		struct{
			int gotoPos;
		}file;
		struct{
			int length;
			u32 *pcmBuf;
		}memnopress;
	};


	DXPAVCONTEXT avContext;
}DXPSOUNDHANDLE;

typedef struct DXPSOUNDDATA
{
	u8 init;
	u8 createSoundDataType;
}DXPSOUNDDATA;

typedef struct DXPSOUNDTHREAD
{
	unsigned used : 1;
	unsigned running : 1;
	unsigned loop : 1;
	int threadId;
	DXPSOUNDHANDLE *pHnd;
}DXPSOUNDTHREAD;

extern DXPSOUNDHANDLE dxpSoundArray[];
extern DXPSOUNDTHREAD dxpSoundThreads[];
extern DXPSOUNDDATA dxpSoundData;

int dxpSoundInit();
int dxpSoundTerm();
int dxpSoundReserveHandle();
int dxpSoundReleaseHandle(int handle);
int dxpSoundReserveThread();
int dxpSoundReleaseThread(int handle);

int dxpSoundMp3Init(DXPAVCONTEXT *av);
int dxpSoundMp3GetSampleLength(DXPAVCONTEXT *av);
int dxpSoundMp3Seek(DXPAVCONTEXT *av,int sample);
int dxpSoundMp3Decode(DXPAVCONTEXT *av);
int dxpSoundMp3End(DXPAVCONTEXT *av);

int dxpSoundCodecInit(DXPSOUNDHANDLE *pHnd);
int dxpSoundCodecGetSampleLength(DXPSOUNDHANDLE *pHnd);
int dxpSoundCodecSeek(DXPSOUNDHANDLE *pHnd,int sample);
int dxpSoundCodecDecode(DXPSOUNDHANDLE *pHnd);
int dxpSoundCodecEnd(DXPSOUNDHANDLE *pHnd);

int dxpSoundThreadFunc(SceSize size,void* argp);