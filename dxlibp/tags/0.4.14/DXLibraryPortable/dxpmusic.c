///*
//*	DX���C�u����Portable	���y�Ǘ���	Ver0.00
//*	�����	�F�����J��
//*
//*	���l	�F�Ƃ��ɂȂ�
//*/
//
///*
//	20090121�@�j�IMP3�t�@�C���̍Đ��Ɋ��S�Ή��I�@���łɃ��C���X���b�h���ז����܂����Ă����̂����P��
//*/
//
///*
//	���Ǖ��j
//	��x�Ɉ��؂�̃f�[�^�����o���֐������
//	���̊֐���؂�ւ��邾���ŕ����̉��y�t�@�C���`���ɑΉ��ł���悤�ɂ���
//		�����̂����肾����C++�őg�݂��������H
//	PlaySound�n�֐��ɉ\�ȋ@�\���܂Ƃ߂āAPlayMusic�֐��͂�������b�p�[����΂����B
//
//
//	�@�\�ꗗ
//	�n���h���ɂ��Ǘ�
//	�S�̂̃{�����[���Ǘ�
//	�X�̃{�����[���E�p���Ǘ�
//	�V�[�N
//	�Đ��ĊJ�ʒu�̎w��ł��郋�[�v�Đ�
//
//	�X�g���[�~���O�Đ�
//	�������Ǘ�
//
//
//	����
//	�Đ��p�̃f�[�^���͂��߂���64���炢����Ă����B
//	�ŁA�X���b�h�J�n���ɓn���f�[�^�Ƃ��Ă��̃C���f�b�N�X��n��
//
//	���؂�̃f�[�^��STREAMDATA����擾���Ďw�肳�ꂽ�o�b�t�@�ɓ˂����ފ֐������
//	������g���ăf�R�[�h�̃^�C�~���O���ق��̊֐������߂���B
//
//	�t���[��������̃T���v���������k�t�H�[�}�b�g�ɂ���ĈႤ��ł����
//	
//*/
//
////int dxpdecode_mp3(DECODEPARAM *param,STREAMDATA *src,void* buf);	//����Ȋ����̊֐��ɂ���B
//
//
//#include "dxlibp.h"
//#include <pspaudiocodec.h>
//#include <pspaudio.h>
//#include <psputility.h>
//#include <malloc.h>
//#include <string.h>
//#include <stdio.h>
//
//typedef struct
//{
//	STREAMDATA *src;
//	void *databuf;
//	u16 *audiobuf[2];
//	u8 playingbuf;
//}AUDIOPARAM;
//
//typedef struct
//{
//	int samples_per_frame;
//	int (*fpdecode)(AUDIOPARAM *param);
//}AUDIODECODEMETHODS;
//
//
///*���m�̖��
//	�Ȃ񂩍Đ�����Ƃ�����������������񂾂��ǁE�E�E�@���C������܂����B�ł������ɏ����������܂�orz
//	�����̓T�E���h�o�b�t�@�̃t���b�v�҂����Ƀ��[�v�����܂����Ă����ƁB���~���b�̃E�F�C�g�����܂���΂���
//*/
//
//#define MUSICFLAG_THREADEXIST	0x0001		//�X���b�h�����݂���B�܂茻�ݎg���Ȃ��n���h��
//#define MUSICFLAG_PLAYING		0x0004		//�Đ����ł���
//#define MUSICFLAG_LOOP			0x0008		//���[�v�Đ��ł���
//#define MUSICFLAG_LOOP_MS		0x1000		//���[�v�̍ĊJ�ʒu�����݂���
//#define MUSICFLAG_LOOP_SP		0x2000
//
//#define MUSICCOMMAND_PLAY		1	//�Đ��J�nor�ĊJ
//#define MUSICCOMMAND_STOP		2	//��~
//#define MUSICCOMMAND_SCRAP		3	//�f�[�^�ƃX���b�h�̔j��
//#define MUSICCOMMAND_SEEK_MS	4	//�~���b�P�ʂŃV�[�N
//#define MUSICCOMMAND_SEEK_SP	5	//�T���v���P�ʂŃV�[�N
//#define MUSICCOMMAND_VOLUME		6	//���ʕύX					�i�O�`�Q�T�T�j
//#define MUSICCOMMAND_PAN		7	//�ʏ�̃p�����g��			�i�}10000�A�{�Ȃ獶��������A�|�Ȃ�E��������j
//#define MUSICCOMMAND_PAN_DXLIB	8	//DX���C�u�����̃p�����g��	�i�}10000�A�{�Ȃ獶��������A�|�Ȃ�E��������j
//#define MUSICCOMMAND_SETLOOP_MS	9	//���[�v���ɂǂ�����J��Ԃ����ݒ�i�~���b�j
//#define MUSICCOMMAND_SETLOOP_SP	10	//�@�@�@�@�V�@�@�@�@�@�@�V�@�@�@�@�i�T���v���j
//
//int mp3_skiptag(STREAMDATA *src);
//
//typedef struct	//���y�Đ����̍\����
//{
//	u16 flag;				//�X���b�h��Đ��̏�
//	u16 command;			//�X���b�h�ɑ΂��閽��
//	u32 command_data;		//�V�[�N�ʒu�Ȃǂ̖��߂Ŏg��
//
//	u32 looppos;			//�ǂ��Ń��[�v����̂�
//	u32 currentpos_ms;		//���ݒn
//	u32 currentpos_sp;
//	u8	volume;				//����
//	u16 pan_l;				//�p��
//	u16 pan_r;
//	int thid;				//�X���b�hID
//	STREAMDATA *src,Src;	//�f�[�^�X�g���[��
//}MUSICDATA;
//
//static MUSICDATA musicdata = 
//{
//	0,
//	0,
//	0,
//
//	0,
//	0,
//	0,
//	PSP_AUDIO_VOLUME_MAX,
//	10000,
//	10000,
//	-1,
//	NULL
//};
//
//
//const static int mp3bitrates[2][15]
//= {
//	{0,	32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 },	//MPEG1
//	{0,	8,	16,	24,	32,	40,	48,	56,	64,	 80,  96,  112,	128, 144, 160 },	//MPEG2
//};
//const static int mp3samplerates[2][4]
//= {
//	{44100,48000,32000,0},
//	{22050,24000,16000,0},
//};
//
//
//static int mp3thread_music(SceSize arglen,void* argp);
//
//
//int StopMusic()
//{
//	if(!(musicdata.flag & MUSICFLAG_THREADEXIST))return 0;
//	if(musicdata.thid >= 0)
//	{
//		musicdata.flag &= ~MUSICFLAG_PLAYING;
//		while(musicdata.flag & MUSICFLAG_THREADEXIST)
//		{
//			ProcessMessage();
//		}
//	}
//}
//
//int PlayMusic_MP3Stream(const char *FileName,int PlayMode);
//
//int PlayMusic(const char *FileName,int PlayMode)
//{
//	StopMusic();
//	return PlayMusic_MP3Stream(FileName,PlayMode);
//}
//
//#define LINETRACE 	printfDx("%d\n",__LINE__);ScreenFlip();
//
//
//int PlayMusic_MP3Stream(const char *FileName,int PlayMode)
//{
//	musicdata.src = &(musicdata.Src);
//	if(SetupSTREAMDATA(FileName,musicdata.src) == -1){
//		return -1;
//	}
//	musicdata.thid = sceKernelCreateThread("mp3thread_music",mp3thread_music,0x08,0x1800,THREAD_ATTR_USER,NULL);
//	if(musicdata.thid < 0)return -1;
//	musicdata.flag = MUSICFLAG_THREADEXIST | MUSICFLAG_PLAYING;
//	sceKernelStartThread(musicdata.thid,0,NULL);
//	if(PlayMode & DX_PLAYTYPE_LOOPBIT)
//	{
//		musicdata.flag |= MUSICFLAG_LOOP;
//	}else
//	if(!(PlayMode & DX_PLAYTYPE_BACKBIT))
//	{
//		while(musicdata.flag & MUSICFLAG_PLAYING)
//		{
//			ProcessMessage();
//			Sleep(1);
//		}
//	}
//	return 0;
//}
//
//
//static int mp3thread_music(SceSize arglen,void* argp) //�X���b�h�J�n���ɂǂ�musicarray�̗v�f���g�����̃C���f�b�N�X��n�����B
//{
//	u8 playbuf = 0;
//	u8 mp3_getEDRAM = 0;
//	u8* mp3_data_buffer = NULL;
//	u16 mp3_sample_per_frame; 
//	u32 mp3_data_start = 0;
//	u32 mp3_data_start_loop = 0;
//
//
//	unsigned long *mp3_codec_buffer = NULL;
//	short *mp3_play_buffer[2];
//
//	mp3_play_buffer[0]	= memalign(64,1152 * 2 * sizeof(short) * 2 + 65 * sizeof(unsigned long));
//	if(mp3_play_buffer[0] == NULL)
//		goto wait;
//	mp3_play_buffer[1]	= mp3_play_buffer[0] + 1152 * 2;
//	mp3_codec_buffer	= (unsigned long*)(mp3_play_buffer[0] + 1152 * 2 * 2);
//
//	sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);//���̊֐��͉��x�Ăяo���Ă��悳�����B
//
//   mp3_sample_per_frame = 1152; 
//    
//   mp3_data_start = mp3_data_start_loop = STSEEK(musicdata.src,0,SEEK_CUR);
//
//   memset(mp3_codec_buffer, 0, sizeof(mp3_codec_buffer)); 
//    
//   if ( sceAudiocodecCheckNeedMem(mp3_codec_buffer, 0x1002) < 0 ) 
//      goto wait; 
//   if ( sceAudiocodecGetEDRAM(mp3_codec_buffer, 0x1002) < 0 ) 
//         goto wait; 
//   mp3_getEDRAM = 1; 
//    
//   if ( sceAudiocodecInit(mp3_codec_buffer, 0x1002) < 0 )
//      goto wait; 
//
//
//   int outputchannel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,mp3_sample_per_frame,PSP_AUDIO_FORMAT_STEREO);
//   if(outputchannel < 0)
//      goto wait; 
//    
//	char eof = 0;
//loop_start:
//
//
//	while( (!eof) && (musicdata.flag & MUSICFLAG_PLAYING))
//	{
//		int samplesdecoded;
//		memset(mp3_play_buffer[playbuf^1], 0, mp3_sample_per_frame*2*2);
//		unsigned char mp3_header_buf[4];
//
//		int sb = mp3_skiptag(musicdata.src);
//		if(sb == -1)
//		{
//			eof = 1;
//			continue;
//		}
//		mp3_data_start += sb;
//		STSEEK(musicdata.src,mp3_data_start,SEEK_SET);
//
//	  if ( STREAD(mp3_header_buf,1,4,musicdata.src) != 4 ) { 
//         eof = 1; 
//         continue; 
//      } 
//      int mp3_header = mp3_header_buf[0]; 
//      mp3_header = (mp3_header<<8) | mp3_header_buf[1];
//      mp3_header = (mp3_header<<8) | mp3_header_buf[2];
//      mp3_header = (mp3_header<<8) | mp3_header_buf[3];
//
//		int version = (mp3_header & 0x180000) >> 19;
//		switch(version)
//		{
//		case 3:
//			version = 0;
//			break;
//		case 2:
//			version = 1;
//			break;
//		default:
//			version = 0;
//			//�G���[����
//		}
//      int bitrate = (mp3_header & 0xf000) >> 12;
//      int padding = (mp3_header & 0x200) >> 9;
//      int samplerate = (mp3_header & 0xc00) >> 10;
//
//	  int frame_size = 144000*mp3bitrates[version][bitrate]/mp3samplerates[version][samplerate] + padding; 
//       
//      if ( mp3_data_buffer ) 
//         free(mp3_data_buffer); 
//      mp3_data_buffer = (u8*)memalign(64, frame_size); 
//       
//	  STSEEK(musicdata.src,mp3_data_start,SEEK_SET);
//
//	  if ( STREAD(mp3_data_buffer,1,frame_size,musicdata.src) != frame_size ) { 
//         eof = 1; 
//         continue; 
//      }
//
//	  mp3_data_start += frame_size;
//
//	  mp3_codec_buffer[6] = (unsigned long)mp3_data_buffer; 
//	  mp3_codec_buffer[8] = (unsigned long)mp3_play_buffer[playbuf^1];
//       
//      mp3_codec_buffer[7] = mp3_codec_buffer[10] = frame_size; 
//      mp3_codec_buffer[9] = mp3_sample_per_frame * 4; 
//    
//      int res = sceAudiocodecDecode(mp3_codec_buffer, 0x1002); 
//      if ( res < 0 ) { 
//         eof = 1; 
//         continue; 
//      }
//      samplesdecoded += mp3_sample_per_frame;
//	  while(sceAudioGetChannelRestLen(outputchannel) > 100)sceKernelDelayThread(1000);
//	  while(sceAudioGetChannelRestLen(outputchannel) > 0);
//	  playbuf ^= 1;
//	  sceAudioOutput(outputchannel,PSP_AUDIO_VOLUME_MAX,mp3_play_buffer[playbuf]);
//   } 
//	if(musicdata.flag & MUSICFLAG_PLAYING && musicdata.flag & MUSICFLAG_LOOP)
//	{
//		STSEEK(musicdata.src,mp3_data_start = mp3_data_start_loop,SEEK_SET);
//		goto loop_start;
//	}
//wait:
//
//	musicdata.flag &= ~MUSICFLAG_PLAYING;
//    
//   if(mp3_play_buffer[0])
//	   free(mp3_play_buffer[0]);
//	STCLOSE(musicdata.src);
//	musicdata.src = NULL;
//   if ( mp3_data_buffer)
//	   free(mp3_data_buffer); 
//   if ( mp3_getEDRAM )
//      sceAudiocodecReleaseEDRAM(mp3_codec_buffer);
//   musicdata.thid = -1;
//   musicdata.flag &= ~MUSICFLAG_THREADEXIST;
//   return 0; 
//}
//
//int mp3_skiptag(STREAMDATA *src)
///*
//Param:
//	filestream ptr
//Return:
//	-1:eof
//others:skip size in bytes
//*/
//{
//	char header[10];
//	if(src == NULL)return -1;
//	int pos = STTELL(src);
//	STREAD(header,1,10,src);
//	if(header[0] == 'T' && header[1] == 'A' && header[2] == 'G')
//	{
//		STSEEK(src,pos,SEEK_SET);
//		return -1;
//	}
//	if(!strncmp(header,"ID3",3) || !strncmp(header,"ea3",3))
//	{
//		u32 tagsize;
//		tagsize = (u8)header[6];
//		tagsize <<= 7;
//		tagsize |= (u8)header[7];
//		tagsize <<= 7;
//		tagsize |= (u8)header[8];
//		tagsize <<= 7;
//		tagsize |= (u8)header[9];
//
//		if(header[5] & 0x10)
//			tagsize += 10;
//
//		STSEEK(src,pos,SEEK_SET);
//		return tagsize + 10;
//	}
//	STSEEK(src,pos,SEEK_SET);
//	return 0;
//}
//
//int IsWave(STREAMDATA *src)
//{
//	if(src == NULL)return -1;
//	char buf[12];
//	STREAD(buf,1,12,src);
//	if(strncmp(buf,"RIFF",4) || strncmp(buf + 8,"WAVE",4))return 0;
//	return 1;
//}
//
//typedef struct
//{
//	char tag[4];
//	int size;
//}RIFF_HEADER;
//
//typedef struct
//{
//	u16 ftag;
//	u16 channels;
//	u32 sample_per_sec;
//	u32 byte_per_sec;
//	u16 balign;
//	u16 bit_per_sample;
//}WAVE_FMT;
//
//int wavethread_music(SceSize arglen,void* argp)
//{
//	u8 eof = 1;
//	RIFF_HEADER hbuf;
//	WAVE_FMT fmt;
//	fmt.ftag = 2;
//	while(STREAD(&hbuf,1,8,musicdata.src) != 8)
//	{
//		if(!strncmp(hbuf.tag,"fmt ",4))
//		{
//			int pos = STTELL(musicdata.src);
//			STREAD(&fmt,sizeof(WAVE_FMT),1,musicdata.src);
//			STSEEK(musicdata.src,pos + hbuf.size,SEEK_SET);
//		}
//		else if(!strncmp(hbuf.tag,"data",4))
//		{
//			eof = 0;
//			break;
//		}
//		else
//		{
//			STSEEK(musicdata.src,hbuf.size,SEEK_CUR);
//		}
//	}
//	if(eof)return -1;
//	//�t�H�[�}�b�g�擾����
////	int chanel = sceAudioChReserve(-1,fmt.bit_per_sample,fmt.channels
//}
//
