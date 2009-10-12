#include <png.h>
#include <malloc.h>
#include <pspgu.h>
#include "dxlibp.h"
#include "dxpstatic.h"

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
