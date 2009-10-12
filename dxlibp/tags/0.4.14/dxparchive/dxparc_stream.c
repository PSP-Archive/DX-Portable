#include "dxlibp.h"
#include "dxparchive.h"
#include <stdio.h>
#include <string.h>

// �񓯊��ǂݍ��ݏ��
#define DXARC_STREAM_ASYNCSTATE_IDLE			(0)				// �������Ă��Ȃ�
#define DXARC_STREAM_ASYNCSTATE_PRESSREAD		(1)				// ���k���ꂽ�t�@�C����ǂݍ��ݒ�
#define DXARC_STREAM_ASYNCSTATE_READ			(2)				// �f�[�^��ǂݍ��ݒ�

//dxparc_decode.c
int		DXA_Decode( void *Src, void *Dest );

/** �t�@�C���|�C���^��ύX����
 *
 *
 */
int		DXA_STREAM_Seek( DXARC_STREAM *DXAStream, int SeekPoint, int SeekMode )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE ) Sleep(1);
	}
	// �V�[�N�^�C�v�ɂ���ď����𕪊�
	switch( SeekMode )
	{
	case SEEK_SET : break ;		
	case SEEK_CUR : SeekPoint += DXAStream->FilePoint ; break ;
	case SEEK_END :	SeekPoint = DXAStream->FileHead->DataSize + SeekPoint ; break ;
	}
	// �␳
	if( SeekPoint > (int)DXAStream->FileHead->DataSize ) SeekPoint = DXAStream->FileHead->DataSize ;
	if( SeekPoint < 0 ) SeekPoint = 0 ;
	// �Z�b�g
	DXAStream->FilePoint = SeekPoint ;
	// EOF�t���O��|��
	DXAStream->EOFFlag = FALSE ;
	return 0 ;
}

/** ���݂̃t�@�C���|�C���^�𓾂�
 *
 *
 */
int		DXA_STREAM_Tell( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE ) Sleep(1);
	}
	return DXAStream->FilePoint ;
}

/** �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
 *
 *
 */
int		DXA_STREAM_Eof( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE ) Sleep(1);
	}
	return DXAStream->EOFFlag ? EOF : 0 ;
}

/** �ǂݍ��ݏ������������Ă��邩�ǂ����𒲂ׂ�
 *
 *
 */
int		DXA_STREAM_IdleCheck( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ł͂Ȃ��ꍇ�͉������� TRUE ��Ԃ�
	if( DXAStream->UseASyncReadFlag == FALSE ) return TRUE ;
	// ��Ԃɂ���ď����𕪊�
	switch( DXAStream->ASyncState )
	{
	case DXARC_STREAM_ASYNCSTATE_IDLE:			// �ҋ@���
		return TRUE;
	case DXARC_STREAM_ASYNCSTATE_PRESSREAD:		// ���k�f�[�^�ǂݍ��ݑ҂�
		// �ǂݍ��ݏI���҂�
		//if( WinFileAccessIdleCheck( DXAStream->Archive->FilePointer ) == FALSE ) return FALSE;
		// �ǂݍ��ݏI�������܂������O��
		DXA_KeyConv( DXAStream->DecodeTempBuffer, DXAStream->FileHead->PressDataSize, DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
		// ��
		DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
		// �������̉��
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;
		// ��Ԃ�ҋ@��Ԃɂ���
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;
	case DXARC_STREAM_ASYNCSTATE_READ:			// �ǂݍ��ݑ҂�
		// �ǂݍ��ݏI���҂�
		//if( WinFileAccessIdleCheck( DXAStream->Archive->FilePointer ) == FALSE ) return FALSE;
		// �ǂݍ��ݏI������献���O��
		DXA_KeyConv( DXAStream->ReadBuffer, DXAStream->ReadSize, DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
		// ��Ԃ�ҋ@��Ԃɂ���
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;
	}
	return TRUE ;
}

/** �t�@�C���̃T�C�Y���擾����
 *
 *
 */
int		DXA_STREAM_Size( DXARC_STREAM *DXAStream )
{
	return DXAStream->FileHead->DataSize ;
}

/** �t�@�C���̓��e��ǂݍ���
 *
 *
 */
int		DXA_STREAM_Read( DXARC_STREAM *DXAStream, void *Buffer, int ReadLength )
{
	int ReadSize ;
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE ) Sleep(1);
	}
	// EOF �t���O�������Ă�����O��Ԃ�
	if( DXAStream->EOFFlag == TRUE ) return 0 ;
	// EOF ���o
	if( DXAStream->FileHead->DataSize == DXAStream->FilePoint )
	{
		DXAStream->EOFFlag = TRUE ;
		return 0 ;
	}
	// �f�[�^��ǂݍ��ޗʂ�ݒ肷��
	ReadSize = ReadLength < (int)( DXAStream->FileHead->DataSize - DXAStream->FilePoint ) ? ReadLength : DXAStream->FileHead->DataSize - DXAStream->FilePoint ;
	// �f�[�^�����k����Ă������ǂ����ŏ����𕪊�
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		// �f�[�^���R�s�[����
		memcpy( Buffer, (BYTE *)DXAStream->DecodeDataBuffer + DXAStream->FilePoint, ReadSize ) ;
	}
	else
	{
		// ��������Ƀf�[�^�����邩�ǂ����ŏ����𕪊�
		if( DXAStream->Archive->MemoryOpenFlag == TRUE )
		{
			// ��������ɂ���ꍇ
			// �f�[�^���R�s�[����
			memcpy( Buffer, (BYTE *)DXAStream->Archive->FilePointer + DXAStream->Archive->Head.DataStartAddress + DXAStream->FileHead->DataAddress + DXAStream->FilePoint, ReadSize ) ;
		}
		else
		{
			// �t�@�C������ǂݍ���ł���ꍇ
			// �A�[�J�C�u�t�@�C���|�C���^�ƁA���z�t�@�C���|�C���^����v���Ă��邩���ׂ�
			// ��v���Ă��Ȃ�������A�[�J�C�u�t�@�C���|�C���^���ړ�����
			DXAStream->ASyncReadFileAddress = (int)( DXAStream->FileHead->DataAddress + DXAStream->Archive->Head.DataStartAddress + DXAStream->FilePoint );
			//if( WinFileAccessTell( DXAStream->Archive->FilePointer ) != DXAStream->ASyncReadFileAddress )
			{
			//	WinFileAccessSeek( DXAStream->Archive->FilePointer, DXAStream->ASyncReadFileAddress, SEEK_SET ) ;
			}
			// �񓯊��ǂݍ��݂̏ꍇ�Ɠ����ǂݍ��݂̏ꍇ�ŏ����𕪊�
			if( DXAStream->UseASyncReadFlag )
			{
				// �t�@�C������ǂݍ���
				//WinFileAccessRead( Buffer, ReadSize, 1, DXAStream->Archive->FilePointer ) ;
				DXAStream->ReadBuffer = Buffer;
				DXAStream->ReadSize = ReadSize;
				DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_READ ;
			}
			else
			{
				// �f�[�^��ǂݍ���
				DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->Archive->FilePointer, DXAStream->Archive->Key,-1 ) ;
			}
		}
	}
	// EOF �t���O��|��
	DXAStream->EOFFlag = FALSE ;
	// �ǂݍ��񂾕������t�@�C���|�C���^���ړ�����
	DXAStream->FilePoint += ReadSize ;
	return ReadSize ;// �ǂݍ��񂾗e�ʂ�Ԃ�
}
	
