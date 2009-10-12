#include "dxlibp.h"
#include "dxparchive.h"
typedef void *HANDLE;
// �t�@�C���A�N�Z�X��p�X���b�h�p�\����
typedef struct tagFILEACCESSTHREAD
{
	DWORD					ThreadID ;							// �X���b�h�h�c
	HANDLE					ThreadHandle ;						// �X���b�h�n���h��
	HANDLE					Handle ;							// �t�@�C���A�N�Z�X�p�n���h��
	HANDLE					FuncEvent ;							// �w�ߎ󂯎��p�n���h��
	HANDLE					CompEvent ;							// �w�ߊ����p�n���h��
	int						Function ;							// �w��( FILEACCESSTHREAD_FUNCTION_OPEN �� )

	int						EndFlag ;							// �I���������A�t���O
	int						ErrorFlag ;							// �G���[�����t���O

	char					FilePath[MAX_PATH] ;				// �t�@�C���p�X
	void					*ReadBuffer ;						// �ǂݍ��ރf�[�^���i�[����o�b�t�@�[
	DWORD					ReadPosition ;						// �ǂݍ��ރf�[�^�̈ʒu
	DWORD					ReadSize ;							// �ǂݍ��ރf�[�^�̃T�C�Y(�ǂݍ��߂��f�[�^�̃T�C�Y)
	long					SeekPoint ;							// �t�@�C���|�C���^���ړ�����ʒu 

	BYTE					*CacheBuffer ;						// �L���b�V���o�b�t�@�ւ̃|�C���^
	DWORD					CachePosition ;						// �L���b�V�����Ă���t�@�C���ʒu
	int						CacheSize ;							// �L���b�V���o�b�t�@�̗L���f�[�^�T�C�Y
} FILEACCESSTHREAD ;

// �t�@�C���A�N�Z�X�����p�\����
typedef struct tagWINFILEACCESS
{
	HANDLE					Handle ;							// �t�@�C���A�N�Z�X�p�n���h��
	int						UseThread ;							// �X���b�h���g�p���邩�ǂ���
	int						UseCacheFlag ;						// �L���b�V�����g�p���邩�ǂ���
	int						UseASyncReadFlag ;					// �񓯊��ǂݍ��݂��s�����ǂ���
	int						EofFlag ;							// �I�[�`�F�b�N�t���O
	DWORD					Position ;							// �A�N�Z�X�ʒu
	DWORD					Size ;								// �T�C�Y

	FILEACCESSTHREAD		ThreadData ;						// �ǂݍ��ݐ�p�X���b�h�̃f�[�^
} WINFILEACCESS ;

extern char *_STRCHR( const char *Str1, char Char );
extern void _MEMSET( void *Memory, unsigned char Char, int Size );
extern void _MEMCPY( void *Buffer1, const void *Buffer2, int Size );
extern void _STRCPY( char *Dest, const char *Src );
extern void _STRCAT( char *Dest, const char *Src );
extern int _STRCMP( const char *Str1, const char *Str2 );
extern char *_STRUPR( char *Str );
extern int _STRNCMP( const char *Str1, const char *Str2, int Size );



