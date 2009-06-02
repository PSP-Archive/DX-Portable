/*
*	DX���C�u����Portable	���y�Đ����䕔	Ver1.0.1
*	�����	�F�J���A�V���[�e�B���O��D��
*	���l	�F
*/

/*
�f�[�^�ǂݍ��݁@���s������ăI�[�v�������݂�
�f�R�[�h

//���m�����̏ꍇ�ł����Ȃ炸�X�e���I�ŏo�͂���B


�f�R�[�h�n�֐����������ł�������B
�Đ��X���b�h�Ƃ����쐬���B


�X�g���[���̃I�[�v��
�Đ��O�̒���

�Đ��ʒu�m�F�A�C��
�f�R�[�h
�Đ�
���[�v

�I���O����
�X�g���[���̃N���[�Y



�ʏ�Đ�
	�����Đ��\

�X�g���[���Đ�
	�����Đ��s�\
	�ȃ�����

�Đ��R���e�L�X�g���g�����@�ɐ؂�ւ���B
�S�̂̃{�����[�������́E�E�E�{�̂̋@�\�ł�����

�������DX���C�u��������d�l������邯�ǁc�������@DX���C�u�����̂͋@�\�ǉ��̌J��Ԃ��ł킯�킩��Ȃ���




���b�p�[�֐�
PlaySoundFile
CheckSoundFile		
StopSoundFile
SetVolumeSoundFile

*/

//�C���N���[�h

#include "dxpmusic2.h"
#include "dxpstatic.h"

//�錾

int musicthread_normal(SceSize arglen,void* argp);
int musicthread_stream(SceSize arglen,void *argp);
int decodeprepare(DXP_MUSICDECODECONTEXT *context);
int decode(DXP_MUSICDECODECONTEXT *context);
int decodefinish(DXP_MUSICDECODECONTEXT *context);
//�ϐ���`

MUSICDATA *musicdataroot = NULL;

//20090410
//���t�@�C���Ǘ��̈�̕ύX�ɔ����ǉ�
#define MusicTableMAX 32
//20090412
//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
//�m�ۂ��Ȃ������ɕύX
//static MUSICDATA* MusicTable[MusicTableMAX];
static MUSICDATA MusicTable[MusicTableMAX];

//�֐���`


MUSICDATA* Handle2MusicDataPtr(int handle)
{
	if ( handle <				0 ) return NULL;
	if ( handle >=	MusicTableMAX ) return NULL;
	//20090410
	//���t�@�C���Ǘ��p��̕ύX
	/*MUSICDATA *ptr = musicdataroot;
	while(ptr != NULL)
	{
		if(ptr->handle == handle)return ptr;
		ptr = ptr->next;
	}
	return ptr;*/
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//return MusicTable[handle];
	return &MusicTable[handle];
}

MUSICDATA* AddMusicData()
{
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//MUSICDATA *ptr = (MUSICDATA*)malloc(sizeof(MUSICDATA));
	//if(ptr == NULL)return NULL;
	//20090410
	//���t�@�C���Ǘ��p��̕ύX
	//MUSICDATA *ptr2 = musicdataroot;
	//ptr->handle = 0;
	//while(ptr2 != NULL)
	//{
	//	if(ptr->handle < ptr2->handle)ptr->handle = ptr2->handle + 1;
	//	ptr2 = ptr2->next;
	//}
	int i;
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//for(i=0;i<MusicTableMAX;i++)
	//{
	//	if(MusicTable[i] == NULL) break;
	//}
	for(i=0;i<MusicTableMAX;i++)
	{
		if(MusicTable[i].useflg == 0) break;
	}
	if(i >= MusicTableMAX) return NULL;
	MUSICDATA *ptr = &MusicTable[i];
	//���̍s�͏����I�ɍ폜�̕����ŁH
	memset(&MusicTable[i],0x00,sizeof(MusicTable[0]));
	ptr->handle = i;
	//ptr->apos = ptr->bpos = 0;
	//ptr->flag = 0;
	//ptr->pcm = NULL;
	//ptr->pcmlen = 0;
	//���̍s�͏����I�ɍ폜�̕�����
	//ptr->next = NULL;//musicdataroot;
	ptr->volume[0] = ptr->volume[1] = ptr->volume[2] = 100;
	//ptr->count = 0;
	ptr->useflg = 1;
	//musicdataroot = ptr;
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//MusicTable[i] = ptr;
	return ptr;
}
int AddMusicDataH()
{
	MUSICDATA *ptr = AddMusicData();
	if(ptr == NULL)return -1;
	return ptr->handle;
}
int RemoveMusicDataH(int handle)
{
	if ( handle <				0 ) return -1;
	if ( handle >=	MusicTableMAX ) return -2;
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//if ( MusicTable[handle] != NULL) goto rm;
	if ( MusicTable[handle].useflg == 0) return -3;
	//20090410
	//���t�@�C���Ǘ��p��̕ύX
	//MUSICDATA *ptr = musicdataroot;
	//MUSICDATA *ptr2 = NULL;
	//while(ptr != NULL)
	//{
	//	if(ptr->handle == handle)goto rm;
	//	ptr2 = ptr;
	//	ptr = ptr->next;
	//}
	//return -3;
//rm:
	//20090410
	//���t�@�C���Ǘ��p��̕ύX
	//if(ptr2 != NULL)ptr2->next = ptr->next;
	//else musicdataroot = ptr->next;
	//free(ptr->pcm);
	//free(ptr);
	//free(MusicTable[handle]->pcm);
	free(MusicTable[handle].pcm);
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//free(MusicTable[handle]);
	//MusicTable[handle] = NULL;
	MusicTable[handle].useflg = 0;
	return 0;
}
int RemoveMusicData(MUSICDATA *ptr)
{
	if(ptr == NULL)return -1;
	return RemoveMusicDataH(ptr->handle);
}

