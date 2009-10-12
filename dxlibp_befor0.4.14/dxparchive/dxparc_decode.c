#include "dxlibp.h"
#include "dxparchive.h"
#include <stdio.h>
#include <string.h>

// �f�[�^���𓀂���( �߂�l:�𓀌�̃f�[�^�T�C�Y )
#define MIN_COMPRESS		(4)						// �Œሳ�k�o�C�g��
#define MAX_SEARCHLISTNUM	(64)					// �ő��v����T���ׂ̃��X�g��H��ő吔
#define MAX_SUBLISTNUM		(65536)					// ���k���ԒZ�k�̂��߂̃T�u���X�g�̍ő吔
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// �Q�ƃA�h���X����R�s�[�o�؂�ő�T�C�Y( ���k�R�[�h���\���ł���R�s�[�T�C�Y�̍ő�l + �Œሳ�k�o�C�g�� )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// �X���C�h�����̍ő�T�C�Y
#define MAX_POSITION		(1 << 24)				// �Q�Ɖ\�ȍő告�΃A�h���X( 16MB )

/**
 *
 *
 */
int		DXA_Decode( void *Src, void *Dest )
{
	DWORD srcsize, destsize, code, indexsize, keycode, conbo, index ;
	BYTE *srcp, *destp, *dp, *sp ;

	destp = (BYTE *)Dest ;
	srcp  = (BYTE *)Src ;
	// �𓀌�̃f�[�^�T�C�Y�𓾂�
	destsize = *((DWORD *)&srcp[0]) ;
	// ���k�f�[�^�̃T�C�Y�𓾂�
	srcsize = *((DWORD *)&srcp[4]) - 9 ;
	// �L�[�R�[�h
	keycode = srcp[8] ;
	// �o�͐悪�Ȃ��ꍇ�̓T�C�Y�����Ԃ�
	if( Dest == NULL )	return destsize ;
	// �W�J�J�n
	sp  = srcp + 9 ;
	dp  = destp ;
	while( srcsize )
	{
		// �L�[�R�[�h�������ŏ����𕪊�
		if( sp[0] != keycode )
		{
			// �񈳏k�R�[�h�̏ꍇ�͂��̂܂܏o��
			*dp = *sp ;
			dp      ++ ;
			sp      ++ ;
			srcsize -- ;
			continue ;
		}
		// �L�[�R�[�h���A�����Ă����ꍇ�̓L�[�R�[�h���̂��o��
		if( sp[1] == keycode )
		{
			*dp = (BYTE)keycode ;
			dp      ++ ;
			sp      += 2 ;
			srcsize -= 2 ;
			
			continue ;
		}
		// ���o�C�g�𓾂�
		code = sp[1] ;
		// �����L�[�R�[�h�����傫�Ȓl�������ꍇ�̓L�[�R�[�h
		// �Ƃ̃o�b�e�B���O�h�~�ׂ̈Ɂ{�P���Ă���̂Ł|�P����
		if( code > keycode ) code -- ;
		sp      += 2 ;
		srcsize -= 2 ;
		// �A�������擾����
		conbo = code >> 3 ;
		if( code & ( 0x1 << 2 ) )
		{
			conbo |= *sp << 5 ;
			sp      ++ ;
			srcsize -- ;
		}
		conbo += MIN_COMPRESS ;	// �ۑ����Ɍ��Z�����ŏ����k�o�C�g���𑫂�
		// �Q�Ƒ��΃A�h���X���擾����
		index = 0;
		indexsize = code & 0x3 ;
		switch( indexsize )
		{
		case 0 :
			index = *sp ;
			sp      ++ ;
			srcsize -- ;
			break ;
			
		case 1 :
			index = *((WORD *)sp) ;
			sp      += 2 ;
			srcsize -= 2 ;
			break ;
			
		case 2 :
			index = *((WORD *)sp) | ( sp[2] << 16 ) ;
			sp      += 3 ;
			srcsize -= 3 ;
			break ;
		}
		index ++ ;		// �ۑ����Ɂ|�P���Ă���̂Ł{�P����
		// �W�J
		if( index < conbo )
		{
			DWORD num ;
			num  = index ;
			while( conbo > num )
			{
				memcpy( dp, dp - num, num ) ;
				dp    += num ;
				conbo -= num ;
				num   += num ;
			}
			if( conbo != 0 )
			{
				memcpy( dp, dp - num, conbo ) ;
				dp += conbo ;
			}
		}
		else
		{
			memcpy( dp, dp - index, conbo ) ;
			dp += conbo ;
		}
	}
	// �𓀌�̃T�C�Y��Ԃ�
	return (int)destsize ;
}