extern int WinFileAccessOpen( const char *Path, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag )
{
	WINFILEACCESS *FileAccess ;
	DWORD Code;
	
//	UseCacheFlag = UseCacheFlag ;
	BlockReadFlag = BlockReadFlag ;

	FileAccess = (WINFILEACCESS *)DXALLOC( sizeof( WINFILEACCESS ) ) ;
	//if( FileAccess == NULL ) return NULL ;
	if( FileAccess == NULL ) return -1;

	_MEMSET( FileAccess, 0, sizeof( WINFILEACCESS ) ) ;

//	// �L���b�V�����g�p���邩�ǂ������X���b�h���g�p���邩�ǂ����ɂ��Ă��܂�
//	FileAccess->UseThread = UseCacheFlag ;

	// �L���b�V�����g�p���邩�ǂ����̃t���O���Z�b�g
	FileAccess->UseCacheFlag = UseCacheFlag ;
	FileAccess->ThreadData.CacheBuffer = NULL;

	// �񓯊��ǂݏ����t���O���Z�b�g
	FileAccess->UseASyncReadFlag = UseASyncReadFlag ;

	// �L���b�V���A�Ⴕ���͔񓯊��ǂݏ������s���ꍇ�̓X���b�h���g�p����
	FileAccess->UseThread = FileAccess->UseCacheFlag || FileAccess->UseASyncReadFlag ;

	// �X���b�h���g�p����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( FileAccess->UseThread == TRUE )
	{
		// �X���b�h���g�p����ꍇ�̓t�@�C���A�N�Z�X��p�X���b�h�𗧂Ă�

		// �ŏ��Ƀt�@�C�����J���邩�ǂ����m���߂Ă���
		//FileAccess->Handle = CreateFile( Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		//if( FileAccess->Handle == INVALID_HANDLE_VALUE )
		{
			DXFREE( FileAccess ) ;
			//return NULL ;
			return -1;
		}
		//FileAccess->Size = GetFileSize( FileAccess->Handle, NULL ) ;
		//CloseHandle( FileAccess->Handle ) ;
		FileAccess->Handle = NULL ;

		// �L���b�V���p�������̊m��
		if( FileAccess->UseCacheFlag )
		{
			FileAccess->ThreadData.CacheBuffer = (BYTE *)DXALLOC( FILEACCESSTHREAD_DEFAULT_CACHESIZE );
			if( FileAccess->ThreadData.CacheBuffer == NULL )
			{
				DXFREE( FileAccess->ThreadData.CacheBuffer ) ;
				DXFREE( FileAccess ) ;
				DXST_ERRORLOG_ADD( "�t�@�C���ǂݍ��݃L���b�V���p�������̊m�ۂɎ��s���܂���\n" ) ;
				//return NULL ;
				return -1;
			}
		}

		// ��p�X���b�h�f�[�^������������
		FileAccess->ThreadData.Handle = NULL ;
		FileAccess->ThreadData.ThreadHandle = NULL ;
		FileAccess->ThreadData.FuncEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		FileAccess->ThreadData.CompEvent = CreateEvent( NULL, TRUE, TRUE, NULL ) ;

		FileAccess->ThreadData.ThreadHandle = CreateThread(
												NULL,
												0,
												(LPTHREAD_START_ROUTINE)FileAccessThreadFunction, 
												&FileAccess->ThreadData,
												0,
												&FileAccess->ThreadData.ThreadID ) ;
		if( FileAccess->ThreadData.ThreadHandle == NULL )
		{
			if( FileAccess->ThreadData.CacheBuffer ) DXFREE( FileAccess->ThreadData.CacheBuffer ) ;
			CloseHandle( FileAccess->ThreadData.FuncEvent ) ;
			CloseHandle( FileAccess->ThreadData.CompEvent ) ;
			DXFREE( FileAccess ) ;
			DXST_ERRORLOG_ADD( "�t�@�C���A�N�Z�X��p�X���b�h�̍쐬�Ɏ��s���܂���\n" ) ;
			//return NULL ;
			return -1;
		}

		// �t�@�C���I�[�v���w�߂͂����Ŋ������Ă��܂�
		FileAccess->ThreadData.Function = FILEACCESSTHREAD_FUNCTION_OPEN ;
		_STRCPY( FileAccess->ThreadData.FilePath, Path ) ;
		ResetEvent( FileAccess->ThreadData.CompEvent ) ;
		SetEvent( FileAccess->ThreadData.FuncEvent ) ;

		// �w�߂��I������܂ő҂�
		WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;
		if( FileAccess->ThreadData.ErrorFlag == TRUE )
		{
			if( FileAccess->ThreadData.CacheBuffer ) DXFREE( FileAccess->ThreadData.CacheBuffer ) ;
			CloseHandle( FileAccess->ThreadData.FuncEvent ) ;
			CloseHandle( FileAccess->ThreadData.CompEvent ) ;
			do
			{
				Sleep(0);
				GetExitCodeThread( FileAccess->ThreadData.ThreadHandle, &Code );
			}while( Code == STILL_ACTIVE );
			CloseHandle( FileAccess->ThreadData.ThreadHandle ) ;
			DXFREE( FileAccess ) ;
			DXST_ERRORLOG_ADD( "�t�@�C���̃I�[�v���Ɏ��s���܂���\n" ) ;
			//return NULL ;
			return -1;
		}
	}
	else
	{
		// �X���b�h���g�p���Ȃ��ꍇ�͂��̏�Ńt�@�C�����J��
		FileAccess->Handle = CreateFile( Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		//if( FileAccess->Handle == INVALID_HANDLE_VALUE )
		{
			DXFREE( FileAccess ) ;
			//return NULL ;
			return -1;
		}
		//FileAccess->Size = GetFileSize( FileAccess->Handle, NULL ) ;
	}
	FileAccess->EofFlag = FALSE ;
	FileAccess->Position = 0 ;

	return (int)FileAccess ;
}

extern int WinFileAccessClose( int Handle )
{
	WINFILEACCESS *FileAccess = ( WINFILEACCESS * )Handle ;
	BOOL Result;
	DWORD Code ;

	// �X���b�h���g�p����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( FileAccess->UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�ɏI���w�߂��o��
		FileAccess->ThreadData.Function = FILEACCESSTHREAD_FUNCTION_EXIT ;
		ResetEvent( FileAccess->ThreadData.CompEvent ) ;
		SetEvent( FileAccess->ThreadData.FuncEvent ) ;

		// �w�߂��I������܂ő҂�
		WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h���I������̂�҂�
		do
		{
			Sleep(0);
			GetExitCodeThread( FileAccess->ThreadData.ThreadHandle, &Code );
		}while( Code == STILL_ACTIVE );

		// �L���b�V�����g�p���Ă����ꍇ�̓L���b�V���p���������J������
		if( FileAccess->ThreadData.CacheBuffer )
			DXFREE( FileAccess->ThreadData.CacheBuffer ) ;

		// �C�x���g��X���b�h�����
		CloseHandle( FileAccess->ThreadData.ThreadHandle ) ;
		CloseHandle( FileAccess->ThreadData.CompEvent ) ;
		CloseHandle( FileAccess->ThreadData.FuncEvent ) ;
		Result = 0 ;
	}
	else
	{
		// �g�p���Ă��Ȃ��ꍇ�͂��̏�Ńn���h������ďI��
		Result = CloseHandle( FileAccess->Handle ) ;
	}
	DXFREE( FileAccess ) ;

	return Result != 0 ? 0 : -1/*EOF*/ ;
}

