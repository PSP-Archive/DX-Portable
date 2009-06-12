// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�A�[�J�C�u����v���O������
// 
// 				Ver 2.25b
// 
// -------------------------------------------------------------------------------


// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h��-----------------------------------------------------------------
//#include "DxLib.h"
#include "dxlibp.h"
#include "dxparchive.h"
#include <stdio.h>
#include <string.h>

//#define DXPDEFARG(ARG) =(ARG)
//#ifndef DX_NON_DXA

// �}�N����`�� ------------------------------------------------------------------

#define DXARCD						DX_ArchiveDirData
#define CHECKMULTIBYTECHAR(CP)		(( (unsigned char)*(CP) >= 0x81 && (unsigned char)*(CP) <= 0x9F ) || ( (unsigned char)*(CP) >= 0xE0 && (unsigned char)*(CP) <= 0xFC ))	// TRUE:�Q�o�C�g����  FALSE:�P�o�C�g����
#define CHARUP(C)					((C) >= 'a' && (C) <= 'z' ? (C) - 'a' + 'A' : (C))

#define DXARC_FILEHEAD_VER1_SIZE	(40)			// Ver0x0001 �� DXARC_FILEHEAD �\���̂̃T�C�Y
#define DXARC_FILEHEAD_VER2_SIZE	(44)			// Ver0x0002 �� DXARC_FILEHEAD �\���̂̃T�C�Y


// �\���̒�`�� ------------------------------------------------------------------

// DXA_DIR_FindFirst ���̏����Ŏg�p����\����
typedef struct tagDXA_DIR_FINDDATA
{
	int							UseArchiveFlag;					// �A�[�J�C�u�t�@�C�����g�p���Ă��邩�t���O
	int							UseArchiveIndex;				// �A�[�J�C�u���g�p���Ă���ꍇ�A�g�p���Ă���A�[�J�C�u�t�@�C���f�[�^�̃C���f�b�N�X
	int							FindHandle;						// �t�@�C�������p�n���h��
} DXA_DIR_FINDDATA;

// DXA_FindFirst ���̏����Ŏg�p����\����
typedef struct tagDXA_FINDDATA
{
	DXARC						*Container;						// �����Ώۂ̂c�w�`�t�@�C��
	char						SearchStr[MAX_PATH];			// ����������
	DXARC_DIRECTORY				*Directory;						// �����Ώۂ̃f�B���N�g��
	DWORD						ObjectCount;					// ���ɓn���f�B���N�g�����I�u�W�F�N�g�̃C���f�b�N�X
} DXA_FINDDATA;

// �������ϐ��錾��-------------------------------------------------------------

// �A�[�J�C�u���f�B���N�g���Ɍ����Ă�ׂ̃f�[�^
DXARC_DIR DX_ArchiveDirData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static DXARC_FILEHEAD *DXA_GetFileInfo( DXARC *DXA, const char *FilePath ) ;				// �t�@�C���̏��𓾂�
static int DXA_ChangeCurrentDirectoryFast( DXARC *DXA, DXARC_SEARCHDATA *SearchData ) ;			// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
static int DXA_DIR_GetArchive( const char *FilePath ) ;										// ���ɊJ����Ă���A�[�J�C�u�̃n���h�����擾����( �߂�l: -1=�������� 0�ȏ�:�n���h�� )
static int DXA_DIR_CloseArchive( int ArchiveHandle ) ;										// �A�[�J�C�u�t�@�C�������
static void DXA_DIR_CloseWaitArchive( void ) ;												// �g�p�����̂�҂��Ă���A�[�J�C�u�t�@�C����S�ĕ���
static int DXA_DIR_ConvertFullPath( const char *Src, char *Dest ) ;							// �S�Ẳp����������啶���ɂ��Ȃ���A�t���p�X�ɕϊ�����
//static int DXA_DIR_AnalysisFileNameAndDirPath( const char *Src, char *FileName = 0, char *DirPath = 0 ) ;
static int DXA_DIR_AnalysisFileNameAndDirPath( const char *Src, char *FileName DXPDEFARG(0), char *DirPath DXPDEFARG(0) ) ;
// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����B�t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ��ADirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
static int DXA_DIR_FileNameCmp( const char *Src, const char *CmpStr );						// CmpStr �̏����� Src ���K�����邩�ǂ����𒲂ׂ�( 0:�K������  -1:�K�����Ȃ� )
static int DXA_DIR_OpenTest( const char *FilePath, int *ArchiveIndex, char *ArchivePath, char *ArchiveFilePath );	// �A�[�J�C�u�t�@�C�����t�H���_�Ɍ����Ăăt�@�C�����J�����̏��𓾂�( -1:�A�[�J�C�u�Ƃ��Ă͑��݂��Ȃ�����  0:���݂��� )
//dxparc_decode.c
int		DXA_Decode( void *Src, void *Dest );
// �v���O������-------------------------------------------------------------------

/** ������������p�̃f�[�^�ɕϊ�( �k�������� \ ����������I�� )
 *
 *
 */
int		DXA_ConvSearchData( DXARC_SEARCHDATA *Dest, const char *Src, int *Length )
{
	int i, StringLength ;
	WORD ParityData ;
	ParityData = 0 ;
	for( i = 0 ; Src[i] != '\0' && Src[i] != '\\' ; )
	{
		if( CHECKMULTIBYTECHAR( &Src[i] ) == TRUE )
		{
			// �Q�o�C�g�����̏ꍇ�͂��̂܂܃R�s�[
			*((WORD *)&Dest->FileName[i]) = *((WORD *)&Src[i]) ;
			ParityData += (BYTE)Src[i] + (BYTE)Src[i+1] ;
			i += 2 ;
		}
		else
		{
			// �������̏ꍇ�͑啶���ɕϊ�
			if( Src[i] >= 'a' && Src[i] <= 'z' )	Dest->FileName[i] = (BYTE)Src[i] - 'a' + 'A' ;
			else									Dest->FileName[i] = Src[i] ;
			ParityData += (BYTE)Dest->FileName[i] ;
			i ++ ;
		}
	}
	// ������̒�����ۑ�
	if( Length != NULL ) *Length = i ;
	// �S�̔{���̈ʒu�܂łO����
	StringLength = ( ( i + 1 ) + 3 ) / 4 * 4 ;
	memset( &Dest->FileName[i], 0, StringLength - i ) ;
	// �p���e�B�f�[�^�̕ۑ�
	Dest->Parity = ParityData ;
	// �p�b�N�f�[�^���̕ۑ�
	Dest->PackNum = StringLength / 4 ;
	// ����I��
	return 0 ;
}
//���g�p�ۂ��̂łƂ肠�����R�����g��
/** �f�[�^�𔽓]������֐�
 *
 *
 */
/*void	DXA_NotConv( void *Data , int Size )
{
	int DwordNum ;
	int ByteNum ;
	DwordNum	= Size / 4 ;
	ByteNum		= Size - DwordNum * 4 ;
	//{
		DWORD *dd;
		dd = ( DWORD * )Data ;
		if( DwordNum != 0 )
		{
			do
			{
				*dd = ~*dd;
				dd++;
			}while( --DwordNum ) ;
		}
		if( ByteNum != 0 )
		{
			do
			{
				*((BYTE *)dd) = ~*((BYTE *)dd) ;
				dd = (DWORD *)(((BYTE *)dd) + 1) ;
			}while( --ByteNum ) ;
		}
	//}
}*/

/** �f�[�^�𔽓]�����ăt�@�C������ǂݍ��ފ֐�
 *
 *
 */
/*void	DXA_NotConvFileRead( void *Data, int Size, DWORD FilePointer )
{
	// �ǂݍ���
	//WinFileAccessRead( Data, Size, 1, FilePointer ) ;
	// �f�[�^�𔽓]
	DXA_NotConv( Data, Size ) ;
}*/

/** ����������쐬
 *
 *
 */
