/*
PMD�t�@�C���ǂݍ��݃��[�`��forWindows
���̃R�[�h�͈ȉ��̃T�C�g�̏������ɍ���Ă��܂��B
http://blog.goo.ne.jp/torisu_tetosuki/e/209ad341d3ece2b1b4df24abf619d6e4
*/


#ifndef PMDREADER_H__
#define PMDREADER_H__

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
typedef unsigned __int16	u16;
typedef unsigned __int8		u8;
typedef unsigned __int32	u32;
#else
#include <psptypes.h>
#endif

typedef struct sPmdHeader
{
	char	magic[3]; // "Pmd"
	float	version; // 00 00 80 3F == 1.00
	char	model_name[20];
	char	comment[256];
}sPmdHeader;

typedef struct sPmdVertex
{
	float	pos[3]; // x, y, z // ���W
	float	normal_vec[3]; // nx, ny, nz // �@���x�N�g��
	float	uv[2]; // u, v // UV���W // MMD�͒��_UV
	u16		bone_num[2]; // �{�[���ԍ�1�A�ԍ�2 // ���f���ό`(���_�ړ�)���ɉe��
	u8		bone_weight; // �{�[��1�ɗ^����e���x // min:0 max:100 // �{�[��2�ւ̉e���x�́A(100 - bone_weight)
	u8		edge_flag; // 0:�ʏ�A1:�G�b�W���� // �G�b�W(�֊s)���L���̏ꍇ
}sPmdVertex;

typedef struct sPmdFace
{
	u16 vertexIndex[3];//�ʂ��`�����钸�_�R��
}sPmdFace;

typedef struct sPmdMaterial
{
	float diffuse_color[3]; // dr, dg, db // �����F
	float alpha;
	float specularity;
	float specular_color[3]; // sr, sg, sb // ����F
	float mirror_color[3]; // mr, mg, mb // ���F(ambient)
	u8 toon_index; // toon??.bmp // 0.bmp:0xFF, 1(01).bmp:0x00 �E�E�E 10.bmp:0x09
	u8 edge_flag; // �֊s�A�e
	u32 face_vert_count; // �ʒ��_�� // �C���f�b�N�X�ɕϊ�����ꍇ�́A�ގ�0���珇�ɉ��Z
	char texture_file_name[21]; //�e�N�X�`���t�@�C�����iPMD�t�@�C���͂Q�O�o�C�g�����ۑ����Ȃ����A0�I�[���������Ƃ�����̂�1���₵�Ă���j
}sPmdMaterial;

typedef struct sPmdBone
{
	char bone_name[20]; // �{�[����
	u16 parent_bone_index; // �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
	u16 tail_pos_bone_index; // tail�ʒu�̃{�[���ԍ�(�`�F�[�����[�̏ꍇ��0xFFFF) // �e�F�q��1�F���Ȃ̂ŁA��Ɉʒu���ߗp
	u8 bone_type; // �{�[���̎��
	u16 ik_parent_bone_index; // IK�{�[���ԍ�(�e��IK�{�[���B�Ȃ��ꍇ��0)
	float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
}sPmdBone;

typedef struct sPmdIkData
{
	u16 ik_bone_index; // IK�{�[���ԍ�
	u16 ik_target_bone_index; // IK�^�[�Q�b�g�{�[���ԍ� // IK�{�[�����ŏ��ɐڑ�����{�[��
	u8 ik_chain_length; // IK�`�F�[���̒���(�q�̐�)
	u16 iterations; // �ċA���Z�� // IK�l1
	float control_weight; // IK�̉e���x // IK�l2
	u16 *ik_child_bone_index; // IK�e�����̃{�[���ԍ�
}sPmdIkData;

typedef struct sPmdSkinVertex
{
	u32 skin_vert_index; // �\��p�̒��_�̔ԍ�(���_���X�g�ɂ���ԍ�)
	union
	{
		float skin_vert_pos[3]; // x, y, z // �\��p�̒��_�̍��W(���_���̂̍��W)
		float skin_vert_pos_offset[3]; // x, y, z // �\��p�̒��_�̍��W�I�t�Z�b�g�l(base�ɑ΂���I�t�Z�b�g)
	};
}sPmdSkinVertex;

typedef struct sPmdSkin
{
	char skin_name[20]; //�@�\�
	u32 skin_vert_count; // �\��p�̒��_��
	u8 skin_type; // �\��̎�� // 0�Fbase�A1�F�܂�A2�F�ځA3�F���b�v�A4�F���̑�
	sPmdSkinVertex *skin_vert_data; // �\��p�̒��_�̃f�[�^(16Bytes/vert)
}sPmdSkin;

typedef struct sPmdBoneDispName
{
	char name[50];
}sPmdBoneDispName;