int LoadSoundMem(const char* filename)
{
	MUSICDATA *md = NULL;
	int length = 0;
	u16 *data = NULL;
	//�f�R�[�h����
	DXP_MUSICDECODECONTEXT context;
	STREAMDATA Src;
	context.src = &Src;
	SetupSTREAMDATA(filename,context.src);
	if(decodeprepare_mp3(&context) != -1)goto mp3dec;
	
	return -1;
mp3dec:
	length = mp3len(context.src);
	if(length <= 0)
	{
		decodefinish_mp3(&context);
		return -1;
	}
	data = (u16*)memalign(64,length * 2 * 2);
	if(data == NULL)
	{
		decodefinish_mp3(&context);
		return -1;
	}
	int i;
	STSEEK(context.src,0,SEEK_SET);
	for(i = 0;i < length / 1152;++i)
	{
		context.out = data + i * 2 * 1152;
		if(decode_mp3(&context) == -1)
		{
			break;
		}
	}
	decodefinish_mp3(&context);
	goto lad;
	return -1;
lad:
	md = AddMusicData();
	if(md == NULL)
	{
		free(data);
		return -1;
	}
	md->pcm = data;
	md->bpos = md->pcmlen = length;
	return md->handle;
}

int PlaySoundMem(int handle,int PlayType)
{
	MUSICDATA *md = Handle2MusicDataPtr(handle);
	if(md == NULL)return -1;
	md->flag &= ~DXP_MUSICCOMMAND_STOP;
	md->flag |= DXP_MUSICCOMMAND_PLAY;
	MUSICDATA_THREADPARAM param;
	param.ptr = md;
	param.flag = 0;
	if(PlayType == DX_PLAYTYPE_NORMAL)
	{
		//�X���b�h�̏����^�C�v��ݒ�
		//20090410
		param.flag |= DXP_MUSICTHREADPARAMFLAG_NORMAL;
		return musicthread_normal(sizeof(MUSICDATA_THREADPARAM),&param);
	}
	//�t���O�̕ύX
	//20090410
	if(PlayType == DX_PLAYTYPE_LOOP)param.flag |= DXP_MUSICTHREADPARAMFLAG_LOOP;
	//PSP SDK�����Ă�l����������Ă�T�E���h�n���C�u������
	//�X���b�h�v���C�I���e�B��0x11�ɂ��Ă�̂ł�����Q�l�� 20090410
	int thid = sceKernelCreateThread("soundthread",musicthread_normal,0x11,0x4000,THREAD_ATTR_USER,NULL);
	if(0 > thid)return -1;
	sceKernelStartThread(thid,sizeof(MUSICDATA_THREADPARAM),&param);
	return 0;
}

int StopSoundMem(int handle)
{
	MUSICDATA *md = Handle2MusicDataPtr(handle);
	if(md == NULL)return -1;
	md->flag &=~ DXP_MUSICCOMMAND_PLAY;
	md->flag |= DXP_MUSICCOMMAND_STOP;
	return 0;
}

