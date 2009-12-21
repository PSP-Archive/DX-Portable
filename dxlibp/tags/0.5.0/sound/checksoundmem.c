#include "../sound.h"

int CheckSoundMem(int handle)
{
	if(!dxpSoundData.init)return -1;
	if(handle < 0 || handle >= DXP_BUILDOPTION_SOUNDHANDLE_MAX)return -1;
	DXPSOUNDHANDLE *pHnd = dxpSoundArray + handle;
	if(!pHnd->used)return -1;
	if(pHnd->threadId == -1)
	{//�����̓s���ŃX���b�h�������I������
		dxpSoundReleaseHandle(handle);
		return -1;
	}
	if(pHnd->playing)return 1;
	return 0;
}