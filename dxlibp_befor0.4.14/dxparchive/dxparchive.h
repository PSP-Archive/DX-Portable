
// �c�w�A�[�J�C�u�֘A

/*
	�f�[�^�}�b�v
		
	DXARC_HEAD
	�t�@�C�����f�[�^
	�t�@�C�����e�[�u��
	DXARC_FILEHEAD �e�[�u��
	DXARC_DIRECTORY �e�[�u��


	�t�@�C�����̃f�[�^�`��
	2byte:������̒���(�o�C�g�T�C�Y���S)
	2byte:������̃p���e�B�f�[�^(�S�Ă̕����̒l�𑫂�������)
	�p���͑啶���ɕϊ����ꂽ�t�@�C�����̃f�[�^(�S�̔{���̃T�C�Y)
	�p�����啶���ɕϊ�����Ă��Ȃ��t�@�C�����̃f�[�^
*/

//�Ƃ肠����vc++�݊�
//�Ǝv�������ǃA���_�[�X�R�A�t���ł����߂Ȃ̂łƂ肠�����萔�w��
//#define MAX_PATH					_MAX_PATH
#define MAX_PATH					260
#define DXA_FILE_NO_SET				0

#define bool						int
#define false						FALSE
#define true						TRUE			
#define FILE_ATTRIBUTE_DIRECTORY	0x01	//���l�͌�ŉ�͂���̂łƂ肠�����_�~�[

#define FILEACCESSTHREAD_DEFAULT_CACHESIZE	(128 * 1024)	// �f�t�H���g�̃L���b�V���T�C�Y

#define DXAHEAD						*((WORD *)"DX")		// �w�b�_
#define DXAVER						(0x0003)			// �o�[�W����
#define DXA_DIR_MAXARCHIVENUM		(512)				// �����ɊJ���Ă�����A�[�J�C�u�t�@�C���̐�
#define DXA_DIR_MAXFILENUM			(512)				// �����ɊJ���Ă�����t�@�C���̐�
#define DXA_KEYSTR_LENGTH			(12)				// ��������̒���
#define DXA_MAXDRIVENUM				(64)				// �Ή�����h���C�u�̍ő吔
// �c�w�A�[�J�C�u�֘A(DxArchive.cpp)
#define ULONGLONG u64
// �A�[�J�C�u�f�[�^�̍ŏ��̃w�b�_
typedef struct tagDXARC_HEAD
{
	WORD						Head ;							// �h�c
	WORD						Version ;						// �o�[�W����
	DWORD						HeadSize ;						// �w�b�_���� DXARC_HEAD �𔲂����S�T�C�Y
	DWORD						DataStartAddress ;				// �ŏ��̃t�@�C���̃f�[�^���i�[����Ă���f�[�^�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	DWORD						FileNameTableStartAddress ;		// �t�@�C�����e�[�u���̐擪�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	DWORD						FileTableStartAddress ;			// �t�@�C���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
	DWORD						DirectoryTableStartAddress ;	// �f�B���N�g���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
																// �A�h���X�O����z�u����Ă��� DXARC_DIRECTORY �\���̂����[�g�f�B���N�g��
} DXARC_HEAD ;

// �t�@�C���̎��ԏ��
typedef struct tagDXARC_FILETIME
{
	ULONGLONG					Create ;						// �쐬����
	ULONGLONG					LastAccess ;					// �ŏI�A�N�Z�X����
	ULONGLONG					LastWrite ;						// �ŏI�X�V����
} DXARC_FILETIME ;

// �t�@�C�����f�[�^�\����
typedef struct tagDXARC_FILENAME
{
	WORD						Length ;						// ������̒������S
	WORD						Parity ;						// �p���e�B���
} DXARC_FILENAME ;

// �t�@�C���i�[���(Ver0x0001 �p)
typedef struct tagDXARC_FILEHEAD_VER1
{
	DWORD						NameAddress ;					// �t�@�C�������i�[����Ă���A�h���X( ARCHIVE_HEAD�\���� �̃����o�ϐ� FileNameTableStartAddress �̃A�h���X���A�h���X�O�Ƃ���) 
	DWORD						Attributes ;					// �t�@�C������
	DXARC_FILETIME				Time ;							// ���ԏ��
	DWORD						DataAddress ;					// �t�@�C�����i�[����Ă���A�h���X
																//			�t�@�C���̏ꍇ�FDXARC_HEAD�\���� �̃����o�ϐ� DataStartAddress �������A�h���X���A�h���X�O�Ƃ���
																//			�f�B���N�g���̏ꍇ�FDXARC_HEAD�\���� �̃����o�ϐ� DirectoryTableStartAddress �̂������A�h���X���A�h���X�O�Ƃ���
	DWORD						DataSize ;						// �t�@�C���̃f�[�^�T�C�Y
} DXARC_FILEHEAD_VER1 ;