#define SAMPLECOUNT_DEFAULT		1024
int musicthread_normal(SceSize arglen,void* argp)
{
	MUSICDATA_THREADPARAM *param = (MUSICDATA_THREADPARAM*)argp;
	if(param == NULL)return -1;
	MUSICDATA *ptr = param->ptr;
	if(ptr == NULL)return -1;
	if(ptr->flag & DXP_MUSICFLAG_STREAM)return -1;
	u16 *buf = (u16*)memalign(64,SAMPLECOUNT_DEFAULT * 4);
	if(buf == NULL)return -1;
	++ptr->count;
	param->ptr->flag |= DXP_MUSICFLAG_PLAYING;
	int pos = 0;
	int reservedchannel = -1;
	int play = 1;

	reservedchannel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,SAMPLECOUNT_DEFAULT,PSP_AUDIO_FORMAT_STEREO);
	while(play && reservedchannel >= 0 && !(ptr->flag & DXP_MUSICCOMMAND_STOP))//�X�g�b�v�R�}���h���o����Ă��Ȃ���΍Đ����s
	{
		float vleft,vright;
		vleft = vright = PSP_AUDIO_VOLUME_MAX;
		vleft *= ptr->volume[0] / 100.0f;
		vright *= ptr->volume[0] / 100.0f;
		vleft *= ptr->volume[1] / 100.0f;
		vright *= ptr->volume[2] / 100.0f;

		int i;
		for(i = 0;i < SAMPLECOUNT_DEFAULT;++i)
		{
			if(pos >= ptr->pcmlen || (ptr->bpos != 0 && pos >= ptr->bpos))
			{
				if(param->flag & DXP_MUSICTHREADPARAMFLAG_LOOP)
					pos = ptr->apos;
				else
					break;
			}
			buf[i * 2] = ptr->pcm[pos * 2];
			buf[i * 2 + 1] = ptr->pcm[pos * 2 + 1];
			++pos;
		}
		if(i < SAMPLECOUNT_DEFAULT)
		{
			play = 0;
			sceAudioSetChannelDataLen(reservedchannel,i);
		}
		else
			sceAudioSetChannelDataLen(reservedchannel,SAMPLECOUNT_DEFAULT);
		sceAudioOutputPannedBlocking(reservedchannel,vleft,vright,buf);
		//20090425
		if(ptr->flag & DXP_MUSICCOMMAND_TOP)
		{
			pos = ptr->apos;
			ptr->flag &=~ DXP_MUSICCOMMAND_TOP;
		}

	}
	if(reservedchannel >= 0)sceAudioChRelease(reservedchannel);
	reservedchannel = -1;

	--ptr->count;
	if(ptr->count <= 0)ptr->flag &=~ DXP_MUSICFLAG_PLAYING;
	free(buf);
	//�{�֐����X���b�h�Ƃ��ċN������Ă����ꍇ�Ɏ��X���b�h��j�����鏈����ǉ� 20090410
	if(ptr->flag & DXP_MUSICTHREADPARAMFLAG_NORMAL) return 0;
	sceKernelExitDeleteThread(0);
	return 0;
}