void	DXA_KeyCreate( const char *Source, unsigned char *Key )
{
	int Len ;
	if( Source == NULL )
	{
		memset( Key, 0xaa, DXA_KEYSTR_LENGTH ) ;
	}
	else
	{
		Len = strlen( Source ) ;
		if( Len > DXA_KEYSTR_LENGTH )
		{
			memcpy( Key, Source, DXA_KEYSTR_LENGTH ) ;
		}
		else
		{
			// �������� DXA_KEYSTR_LENGTH ���Z�������烋�[�v����
			int i ;
			for( i = 0 ; i + Len <= DXA_KEYSTR_LENGTH ; i += Len )
				memcpy( Key + i, Source, Len ) ;
			if( i < DXA_KEYSTR_LENGTH )
				memcpy( Key + i, Source, DXA_KEYSTR_LENGTH - i ) ;
		}
	}
	Key[0] = ~Key[0] ;
	Key[1] = ( Key[1] >> 4 ) | ( Key[1] << 4 ) ;
	Key[2] = Key[2] ^ 0x8a ;
	Key[3] = ~( ( Key[3] >> 4 ) | ( Key[3] << 4 ) ) ;
	Key[4] = ~Key[4] ;
	Key[5] = Key[5] ^ 0xac ;
	Key[6] = ~Key[6] ;
	Key[7] = ~( ( Key[7] >> 3 ) | ( Key[7] << 5 ) ) ;
	Key[8] = ( Key[8] >> 5 ) | ( Key[8] << 3 ) ;
	Key[9] = Key[9] ^ 0x7f ;
	Key[10] = ( ( Key[10] >> 4 ) | ( Key[10] << 4 ) ) ^ 0xd6 ;
	Key[11] = Key[11] ^ 0xcc ;
}

/** ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
 *
 *
 */
void DXA_KeyConv( void *Data, int Size, int Position, unsigned char *Key )
{
	int i, j ;
	j = Position ;
	for( i = 0 ; i < Size ; i ++ )
	{
		((u8 *)Data)[i] ^= Key[j] ;
		j ++ ;
		if( j == DXA_KEYSTR_LENGTH ) j = 0 ;
	}
}

/** �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
 *
 *
 */
void	DXA_KeyConvFileRead( void *Data, int Size, DWORD FilePointer, unsigned char *Key, int Position )
{
	int pos ;
	// �t�@�C���̈ʒu���擾���Ă���
	if( Position == -1 ) pos = Position ;//WinFileAccessTell( FilePointer ) ;
	else                 pos = Position ;
	// �ǂݍ���
	//WinFileAccessRead( Data, Size, 1, FilePointer ) ;
	//while( WinFileAccessIdleCheck( FilePointer ) == FALSE ) Sleep(0);
	// �f�[�^������������g���� Xor ���Z
	DXA_KeyConv( Data, Size, pos, Key ) ;
}

/** �����ɓK������I�u�W�F�N�g����������(�����Ώۂ� ObjectCount ���C���f�b�N�X�Ƃ����Ƃ��납��)(�߂�l -1:�G���[ 0:����)
 *
 *
 */
int		DXA_FindProcess( DXA_FINDDATA *FindData, FILEINFO *FileInfo )
{
	DXARC_DIRECTORY *dir;
	DXARC_FILEHEAD *file;
	BYTE *nameTable;
	DXARC *DXA;
	int i, num, addnum;
	char *str, *name;
	DWORD fileHeadSize;
	DXA = FindData->Container;
	dir = FindData->Directory;
	str = FindData->SearchStr;
	num = dir->FileHeadNum;
	nameTable = DXA->Table.NameTable;
	addnum = dir->ParentDirectoryAddress == 0xffffffff ? 1 : 2;
	fileHeadSize = DXA->Head.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ;
	if( FindData->ObjectCount == (DWORD)( num + addnum ) ) return -1;
	file = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + dir->FileHeadAddress + fileHeadSize * ( FindData->ObjectCount - addnum ) ) ;
	for( i = FindData->ObjectCount; i < num; i ++, file = (DXARC_FILEHEAD *)( (BYTE *)file + fileHeadSize ) )
	{
		if( i < addnum )
		{
			     if( i == 0 ){ if( DXA_DIR_FileNameCmp( ".",  str ) == 0 ) break; }
			else if( i == 1 ){ if( DXA_DIR_FileNameCmp( "..", str ) == 0 ) break; }
		}
		else
		{
			name = (char *)( nameTable + file->NameAddress + 4 );
			if( DXA_DIR_FileNameCmp( name, str ) == 0 ) break;
		}
	}
	FindData->ObjectCount = i;
	if( i == num + addnum ) return -1;
	if( FileInfo )
	{
		if( i < addnum )
		{
			switch( i )
			{
			case 0: strcpy( FileInfo->Name, "."  ); break;
			case 1: strcpy( FileInfo->Name, ".." ); break;
			}
			FileInfo->DirFlag = 1;
			FileInfo->Size    = 0;
			memset( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) );
			memset( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) );
		}
		else
		{
			name = (char *)( nameTable + file->NameAddress );
			strcpy( FileInfo->Name, name + ((WORD *)name)[0] * 4 + 4 );
			FileInfo->DirFlag = (file->Attributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? TRUE : FALSE;
			FileInfo->Size = (LONGLONG)file->DataSize;
			//_FileTimeToLocalDateData( (FILETIME *)&file->Time.Create,    &FileInfo->CreationTime  );
			//_FileTimeToLocalDateData( (FILETIME *)&file->Time.LastWrite, &FileInfo->LastWriteTime );
		}
	}
	return 0;
}

/** �A�[�J�C�u�t�@�C���������ׂ̍\���̂�����������
 *
 *
 */
int		DXA_Initialize( DXARC *DXA )
{
	memset( DXA, 0, sizeof( DXARC ) ) ;
	return 0 ;
}

/** �A�[�J�C�u�t�@�C���������ׂ̍\���̂̌�n��������
 *
 *
 */
int		DXA_Terminate( DXARC *DXA )
{
	DXA_CloseArchive( DXA ) ;
	memset( DXA, 0, sizeof( DXARC ) ) ;
	return 0 ;
}

/** ��������ɂ���A�[�J�C�u�t�@�C���C���[�W���J��( 0:����  -1:���s )
 *
 *
 */
int		DXA_OpenArchiveFromMem( DXARC *DXA, void *ArchiveImage, int ArchiveSize, const char *KeyString )
{
	BYTE *datp ;
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->FilePointer != DXA_FILE_NO_SET ) return -1;
	// ���̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	// �ŏ��Ƀw�b�_�̕����𔽓]����
	memcpy( &DXA->Head, ArchiveImage, sizeof( DXARC_HEAD ) ) ;
	DXA_KeyConv( &DXA->Head, sizeof( DXARC_HEAD ), 0, DXA->Key ) ;
	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( DXA->Head.Head != DXAHEAD )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		memset( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;
		memcpy( &DXA->Head, ArchiveImage, sizeof( DXARC_HEAD ) ) ;
		DXA_KeyConv( &DXA->Head, sizeof( DXARC_HEAD ), 0, DXA->Key ) ;
		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( DXA->Head.Head != DXAHEAD )	goto ERR ;
	}
	// ���ׂẴf�[�^�𔽓]����
	DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
	// �|�C���^��ۑ�
	DXA->FilePointer = (DWORD)ArchiveImage ;
	datp = (BYTE *)ArchiveImage ;
	// �w�b�_����͂���
	{
		memcpy( &DXA->Head, datp, sizeof( DXARC_HEAD ) ) ;
		datp += sizeof( DXARC_HEAD ) ;
		// �h�c�̌���
		if( DXA->Head.Head != DXAHEAD ) goto ERR ;
		// �o�[�W��������
		if( DXA->Head.Version > DXAVER ) goto ERR ;
		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.Top				= (BYTE *)DXA->FilePointer + DXA->Head.FileNameTableStartAddress ;
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
	}
	// �J�����g�f�B���N�g���̃Z�b�g
	DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	DXA->MemoryOpenFlag					= TRUE ;			// �������C���[�W����J���Ă���t���O�𗧂Ă�
	DXA->UserMemoryImageFlag			= TRUE ;			// ���[�U�[�̃C���[�W����J�����t���O�𗧂Ă�
	DXA->MemoryImageSize				= ArchiveSize ;		// �T�C�Y���ۑ����Ă���
	return 0 ;
ERR :
	// ���]�����f�[�^�����ɖ߂�
	DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
	return -1 ;
}

/** �A�[�J�C�u�t�@�C�����������������������𓾂�( TRUE:�����Ă���  FALSE:�����Ă��Ȃ� )
 *
 *
 */