extern long WinFileAccessTell( int Handle )
{
	WINFILEACCESS *FileAccess = ( WINFILEACCESS * )Handle ;

	return FileAccess->Position ;
}

extern int WinFileAccessSeek( int Handle, long SeekPoint, int SeekType )
{
	WINFILEACCESS *FileAccess = ( WINFILEACCESS * )Handle ;
	LONG Pos ;
	DWORD Result ;

	switch( SeekType )
	{
	case SEEK_CUR : Pos = ( LONG )FileAccess->Position + SeekPoint ; break ;
	case SEEK_END : Pos = ( LONG )FileAccess->Size + SeekPoint ; break ;
	case SEEK_SET : Pos = SeekPoint ; break ;
	}

	// �X���b�h���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( FileAccess->UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�Ƀt�@�C���ʒu�ύX�w�߂��o��
		FileAccess->ThreadData.Function = FILEACCESSTHREAD_FUNCTION_SEEK ;
		FileAccess->ThreadData.SeekPoint = Pos ;
		ResetEvent( FileAccess->ThreadData.CompEvent ) ;
		SetEvent( FileAccess->ThreadData.FuncEvent ) ;
	}
	else
	{
		// �t�@�C���A�N�Z�X�ʒu��ύX����
		Result = SetFilePointer( FileAccess->Handle, Pos, NULL, FILE_BEGIN ) ;
		if( Result == 0xFFFFFFFF ) return -1 ;
	}

	// �ʒu��ۑ����Ă���
	FileAccess->Position = (DWORD)Pos ;

	// �I�[�`�F�b�N�t���O��|��
	FileAccess->EofFlag = FALSE ;

	// �I��
	return 0 ;
}

