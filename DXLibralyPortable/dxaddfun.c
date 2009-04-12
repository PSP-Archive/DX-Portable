#include "dxlibp.h"
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include "dxpstatic.h"
#include <pspctrl.h>

//#include "c_common.h"
#include <pspkernel.h>
#include <stdlib.h>

#define	FILE_READ_MAX_BYTE	30000
	char	Linewrk[6][FILE_READ_MAX_BYTE];

	SceUID fd;

//�ȉ��͗��_�^���񏊂̃T���v���\�[�X 50�͂Ŏg�p���Ă��関�����֐��ɂȂ�܂��B
int	DrawFormatStringToHandle( int x, int y, int Color, int FontHandle, const char *FormatString, ... )
{
	return 0;
}

int	CreateFontToHandle( const char *FontName, int Size, int Thick, int FontType , int CharSet , int EdgeSize , int Italic  , int DataIndex , int ID )
{
	return 0;
}

//PC�̂悤�Ƀ��������\���ɂȂ��̂ł��̊֐��̑��݈Ӌ`�͂Ȃ������E�E�E
int	SetCreateSoundDataType( int SoundDataType )
{
	return 0;
}

/*int WaveFileChk( const char *FileData)
{
	
	return 0;
}
*/

//�ȉ��̊֐���PSP�����ɍœK���@�\���������邽�߂ɒǉ������֐�
void*	TsFileLoad(const char* filename,int* FileSize)
{
	void *wfp;
	if (!(fd = sceIoOpen((char *)filename, PSP_O_RDONLY, 0777))) return NULL;
//	unsigned long size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	unsigned long size = FileRead_size(filename);
	wfp = malloc(size);
	if ( wfp == NULL) {
		sceIoClose(fd);
		return NULL;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	sceIoRead(fd,wfp, size);
	sceIoClose(fd);
	if (FileSize != NULL) *FileSize = size;
	return wfp;
}

int	TsFileSave(const char* filename,unsigned int FileSize,const char* writedata)
{
	if (!(fd = sceIoOpen((char *)filename, PSP_O_CREAT | PSP_O_WRONLY, 0777))) return -1;
	sceIoWrite(fd,writedata, FileSize);
	sceIoClose(fd);
	return 0;
}

