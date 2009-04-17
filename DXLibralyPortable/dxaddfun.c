#include "dxlibp.h"
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include "dxpstatic.h"
#include <pspctrl.h>
#include <stdlib.h>

/*int WaveFileChk( const char *FileData)
{
	
	return 0;
}
*/

void*	TsFileLoad(const char* filename,int* FileSize)
{
	STREAMDATA src;
	STREAMDATA *Src = &src;
	if(SetupSTREAMDATA(filename,Src) == -1)
	{
		return NULL;
	}
	void *wfp;
	unsigned long size = FileRead_size(filename);
	wfp = malloc(size);
	if ( wfp == NULL) {
		STCLOSE(Src);
		return NULL;
	}
	//20090415 ����SEEK_SET���Ăǂ��ɂ����Ă�́H
	//STSEEK(Src,0,SEEK_SET);
	STSEEK(Src,0,PSP_SEEK_SET);
	STREAD(wfp,size,1,Src);
	STCLOSE(Src);
	if (FileSize != NULL) *FileSize = size;
	return wfp;
}

int	TsFileSave(const char* filename,unsigned int FileSize,const char* writedata)
{
	STREAMDATA src;
	STREAMDATA *Src = &src;
	//�b�����
	//�������݂Ŏg���ꍇ�̓t�@�C�������݂��Ȃ����ɍ쐬�ł���
    //�I�����Ă��܂��B
	if(SetupSTREAMDATA(filename,Src) == -1)
	{
		return -1;
	}
	STWRITE(&writedata,FileSize,1,Src);
	STCLOSE(Src);
	return 0;
}