int		DXA_CheckIdle( DXARC *DXA )
{
	// �񓯊��I�[�v�����ł͂Ȃ���Γ��ɂ�邱�Ƃ͂Ȃ�
	if( DXA->ASyncOpenFlag == FALSE ) return TRUE ;
	// �t�@�C���ǂݍ��݂��������Ă��邩���ׂ�
	//if( WinFileAccessIdleCheck( DXA->ASyncOpenFilePointer ) == FALSE ) return FALSE ;
	// �t�@�C�������
	//WinFileAccessClose( DXA->ASyncOpenFilePointer ) ;
	DXA->ASyncOpenFilePointer = 0;
	// ���ׂẴf�[�^�𔽓]����
	DXA_KeyConv( (void *)DXA->FilePointer, DXA->MemoryImageSize, 0, DXA->Key ) ;
	// �񓯊��I�[�v�����t���O��|��
	DXA->ASyncOpenFlag = FALSE ;
	return TRUE ;
}

/** �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
 *
 *
 */
int		DXA_OpenArchiveFromFileUseMem( DXARC *DXA, const char *ArchivePath, const char *KeyString , int ASync )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->FilePointer != DXA_FILE_NO_SET) return -1 ;
	// ���̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	// �w�b�_����������ɓǂݍ���
	DXA->ASyncOpenFilePointer = 0;
	DXA->FilePointer = 0;
	//DXA->ASyncOpenFilePointer = WinFileAccessOpen( ArchivePath, FALSE, TRUE, TRUE ) ;
	if( DXA->ASyncOpenFilePointer == 0) return -1 ;
	// �t�@�C���̃T�C�Y���擾����
	//WinFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_END ) ;
	//DXA->MemoryImageSize = WinFileAccessTell( DXA->ASyncOpenFilePointer ) ;
	//WinFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
	// �t�@�C���̓��e��S�ă������ɓǂݍ��ވׂ̃������̈�̊m��
	DXA->FilePointer = (DWORD)DXALLOC( DXA->MemoryImageSize ) ;
	// �w�b�_����͂���
	{
		DXA_KeyConvFileRead( &DXA->Head, sizeof( DXARC_HEAD ), DXA->ASyncOpenFilePointer, DXA->Key ,-1) ;
		// �h�c�̌���
		if( DXA->Head.Head != DXAHEAD )
		{
			// �o�[�W�����Q�ȑO�����ׂ�
			memset( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;
			//WinFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
			DXA_KeyConvFileRead( &DXA->Head, sizeof( DXARC_HEAD ), DXA->ASyncOpenFilePointer, DXA->Key,-1) ;
			// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
			if( DXA->Head.Head != DXAHEAD )	goto ERR ;
		}
		// �o�[�W��������
		if( DXA->Head.Version > DXAVER ) goto ERR ;
		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL ) goto ERR ;
		// ���e�[�u�����������ɓǂݍ���
		//WinFileAccessSeek( DXA->ASyncOpenFilePointer, DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key,-1 ) ;
		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
	}
	// �J�����g�f�B���N�g���̃Z�b�g
	DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	// ���߂ăt�@�C�����ۂ��Ɠǂݍ���
	//WinFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
	//WinFileAccessRead( (void *)DXA->FilePointer, DXA->MemoryImageSize, 1, DXA->ASyncOpenFilePointer );
	// �t�@�C���񓯊��I�[�v�������Ƃ������Ƃ��Z�b�g���Ă���
	DXA->ASyncOpenFlag = TRUE ;
	// �����I�[�v���̏ꍇ�͂����ŊJ���I���̂�҂�
	if( ASync == FALSE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
/*
	// �t�@�C���̓��e��S�ă������ɓǂݍ���
	{
		fp = WinFileAccessOpen( ArchivePath, FALSE, TRUE, FALSE ) ;
		if( fp == NULL ) return -1 ;
		WinFileAccessSeek( fp, 0L, SEEK_END ) ;
		DXA->MemoryImageSize = WinFileAccessTell( fp ) ;
		WinFileAccessSeek( fp, 0L, SEEK_SET ) ;
		DXA->FilePointer = (DWORD)DXALLOC( ArchiveSize ) ;
		if( DXA->FilePointer == 0 )
		{
			WinFileAccessClose( fp ) ;
			return -1 ;
		}
		WinFileAccessRead( (void *)DXA->FilePointer, ArchiveSize, 1, fp ) ;
		WinFileAccessClose( fp ) ;
	}
	// �ŏ��Ƀw�b�_�̕����𔽓]����
	memcpy( &DXA->Head, (void *)DXA->FilePointer, sizeof( DXARC_HEAD ) ) ;
	DXA_KeyConv( &DXA->Head, sizeof( DXARC_HEAD ), 0, DXA->Key ) ;
	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( DXA->Head.Head != DXAHEAD )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		memset( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;
		memcpy( &DXA->Head, (void *)DXA->FilePointer, sizeof( DXARC_HEAD ) ) ;
		DXA_KeyConv( &DXA->Head, sizeof( DXARC_HEAD ), 0, DXA->Key ) ;
		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( DXA->Head.Head != DXAHEAD )
			goto ERR ;
	}
	// ���ׂẴf�[�^�𔽓]����
	DXA_KeyConv( (void *)DXA->FilePointer, ArchiveSize, 0, DXA->Key ) ;
	// �w�b�_����͂���
	{
		memcpy( &DXA->Head, (BYTE *)DXA->FilePointer, sizeof( DXARC_HEAD ) ) ;
		
		// �h�c�̌���
		if( DXA->Head.Head != DXAHEAD ) goto ERR ;
		
		// �o�[�W��������
		if( DXA->Head.Version > DXAVER ) goto ERR ;
		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.Top				= (BYTE *)DXA->FilePointer + DXA->Head.FileNameTableStartAddress ;
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
	}
	// �J�����g�f�B���N�g���̃Z�b�g
	DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
*/
	DXA->MemoryOpenFlag					= TRUE ;			// �������C���[�W����J���Ă���t���O�𗧂Ă�
	DXA->UserMemoryImageFlag			= FALSE ;			// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	return 0 ;
ERR :
	if( DXA->ASyncOpenFilePointer )
	{
		//WinFileAccessClose( DXA->ASyncOpenFilePointer );
		DXA->ASyncOpenFilePointer = 0;
	}
	if( DXA->FilePointer )
	{
		DXFREE( (void *)DXA->FilePointer );
		DXA->FilePointer = 0;
	}
	DXA->ASyncOpenFlag = FALSE ;
	return -1 ;
}

/** �A�[�J�C�u�t�@�C�����J��( 0:����  -1:���s )
 *
 *
 */
int		DXA_OpenArchiveFromFile( DXARC *DXA, const char *ArchivePath, const char *KeyString )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->FilePointer != DXA_FILE_NO_SET) return -1 ;
	// �A�[�J�C�u�t�@�C�����J�����Ǝ��݂�
	//DXA->FilePointer = WinFileAccessOpen( ArchivePath, FALSE, TRUE, TRUE ) ;
	if( DXA->FilePointer == DXA_FILE_NO_SET ) return -1 ;
	// ��������̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;
	// �w�b�_����͂���
	{
		DXA_KeyConvFileRead( &DXA->Head, sizeof( DXARC_HEAD ), DXA->FilePointer, DXA->Key,-1) ;
		// �h�c�̌���
		if( DXA->Head.Head != DXAHEAD )
		{
			// �o�[�W�����Q�ȑO�����ׂ�
			memset( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;
			//WinFileAccessSeek( DXA->FilePointer, 0L, SEEK_SET ) ;
			DXA_KeyConvFileRead( &DXA->Head, sizeof( DXARC_HEAD ), DXA->FilePointer, DXA->Key,-1 ) ;
			// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
			if( DXA->Head.Head != DXAHEAD )	goto ERR ;
		}
		// �o�[�W��������
		if( DXA->Head.Version > DXAVER ) goto ERR ;
		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL ) goto ERR ;
		// ���e�[�u�����������ɓǂݍ���
		//WinFileAccessSeek( DXA->FilePointer, DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->FilePointer, DXA->Key,-1 ) ;
		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
	}
	// �J�����g�f�B���N�g���̃Z�b�g
	DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	DXA->MemoryOpenFlag					= FALSE ;			// �������C���[�W����J���Ă���t���O��|��
	DXA->UserMemoryImageFlag			= FALSE ;			// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	return 0 ;
ERR :
	if( DXA->FilePointer != DXA_FILE_NO_SET )
	{
		//WinFileAccessClose( DXA->FilePointer ) ;
		DXA->FilePointer = DXA_FILE_NO_SET ;
	}
	if( DXA->Table.Top != NULL )
	{
		DXFREE( DXA->Table.Top ) ;
		DXA->Table.Top = NULL ;
	}
	return -1 ;
}