extern size_t WinFileAccessRead( void *Buffer, size_t BlockSize, size_t DataNum, int Handle )
{
	WINFILEACCESS *FileAccess = ( WINFILEACCESS * )Handle ;
	DWORD Result, BytesRead ;

	if( BlockSize == 0 ) return 0 ;

	// �I�[�`�F�b�N
	if( FileAccess->Position == FileAccess->Size )
	{
		FileAccess->EofFlag = TRUE ;
		return 0 ;
	}

	// ���ڐ�����
	if( BlockSize * DataNum + FileAccess->Position > FileAccess->Size )
	{
		DataNum = ( FileAccess->Size - FileAccess->Position ) / BlockSize ;
	}
	
	if( DataNum == 0 )
	{
		FileAccess->EofFlag = TRUE ;
		return 0 ;
	}

	// �X���b�h���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( FileAccess->UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�Ƀt�@�C���ǂݍ��ݎw�߂��o��
		FileAccess->ThreadData.Function = FILEACCESSTHREAD_FUNCTION_READ ;
		FileAccess->ThreadData.ReadBuffer = Buffer ;
		FileAccess->ThreadData.ReadPosition = FileAccess->Position ;
		FileAccess->ThreadData.ReadSize = BlockSize * DataNum ;
		ResetEvent( FileAccess->ThreadData.CompEvent ) ;
		SetEvent( FileAccess->ThreadData.FuncEvent ) ;

		// �񓯊����ǂ����ŏ����𕪊�
		if( FileAccess->UseASyncReadFlag == FALSE )
		{
			// �����ǂݍ��݂̏ꍇ�͎w�߂���������܂ő҂�
			WaitForSingleObject( FileAccess->ThreadData.CompEvent, INFINITE ) ;
		}

		BytesRead = BlockSize * DataNum ;
		Result = 1 ;
	}
	else
	{
		Result = ReadFile( FileAccess->Handle, Buffer, BlockSize * DataNum, &BytesRead, NULL ) ;
	}

	FileAccess->Position += DataNum * BlockSize ;
	return Result != 0 ? BytesRead / BlockSize : 0 ;
}

extern int WinFileAccessEof( int Handle )
{
	WINFILEACCESS *FileAccess = (WINFILEACCESS *)Handle ;

	return FileAccess->EofFlag ? EOF : 0 ;
}

extern int WinFileAccessIdleCheck( int Handle )
{
	WINFILEACCESS *FileAccess = (WINFILEACCESS *)Handle ;

	if( FileAccess->UseThread == TRUE )
	{
		return WaitForSingleObject( FileAccess->ThreadData.CompEvent, 0 ) == WAIT_TIMEOUT ? FALSE : TRUE ;
	}
	else
	{
		return TRUE ;
	}
}

extern int WinFileAccessChDir( const char *Path )
{
	return SetCurrentDirectory( Path ) ;
}

extern int WinFileAccessGetDir( char *Buffer )
{
	return GetCurrentDirectory( MAX_PATH, Buffer ) ;
}