typedef struct sPmdBoneDispList
{
	u16 bone_index; // �g�p�{�[���ԍ�
	u8 bone_disp_frame_index; // �\���g�ԍ�
}sPmdBoneDispList;

typedef struct sPmdEnglishHeader
{
	char model_name_eg[20]; // ���f����(�p��)
	char comment_eg[256]; // �R�����g(�p��)
}sPmdEnglishHeader;

typedef struct sPmdEnglishBoneName
{
	char name[20];
}sPmdEnglishBoneName;

typedef struct sPmdEnglishSkinName
{
	char name[20];
}sPmdEnglishSkinName;

typedef struct sPmdEnglishBoneDispName
{
	char name[50];
}sPmdEnglishBoneDispName;

typedef struct sPmdToonFileName
{
	char name[100];
}sPmdToonFileName;

typedef struct sPmdRigidBody
{
	char rigidbody_name[20]; // ���f�[�^�F���� // ��
	u16 rigidbody_rel_bone_index; // ���f�[�^�F�֘A�{�[���ԍ� // 03 00 == 3 // ��
	u8 rigidbody_group_index; // ���f�[�^�F�O���[�v // 00
	u16 rigidbody_group_target; // ���f�[�^�F�O���[�v�F�Ώ� // 0xFFFF�Ƃ̍� // 38 FE
	u8 shape_type; // �`��F�^�C�v(0:���A1:���A2:�J�v�Z��) // 00 // ��
	float shape_w; // �`��F���a(��) // CD CC CC 3F // 1.6
	float shape_h; // �`��F���� // CD CC CC 3D // 0.1
	float shape_d; // �`��F���s // CD CC CC 3D // 0.1
	float pos_pos[3]; // �ʒu�F�ʒu(x, y, z)
	float pos_rot[3]; // �ʒu�F��](rad(x), rad(y), rad(z))
	float rigidbody_weight; // ���f�[�^�F���� // 00 00 80 3F // 1.0
	float rigidbody_pos_dim; // ���f�[�^�F�ړ��� // 00 00 00 00
	float rigidbody_rot_dim; // ���f�[�^�F��]�� // 00 00 00 00
	float rigidbody_recoil; // ���f�[�^�F������ // 00 00 00 00
	float rigidbody_friction; // ���f�[�^�F���C�� // 00 00 00 00
	u8 rigidbody_type; // ���f�[�^�F�^�C�v(0:Bone�Ǐ]�A1:�������Z�A2:�������Z(Bone�ʒu����)) // 00 // Bone�Ǐ]
}sPmdRigidBody;

typedef struct sPmdJoint
{
	char joint_name[20]; // ���f�[�^�F���� // �E��1
	u32 joint_rigidbody_a; // ���f�[�^�F����A
	u32 joint_rigidbody_b; // ���f�[�^�F����B
	float joint_pos[3]; // ���f�[�^�F�ʒu(x, y, z) // ���f�[�^�F�ʒu�����ł��ݒ��
	float joint_rot[3]; // ���f�[�^�F��](rad(x), rad(y), rad(z))
	float constrain_pos_1[3]; // �����F�ړ�1(x, y, z)
	float constrain_pos_2[3]; // �����F�ړ�2(x, y, z)
	float constrain_rot_1[3]; // �����F��]1(rad(x), rad(y), rad(z))
	float constrain_rot_2[3]; // �����F��]2(rad(x), rad(y), rad(z))
	float spring_pos[3]; // �΂ˁF�ړ�(x, y, z)
	float spring_rot[3]; // �΂ˁF��](rad(x), rad(y), rad(z))
}sPmdJoint;

typedef struct sPmdFile
{
	sPmdHeader header;

	u32 vertexNum;
	sPmdVertex *vertex;

	u32 faceNum;
	sPmdFace *face;

	u32 materialNum;
	sPmdMaterial *material;

	u16 boneNum;
	sPmdBone *bone;

	u16 ikDataNum;
	sPmdIkData *ikData;

	u16 skinNum;
	sPmdSkin *skin;

	u8 skinIndexNum;
	u16 *skinIndex;

	u8 boneDispNameNum;
	sPmdBoneDispName *boneDispName;

	u16 boneDispListNum;
	sPmdBoneDispList *boneDispList;

	u8 englishExpansionFlag;
	sPmdEnglishHeader englishHeader;
	sPmdEnglishBoneName *englishBoneName;
	sPmdEnglishSkinName *englishSkinName;
	sPmdEnglishBoneDispName *englishBoneDispName;

	sPmdToonFileName toonFileName[10];

	u16 rigidBodyNum;
	sPmdRigidBody *rigidBody;

	u32 jointNum;
	sPmdJoint *joint;
}sPmdFile;

sPmdFile* PmdPerse(const char *filename);
void PmdDestruct(sPmdFile *pmd);

#ifdef __cplusplus
}
#endif


#endif