/** �A�[�J�C�u�t�@�C�������
 *
 *
 */
int		DXA_CloseArchive( DXARC *DXA )
{
	// ���ɕ��Ă����牽�������I��
	if( DXA->FilePointer == DXA_FILE_NO_SET) return 0 ;
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// ����������J���Ă��邩�ǂ����ŏ����𕪊�
	if( DXA->MemoryOpenFlag == TRUE )
	{
		// �A�[�J�C�u�v���O�������������ɓǂݍ��񂾏ꍇ�Ƃ����łȂ��ꍇ�ŏ����𕪊�
		if( DXA->UserMemoryImageFlag == TRUE )
		{
			// ���[�U�[����n���ꂽ�f�[�^�������ꍇ
			// ���]�����f�[�^�����ɖ߂�
			DXA_KeyConv( (void *)DXA->FilePointer, DXA->MemoryImageSize, 0, DXA->Key ) ;
		}
		else
		{
			// �A�[�J�C�u�v���O�������������ɓǂݍ��񂾏ꍇ
			// �m�ۂ��Ă������������J������
			DXFREE( (void *)DXA->FilePointer ) ;
		}
	}
	else
	{
		// �A�[�J�C�u�t�@�C�������
		//WinFileAccessClose( DXA->FilePointer ) ;
		// ���e�[�u�����i�[���Ă����������̈�����
		DXFREE( DXA->Table.Top ) ;
	}
	memset( DXA, 0, sizeof( DXARC ) ) ;// ������
	return 0 ;
}

/** �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
 *
 *
 */
int		DXA_ChangeCurrentDirectoryFast( DXARC *DXA, DXARC_SEARCHDATA *SearchData )
{
	DXARC_FILEHEAD *FileH ;
	int i, j, k, Num ;
	BYTE *NameData, *PathData ;
	WORD PackNum, Parity ;
	DWORD FileHeadSize ;
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	PackNum  = SearchData->PackNum ;
	Parity   = SearchData->Parity ;
	PathData = SearchData->FileName ;
	// �J�����g�f�B���N�g�����瓯���̃f�B���N�g����T��
	FileH = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
	Num = (int)DXA->CurrentDirectory->FileHeadNum ;
	FileHeadSize = DXA->Head.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 ) continue ;
		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( PackNum != ((WORD *)NameData)[0] || Parity != ((WORD *)NameData)[1] ) continue ;
		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			if( *((DWORD *)&PathData[k]) != *((DWORD *)&NameData[k]) ) break ;
		// �K�������f�B���N�g�����������炱���ŏI��
		if( PackNum == j ) break ;
	}
	// ����������G���[
	if( i == Num ) return -1 ;
	// �݂�����J�����g�f�B���N�g����ύX
	DXA->CurrentDirectory = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + FileH->DataAddress ) ;
	return 0 ;
}

/** �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
 *
 *
 */
int		DXA_ChangeCurrentDirectoryBase( DXARC *DXA, const char *DirectoryPath, bool ErrorIsDirectoryReset, DXARC_SEARCHDATA *LastSearchData )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_SEARCHDATA SearchData ;
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// �����ɗ��܂�p�X�������疳��
	if( strcmp( DirectoryPath, "." ) == 0 ) return 0 ;
	// �w\�x�����̏ꍇ�̓��[�g�f�B���N�g���ɖ߂�
	if( strcmp( DirectoryPath, "\\" ) == 0 )
	{
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
		return 0 ;
	}
	// ���Ɉ������p�X�������珈���𕪊�
	if( strcmp( DirectoryPath, ".." ) == 0 )
	{
		// ���[�g�f�B���N�g���ɋ�����G���[
		if( DXA->CurrentDirectory->ParentDirectoryAddress == 0xffffffff ) return -1 ;
		// �e�f�B���N�g�����������炻����Ɉڂ�
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + DXA->CurrentDirectory->ParentDirectoryAddress ) ;
		return 0 ;
	}
	// ����ȊO�̏ꍇ�͎w��̖��O�̃f�B���N�g����T��
	// �ύX�ȑO�̃f�B���N�g����ۑ����Ă���
	OldDir = DXA->CurrentDirectory ;
	// �p�X���Ɂw\�x�����邩�ǂ����ŏ����𕪊�
	if(strchr( DirectoryPath, '\\' ) == NULL )
	{
		// �t�@�C������������p�̌`���ɕϊ�����
		DXA_ConvSearchData( &SearchData, DirectoryPath, NULL ) ;
		// �f�B���N�g����ύX
		if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 ) goto ERR ;
	}
	else
	{
		// \ ������ꍇ�͌q�������f�B���N�g������ÂύX���Ă䂭
		int Point, StrLength ;
		Point = 0 ;
		// ���[�v
		for(;;)
		{
			// ��������擾����
			DXA_ConvSearchData( &SearchData, &DirectoryPath[Point], &StrLength ) ;
			Point += StrLength ;
			// ���������[�� \ �������ꍇ�̓��[�g�f�B���N�g���ɗ��Ƃ�
			if( StrLength == 0 && DirectoryPath[Point] == '\\' )
			{
				DXA_ChangeCurrentDirectoryBase( DXA, "\\", false , NULL) ;
			}
			else
			{
				// ����ȊO�̏ꍇ�͕��ʂɃf�B���N�g���ύX
				if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 )
				{
					// �G���[���N���āA�X�ɃG���[���N�������Ɍ��̃f�B���N�g���ɖ߂���
					// �t���O�������Ă���ꍇ�͌��̃f�B���N�g���ɖ߂�
					if( ErrorIsDirectoryReset == true ) DXA->CurrentDirectory = OldDir ;
					// �G���[�I��
					goto ERR ;
				}
			}
			// �����I�[�����ŏI�������ꍇ�̓��[�v���甲����
			// ���͂��� \ �����Ȃ��ꍇ�����[�v���甲����
			if( DirectoryPath[Point] == '\0' || ( DirectoryPath[Point] == '\\' && DirectoryPath[Point+1] == '\0' ) ) break ;
			Point ++ ;
		}
	}
	if( LastSearchData != NULL )
	{
		memcpy( LastSearchData->FileName, SearchData.FileName, SearchData.PackNum * 4 ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}
	return 0 ;// ����I��
ERR:
	if( LastSearchData != NULL )
	{
		memcpy( LastSearchData->FileName, SearchData.FileName, SearchData.PackNum * 4 ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}
	return -1 ;// �G���[�I��
}

/** �t�@�C���̏��𓾂�
 *
 *
 */
DXARC_FILEHEAD *DXA_GetFileInfo( DXARC *DXA, const char *FilePath )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_FILEHEAD *FileH ;
	DWORD FileHeadSize ;
	BYTE *NameData ;
	int i, j, k, Num ;
	DXARC_SEARCHDATA SearchData ;
	OldDir = DXA->CurrentDirectory ;		// ���̃f�B���N�g����ۑ����Ă���
	if(strchr( FilePath, '\\' ) != NULL )	// �t�@�C���p�X�� \ ���܂܂�Ă���ꍇ�A�f�B���N�g���ύX���s��
	{
		// �J�����g�f�B���N�g����ړI�̃t�@�C��������f�B���N�g���ɕύX����
		if( DXA_ChangeCurrentDirectoryBase( DXA, FilePath, false, &SearchData ) >= 0 )
		{
			// �G���[���N���Ȃ������ꍇ�̓t�@�C�������f�B���N�g�����������ƂɂȂ�̂ŃG���[
			goto ERR ;
		}
	}
	else
	{
		// �t�@�C�����������p�f�[�^�ɕϊ�����
		DXA_ConvSearchData( &SearchData, FilePath, NULL ) ;
	}
	// �����̃t�@�C����T��
	FileHeadSize = DXA->Head.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ;
	FileH        = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
	Num          = ( int )DXA->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ) continue ;
		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( SearchData.PackNum != ((WORD *)NameData)[0] || SearchData.Parity != ((WORD *)NameData)[1] ) continue ;
		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
			if( *((DWORD *)&SearchData.FileName[k]) != *((DWORD *)&NameData[k]) ) break ;
		// �K�������t�@�C�����������炱���ŏI��
		if( SearchData.PackNum == j ) break ;
	}
	// ����������G���[
	if( i == Num ) goto ERR ;
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectory = OldDir ;
	// �ړI�̃t�@�C���̃A�h���X��Ԃ�
	return FileH ;