static void _WIN32_FIND_DATA_To_FILEINFO( WIN32_FIND_DATA *FindData, FILEINFO *FileInfo )
{
	// �t�@�C�����̃R�s�[
	_STRCPY( FileInfo->Name, FindData->cFileName );

	// �f�B���N�g�����ǂ����̃t���O���Z�b�g
	FileInfo->DirFlag = (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? TRUE : FALSE;

	// �t�@�C���̃T�C�Y���Z�b�g
	((DWORD *)&FileInfo->Size)[0] = FindData->nFileSizeLow;
	((DWORD *)&FileInfo->Size)[1] = FindData->nFileSizeHigh;

	// �t�@�C���^�C����ۑ�
	_FileTimeToLocalDateData( &FindData->ftCreationTime, &FileInfo->CreationTime );
	_FileTimeToLocalDateData( &FindData->ftLastWriteTime, &FileInfo->LastWriteTime );
}

// �߂�l: -1=�G���[  -1�ȊO=FindHandle
extern int WinFileAccessFindFirst( const char *FilePath, FILEINFO *Buffer )
{
	WIN32_FIND_DATA FindData;
	HANDLE FindHandle;

	FindHandle = FindFirstFile( FilePath, &FindData );
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1;

	if( Buffer ) _WIN32_FIND_DATA_To_FILEINFO( &FindData, Buffer );

	return (int)FindHandle;
}

// �߂�l: -1=�G���[  0=����
extern int WinFileAccessFindNext( int FindHandle, FILEINFO *Buffer )
{
	WIN32_FIND_DATA FindData;

	if( FindNextFile( (HANDLE)FindHandle, &FindData ) == 0 ) return -1;

	if( Buffer ) _WIN32_FIND_DATA_To_FILEINFO( &FindData, Buffer );

	return 0;
}

// �߂�l: -1=�G���[  0=����
extern int WinFileAccessFindClose( int FindHandle )
{
	// �O�ȊO���Ԃ��Ă����琬��
	return FindClose( (HANDLE)FindHandle ) != 0 ? 0 : -1;
}



// �e���|�����t�@�C�����쐬����
extern HANDLE CreateTemporaryFile( char *TempFileNameBuffer )
{
	char String1[MAX_PATH], String2[MAX_PATH] ;
	HANDLE FileHandle ;
	int Length ;

	// �e���|�����t�@�C���̃f�B���N�g���p�X���擾����
	if( GetTempPath( 256, String1 ) == NULL ) return NULL ;

	// ������̍Ō�Ɂ��}�[�N������
	Length = lstrlen( String1 ) ;
	if( String1[Length-1] != '\\' ) 
	{
		String1[Length] = '\\' ;
		String1[Length+1] = '\0' ;
	}

	// �e���|�����t�@�C���̃t�@�C�������쐬����
	if( GetTempFileName( String1, "tmp", 0, String2 ) == 0 ) return NULL ;

	// �t���p�X�ɕϊ�
	_ConvertFullPath( String2, String1, NULL ) ;

	// �e���|�����t�@�C�����J��
	DeleteFile( String1 ) ;
	FileHandle = CreateFile( String1, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( FileHandle == NULL ) return NULL ;

	// �e���|�����t�@�C������ۑ�
	if( TempFileNameBuffer != NULL ) lstrcpy( TempFileNameBuffer, String1 ) ;

	// �n���h����Ԃ�
	return FileHandle ;
}

// �t�@�C���A�N�Z�X��p�X���b�h�p�֐�
DWORD WINAPI FileAccessThreadFunction( void *FileAccessThreadData )
{
	FILEACCESSTHREAD *dat = (FILEACCESSTHREAD *)FileAccessThreadData ;
	DWORD res, ReadSize ;

	for(;;)
	{
		for(;;)
		{
			// �L���b�V�����g�p�����ǂ����ŏ����𕪊�
			if( dat->CacheBuffer )
			{
				// �w�߂�����܂ł�����Ƒ҂�
				res = WaitForSingleObject( dat->FuncEvent, 100 ) ;

				// �w�߂����Ă������ꍇ�Ńt�@�C�����J���Ă���ꍇ�̓L���b�V���O�������s��
				if( res == WAIT_TIMEOUT && dat->Handle != NULL )
				{
					// �����L���b�V������t�������牽�����Ȃ�
					if( dat->CacheSize != FILEACCESSTHREAD_DEFAULT_CACHESIZE )
					{
						// �ǂݍ��݊J�n�ʒu�Z�b�g
						SetFilePointer( dat->Handle, dat->CachePosition + dat->CacheSize, NULL, FILE_BEGIN ) ;

						// �ǂݍ���
						ReadFile( dat->Handle, &dat->CacheBuffer[dat->CacheSize], FILEACCESSTHREAD_DEFAULT_CACHESIZE - dat->CacheSize, &ReadSize, NULL ) ;
						
						// �L���ȃT�C�Y�𑝂₷
						dat->CacheSize += ReadSize ;
					}
				}
				else
				{
					break ;
				}
			}
			else
			{
				// �w�߂�����܂ő҂�
				res = WaitForSingleObject( dat->FuncEvent, INFINITE ) ;
				if( res == WAIT_TIMEOUT && dat->Handle != NULL ) continue;
				break;
			}
		}

//		WaitForSingleObject( dat->FuncEvent, INFINITE ) ;

		// �C�x���g�̃V�O�i����Ԃ���������
		ResetEvent( dat->FuncEvent ) ;
		ResetEvent( dat->CompEvent ) ;

		// �w�߂������画�f����
		switch( dat->Function )
		{
		case FILEACCESSTHREAD_FUNCTION_OPEN :
			dat->Handle = CreateFile( dat->FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
			if( dat->Handle == INVALID_HANDLE_VALUE )
			{
				dat->ErrorFlag = TRUE ;
				goto END ;
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_CLOSE :
			CloseHandle( dat->Handle ) ;
			dat->Handle = NULL ;
			break ;

		case FILEACCESSTHREAD_FUNCTION_READ :
			// �L���b�V���Ɠǂݍ��݈ʒu����v���Ă���ꍇ�̓L���b�V������f�[�^��]������
			if( dat->CacheBuffer && dat->ReadPosition == dat->CachePosition && dat->CacheSize != 0 )
			{
				DWORD MoveSize ;

				// �]������T�C�Y�𒲐�
				MoveSize = dat->ReadSize ;
				if( MoveSize > (DWORD)dat->CacheSize ) MoveSize = dat->CacheSize ;

				// �]��
				_MEMCPY( dat->ReadBuffer, dat->CacheBuffer, MoveSize ) ;

				// �ǂݍ��݃T�C�Y�Ɠǂݍ��݈ʒu���ړ�����
				dat->ReadBuffer = (void *)( (BYTE *)dat->ReadBuffer + MoveSize ) ;
				dat->ReadPosition += MoveSize ;
				dat->ReadSize -= MoveSize ;
				
				// �L���b�V���̏����X�V
				dat->CachePosition += MoveSize ;
				dat->CacheSize     -= MoveSize ;
				if( dat->CacheSize != 0 ) _MEMMOVE( &dat->CacheBuffer[0], &dat->CacheBuffer[MoveSize], dat->CacheSize ) ;
			}

			// ��]�̃f�[�^���S�ēǂ߂Ă��Ȃ��ꍇ�͍X�Ƀt�@�C������ǂݍ���
			if( dat->ReadSize != 0 )
			{
				SetFilePointer( dat->Handle, dat->ReadPosition, NULL, FILE_BEGIN ) ;
				ReadFile( dat->Handle, dat->ReadBuffer, dat->ReadSize, &dat->ReadSize, NULL ) ;

				// �L���b�V��������������
				if( dat->CacheBuffer )
				{
					dat->CachePosition = dat->ReadPosition + dat->ReadSize ;
					dat->CacheSize = 0 ;
				}
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_SEEK :
			SetFilePointer( dat->Handle, dat->SeekPoint, NULL, FILE_BEGIN ) ;

			// �L���b�V��������������
			if( dat->CacheBuffer )
			{
				dat->CachePosition = (DWORD)dat->SeekPoint ;
				dat->CacheSize = 0 ;
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_EXIT :
			if( dat->Handle != NULL ) CloseHandle( dat->Handle ) ;
			dat->Handle = NULL ;
			goto END ;
		}

		// �w�߂����������犮���C�x���g���V�O�i����Ԃɂ���
		SetEvent( dat->CompEvent ) ;
	}

END:
	// �G���[���ׂ̈Ɋ����C�x���g���V�O�i����Ԃɂ���
	SetEvent( dat->CompEvent ) ;
	dat->EndFlag = TRUE ;
	ExitThread( 1 ) ;

	return 0 ;
}