/** �A�[�J�C�u�t�@�C�����̃t�@�C�������
 *
 *
 */
int		DXA_STREAM_Terminate( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE ) Sleep(1);
	}
	// �������̉��
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeDataBuffer ) ;
		DXAStream->DecodeDataBuffer = NULL ;
	}
	if( DXAStream->DecodeTempBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;
	}
	// �[��������
	memset( DXAStream, 0, sizeof( DXARC_STREAM ) ) ;
	return 0 ;
}

/** �A�[�J�C�u�t�@�C�����̃t�@�C�����J��(�t�@�C�������Ƃ͕K�v�Ȃ�)
 *
 *
 */
int		DXA_STREAM_Initialize( DXARC_STREAM *DXAStream, DXARC *DXA, const char *FilePath)//, int UseASyncReadFlag )
{
	DXARC_FILEHEAD *FileH ;
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// �w��̃t�@�C���̏��𓾂�
	FileH = DXA_GetFileInfo( DXA, FilePath ) ;
	if( FileH == NULL ) return -1 ;
	// �f�[�^�̃Z�b�g
	DXAStream->Archive          = DXA ;
	DXAStream->FileHead         = FileH ;
	DXAStream->EOFFlag          = FALSE ;
	DXAStream->FilePoint        = 0 ;
	DXAStream->DecodeDataBuffer = NULL ;
	DXAStream->DecodeTempBuffer = NULL ;
	DXAStream->UseASyncReadFlag = UseASyncReadFlag ;
	DXAStream->ASyncState       = DXARC_STREAM_ASYNCSTATE_IDLE ;
	// �t�@�C�������k����Ă���ꍇ�͂����œǂݍ���ŉ𓀂��Ă��܂�
	if( DXA->Head.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
	{
		// �𓀃f�[�^�����܂郁�����̈�̊m��
		DXAStream->DecodeDataBuffer = DXALLOC( FileH->DataSize ) ;
		// ��������ɓǂݍ��܂�Ă��邩�ǂ����ŏ����𕪊�
		if( DXA->MemoryOpenFlag == TRUE )
		{
			// ��
			DXA_Decode( (BYTE *)DXA->FilePointer + DXA->Head.DataStartAddress + FileH->DataAddress, DXAStream->DecodeDataBuffer ) ;
		}
		else
		{
			// ���k�f�[�^�����܂郁�����̈�̊m��
			DXAStream->DecodeTempBuffer = DXALLOC( FileH->PressDataSize ) ;
			// ���k�f�[�^�̓ǂݍ���
			DXAStream->ASyncReadFileAddress = DXA->Head.DataStartAddress + FileH->DataAddress;
			//WinFileAccessSeek( DXA->FilePointer, DXAStream->ASyncReadFileAddress, SEEK_SET ) ;
			// �񓯊��̏ꍇ�͓ǂݍ��݂ƌ�������ʁX�ɍs��
			if( DXAStream->UseASyncReadFlag == TRUE )
			{
				// �t�@�C������ǂݍ���
				//WinFileAccessRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, 1, DXA->FilePointer ) ;
				DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_PRESSREAD ;
			}
			else
			{
				DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXA->FilePointer, DXA->Key,-1 ) ;
				// ��
				DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				// �������̉��
				DXFREE( DXAStream->DecodeTempBuffer ) ;
				DXAStream->DecodeTempBuffer = NULL ;
			}
		}
	}
	return 0 ;
}

