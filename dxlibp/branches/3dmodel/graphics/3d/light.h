//���ӁBUpdate����̂�sceGuStart-sceGuFinish�Ԃł���K�v������܂��B

struct DXPLIGHT
{
	int Type;//���C�g�^�C�v
	int Component;//�L������
	ScePspFVector3 Position;//�ꏊ
	ScePspFVector3 Direction;//����
	float Attenuation[3];//���x�������~�������������^�iA0�{A1�~�����{A2�~�����~�����j//�����l��1,0,0
	u32 AmbientColor;//����
	u32 DiffuseColor;//�g�U��
	u32 SpecularColor;//���ˌ�
	float SpotlightExponent;//�X�|�b�g���C�g���[��������
	float SpotlightAngle;//�X�|�b�g���C�g�p�x
	DXPLIGHT();
	static int Update(int id,const DXPLIGHT *param);//NULL���w�肳�ꂽ�烉�C�gOFF�BID��0-3
};
