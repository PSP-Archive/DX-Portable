#ifndef DXPMUSIC_H__
#define DXPMUSIC_H__
#include "dxlibp.h"
#include <pspaudiocodec.h>
#include <pspaudio.h>
#include <psputility.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

/*
�f�[�^��ێ�����ꏊ
	�n���h���Ǘ��z��
	�X���b�h��

�f�[�^����
	�t�@�C���X�g���[��
	�t�@�C���^�C�v
	�X���b�hID
	�Đ��`���l��
	�p���A�{�����[��
	�X�g���[�����ʏ킩
	�Đ��o�b�t�@
	�R�[�f�b�N�o�b�t�@


�ʏ�Đ��̗���
	�t�@�C�����擾
	�S����
	�X���b�h����
	�X���b�h���K�v�ȕ������g�p
	�f�[�^�j���ɂ͎Q�ƃJ�E���^���g��

�X�g���[���Đ��̗���
	�t�@�C�����擾
	�X���b�h����
	�X���b�h���K�v�ȕ������𓀂��g�p

�f�[�^�j�����@
	�j���t���O�𗧂Ă�
	�Q�ƃJ�E���^���O�ɂȂ������_�Ń��C���X���b�h���j���������s��

���֐����X�g
LoadSoundMem		//�ʏ�Đ������Ƀt�@�C�����������ɓǂݍ���
LoadStreamSoundMem	//�X�g���[���Đ������Ƀt�@�C�����I�[�v������B
PlaySoundMem		//�Đ��J�n
DeleteSoundMem		//�n���h������������i���߂̃t���O�𗧂Ă�
GetPlayNumSoundMem	//���܂����Đ�����Ă���̂��擾����
CheckSoundMem		//�Đ�����Ă��邩�ǂ����擾����BGetPlayNumSoundMem����[�����ǂ����Ƃ�������

�f�R�[�h�̓f�R�[�h�p�R���e�N�X�g���g���B
�ʏ�Đ��ƃX�g���[���Đ��Ŋ֐��𕪂���B

*/

typedef enum
{
	DXPMFT_WAVE = 0,
	DXPMFT_MP3,

	DXPMFT_MAX
}MUSICFILE_TYPE;



typedef struct
{
	unsigned long *codec_buffer;
}DXP_MUSICUNION_MP3STREAM;

typedef struct
{
	//44.1kHz�O��Ƃ���B����ȊO�͒e���B
	unsigned monoflag:1;
	unsigned s8bitflag:1;
}DXP_MUSICUNION_WAVESTREAM;

#define MUSICHANDLE_MAX	64
#define SOUNDDATA_INDEX MUSICHANDLE_MAX

#define MUSICFLAG_USED		0x0001	//�g�p��


typedef struct
{
	MUSICFILE_TYPE filetype;
	int nextframe;
//	STREAMDATA Src;
	STREAMDATA *src;
	u16 *pcm;
	union
	{
		DXP_MUSICUNION_WAVESTREAM	wavestream;
		DXP_MUSICUNION_MP3STREAM	mp3stream;
	};
	u16 *out;
}DXP_MUSICDECODECONTEXT;



//20090410 �t���O�֘A�̕ύX
/*��
#define DXP_MUSICFLAG_STREAM	0x00000001
#define DXP_MUSICFLAG_PLAYING	0x00000002
#define DXP_MUSICFLAG_LOOP		0x00000004
#define DXP_MUSICCOMMAND_PLAY	0x80000000
#define DXP_MUSICCOMMAND_STOP	0x40000000	//�Ƃ܂�I
#define DXP_MUSICCOMMAND_QUIT	0x20000000
*/
//�V
#define DXP_MUSICFLAG_STREAM			0x00000001
#define DXP_MUSICFLAG_PLAYING			0x00000002
#define DXP_MUSICFLAG_LOOP				0x00000004

#define DXP_MUSICCOMMAND_PLAY			0x80000000
#define DXP_MUSICCOMMAND_STOP			0x40000000	//�Ƃ܂�I
#define DXP_MUSICCOMMAND_QUIT			0x20000000
#define DXP_MUSICCOMMAND_TOP			0x08000000 //20090425add

#define DXP_MUSICTHREADPARAMFLAG_NORMAL	0x00000010
#define DXP_MUSICTHREADPARAMFLAG_BACK	0x00000020
#define DXP_MUSICTHREADPARAMFLAG_LOOP	0x00000040

typedef struct MUSICDATA__	//���y�f�[�^�̃n���h���p�\����
{
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//struct MUSICDATA__ *next;
	unsigned useflg:1;		//0)�� 1)���蓖�čς�
	int apos,bpos;	//���s�[�g�ʒu�BA-B���s�[�g�ɑΉ�������B
	u32 flag;		//�ʏ�/�X�g���[�� ���s�[�g �Đ�/��~
	int handle;
	int count;		//�Q�ƃJ�E���^
	u16 *pcm;		//�ʏ�X�g���[���̏ꍇ�̂ݔ�NULL;
	int pcmlen;		//���T���v������̂�
	u8 volume[4];	//�{�����[�� �S�́A���A�E�̏� �ő�100
	STREAMDATA Src;
}MUSICDATA;

//20090410
//��̏C�������Ɉړ�
//#define DXP_MUSICTHREADPARAMFLAG_LOOP	0x01
typedef struct
{
	MUSICDATA *ptr;
	u8 flag;
}MUSICDATA_THREADPARAM;

typedef struct
{
	MUSICDATA *md;
	u8 flag;
	DXP_MUSICDECODECONTEXT context;
}MUSICDATA_THREADPARAM_STREAM;

int getfreehandle();//�󂫃n���h���̒T��
int reserveoutchannel(int handle);//�󂫏o�̓`�����l���̎擾
int releaseoutchannel(int handle);
int musicthread(SceSize arglen,void* argp);
int id3skip(STREAMDATA *src);

int waveseek(DXP_MUSICDECODECONTEXT *context);
int decodeprepare_wave(DXP_MUSICDECODECONTEXT *context);
int decode_wave(DXP_MUSICDECODECONTEXT *context);
int decodefinish_wave(DXP_MUSICDECODECONTEXT *context);

int mp3seek(STREAMDATA *src,int frame);
int mp3len(STREAMDATA *src);
int mp3framesize(int header);
int decodeprepare_mp3(DXP_MUSICDECODECONTEXT *context);
int decode_mp3(DXP_MUSICDECODECONTEXT *context);
int decodefinish_mp3(DXP_MUSICDECODECONTEXT *context);


int readframe(DXP_MUSICDECODECONTEXT *context);

inline int sample_per_frame(MUSICFILE_TYPE type);

#endif