#define TEST09041600
#ifdef	TEST09041600
int LoadStreamSound(const char *filename,int a0,int* a1)
{
	MUSICDATA_THREADPARAM_STREAM param,*p;
	p = &param;
	if((param.md = AddMusicData()) == NULL)return -1;
	if(SetupSTREAMDATA(filename,param.context.src = &param.md->Src) == -1)
	{
		RemoveMusicData(param.md);
		return -1;
	}
	param.md->flag |= DXP_MUSICFLAG_STREAM;
	if(decodeprepare_mp3(&param.context) != -1)goto mp3setup;
	STCLOSE(param.context.src);
	RemoveMusicData(param.md);
	return -1;	
mp3setup:
	param.md->pcmlen = 0x7fffff;//�Ƃ肠����int�^�̍ő�l��ݒ肵�Ă���

	int thid = sceKernelCreateThread("soundthreads",musicthread_stream,0x11,0x4000,THREAD_ATTR_USER,NULL);
	if(0 > thid)
	{
		decodefinish(&param.context);
		RemoveMusicData(param.md);
		return -1;
	}
	sceKernelStartThread(thid,sizeof(MUSICDATA_THREADPARAM_STREAM),p);
	return param.md->handle;
}
#else
int LoadStreamSound(const char *filename,int SetPcmLen,int* AnsPcmLen)
{
	MUSICDATA_THREADPARAM_STREAM param,*p;
	p = &param;
	if((param.md = AddMusicData()) == NULL)return -1;
	if(SetupSTREAMDATA(filename,param.context.src = &param.md->Src) == -1)
	{
		RemoveMusicData(param.md);
		return -1;
	}
	param.md->flag |= DXP_MUSICFLAG_STREAM;
	if(decodeprepare_mp3(&param.context) != -1)goto mp3setup;
	STCLOSE(param.context.src);
	RemoveMusicData(param.md);
	return -1;	
mp3setup:
	if(SetPcmLen <= 0)
	//Pcm�̒������Z�b�g����ĂȂ�(�f�t�H���g��-1)�ꍇ��
	//���O��mp3�t�H�[�}�b�g�𒲂ׂ�pcm���̒��������
	//AnsPcmLen�ɃA�h���X���������ꍇ��Pcm�̒������Z�b�g����
	//Pcm�̒������Z�b�g����Ă���Ή�͂��������͂��ꂽ����
	//�����̂܂܃Z�b�g����B
	{
		//�ʏ�̏���
		param.md->bpos = param.md->pcmlen = mp3len(param.context.src);
		if(AnsPcmLen !=NULL) *AnsPcmLen = param.md->pcmlen;
	}
	else
	{
		param.md->bpos = param.md->pcmlen = SetPcmLen;
	}
	//param.md->pcmlen = mp3len(param.context.src); 2�񓯂����������Ă�̂ŃR�����g 20090410

	//PSP SDK�����Ă�l����������Ă�T�E���h�n���C�u������
	//�X���b�h�v���C�I���e�B��0x11�ɂ��Ă�̂ł�����Q�l�� 20090410
	int thid = sceKernelCreateThread("soundthreads",musicthread_stream,0x11,0x4000,THREAD_ATTR_USER,NULL);
	if(0 > thid)
	{
		decodefinish(&param.context);
		RemoveMusicData(param.md);
		return -1;
	}
	sceKernelStartThread(thid,sizeof(MUSICDATA_THREADPARAM_STREAM),p);
	return param.md->handle;
}
#endif
int PlayStreamSound(int handle,int playtype)
{
	MUSICDATA *md = Handle2MusicDataPtr(handle);
	if(md == NULL)
	{
		return -1;
	}
	if(!md->flag & DXP_MUSICFLAG_STREAM)
	{
		return -1;
	}
	//if(playtype == DX_PLAYTYPE_LOOP)md->flag |= DXP_MUSICFLAG_LOOP;
	//else md->flag &=~ DXP_MUSICFLAG_LOOP;
	//�t���O�̎�ނ�ύX 20090410
	if(playtype == DX_PLAYTYPE_LOOP)md->flag |= DXP_MUSICTHREADPARAMFLAG_LOOP;
	else md->flag &=~ DXP_MUSICTHREADPARAMFLAG_LOOP;
	md->flag &=~ DXP_MUSICCOMMAND_STOP;
	md->flag |= DXP_MUSICCOMMAND_PLAY;
	if(playtype == DX_PLAYTYPE_NORMAL)
	{
		//�X�g���[�������p�^�X�N���X���b�h�x�~���畜������܂ł̎��ԉ҂���ǉ� 20090410
		sceKernelDelayThread(100000);
		while(md->flag & DXP_MUSICFLAG_PLAYING)sceKernelDelayThread(1000);
	}
	return 0;
}