// �t�@�C���i�[���
typedef struct tagDXARC_FILEHEAD
{
	DWORD						NameAddress ;					// �t�@�C�������i�[����Ă���A�h���X( ARCHIVE_HEAD�\���� �̃����o�ϐ� FileNameTableStartAddress �̃A�h���X���A�h���X�O�Ƃ���) 
	DWORD						Attributes ;					// �t�@�C������
	DXARC_FILETIME				Time ;							// ���ԏ��
	DWORD						DataAddress ;					// �t�@�C�����i�[����Ă���A�h���X
																//			�t�@�C���̏ꍇ�FDXARC_HEAD�\���� �̃����o�ϐ� DataStartAddress �������A�h���X���A�h���X�O�Ƃ���
																//			�f�B���N�g���̏ꍇ�FDXARC_HEAD�\���� �̃����o�ϐ� DirectoryTableStartAddress �̂������A�h���X���A�h���X�O�Ƃ���
	DWORD						DataSize ;						// �t�@�C���̃f�[�^�T�C�Y
	DWORD						PressDataSize ;					// ���k��̃f�[�^�̃T�C�Y( 0xffffffff:���k����Ă��Ȃ� ) ( Ver0x0002 �Œǉ����ꂽ )
} DXARC_FILEHEAD ;

// �f�B���N�g���i�[���
typedef struct tagDXARC_DIRECTORY
{
	DWORD						DirectoryAddress ;				// ������ DXARC_FILEHEAD ���i�[����Ă���A�h���X( DXARC_HEAD �\���� �̃����o�ϐ� FileTableStartAddress �������A�h���X���A�h���X�O�Ƃ���)
	DWORD						ParentDirectoryAddress ;		// �e�f�B���N�g���� DXARC_DIRECTORY ���i�[����Ă���A�h���X( DXARC_HEAD�\���� �̃����o�ϐ� DirectoryTableStartAddress �������A�h���X���A�h���X�O�Ƃ���)
	DWORD						FileHeadNum ;					// �f�B���N�g�����̃t�@�C���̐�
	DWORD						FileHeadAddress ;				// �f�B���N�g�����̃t�@�C���̃w�b�_�񂪊i�[����Ă���A�h���X( DXARC_HEAD�\���� �̃����o�ϐ� FileTableStartAddress �������A�h���X���A�h���X�O�Ƃ���) 
} DXARC_DIRECTORY ;



// �t�@�C���������p�f�[�^�\����
typedef struct tagDXARC_SEARCHDATA
{
	BYTE						FileName[1024] ;				// �t�@�C����
	WORD						Parity ;						// �p���e�B���
	WORD						PackNum ;						// ������̒������S
} DXARC_SEARCHDATA ;

// ���e�[�u���\����
typedef struct tagDXARC_TABLE
{
	BYTE						*Top ;							// ���e�[�u���̐擪�|�C���^
	BYTE						*FileTable ;					// �t�@�C�����e�[�u���ւ̃|�C���^
	BYTE						*DirectoryTable ;				// �f�B���N�g�����e�[�u���ւ̃|�C���^
	BYTE						*NameTable ;					// ���O���e�[�u���ւ̃|�C���^
} DXARC_TABLE ;

// �A�[�J�C�u�����p���\����
typedef struct tagDXARC
{
	DXARC_HEAD					Head ;							// �A�[�J�C�u�̃w�b�_
	DWORD						FilePointer ;					// �A�[�J�C�u�t�@�C���̃|�C���^	
	DXARC_TABLE					Table ;							// �e�e�[�u���ւ̐擪�A�h���X���i�[���ꂽ�\����
	DXARC_DIRECTORY				*CurrentDirectory ;				// �J�����g�f�B���N�g���f�[�^�ւ̃|�C���^

	unsigned char				Key[DXA_KEYSTR_LENGTH] ;		// ��������
	int							MemoryOpenFlag ;				// ��������̃t�@�C�����J���Ă��邩�A�t���O
	int							UserMemoryImageFlag ;			// ���[�U�[���W�J�����������C���[�W���g�p���Ă��邩�A�t���O
	int							MemoryImageSize ;				// ��������̃t�@�C������J���Ă����ꍇ�̃C���[�W�̃T�C�Y

	int							ASyncOpenFlag ;					// �񓯊��ǂݍ��ݒ����t���O( TRUE:�񓯊��ǂݍ��ݒ� FALSE:�Ⴄ )
	DWORD						ASyncOpenFilePointer ;			// �񓯊��I�[�v�������Ɏg�p����t�@�C���̃|�C���^
} DXARC ;

