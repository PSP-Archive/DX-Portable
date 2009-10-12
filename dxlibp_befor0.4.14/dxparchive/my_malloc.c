#include <stddef.h>
// �������m�یn�֐�

// �������m�ۂ̏��
typedef struct tagALLOCMEM
{
	char					Name[16] ;							// �t�@�C���p�X
	unsigned short			ID ;								// �h�c
	unsigned short			Line ;								// �s�ԍ�
	unsigned int			Size ;								// �m�ۃT�C�Y
	struct tagALLOCMEM		*Back, *Next ;						// ���ƑO�̊m�ۃ��������ւ̃|�C���^
} ALLOCMEM, *LPALLOCMEM ;

#define DXADDRESS	(64)

// ���������m�ۂ���
extern void *ST_DxAlloc( size_t AllocSize, const char *File, int Line )
{
/*	ALLOCMEM *mem ;

	// �������̊m��
	mem = (ALLOCMEM *)AllocWrap( AllocSize + DXADDRESS ) ;
	if( mem == NULL )
	{
		// ������������Ȃ������炻�̎��̃��������_���v����
		ST_DxDumpAlloc() ;

		// �G���[�����o�͂���
		ST_DxErrorCheckAlloc() ;

		return NULL ;
	}
		
	// ���������ǉ�����
	{
		// ���̕ۑ�
		mem->Size = AllocSize ;
		if( File != NULL )
		{
			int len = lstrlen( File ) ;
			_STRCPY( mem->Name, &File[ len < 15 ? 0 : len - 15 ] ) ;
		}
		else
		{
			mem->Name[0] = '\0' ;
		}
		mem->Line = ( unsigned short )Line ;

		mem->ID = WinData.AllocMemoryID ;
		WinData.AllocMemoryID ++ ;

		// ���X�g�ɒǉ�
		if( WinData.AllocMemoryFirst == NULL )
			WinData.AllocMemoryFirst = &WinData.AllocMemoryAnchor ;
		WinData.AllocMemoryFirst->Next = mem ;
		mem->Back = WinData.AllocMemoryFirst ;
		WinData.AllocMemoryFirst = mem ;
		mem->Next = NULL ;

		// ������ۑ�
		DxCopyAlloc( mem ) ;
		DxCopyAlloc( mem->Back ) ;

		// �m�ۂ����������̑��ʂƑ��������Z����
		WinData.AllocMemorySize += AllocSize ;
		WinData.AllocMemoryNum ++ ;
	}

	// �����������Ă���ꍇ�̓��O���o�͂���
	if( (int)WinData.AllocTrapSize < 0 || WinData.AllocTrapSize == AllocSize || WinData.AllocMemoryPrintFlag == TRUE )
	{
		DXST_ERRORLOG_ADD( "mem alloc  " ) ;
		DxPrintAlloc( mem ) ;
	}

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( WinData.AllocMemorySizeOutFlag == TRUE )
		DxPrintAllocSize() ;

	// �������j��̃`�F�b�N
	if( WinData.AllocMemoryErrorCheckFlag == TRUE )
		ST_DxErrorCheckAlloc() ;
	
	// �������A�h���X��Ԃ�
	return (char *)mem + DXADDRESS ;
	*/
	return NULL;
}

// ���������m�ۂ��ĂO�ŏ���������
extern void *ST_DxCalloc( size_t AllocSize, const char *File, int Line )
{
/*	void *buf ;

	// �������̊m��
	buf = ST_DxAlloc( AllocSize, File, Line ) ;
	if( buf == NULL ) return NULL ;
	
	// �m�ۂ�����������������
	_MEMSET( buf, 0, AllocSize ) ;

	// �m�ۂ����������̃A�h���X��Ԃ�
	return buf ;
	*/
	return NULL;
}