ERR :
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectory = OldDir ;
	return NULL ;// �G���[�I��
}

/** �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
 *
 *
 */
int		DXA_ChangeCurrentDir( DXARC *DXA, const char *DirPath )
{
	return DXA_ChangeCurrentDirectoryBase( DXA, DirPath, true ,NULL) ;
}

/** �A�[�J�C�u���̃J�����g�f�B���N�g���p�X���擾����
 *
 *
 */
int		DXA_GetCurrentDir( DXARC *DXA, char *DirPathBuffer, int BufferSize )
{
	char DirPath[256] ;
	DXARC_DIRECTORY *Dir[200], *DirTempP ;
	int Depth, i ;
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// ���[�g�f�B���N�g���ɒ����܂Ō�������
	Depth = 0 ;
	DirTempP = DXA->CurrentDirectory ;
	while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
	{
		Dir[Depth] = DirTempP ;
		DirTempP = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + DirTempP->ParentDirectoryAddress ) ;
		Depth ++ ;
	}
	// �p�X����A������
	DirPath[0] = '\0' ;
	for( i = Depth - 1 ; i >= 0 ; i -- )
	{
		strcat( DirPath, "\\" ) ;
		strcat( DirPath, (char *)DXA->Table.NameTable + ((DXARC_FILEHEAD *)( DXA->Table.FileTable + Dir[i]->DirectoryAddress ))->NameAddress ) ;
	}
	// �o�b�t�@�̒������O���A����������Ȃ��Ƃ��̓f�B���N�g�����̒�����Ԃ�
	if( BufferSize == 0 || BufferSize < (int)strlen( DirPath ) )
	{
		return strlen( DirPath ) + 1 ;
	}
	else
	{
		// �f�B���N�g�������o�b�t�@�ɓ]������
		strcpy( DirPathBuffer, DirPath ) ;
	}
	return 0 ;
}

/** �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ -1�ȊO:DXA�����n���h�� )
 *
 *
 */
int		DXA_FindFirst( DXARC *DXA, const char *FilePath, FILEINFO *Buffer )
{
	DXA_FINDDATA *find;
	char Dir[256], Name[256];
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// �������̊m��
	find = (DXA_FINDDATA *)DXALLOC( sizeof( *find ) );
	if( find == NULL ) return -1;
	find->Container = DXA;
	DXA_DIR_AnalysisFileNameAndDirPath( FilePath, Name, Dir );
	// �S�đ啶���ɂ���
	strupr( Dir );
	strupr( Name );
	// �����Ώۂ̃f�B���N�g�����擾
	if( Dir[0] == '\0' )
	{
		find->Directory = DXA->CurrentDirectory;
	}
	else
	{
		DXARC_DIRECTORY *OldDir;
		OldDir = DXA->CurrentDirectory;
		// �w��̃f�B���N�g���������ꍇ�̓G���[
		if( DXA_ChangeCurrentDirectoryBase( DXA, Dir, false ,NULL) == -1 )
		{
			DXFREE( find );
			return -1;
		}
		find->Directory = DXA->CurrentDirectory;
		DXA->CurrentDirectory = OldDir;
	}
	find->ObjectCount = 0;
	strcpy( find->SearchStr, Name );
	// �K������ŏ��̃t�@�C������������
	if( DXA_FindProcess( find, Buffer ) == -1 )
	{
		DXFREE( find );
		return -1;
	}
	find->ObjectCount ++ ;
	return (int)find;// �n���h����Ԃ�
}

/** �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ 0:���� )
 *
 *
 */
int		DXA_FindNext( int DxaFindHandle, FILEINFO *Buffer )
{
	DXA_FINDDATA *find;
	find = (DXA_FINDDATA *)DxaFindHandle;
	if( DXA_FindProcess( find, Buffer ) == -1 ) return -1;
	find->ObjectCount ++ ;
	return 0;
}

/** �A�[�J�C�u���̃I�u�W�F�N�g�������I������
 *
 *
 */
int		DXA_FindClose( int DxaFindHandle )
{
	DXA_FINDDATA *find;
	find = (DXA_FINDDATA *)DxaFindHandle;
	DXFREE( find );
	return 0;
}

/** �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����������ɓǂݍ���( -1:�G���[ 0�ȏ�:�t�@�C���T�C�Y )
 *
 *
 */
int		DXA_LoadFile( DXARC *DXA, const char *FilePath, void *Buffer, unsigned int BufferSize )
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
	// �t�@�C���T�C�Y������Ă��邩���ׂ�A����Ă��Ȃ����A�o�b�t�@�A���̓T�C�Y���O��������T�C�Y��Ԃ�
	if( BufferSize < FileH->DataSize || BufferSize == 0 || Buffer == NULL )
	{
		return ( int )FileH->DataSize ;
	}
	// ����Ă���ꍇ�̓o�b�t�@�[�ɓǂݍ���
	// �t�@�C�������k����Ă��邩�ǂ����ŏ����𕪊�
	if( DXA->Head.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
	{
		// ���k����Ă���ꍇ
		// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
		if( DXA->MemoryOpenFlag == TRUE )
		{
			// ��������̈��k�f�[�^���𓀂���
			DXA_Decode( (BYTE *)DXA->FilePointer + DXA->Head.DataStartAddress + FileH->DataAddress, Buffer ) ;
		}
		else
		{
			void *temp ;
			// ���k�f�[�^���������ɓǂݍ���ł���𓀂���
			// ���k�f�[�^�����܂郁�����̈�̊m��
			temp = DXALLOC( FileH->PressDataSize ) ;
			// ���k�f�[�^�̓ǂݍ���
			//WinFileAccessSeek( DXA->FilePointer, DXA->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
			DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->FilePointer, DXA->Key,-1 ) ;
			// ��
			DXA_Decode( temp, Buffer ) ;
			// �������̉��
			DXFREE( temp ) ;
		}
	}
	else
	{
		if( DXA->MemoryOpenFlag == TRUE )
		{
			// �R�s�[
			memcpy( Buffer, (BYTE *)DXA->FilePointer + DXA->Head.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
		}
		else
		{
			// �t�@�C���|�C���^���ړ�
			//WinFileAccessSeek( DXA->FilePointer, DXA->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
			// �ǂݍ���
			DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->FilePointer, DXA->Key,-1 ) ;
		}
	}
	return 0 ;
}

/** �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( DXA_OpenArchiveFromFileUseMem �Ⴕ���� DXA_OpenArchiveFromMem �ŊJ�����ꍇ�ɗL�� )
 *
 *
 */
void	*DXA_GetFileImage( DXARC *DXA )
{
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE ) Sleep(0);
	}
	// �������C���[�W����J���Ă��Ȃ�������G���[
	if( DXA->MemoryOpenFlag == FALSE ) return NULL ;
	return (void *)DXA->FilePointer ;// �擪�A�h���X��Ԃ�
}

/** �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
 *
 *
 */
/*int		DXA_GetFileInfo( DXARC *DXA, const char *FilePath, int *Position, int *Size )
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
	// �t�@�C���̃f�[�^������ʒu�ƃt�@�C���T�C�Y��ۑ�����
	if( Position != NULL ) *Position = DXA->Head.DataStartAddress + FileH->DataAddress ;
	if( Size     != NULL ) *Size     = FileH->DataSize ;
	return 0 ;
}*/

/** �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
 *
 *
 */