// �A�[�J�C�u���̃t�@�C����ʏ�̃t�@�C���ǂݍ��݂̂悤�ɏ�������ׂ̍\����
typedef struct tagDXARC_STREAM
{
	DXARC						*Archive ;						// �A�[�J�C�u�f�[�^�ւ̃|�C���^
	DXARC_FILEHEAD				*FileHead ;						// �t�@�C�����ւ̃|�C���^
	void						*DecodeDataBuffer ;				// �𓀂����f�[�^���i�[����Ă��郁�����̈�ւ̃|�C���^( �t�@�C�������k�f�[�^�������ꍇ�̂ݗL�� )
	void						*DecodeTempBuffer ;				// ���k�f�[�^�ꎞ�ۑ��p�������̈�ւ̃|�C���^

	int							EOFFlag ;						// EOF�t���O
	DWORD						FilePoint ;						// �t�@�C���|�C���^
	int							UseASyncReadFlag ;				// �񓯊��ǂݍ��݃t���O
	int							ASyncState ;					// �񓯊��ǂݍ��ݏ��( DXARC_STREA_ASYNCSTATE �n )
	int							ASyncReadFileAddress ;			// �񓯊��ǂݍ��ݎ��̃t�@�C���|�C���^

	void						*ReadBuffer;					// �񓯊��ǂݍ��ݎ��̈����ɓn���ꂽ�o�b�t�@�ւ̃|�C���^
	int							ReadSize;						// �񓯊��ǂݍ��ݎ��̈����ɓn���ꂽ�ǂݍ��݃T�C�Y�ւ̃|�C���^
} DXARC_STREAM ;

// �A�[�J�C�u�t�@�C�����f�B���N�g���Ɍ����Ă鏈���p�̊J���Ă���A�[�J�C�u�t�@�C���̏��
typedef struct tagDXARC_DIR_ARCHIVE
{
	int							UseCounter ;					// ���̃A�[�J�C�u�t�@�C�����g�p����Ă��鐔
	DXARC						Archive ;						// �A�[�J�C�u�t�@�C���f�[�^
	char						Path[256] ;						// �A�[�J�C�u�t�@�C���̃p�X
} DXARC_DIR_ARCHIVE ;

// �A�[�J�C�u�t�@�C�����f�B���N�g���Ɍ����Ă鏈���p�̊J���Ă���A�[�J�C�u�t�@�C�����̃t�@�C���̏��
typedef struct tagDXARC_DIR_FILE
{
	int							UseArchiveFlag ;				// �A�[�J�C�u�t�@�C�����g�p���Ă��邩�t���O
	DWORD						FilePointer ;					// �A�[�J�C�u���g�p���Ă��Ȃ��ꍇ�́A�t�@�C���|�C���^
	DWORD						UseArchiveIndex ;				// �A�[�J�C�u���g�p���Ă���ꍇ�A�g�p���Ă���A�[�J�C�u�t�@�C���f�[�^�̃C���f�b�N�X
	DXARC_STREAM				DXAStream ;						// �A�[�J�C�u�t�@�C�����g�p���Ă���ꍇ�̃t�@�C���A�N�Z�X�p�f�[�^
} DXARC_DIR_FILE ;

// �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���p�̍\����
typedef struct tagDXARC_DIR
{
	DXARC_DIR_ARCHIVE			*Archive[DXA_DIR_MAXARCHIVENUM] ;	// �g�p���Ă���A�[�J�C�u�t�@�C���̃f�[�^
	DXARC_DIR_FILE				*File[DXA_DIR_MAXFILENUM];			// �J���Ă���t�@�C���̃f�[�^
	char						ArcExt[64];							// �A�[�J�C�u�t�@�C���̊g���q
	int							ArcExtLen;							// �A�[�J�C�u�t�@�C���̊g���q�̒���

	int							ValidKeyString ;					// KeyString ���L�����ǂ���
	char						KeyString[DXA_KEYSTR_LENGTH + 1 ] ;	// ��������

	int							ArchiveNum ;					// �g�p���Ă���A�[�J�C�u�t�@�C���̐�
	int							FileNum ;						// �J���Ă���t�@�C���̐�

	int							BUArcIndex;						// �O��g�p�����A�[�J�C�u�̃C���f�b�N�X
	char						BUDir[256];						// �O��g�p�����f�B���N�g���p�X
	int							BUDirPathLen;					// �O��g�p�����f�B���N�g���p�X�̒���
} DXARC_DIR ;

// �^�C���f�[�^�^
typedef struct tagDATEDATA
{
	int						Year ;							// �N
	int						Mon ;							// ��
	int						Day ;							// ��
	int						Hour ;							// ����
	int						Min ;							// ��
	int						Sec ;							// �b
} DATEDATA, *LPDATEDATA ;

// �t�@�C�����\����
typedef struct tagFILEINFO
{
	char					Name[260] ;			// �I�u�W�F�N�g��
	int						DirFlag ;			// �f�B���N�g�����ǂ���( TRUE:�f�B���N�g��  FALSE:�t�@�C�� )
	LONGLONG				Size ;				// �T�C�Y
	DATEDATA				CreationTime ;		// �쐬����
	DATEDATA				LastWriteTime ;		// �ŏI�X�V����
} FILEINFO, *LPFILEINFO ;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME; 


// �c�w�A�[�J�C�u�֘A(DxArchive.cpp)
//DXPDEFARG
extern	int	DXA_Initialize( DXARC *DXA ) ;												// �A�[�J�C�u�t�@�C���������ׂ̍\���̂�����������
extern	int DXA_Terminate( DXARC *DXA ) ;												// �A�[�J�C�u�t�@�C���������ׂ̍\���̂̌�n��������
//extern	int	DXA_OpenArchiveFromFile( DXARC *DXA, const char *ArchivePath, const char *KeyString = NULL ) ;
extern	int	DXA_OpenArchiveFromFile( DXARC *DXA, const char *ArchivePath, const char *KeyString DXPDEFARG(NULL) ) ;
// �A�[�J�C�u�t�@�C�����J��( 0:����  -1:���s )
//extern	int	DXA_OpenArchiveFromFileUseMem( DXARC *DXA, const char *ArchivePath, const char *KeyString = NULL , int ASync = FALSE ) ;
extern	int	DXA_OpenArchiveFromFileUseMem( DXARC *DXA, const char *ArchivePath, const char *KeyString DXPDEFARG(NULL) , int ASync DXPDEFARG(FALSE) ) ;
// �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
//extern	int	DXA_OpenArchiveFromMem( DXARC *DXA, void *ArchiveImage, int ArchiveSize, const char *KeyString = NULL ) ;
extern	int	DXA_OpenArchiveFromMem( DXARC *DXA, void *ArchiveImage, int ArchiveSize, const char *KeyString DXPDEFARG(NULL) ) ;
// ��������ɂ���A�[�J�C�u�t�@�C���C���[�W���J��( 0:����  -1:���s )
extern	int DXA_CheckIdle( DXARC *DXA ) ;												// �A�[�J�C�u�t�@�C�����������������������𓾂�( TRUE:�����Ă���  FALSE:�����Ă��Ȃ� )
extern	int	DXA_CloseArchive( DXARC *DXA ) ;											// �A�[�J�C�u�t�@�C�������

