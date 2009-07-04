#include "dxlibp.h"
#include "dxpstatic.h"
#include <pspjpeg.h>
#include <malloc.h>
#include <string.h>

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