int		DXA_DIR_ConvertFullPath( const char *Src, char *Dest )
{
	int i, j, k ;
	char iden[256], CurrentDir[MAX_PATH] ;
	// �J�����g�f�B���N�g���𓾂�
	//GetCurrentDirectory( MAX_PATH, CurrentDir ) ;
	strupr( CurrentDir ) ;
	if( Src == NULL )
	{
		strcpy( Dest, CurrentDir ) ;
		goto END ;
	}
	i = 0 ;
	j = 0 ;
	k = 0 ;
	// �ŏ��Ɂw\�x���́w/�x���Q��A���ő����Ă���ꍇ�̓l�b�g���[�N����Ă���Ɣ��f
	if( ( Src[0] == '\\' && Src[1] == '\\' ) ||
		( Src[0] == '/'  && Src[1] == '/'  ) )
	{
		Dest[0] = '\\';
		Dest[1] = '\0';
		i += 2;
		j ++ ;
	}
	else
	// �ŏ����w\�x���́w/�x�̏ꍇ�̓J�����g�h���C�u�̃��[�g�f�B���N�g���܂ŗ�����
	if( Src[0] == '\\' || Src[0] == '/' )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = '\0' ;

		i ++ ;
		j = 2 ;
	}
	else
	// �h���C�u����������Ă����炻�̃h���C�u��
	if( Src[1] == ':' )
	{
		Dest[0] = CHARUP(Src[0]) ;
		Dest[1] = Src[1] ;
		Dest[2] = '\0' ;
		i = 2 ;
		j = 2 ;
		if( Src[i] == '\\' ) i ++ ;
	}
	else
	// ����ȊO�̏ꍇ�̓J�����g�f�B���N�g��
	{
		strcpy( Dest, CurrentDir ) ;
		//j = lstrlen( Dest ) ;
		j = strlen( Dest ) ;
		if( Dest[j-1] == '\\' || Dest[j-1] == '/' )
		{
			Dest[j-1] = '\0' ;
			j -- ;
		}
	}
	for(;;)
	{
		switch( Src[i] )
		{
		case '\0' :
			if( k != 0 )
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			// �����񂪖���������X�L�b�v
			if( k == 0 )
			{
				i ++ ;
				break;
			}
			if( strcmp( iden, "." ) == 0 )
			{
				// �Ȃɂ����Ȃ�
			}
			else
			if( strcmp( iden, ".." ) == 0 )
			{
				// ����̃f�B���N�g����
				j -- ;
				while( Dest[j] != '\\' && Dest[j] != '/' && Dest[j] != ':' ) j -- ;
				if( Dest[j] != ':' ) Dest[j] = '\0' ;
				else j ++ ;
			}
			else
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
			}
			k = 0 ;
			i ++ ;
			break ;
		default :
			if( CHECKMULTIBYTECHAR( &Src[i] ) == FALSE  )
			{
				iden[k] = CHARUP(Src[i]) ;
				iden[k+1] = '\0' ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*((WORD *)&iden[k]) = *((WORD *)&Src[i]) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
			break ;
		}
	}
END :
	return 0 ;//����I��
}

/** �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
 * �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
 * DirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
 */
int		DXA_DIR_AnalysisFileNameAndDirPath( const char *Src, char *FileName, char *DirPath )
{
	int i, Last ;
	// �t�@�C�����𔲂��o��
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( CHECKMULTIBYTECHAR( &Src[i] ) == FALSE )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) strcpy( FileName, &Src[Last+1] ) ;
		else             strcpy( FileName, Src ) ;
	}
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			memcpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	return 0 ;
}

/** CmpStr �̏����� Src ���K�����邩�ǂ����𒲂ׂ�( 0:�K������  -1:�K�����Ȃ� )
 *
 *
 */
int		DXA_DIR_FileNameCmp( const char *Src, const char *CmpStr )
{
	const char *s, *c;
	s = Src;
	c = CmpStr;
	while( *c != '\0' || *s != '\0' )
	{
		if( CHECKMULTIBYTECHAR( c ) == TRUE )
		{
			if( *((WORD *)s) != *((WORD *)c) ) return -1;
			c += 2;
			s += 2;
		}
		else
		{
			switch( *c )
			{
			case '?':
				c ++ ;
				s ++ ;
				break;

			case '*':
				while( *c == '*' ) c ++ ;
				if( *c == '\0' ) return 0;
				while( *s != '\0' && *s != *c ) s ++ ;
				if( *s == '\0' ) return -1;
				c ++ ;
				s ++ ;
				break;
			default:
				if( *c != *s ) return -1;
				c ++ ;
				s ++ ;
				break;
			}
		}
		if( ( *c == '\0' && *s != '\0' ) || ( *c != '\0' && *s == '\0' ) ) return -1;
	}
	return 0;
}

/** �A�[�J�C�u�t�@�C�����J��
 *
 *
 */
int		DXA_DIR_OpenArchive( const char *FilePath, int ArchiveIndex, int OnMemory, int ASync )
{
	int i, index, newindex ;
	DXARC_DIR_ARCHIVE *arc ;
	DXARC temparc ;
	// �A�[�J�C�u�̎w�肪����ꍇ�͂��̂܂܎g�p����
	if( ArchiveIndex != -1 )
	{
		if( DXARCD.Archive[ArchiveIndex] != NULL && strcmp( FilePath, DXARCD.Archive[ArchiveIndex]->Path ) == 0 )
		{
			DXARCD.Archive[ArchiveIndex]->UseCounter ++ ;
			return ArchiveIndex ;
		}
	}
	// ���ɊJ����Ă��邩���ׂ�
	newindex = -1 ;
	index    = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[index] ;
		if( arc == NULL )
		{
			newindex = index ;
			continue ;
		}
		i ++ ;
		if( strcmp( arc->Path, FilePath ) == 0 )
		{
			// ���ɊJ����Ă����ꍇ�͂��̃C���f�b�N�X��Ԃ�
			arc->UseCounter ++ ;
			return index ;
		}
	}
	// �Ȃ������ꍇ�͐V�K�Ƀf�[�^��ǉ�����
	// �n���h�������Ɉ�t�̏ꍇ�͎g�p����Ă��Ȃ��A�[�J�C�u�n���h�����������
	if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
	{
		// ���g�p�̃n���h�������
		DXA_DIR_CloseWaitArchive() ;
		// ����ł���t�ł���ꍇ�̓G���[
		if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
			return -1 ;
	} 
	if( newindex == -1 )
	{
		for( newindex = 0 ; DXARCD.Archive[newindex] != NULL ; newindex ++ ){}
	}
	// �A�[�J�C�u�t�@�C�������݂��Ă��邩�m�F���Ă珉��������
	DXA_Initialize( &temparc ) ;
	if( OnMemory == TRUE )
	{
		// �������ɓǂݍ��ޏꍇ
		if( DXA_OpenArchiveFromFileUseMem( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL, ASync ) < 0 )
			return -1 ;
	}
	else
	{
		// �t�@�C������ǂݍ��ޏꍇ
		if( DXA_OpenArchiveFromFile( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL ) < 0 )
			return -1 ;
	}
	// �V�����A�[�J�C�u�f�[�^�p�̃��������m�ۂ���
	arc = DXARCD.Archive[newindex] = (DXARC_DIR_ARCHIVE *)DXALLOC( sizeof( DXARC_DIR_ARCHIVE ) ) ;
	if( DXARCD.Archive[newindex] == NULL )
	{
		DXA_CloseArchive( &temparc ) ;
		DXA_Terminate( &temparc ) ;
		return -1 ;
	}
	// ���Z�b�g
	memcpy( &arc->Archive, &temparc, sizeof( DXARC ) ) ;
	arc->UseCounter = 1 ;
	strcpy( arc->Path, FilePath ) ;
	// �g�p���̃A�[�J�C�u�̐��𑝂₷
	DXARCD.ArchiveNum ++ ;
	return newindex ;// �C���f�b�N�X��Ԃ�
}

/** ���ɊJ����Ă���A�[�J�C�u�̃n���h�����擾����( �߂�l: -1=�������� 0�ȏ�:�n���h�� )
 *
 *
 */
int		DXA_DIR_GetArchive( const char *FilePath )
{
	int i, index ;
	DXARC_DIR_ARCHIVE *arc ;
	index = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[index] ;
		if( arc == NULL ) continue ;
		i ++ ;
		if( strcmp( arc->Path, FilePath ) == 0 )
			return index ;
	}
	return -1 ;
}

/** �A�[�J�C�u�t�@�C�������
 *
 *
 */
int		DXA_DIR_CloseArchive( int ArchiveHandle )
{
	DXARC_DIR_ARCHIVE *arc ;
	// �g�p����Ă��Ȃ������牽�������I��
	arc = DXARCD.Archive[ArchiveHandle] ;
	if( arc == NULL || arc->UseCounter == 0 ) return -1 ;
	// �Q�ƃJ�E���^�����炷
	arc->UseCounter -- ;
	return 0 ;
}

/** �g�p�����̂�҂��Ă���A�[�J�C�u�t�@�C����S�ĕ���
 *
 *
 */