int musicthread_stream(SceSize arglen,void *argp)
{
	MUSICDATA_THREADPARAM_STREAM *ptr = (MUSICDATA_THREADPARAM_STREAM*)argp;
	if(ptr == NULL)
	{
		return -1;
	}
	int pos = 0;
	int reservedchannel = -1;
	int spf = sample_per_frame(ptr->context.filetype);
	u8 play = 0;
	u16 *playbuf,*outbuf,*buf;
	int bufsize = spf * 2 * sizeof(u16) * 2;
	if(bufsize <= 0)
	{
		decodefinish(&ptr->context);
		return -1;
	}
	buf = memalign(64,bufsize);
	if(buf == NULL)
	{
		decodefinish(&ptr->context);
		return -1;
	}
	reservedchannel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,spf,PSP_AUDIO_FORMAT_STEREO);
	playbuf = buf;
	outbuf = buf + spf * 2;
	++ptr->md->count;
	/*ClearDrawScreen();
	clsDx();
	printfDx("spf    = %d\n",spf);
	printfDx("count  = %d\n",ptr->md->count);
	printfDx("pos    = %d\n",pos);
	printfDx("apos   = %d\n",ptr->md->apos);
	printfDx("bpos   = %d\n",ptr->md->bpos);
	printfDx("pcmlen = %d\n",ptr->md->pcmlen);
	printfDx("next f = %d\n",ptr->context.nextframe);
	ScreenFlip();*/
	while( reservedchannel >= 0)
	{
		if(ptr->md->flag & DXP_MUSICCOMMAND_QUIT)break;
		if(play)
		{
			if(ptr->md->flag & DXP_MUSICCOMMAND_STOP)
			{
				play = 0;
				ptr->md->flag &=~ DXP_MUSICFLAG_PLAYING;
				continue;
			}
			ptr->context.nextframe = -1;
			if((pos >= ptr->md->bpos && ptr->md->bpos != 0) || (pos >= ptr->md->pcmlen && ptr->md->pcmlen != 0))
			{
playendproc:
				if(ptr->md->flag & DXP_MUSICTHREADPARAMFLAG_LOOP)
				{
					pos = ptr->md->apos - ptr->md->apos % spf;
					ptr->context.nextframe = pos / spf;
				}
				else
				{
					play = 0;
					ptr->context.nextframe = (ptr->md->apos - ptr->md->apos / spf) / spf;
					ptr->md->flag &=~ DXP_MUSICFLAG_PLAYING;//20090410
															//���ꂪ�Ȃ���DX_PLAYTYPE_NORMAL
															//�ōĐ��I���̍ۂɃ��C���^�X�N�����֐i�߂Ȃ�
					continue;
				}
			}
			float vleft,vright;
			vleft = vright = PSP_AUDIO_VOLUME_MAX;
			vleft *= ptr->md->volume[0] / 100.0f;
			vright *= ptr->md->volume[0] / 100.0f;
			vleft *= ptr->md->volume[1] / 100.0f;
			vright *= ptr->md->volume[2] / 100.0f;

			ptr->context.out = outbuf;
#ifdef TEST09041600
			if(decode(&ptr->context) == -1)
			{
				ptr->md->pcmlen = pos - spf;
				
				ptr->md->bpos = MAX(ptr->md->bpos, ptr->md->pcmlen);
				goto playendproc;
			}
#else
			if(decode(&ptr->context) == -1)goto err;
#endif
			u16 *tmp = playbuf;
			playbuf = outbuf;
			//outbuf = playbuf; �񍐂̂������o�O�����̏C�� 20090410
			outbuf = tmp;
			while(sceAudioGetChannelRestLength(reservedchannel) > 0)sceKernelDelayThread(500);
			sceAudioOutputPanned(reservedchannel,(int)vleft,(int)vright,playbuf);
			pos += spf;
		}
		else
		{
			if(ptr->md->flag & DXP_MUSICCOMMAND_PLAY)
			{
				ptr->md->flag &=~ DXP_MUSICCOMMAND_PLAY;//20090410
														//���ꂪ�Ȃ��ƃf�b�h���[�v�ɓ���
				play = 1;
				ptr->md->flag |= DXP_MUSICFLAG_PLAYING;
				continue;
			}
			sceKernelDelayThread(3000);
		}
	}
	if(reservedchannel >= 0)sceAudioChRelease(reservedchannel);
	--ptr->md->count;
	decodefinish(&ptr->context);
	ptr->md->flag &=~ DXP_MUSICFLAG_PLAYING;
	//�X���b�h�̔j�������ǉ� 20090410
	sceKernelExitDeleteThread(0);
	return 0;
#ifndef TEST09041600
err:
	if(reservedchannel >= 0)sceAudioChRelease(reservedchannel);
	--ptr->md->count;
	decodefinish(&ptr->context);
	ptr->md->flag &=~ DXP_MUSICFLAG_PLAYING;
	//�X���b�h�̔j�������ǉ� 20090410
	sceKernelExitDeleteThread(0);
	return -1;