extern	int	DXA_LoadFile( DXARC *DXA, const char *FilePath, void *Buffer, unsigned int BufferSize ) ;	// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����������ɓǂݍ���( -1:�G���[ 0�ȏ�:�t�@�C���T�C�Y )
extern	void *DXA_GetFileImage( DXARC *DXA ) ;											// �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( DXA_OpenArchiveFromFileUseMem �Ⴕ���� DXA_OpenArchiveFromMem �ŊJ�����ꍇ�ɗL���A�f�[�^�����k����Ă���ꍇ�͒��� )
//extern	int	DXA_GetFileInfo( DXARC *DXA, const char *FilePath, int *Position, int *Size ) ;	// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
extern	int	DXA_ChangeCurrentDir( DXARC *DXA, const char *DirPath ) ;					// �A�[�J�C�u���̃J�����g�f�B���N�g����ύX����( 0:����  -1:���s )
extern	int DXA_GetCurrentDir( DXARC *DXA, char *DirPathBuffer, int BufferSize ) ;		// �A�[�J�C�u���̃J�����g�f�B���N�g�����擾����
extern	int DXA_FindFirst( DXARC *DXA, const char *FilePath, FILEINFO *Buffer ) ;		// �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ -1�ȊO:DXA�����n���h�� )
extern	int DXA_FindNext( int DxaFindHandle, FILEINFO *Buffer ) ;						// �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ 0:���� )
extern	int DXA_FindClose( int DxaFindHandle ) ;											// �A�[�J�C�u���̃I�u�W�F�N�g�������I������

extern	int DXA_STREAM_Initialize( DXARC_STREAM *DXAStream, DXARC *DXA, const char *FilePath/*, int UseASyncReadFlag*/ ) ;	// �A�[�J�C�u�t�@�C�����̃t�@�C�����J��
extern	int DXA_STREAM_Terminate( DXARC_STREAM *DXAStream ) ;							// �A�[�J�C�u�t�@�C�����̃t�@�C�������
extern	int DXA_STREAM_Read( DXARC_STREAM *DXAStream, void *Buffer, int ReadLength ) ;	// �t�@�C���̓��e��ǂݍ���
extern	int DXA_STREAM_Seek( DXARC_STREAM *DXAStream, int SeekPoint, int SeekMode ) ;	// �t�@�C���|�C���^��ύX����
extern	int DXA_STREAM_Tell( DXARC_STREAM *DXAStream ) ;								// ���݂̃t�@�C���|�C���^�𓾂�
extern	int DXA_STREAM_Eof( DXARC_STREAM *DXAStream ) ;									// �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
extern	int	DXA_STREAM_IdleCheck( DXARC_STREAM *DXAStream ) ;							// �ǂݍ��ݏ������������Ă��邩�ǂ����𒲂ׂ�
extern	int DXA_STREAM_Size( DXARC_STREAM *DXAStream ) ;								// �t�@�C���̃T�C�Y���擾����


extern	int DXA_DIR_Initialize( void ) ;												// �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̏�����
extern	int DXA_DIR_Terminate( void ) ;													// �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̌�n��

extern void *ST_DxAlloc( size_t AllocSize, const char *File, int Line );
extern void *ST_DxCalloc( size_t AllocSize, const char *File, int Line );
extern void *ST_DxRealloc( void *Memory, size_t AllocSize, const char *File, int Line );
extern void ST_DxFree( void *Memory );

// �������m�یn�֐�
//#ifdef DX_USE_DXLIB_MEM_DUMP
#ifdef _DEBUG
	#define DXALLOC( size )			ST_DxAlloc( (size), __FILE__, __LINE__ )
	#define DXCALLOC( size )		ST_DxCalloc( (size), __FILE__, __LINE__ )
	#define DXREALLOC( mem, size )	ST_DxRealloc( (mem), (size), __FILE__, __LINE__ )
	#define DXFREE( mem )			ST_DxFree( (mem) )
#else
	#define DXALLOC( size )			ST_DxAlloc( (size), "", 0 )
	#define DXCALLOC( size )		ST_DxCalloc( (size), "", 0 )
	#define DXREALLOC( mem, size )	ST_DxRealloc( (mem), (size), "", 0 )
	#define DXFREE( mem )			ST_DxFree( (mem) )
#endif

//#ifdef	__cplusplus
//#define	DXPDEFARG(ARG)	=(ARG)		/*C++�̃f�t�H���g�����̋@�\��C�ł̓G���[�ɂȂ�̂ő΍�*/
//extern "C" {
//#else
//#define	DXPDEFARG(ARG)
//#endif

//static void DXA_KeyConvFileRead( void *Data, int Size, DWORD FilePointer, unsigned char *Key, int Position = -1 ) ;
//static void DXA_KeyConvFileRead( void *Data, int Size, DWORD FilePointer, unsigned char *Key, int Position DXPDEFARG(-1) ) ;
//void DXA_KeyConvFileRead( void *Data, int Size, DWORD FilePointer, unsigned char *Key, int Position DXPDEFARG(-1) ) ;

//#ifdef	__cplusplus
//};
//#endif