void	DXA_DIR_CloseWaitArchive( void )
{
	int i, Num, index ;
	DXARC_DIR_ARCHIVE *arc ;
	
	Num = DXARCD.ArchiveNum ;
	for( i = 0, index = 0 ; i < Num ; arc ++, index ++ )
	{
		if( DXARCD.Archive[index] == NULL ) continue ;
		i ++ ;

		arc = DXARCD.Archive[index] ;

		// �g���Ă����������Ȃ�
		if( arc->UseCounter > 0 ) continue ;

		// ��n��
		DXA_CloseArchive( &arc->Archive ) ;
		DXA_Terminate( &arc->Archive ) ;
		DXFREE( arc ) ;
		DXARCD.Archive[index] = NULL ;
		
		// �A�[�J�C�u�̐������炷
		DXARCD.ArchiveNum -- ;
	}
}

/** �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̏�����
 *
 *
 */
int		DXA_DIR_Initialize( void )
{
	memset( &DXARCD, 0, sizeof( DXARC_DIR ) ) ;
	return 0 ;
}

/** �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̌�n��
 *
 *
 */
int		DXA_DIR_Terminate( void )
{
	// �g�p����Ă��Ȃ��A�[�J�C�u�t�@�C�����������
	DXA_DIR_CloseWaitArchive() ;
	return 0 ;
}

/** �A�[�J�C�u�t�@�C���̊g���q��ݒ肷��
 *
 *
 */
int		DXA_DIR_SetArchiveExtension( const char *Extension )
{
	int Length ;
	Length = strlen( Extension ) ;
	if( Length >= 64 || Extension == NULL || Extension[0] == '\0' )
	{
		DXARCD.ArchiveExtension[0] = 0 ;
		DXARCD.ArchiveExtensionLength = 0 ;
	}
	else
	{
		DXARCD.ArchiveExtensionLength = Length ;
		memcpy( DXARCD.ArchiveExtension, Extension, Length + 1 ) ;
	}
	return 0 ;
}

/** �A�[�J�C�u�t�@�C���̌��������ݒ肷��
 *
 *
 */
int		DXA_DIR_SetKeyString( const char *KeyString )
{
	if( KeyString == NULL )
	{
		DXARCD.ValidKeyString = FALSE ;
	}
	else
	{
		DXARCD.ValidKeyString = TRUE ;
		if( strlen( KeyString ) > DXA_KEYSTR_LENGTH )
		{
			memcpy( DXARCD.KeyString, KeyString, DXA_KEYSTR_LENGTH ) ;
			DXARCD.KeyString[ DXA_KEYSTR_LENGTH ] = '\0' ;
		}
		else
		{
			strcpy( DXARCD.KeyString, KeyString ) ;
		}
	}
	return 0 ;
}

/** �t�@�C���|�C���^�̈ʒu���擾����
 *
 *
 */
long	DXA_DIR_Tell( int Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1 ;
	//if( file->UseArchiveFlag == 0 )	return WinFileAccessTell( file->FilePointer ) ;
	//else							return DXA_STREAM_Tell( &file->DXAStream ) ;
	return 0;
}

/** �t�@�C���̏I�[�𒲂ׂ�
 *
 *
 */
int		DXA_DIR_Eof( int Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1 ;
	//if( file->UseArchiveFlag == 0 ) return WinFileAccessEof( file->FilePointer ) ;
	//else							return DXA_STREAM_Eof( &file->DXAStream ) ;
	return 0;
}

/**
 *
 *
 */
int		DXA_DIR_ChDir( const char *Path )
{
	//::SetCurrentDirectory( Path ) ;
	return 0 ;
}

/**
 *
 *
 */
int		DXA_DIR_GetDir( char *Buffer )
{
	//::GetCurrentDirectory( 256, Buffer ) ;
	return 0 ;
}

/**
 *
 *
 */
int		DXA_DIR_IdleCheck( int Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1 ;
	//if( file->UseArchiveFlag == 0 )	return WinFileAccessIdleCheck( file->FilePointer ) ;
	//else							return DXA_STREAM_IdleCheck( &file->DXAStream ) ;
	return 0;
}

/** �߂�l: -1=�G���[  -1�ȊO=FindHandle
 *
 *
 */
int		DXA_DIR_FindFirst( const char *FilePath, FILEINFO *Buffer )
{
	DXA_DIR_FINDDATA *find;
	char nPath[256];
	// �������̊m��
	find = ( DXA_DIR_FINDDATA * )DXALLOC( sizeof( DXA_DIR_FINDDATA ) );
	if( find == NULL ) return -1;
	memset( find, 0, sizeof( *find ) );
	// �w��̃I�u�W�F�N�g���A�[�J�C�u�t�@�C���������ׂ�
	if( DXA_DIR_OpenTest( FilePath, &find->UseArchiveIndex, NULL, nPath ) == -1 )
	{
		// �A�[�J�C�u�t�@�C�����ł͂Ȃ������ꍇ�̓t�@�C�����猟������
		find->UseArchiveFlag = 0;
		//find->FindHandle = WinFileAccessFindFirst( FilePath, Buffer );
	}
	else
	{
		// �A�[�J�C�u�t�@�C�����̏ꍇ�̓A�[�J�C�u�t�@�C�������猟������
		find->UseArchiveFlag = 1;
		find->FindHandle = DXA_FindFirst( &DXARCD.Archive[ find->UseArchiveIndex ]->Archive, nPath, Buffer );
	}
	// �����n���h�����擾�ł��Ȃ������ꍇ�̓G���[
	if( find->FindHandle == -1 )
	{
		DXFREE( find );
		return -1;
	}
	return (int)find;	// �n���h����Ԃ�
}

/** �߂�l: -1=�G���[  0=����
 *
 *
 */
int		DXA_DIR_FindNext( int FindHandle, FILEINFO *Buffer )
{
	DXA_DIR_FINDDATA *find;
	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
		return 0;//WinFileAccessFindNext( find->FindHandle, Buffer );
	else
		return DXA_FindNext( find->FindHandle, Buffer );
}

/** �߂�l: -1=�G���[  0=����
 *
 *
 */
int		DXA_DIR_FindClose( int FindHandle )
{
	DXA_DIR_FINDDATA *find;
	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
	{
		//WinFileAccessFindClose( find->FindHandle );
	}
	else
	{
		DXA_FindClose( find->FindHandle );
		DXA_DIR_CloseArchive( find->UseArchiveIndex ) ;
	}
	DXFREE( find );
	return 0;
}

/** �w��̂c�w�`�t�@�C�����ۂ��ƃ������ɓǂݍ���( �߂�l: -1=�G���[  0=���� )
 *
 *
 */
int		ST_DXArchivePreLoad( const char *FilePath , int ASync )
{
	char fullpath[256];
	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
	DXA_DIR_ConvertFullPath( FilePath, fullpath ) ;
	return DXA_DIR_OpenArchive( fullpath, -1, TRUE, ASync ) == -1 ? -1 : 0;
}

/** �w��̂c�w�`�t�@�C���̎��O�ǂݍ��݂������������ǂ������擾����( �߂�l�F TRUE=�������� FALSE=�܂� )
 *
 *
 */
int		ST_DXArchiveCheckIdle( const char *FilePath )
{
	int handle;
	char fullpath[256];
	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
	DXA_DIR_ConvertFullPath( FilePath, fullpath ) ;
	// �t�@�C���p�X����n���h�����擾����
	handle = DXA_DIR_GetArchive( fullpath );
	if( handle == -1 ) return 0 ;
	// �����������������ǂ����𓾂�
	return DXA_CheckIdle( &DXARCD.Archive[handle]->Archive );
}

/** �w��̂c�w�`�t�@�C��������������������
 *
 *
 */
int		ST_DXArchiveRelease( const char *FilePath )
{
	int handle;
	char fullpath[256];
	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
	DXA_DIR_ConvertFullPath( FilePath, fullpath ) ;
	// �t�@�C���p�X����n���h�����擾����
	handle = DXA_DIR_GetArchive( fullpath );
	if( handle == -1 ) return 0 ;
	DXA_DIR_CloseArchive( handle ) ;
	DXA_DIR_CloseWaitArchive() ;
	return 0 ;
}

/** �t�@�C�������
 *
 *
 */