#endif
}
inline int sample_per_frame(MUSICFILE_TYPE type)
{
	switch(type)
	{
	case DXPMFT_WAVE:
		return 1024;
	case DXPMFT_MP3:
		return 1152;
	default:
		return -1;
	}
}

int decodeprepare(DXP_MUSICDECODECONTEXT *context)
{
	if(context == NULL)return -1;
	if(context->src == NULL)return -1;
	if(decodeprepare_mp3(context) != -1)return 0;
	if(decodeprepare_wave(context) != -1)return 0;
	return -1;
}
int decode(DXP_MUSICDECODECONTEXT *context)
{
	if(context == NULL)return -1;
	switch(context->filetype)
	{
	case DXPMFT_MP3:
		return decode_mp3(context);
	case DXPMFT_WAVE:
		return decode_wave(context);
	default:
		return -1;
	}
}

int decodefinish(DXP_MUSICDECODECONTEXT *context)
{
	if(context == NULL)return -1;
	switch(context->filetype)
	{
	case DXPMFT_MP3:
		return decodefinish_mp3(context);
	case DXPMFT_WAVE:
		return decodefinish_wave(context);
	default:
		return -1;
	}
}

int getRefCount(int sh)
{
	MUSICDATA *md = Handle2MusicDataPtr(sh);
	if(md == NULL)return -1;
	return md->count;
}

//�֐��̒ǉ� 20090410
int	CheckSoundMem( int handle )
{
	if( handle <	0				)	return -1;
	if( handle >=	MusicTableMAX	)	return -2;
	//20090412
	//�Ǘ��Ɋւ���G���A���������f�Љ���h�����߂�malloc��
	//�m�ۂ��Ȃ������ɕύX
	//if( MusicTable[handle]	== NULL)	return -3;
	//if( MusicTable[handle]->flag & DXP_MUSICFLAG_PLAYING) return 1;
	if( MusicTable[handle].useflg == 0)	return -3;
	if( MusicTable[handle].flag & DXP_MUSICFLAG_PLAYING) return 1;
	return 0;
}

//20090410
//�ȉ��g�����Ƃ肠�����ǉ�
//dxlibp.h�Ƀv���g�^�C�v�錾�v�ǉ�
int	ChangeVolumeSoundMem( int VolumePal, int SoundHandle )
{
	return 0;
}

int	DeleteSoundMem( int SoundHandle, int LogOutFlag )
{
	return 0;
}

int	SetLoopPosSoundMem( int LoopTime, int SoundHandle )
{
	return 0;
}

//20090415
//�e�X�g�ǉ�
//DXL�{�Ƃ͖{�֐��ő��d�Đ���F�߂Ă��Ȃ��悤�Ȃ̂�
//�t���O�ŊǗ�
int PlaySoundFileHandle = -1;

int PlaySoundFile(const char *FileName , int PlayType ,int SetPcmLen,int* AnsPcmLen)
{
	if(PlaySoundFileHandle != -1) return -1;
	PlaySoundFileHandle = LoadStreamSound(FileName,SetPcmLen,AnsPcmLen);
	if(PlaySoundFileHandle == -1) return -1;
	PlayStreamSound(PlaySoundFileHandle,PlayType);
	return 0;
}

int CheckSoundFile()
{
	if(PlaySoundFileHandle == -1) return -1;
	return CheckSoundMem(PlaySoundFileHandle);
}

int StopSoundFile()
{
	//�炵�ĂȂ����ǌĂ񂾏ꍇ�ɂ̓G���[�ɂ��Ȃ�
	if(PlaySoundFileHandle == -1) return 0;
	MUSICDATA *md = Handle2MusicDataPtr(PlaySoundFileHandle);
	md->flag |= DXP_MUSICCOMMAND_STOP;
	sceKernelDelayThread(3000);
	md->flag |= DXP_MUSICCOMMAND_QUIT;
	while(!md->flag & DXP_MUSICFLAG_PLAYING) sceKernelDelayThread(1000);
	PlaySoundFileHandle = -1;
	return 0;
}

int	SetCreateSoundDataType( int SoundDataType )
{
	return 0;
}

int	SetTopPosSoundMem(int handle )
{
	if( handle <	0				)	return -1;
	if( handle >=	MusicTableMAX	)	return -2;
	if( MusicTable[handle].useflg == 0)	return -3;
	MusicTable[handle].flag |= DXP_MUSICCOMMAND_TOP;
	return 0;
}