// �������̍Ċm�ۂ��s��
extern void *ST_DxRealloc( void *Memory, size_t AllocSize, const char *File, int Line )
{
/*	ALLOCMEM *mem, *back ;
	void *Result ;

	// �������̍Ċm��
	back = (ALLOCMEM *)( (char *)Memory - DXADDRESS ) ;
	if( Memory == NULL )
	{
		Result = ST_DxAlloc( AllocSize + DXADDRESS, File, Line ) ;

		return Result ;
	}
	else
	{
		mem = (ALLOCMEM *)ReallocWrap( (char *)Memory - DXADDRESS, AllocSize + DXADDRESS ) ;
	}
	if( mem == NULL )
	{
		// ������������Ȃ������炻�̎��̃��������_���v����
		ST_DxDumpAlloc() ;

		return NULL ;
	}

	// ���̏C��
	{
		// ���̕ۑ�
		if( File != NULL )
		{
			int len = lstrlen( File ) ;
			_STRCPY( mem->Name, &File[ len < 15 ? 0 : len - 15 ] ) ;
		}
		else
		{
			mem->Name[0] = '\0' ;
		}
		mem->Line = Line ;
		mem->ID = WinData.AllocMemoryID ;
		WinData.AllocMemoryID ++ ;

		// �O��̏��̍X�V
		mem->Back->Next = mem ;
		if( mem->Next != NULL ) mem->Next->Back = mem ;
		if( Memory != NULL && back == WinData.AllocMemoryFirst ) WinData.AllocMemoryFirst = mem ;

		// �m�ۂ����������̑��ʂ̏C��
		WinData.AllocMemorySize -= mem->Size ;
		mem->Size = AllocSize ;
		WinData.AllocMemorySize += AllocSize ;

		// ������ۑ�
		DxCopyAlloc( mem ) ;
		DxCopyAlloc( mem->Back ) ;
		if( mem->Next != NULL ) DxCopyAlloc( mem->Next ) ;
	}

	// �����������Ă���ꍇ�̓��O���o�͂���
	if( (int)WinData.AllocTrapSize < 0 || WinData.AllocTrapSize == AllocSize || WinData.AllocMemoryPrintFlag == TRUE )
	{
		DXST_ERRORLOG_ADD( "mem realloc  " ) ;
		DxPrintAlloc( mem ) ;
	}

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( WinData.AllocMemorySizeOutFlag == TRUE )
		DxPrintAllocSize() ;

	// �������j��̃`�F�b�N
	if( WinData.AllocMemoryErrorCheckFlag == TRUE )
		ST_DxErrorCheckAlloc() ;

	// �m�ۂ����������A�h���X��Ԃ�
	return (char *)mem + DXADDRESS ;
	*/
	return NULL;
}

// ���������������
extern void ST_DxFree( void *Memory )
{
/*	ALLOCMEM *mem ;

	// NULL ���n���ꂽ�ꍇ�͉������Ȃ�
	if( Memory == NULL ) return ;

	mem = (ALLOCMEM *)( (char *)Memory - DXADDRESS ) ;

	// ������郁�����̕������m�ۂ����������̑��ʂƐ������炷
	WinData.AllocMemorySize -= mem->Size ;
	WinData.AllocMemoryNum -- ;

	// �������j��̃`�F�b�N
	if( WinData.AllocMemoryErrorCheckFlag == TRUE )
		ST_DxErrorCheckAlloc() ;

	// ���f�[�^�A�N�Z�X
	{
		// ���X�g����O��
		if( mem->Back != NULL )	mem->Back->Next = mem->Next ;
		if( mem->Next != NULL ) mem->Next->Back = mem->Back ;
		if( mem == WinData.AllocMemoryFirst ) WinData.AllocMemoryFirst = mem->Back ;

		// ������ۑ�
		if( mem->Next != NULL )	DxCopyAlloc( mem->Next ) ;
		if( mem->Back != NULL ) DxCopyAlloc( mem->Back ) ;
	}
	
	// �����������Ă���ꍇ�̓��O���o�͂���
	if( (int)WinData.AllocTrapSize < 0 || WinData.AllocTrapSize == mem->Size || WinData.AllocMemoryPrintFlag == TRUE )
	{
		DXST_ERRORLOG_ADD( "mem free  " ) ;
		DxPrintAlloc( mem ) ;
	}

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( WinData.AllocMemorySizeOutFlag == TRUE )
		DxPrintAllocSize() ;
	
	// �������̉��
	FreeWrap( mem ) ;
	*/
}

// �񋓑Ώۂɂ��郁�����̊m�ۗe�ʂ��Z�b�g����
extern size_t ST_DxSetAllocSizeTrap( size_t Size )
{
	/*size_t trapsize ;

	trapsize = WinData.AllocTrapSize ;
	WinData.AllocTrapSize = Size ;
		
	return trapsize ;
	*/
	return 0;
}