int		DXA_DIR_Close( int Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1;		// �g�p����Ă��Ȃ������牽�������I��
	if( file->UseArchiveFlag == FALSE )	// �A�[�J�C�u���g�p���Ă��邩�ǂ����ŕ���
	{
		//WinFileAccessClose( file->FilePointer ) ;
		file->FilePointer = DXA_FILE_NO_SET;
	}
	else
	{
		// �A�[�J�C�u�t�@�C���̎Q�Ɛ������炷
		DXA_DIR_CloseArchive( file->UseArchiveIndex ) ;
		// �A�[�J�C�u�t�@�C���̌�n��
		DXA_STREAM_Terminate( &file->DXAStream ) ;
	}
	DXFREE( file ) ;	// ���������������
	DXARCD.File[Handle & 0x0FFFFFFF] = NULL ;
	DXARCD.FileNum -- ;	// �������炷
	return 0 ;
}

/* �t�@�C������f�[�^��ǂݍ���
 *
 *
 */
size_t	DXA_DIR_Read( void *Buffer, size_t BlockSize, size_t BlockNum, int Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1 ;
	//if( file->UseArchiveFlag == 0 )	return WinFileAccessRead( Buffer, BlockSize, BlockNum, file->FilePointer ) ;
	//else							return DXA_STREAM_Read( &file->DXAStream, Buffer, BlockSize * BlockNum ) ;
	return 0;
}

/** �t�@�C���|�C���^�̈ʒu��ύX����
 *
 *
 */
int		DXA_DIR_Seek( int Handle, long SeekPoint, int SeekType )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL ) return -1 ;
	//if( file->UseArchiveFlag == 0 )	return WinFileAccessSeek( file->FilePointer, SeekPoint, SeekType ) ;
	//else							return DXA_STREAM_Seek( &file->DXAStream, SeekPoint, SeekType ) ;
	return 0;
}

/** �A�[�J�C�u�t�@�C�����t�H���_�Ɍ����Ăăt�@�C�����J�����̏��𓾂�( -1:�A�[�J�C�u�Ƃ��Ă͑��݂��Ȃ�����  0:���݂��� )
 *
 *
 */
int		DXA_DIR_OpenTest( const char *FilePath, int *ArchiveIndex, char *ArchivePath, char *ArchiveFilePath )
{
	int i, len, arcindex ;
	char fullpath[256], path[256], temp[256], dir[256], *p ;
	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
	DXA_DIR_ConvertFullPath( FilePath, fullpath ) ;
	// �O��Ǝg�p����A�[�J�C�u�̃p�X�������ꍇ�͓����A�[�J�C�u���g�p����
	if( DXARCD.BUDirPathLen != 0 &&
		strncmp( fullpath, DXARCD.BUDir, DXARCD.BUDirPathLen ) == 0 &&
		( fullpath[DXARCD.BUDirPathLen] == '\\' ||
		fullpath[DXARCD.BUDirPathLen] == '/' ) )
	{
		// �O��g�p�����c�w�`�t�@�C�����J��
		arcindex = DXA_DIR_OpenArchive( DXARCD.BUDir, DXARCD.BUArcIndex ,FALSE,FALSE ) ;
		if( arcindex == -1 ) return -1;
		// �c�w�`�t�@�C��������ꏊ�ȍ~�̃p�X���쐬����
		p = &fullpath[ DXARCD.BUDirPathLen + 1 ] ;
	}
	else
	{
		// �O��Ƃ͈Ⴄ�p�X�̏ꍇ�͈ꂩ�璲�ׂ�
		// �f�B���N�g�������ǂ��čs��
		p = fullpath ;
		len = 0 ;
		for(;;)
		{
			// �l�b�g���[�N����Ă����ꍇ�̐�p����
			if( p - fullpath == 0 && fullpath[0] == '\\' && fullpath[1] == '\\' )
			{
				path[0] = '\\';
				path[1] = '\\';
				path[2] = '\0';
				len += 2;
				p += 2;
			}
			// �f�B���N�g��������
			for( i = 0 ; *p != '\0' && *p !=  '/' && *p != '\\' ; p ++, i ++ )
			{
				dir[i] = path[len+i] = *p ;
			}
			if( *p == '\0' || i == 0 ) return -1;
			p ++ ;
			dir[i] = path[len+i] = '\0' ;
			len += i ;
			// �t�H���_����DX�A�[�J�C�u�t�@�C�����ɂ���
			memcpy( temp, path, len ) ;
			temp[len] = '.' ;
			if( DXARCD.ArcExtLen == 0 )	memcpy( &temp[len+1], "DXA", 4 ) ;
			else						memcpy( &temp[len+1], DXARCD.ArcExt, DXARCD.ArcExtLen + 1 ) ;
			// �c�w�`�t�@�C���Ƃ��ĊJ���Ă݂�
			arcindex = DXA_DIR_OpenArchive( temp ,-1,FALSE,FALSE) ;
			if( arcindex != -1 ) break ;
			// �J���Ȃ������玟�̊K�w��
			path[len] = '\\' ;
			len ++ ;
		}
		// �J�����獡��̏���ۑ�����
		if( DXARCD.ArcExtLen == 0 )
			memcpy( DXARCD.BUDir, temp, len + 3 + 2 ) ;
		else
			memcpy( DXARCD.BUDir, temp, len + DXARCD.ArcExtLen + 2 ) ;
		DXARCD.BUDirPathLen = len ;
		DXARCD.BUArcIndex   = arcindex ;
	}
	// �����Z�b�g����
	*ArchiveIndex = arcindex;
	if( ArchivePath     ) strcpy( ArchivePath,     DXARCD.BUDir );
	if( ArchiveFilePath ) strcpy( ArchiveFilePath, p                       );
	return 0;
}

/** �t�@�C�����J��( �G���[�F-1  �����F�n���h�� )
 *
 * FileRead_open�ƍ�������
 */
int		DXA_DIR_Open( const char *FilePath)//, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag )
{
	int index ;
	DXARC_DIR_FILE *file ;
	char DxaInFilePath[256];
	if( DXARCD.FileNum == DXA_DIR_MAXFILENUM )	return -1;// �󂫃f�[�^������
	for( index = 0 ; DXARCD.File[index] != NULL ; index ++ ){}
	// �������̊m��
	DXARCD.File[index] = (DXARC_DIR_FILE *)DXALLOC( sizeof( DXARC_DIR_FILE ) ) ;
	if( DXARCD.File[index] == NULL )			return -1;//�b��I�Ƀn���h����Ԃ�
	file = DXARCD.File[index] ;
	// �A�[�J�C�u�t�@�C�������������ׂ�
	if( DXA_DIR_OpenTest( FilePath, (int *)&file->UseArchiveIndex, NULL, DxaInFilePath ) == 0 )
	{
		file->UseArchiveFlag = 1 ;		// �A�[�J�C�u���g�p���Ă���t���O�𗧂Ă�
		// �f�B���N�g�����Ɠ����̂c�w�`�t�@�C�����J�����炻�̒�����w��̃t�@�C����ǂݍ������Ƃ���
		if( DXA_STREAM_Initialize( &file->DXAStream,
			&DXARCD.Archive[ file->UseArchiveIndex ]->Archive,
			DxaInFilePath/*, UseASyncReadFlag*/ ) < 0 )
		{
			DXA_DIR_CloseArchive( file->UseArchiveIndex ) ;
			goto ERR ;
		}
	}
	else
	{
		file->UseArchiveFlag = 0;		// �J�����畁�ʂ̃t�@�C������ǂݍ��ސݒ���s��
		file->FilePointer = FileRead_open(FilePath);
		if( file->FilePointer == DXA_FILE_NO_SET ) goto ERR ;
	}
	DXARCD.FileNum ++ ;			// �n���h���̐��𑝂₷
	return index | 0xF0000000 ;	// �C���f�b�N�X��Ԃ�
ERR:
	// �������̉��
	if( DXARCD.File[index] != NULL ) DXFREE( DXARCD.File[index] ) ;
	DXARCD.File[index] = NULL ;
	return -1;
}

/** �t�@�C�����ۂ��Ɠǂݍ��ފ֐�
 *
 *
 */
/*int		DXA_DIR_LoadFile( const char *FilePath, void *Buffer, int BufferSize )
{
	int siz ;
	int handle ;
	handle = DXA_DIR_Open( FilePath,FALSE,TRUE,FALSE ) ;
	if( handle == -1 ) return false ;
	DXA_DIR_Seek( handle, 0L, SEEK_END ) ;
	siz = DXA_DIR_Tell( handle ) ;
	DXA_DIR_Seek( handle, 0L, SEEK_SET ) ;
	if( siz <= BufferSize )	DXA_DIR_Read( Buffer, siz, 1, handle ) ;
	DXA_DIR_Close( handle ) ;
	return siz ;
}*/
