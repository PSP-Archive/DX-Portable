#include "dxlibp.h"
#include <pspdebug.h>
#include <pspinit.h>
#include <pspthreadman.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pspgu.h>
#include <math.h>
#include "graphics/3d/model.h"
#include "graphics/3d/light.h"

PSP_MODULE_INFO("TEST",0,1,1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


#ifndef PMDREADER_H__
#define PMDREADER_H__

typedef struct sPmdHeader
{
	char	magic[3]; // "Pmd"
	float	version; // 00 00 80 3F == 1.00
	char	model_name[20];
	char	comment[256];
}sPmdHeader;

typedef struct sPmdVertex
{
	float	pos[3]; // x, y, z // 座標
	float	normal_vec[3]; // nx, ny, nz // 法線ベクトル
	float	uv[2]; // u, v // UV座標 // MMDは頂点UV
	u16		bone_num[2]; // ボーン番号1、番号2 // モデル変形(頂点移動)時に影響
	u8		bone_weight; // ボーン1に与える影響度 // min:0 max:100 // ボーン2への影響度は、(100 - bone_weight)
	u8		edge_flag; // 0:通常、1:エッジ無効 // エッジ(輪郭)が有効の場合
}sPmdVertex;

typedef struct sPmdFace
{
	u16 vertexIndex[3];//面を形成する頂点３つ
}sPmdFace;

typedef struct sPmdMaterial
{
	float diffuse_color[3]; // dr, dg, db // 減衰色
	float alpha;
	float specularity;
	float specular_color[3]; // sr, sg, sb // 光沢色
	float mirror_color[3]; // mr, mg, mb // 環境色(ambient)
	u8 toon_index; // toon??.bmp // 0.bmp:0xFF, 1(01).bmp:0x00 ・・・ 10.bmp:0x09
	u8 edge_flag; // 輪郭、影
	u32 face_vert_count; // 面頂点数 // インデックスに変換する場合は、材質0から順に加算
	char texture_file_name[21]; //テクスチャファイル名（PMDファイルは２０バイトしか保存しないが、0終端が無いことがあるので1増やしている）
}sPmdMaterial;

typedef struct sPmdBone
{
	char bone_name[20]; // ボーン名
	u16 parent_bone_index; // 親ボーン番号(ない場合は0xFFFF)
	u16 tail_pos_bone_index; // tail位置のボーン番号(チェーン末端の場合は0xFFFF) // 親：子は1：多なので、主に位置決め用
	u8 bone_type; // ボーンの種類
	u16 ik_parent_bone_index; // IKボーン番号(影響IKボーン。ない場合は0)
	float bone_head_pos[3]; // x, y, z // ボーンのヘッドの位置
}sPmdBone;

typedef struct sPmdIkData
{
	u16 ik_bone_index; // IKボーン番号
	u16 ik_target_bone_index; // IKターゲットボーン番号 // IKボーンが最初に接続するボーン
	u8 ik_chain_length; // IKチェーンの長さ(子の数)
	u16 iterations; // 再帰演算回数 // IK値1
	float control_weight; // IKの影響度 // IK値2
	u16 *ik_child_bone_index; // IK影響下のボーン番号
}sPmdIkData;

typedef struct sPmdSkinVertex
{
	u32 skin_vert_index; // 表情用の頂点の番号(頂点リストにある番号)
	union
	{
		float skin_vert_pos[3]; // x, y, z // 表情用の頂点の座標(頂点自体の座標)
		float skin_vert_pos_offset[3]; // x, y, z // 表情用の頂点の座標オフセット値(baseに対するオフセット)
	};
}sPmdSkinVertex;

typedef struct sPmdSkin
{
	char skin_name[20]; //　表情名
	u32 skin_vert_count; // 表情用の頂点数
	u8 skin_type; // 表情の種類 // 0：base、1：まゆ、2：目、3：リップ、4：その他
	sPmdSkinVertex *skin_vert_data; // 表情用の頂点のデータ(16Bytes/vert)
}sPmdSkin;

typedef struct sPmdBoneDispName
{
	char name[50];
}sPmdBoneDispName;

typedef struct sPmdBoneDispList
{
	u16 bone_index; // 枠用ボーン番号
	u8 bone_disp_frame_index; // 表示枠番号
}sPmdBoneDispList;

typedef struct sPmdEnglishHeader
{
	char model_name_eg[20]; // モデル名(英語)
	char comment_eg[256]; // コメント(英語)
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
	char rigidbody_name[20]; // 諸データ：名称 // 頭
	u16 rigidbody_rel_bone_index; // 諸データ：関連ボーン番号 // 03 00 == 3 // 頭
	u8 rigidbody_group_index; // 諸データ：グループ // 00
	u16 rigidbody_group_target; // 諸データ：グループ：対象 // 0xFFFFとの差 // 38 FE
	u8 shape_type; // 形状：タイプ(0:球、1:箱、2:カプセル) // 00 // 球
	float shape_w; // 形状：半径(幅) // CD CC CC 3F // 1.6
	float shape_h; // 形状：高さ // CD CC CC 3D // 0.1
	float shape_d; // 形状：奥行 // CD CC CC 3D // 0.1
	float pos_pos[3]; // 位置：位置(x, y, z)
	float pos_rot[3]; // 位置：回転(rad(x), rad(y), rad(z))
	float rigidbody_weight; // 諸データ：質量 // 00 00 80 3F // 1.0
	float rigidbody_pos_dim; // 諸データ：移動減 // 00 00 00 00
	float rigidbody_rot_dim; // 諸データ：回転減 // 00 00 00 00
	float rigidbody_recoil; // 諸データ：反発力 // 00 00 00 00
	float rigidbody_friction; // 諸データ：摩擦力 // 00 00 00 00
	u8 rigidbody_type; // 諸データ：タイプ(0:Bone追従、1:物理演算、2:物理演算(Bone位置合せ)) // 00 // Bone追従
}sPmdRigidBody;

typedef struct sPmdJoint
{
	char joint_name[20]; // 諸データ：名称 // 右髪1
	u32 joint_rigidbody_a; // 諸データ：剛体A
	u32 joint_rigidbody_b; // 諸データ：剛体B
	float joint_pos[3]; // 諸データ：位置(x, y, z) // 諸データ：位置合せでも設定可
	float joint_rot[3]; // 諸データ：回転(rad(x), rad(y), rad(z))
	float constrain_pos_1[3]; // 制限：移動1(x, y, z)
	float constrain_pos_2[3]; // 制限：移動2(x, y, z)
	float constrain_rot_1[3]; // 制限：回転1(rad(x), rad(y), rad(z))
	float constrain_rot_2[3]; // 制限：回転2(rad(x), rad(y), rad(z))
	float spring_pos[3]; // ばね：移動(x, y, z)
	float spring_rot[3]; // ばね：回転(rad(x), rad(y), rad(z))
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


#endif

float fps()
{
	static int prev = 0;
	int current = GetNowCount();
	float result = 1000.0f / (current - prev);
	prev = current;
	return result;
}

int main(int argc,char *argv[])
{
	DxLib_Init();
	sPmdFile *pmd = PmdPerse("OreNoYome.pmd");
	if(pmd == NULL)
	{
		pspDebugScreenInit();
		pspDebugScreenPrintf("unko");
		sceKernelDelayThread(10000000);
		sceKernelExitGame();
	}

	cModel model(1);
	cModel miku("OreNoYome.pmd");//1-32,33,34

	//for(int i = 0;i < miku.Meshes->Buf[1].MicroMesh->Buf[33].VertexBuffer->Length;++i)
	//{
	//	cModel::sVertex &v = miku.Meshes->Buf[1].MicroMesh->Buf[33].VertexBuffer->Buf[i];
	//	printfDx("(%f,%f,%f),",v.pos[0],v.pos[1],v.pos[2]);
	//}
	//printfDx("\n");
	//for(int i = 0;i < miku.Meshes->Buf[1].MicroMesh->Buf[33].IndexBuffer->Length / 3;++i)
	//{
	//	u16 *idx = miku.Meshes->Buf[1].MicroMesh->Buf[33].IndexBuffer->Buf + i * 3;
	//	printfDx("(%d,%d,%d),",(int)idx[0],(int)idx[1],(int)idx[2]);
	//}






	SetUseZBuffer3D(1);
	SetWriteZBuffer3D(1);
	float x = 100,y  = 50;
	float theta = 0;
	while(ProcessMessage() != -1)
	{
		theta += 0.03;
		if(theta > 6.28)theta = 0;
		model.Bones->Buf[0].Rotation = QGetRotX(theta);
		if(GetInputState() & DXP_INPUT_UP)y += 2;
		if(GetInputState() & DXP_INPUT_DOWN)y -= 2;
		if(GetInputState() & DXP_INPUT_LEFT)x -= 2;
		if(GetInputState() & DXP_INPUT_RIGHT)x += 2;

		miku.Bones->Buf[1].Rotation = QGetRotY(theta);


		MATRIX mat,mat2;

		mat2 =MGetRotX(3.14159265358979f / 2);

		mat.m[0][0] = 10;mat.m[0][1] = 0;mat.m[0][2] = 0;mat.m[0][3] = 0;
		mat.m[1][0] = 0;mat.m[1][1] = 10;mat.m[1][2] = 0;mat.m[1][3] = 0;
		mat.m[2][0] = 0;mat.m[2][1] = 0;mat.m[2][2] = 10;mat.m[2][3] = 0;
		mat.m[3][0] = x;mat.m[3][1] = y;mat.m[3][2] = 0;mat.m[3][3] = 1;

//		mat = MMult(mat,mat2);


		SetTransformToWorld(&mat);

		ClearDrawScreen();
		clsDx();
		printfDx("fps::%f\n",fps());
//		clsDx();
		//int mId = 0;
		//int m = 0;
		//VECTOR pos[3];
		//int color;
		//for(int i = 0;i < pmd->faceNum;++i)
		//{
		//	color = GetColor(
		//		255 * pmd->material[mId].diffuse_color[0],
		//		255 * pmd->material[mId].diffuse_color[1],
		//		255 * pmd->material[mId].diffuse_color[2]
		//	);
		//	for(int j = 0;j < 3;++j)
		//	{
		//		pos[j].x = pmd->vertex[pmd->face[i].vertexIndex[j]].pos[0];
		//		pos[j].y = pmd->vertex[pmd->face[i].vertexIndex[j]].pos[1];
		//		pos[j].z = pmd->vertex[pmd->face[i].vertexIndex[j]].pos[2];
		//	}
		//	DrawTriangle3D(pos[0],pos[1],pos[2],color,1);
		//	++m;
		//	if(m * 3 >= pmd->material[mId].face_vert_count){m = 0;++mId;}
		//}
		DXPLIGHT light;
		light.Type = GU_DIRECTIONAL;
		light.Position.x = -100;
		light.Position.y = 300;
		light.Direction.y = 100;
		light.Update(0,&light);
		sceGuEnable(GU_LIGHTING);
		sceGuShadeModel(GU_SMOOTH);

		//model.Draw();
		miku.Draw();

		ScreenFlip();
	}
	DxLib_End();
	PmdDestruct(pmd);
return 0;
}

sPmdFile* PmdPerse(const char *filename)
{
	unsigned int i,j;
	sPmdFile *pmd;
	FILE *fp;
	pmd = (sPmdFile*)malloc(sizeof(sPmdFile));
	if(!pmd)return NULL;

	memset(pmd,0,sizeof(sPmdFile));

	//ファイルオープン
	fp = fopen(filename,"rb");
	if(!fp)return NULL;
	
	//PMDファイルヘッダ読み出し
	fread(pmd->header.magic,1,3,fp);
	fread(&pmd->header.version,4,1,fp);
	fread(pmd->header.model_name,1,20,fp);
	fread(pmd->header.comment,1,256,fp);

	//頂点読み出し
	fread(&pmd->vertexNum,4,1,fp);
	if(pmd->vertexNum)
	{
		pmd->vertex = (sPmdVertex*)malloc(sizeof(sPmdVertex) * pmd->vertexNum);
		if(!pmd->vertex)goto err;
		for(i = 0;i < pmd->vertexNum;++i)
		{
			fread(pmd->vertex[i].pos,4,3,fp);
			fread(pmd->vertex[i].normal_vec,4,3,fp);
			fread(pmd->vertex[i].uv,4,2,fp);
			fread(pmd->vertex[i].bone_num,2,2,fp);
			fread(&pmd->vertex[i].bone_weight,1,1,fp);
			fread(&pmd->vertex[i].edge_flag,1,1,fp);
		}
	}
	
	//面読み出し
	fread(&pmd->faceNum,4,1,fp);
	if(pmd->faceNum % 3)goto err;
	pmd->faceNum /= 3;
	if(pmd->faceNum)
	{
		pmd->face = (sPmdFace*)malloc(sizeof(sPmdFace) * pmd->faceNum);
		if(!pmd->face)goto err;
		for(i = 0;i < pmd->faceNum;++i)
			fread(pmd->face[i].vertexIndex,2,3,fp);
	}

	//マテリアル読み出し
	fread(&pmd->materialNum,4,1,fp);
	if(pmd->materialNum)
	{
		pmd->material = (sPmdMaterial*)malloc(sizeof(sPmdMaterial) * pmd->materialNum);
		if(!pmd->material)goto err;
		for(i = 0;i < pmd->materialNum;++i)
		{
			fread(pmd->material[i].diffuse_color,4,3,fp);
			fread(&pmd->material[i].alpha,4,1,fp);
			fread(&pmd->material[i].specularity,4,1,fp);
			fread(pmd->material[i].specular_color,4,3,fp);
			fread(pmd->material[i].mirror_color,4,3,fp);
			fread(&pmd->material[i].toon_index,1,1,fp);
			fread(&pmd->material[i].edge_flag,1,1,fp);
			fread(&pmd->material[i].face_vert_count,4,1,fp);
			fread(pmd->material[i].texture_file_name,1,20,fp);
			pmd->material[i].texture_file_name[20] = '\0';
		}
	}

	//ボーン読み出し
	fread(&pmd->boneNum,2,1,fp);
	if(pmd->boneNum)
	{
		pmd->bone = (sPmdBone*)malloc(sizeof(sPmdBone) * pmd->boneNum);
		if(!pmd->bone)goto err;
		for(i = 0;i < pmd->boneNum;++i)
		{
			fread(pmd->bone[i].bone_name,1,20,fp);
			fread(&pmd->bone[i].parent_bone_index,2,1,fp);
			fread(&pmd->bone[i].tail_pos_bone_index,2,1,fp);
			fread(&pmd->bone[i].bone_type,1,1,fp);
			fread(&pmd->bone[i].ik_parent_bone_index,2,1,fp);
			fread(pmd->bone[i].bone_head_pos,4,3,fp);
		}
	}
	
	//IKデータ読み出し
	fread(&pmd->ikDataNum,2,1,fp);
	if(pmd->ikDataNum)
	{
		pmd->ikData = (sPmdIkData*)malloc(sizeof(sPmdIkData) * pmd->ikDataNum);
		if(!pmd->ikData)goto err;
		memset(pmd->ikData,0,sizeof(sPmdIkData) * pmd->ikDataNum);
		for(i = 0;i < pmd->ikDataNum;++i)
		{
			fread(&pmd->ikData[i].ik_bone_index,2,1,fp);
			fread(&pmd->ikData[i].ik_target_bone_index,2,1,fp);
			fread(&pmd->ikData[i].ik_chain_length,1,1,fp);
			fread(&pmd->ikData[i].iterations,2,1,fp);
			fread(&pmd->ikData[i].control_weight,4,1,fp);
			if(pmd->ikData[i].ik_chain_length)
			{
				pmd->ikData[i].ik_child_bone_index = (u16*)malloc(2 * pmd->ikData[i].ik_chain_length);
				if(!pmd->ikData[i].ik_target_bone_index)goto err;
				fread(pmd->ikData[i].ik_child_bone_index,2,pmd->ikData[i].ik_chain_length,fp);
			}
		}
	}


	//表情読み出し
	fread(&pmd->skinNum,2,1,fp);
	if(pmd->skinNum)
	{
		pmd->skin = (sPmdSkin*)malloc(sizeof(sPmdSkin) * pmd->skinNum);
		if(!pmd->skin)goto err;
		memset(pmd->skin,0,sizeof(sPmdSkin) * pmd->skinNum);
		for(i = 0;i < pmd->skinNum;++i)
		{
			fread(pmd->skin[i].skin_name,1,20,fp);
			fread(&pmd->skin[i].skin_vert_count,4,1,fp);
			fread(&pmd->skin[i].skin_type,1,1,fp);
			if(pmd->skin[i].skin_vert_count)
			{
				pmd->skin[i].skin_vert_data = (sPmdSkinVertex*)malloc(16 * pmd->skin[i].skin_vert_count);
				if(!pmd->skin[i].skin_vert_data)goto err;
				for(j = 0;j < pmd->skin[i].skin_vert_count;++j)
				{
					fread(&pmd->skin[i].skin_vert_data[j].skin_vert_index,4,1,fp);
					fread(pmd->skin[i].skin_vert_data[j].skin_vert_pos,4,3,fp);
				}
			}
		}
	}


	//表情枠用表示リスト読み出し
	fread(&pmd->skinIndexNum,1,1,fp);
	if(pmd->skinIndexNum)
	{
		pmd->skinIndex = (u16*)malloc(2 * pmd->skinIndexNum);
		if(!pmd->skinIndex)goto err;
		fread(pmd->skinIndex,2,pmd->skinIndexNum,fp);
	}

	//ボーン枠用枠名読み出し
	fread(&pmd->boneDispNameNum,1,1,fp);
	if(pmd->boneDispNameNum)
	{
		pmd->boneDispName = (sPmdBoneDispName*)malloc(sizeof(sPmdBoneDispName) * pmd->boneDispNameNum);
		if(!pmd->boneDispName)goto err;
		for(i = 0;i < pmd->boneDispNameNum;++i)
		{
			fread(pmd->boneDispName[i].name,50,1,fp);
			for(j = 0;j < 50;++j)
				if(pmd->boneDispName[i].name[j] == 0)
				{
					if(pmd->boneDispName[i].name[j - 1] == 0x0a)
						pmd->boneDispName[i].name[j - 1] = '\0';
					break;
				}
		}
	}

	//ボーン枠用表示リスト読み出し
	fread(&pmd->boneDispListNum,4,1,fp);
	if(pmd->boneDispListNum)
	{
		pmd->boneDispList = (sPmdBoneDispList*)malloc(sizeof(sPmdBoneDispList) * pmd->boneDispListNum);
		if(!pmd->boneDispList)goto err;
		for(i = 0;i < pmd->boneDispListNum;++i)
		{
			fread(&pmd->boneDispList[i].bone_index,2,1,fp);
			fread(&pmd->boneDispList[i].bone_disp_frame_index,1,1,fp);
		}
	}

	//英名拡張判定
	if(fread(&pmd->englishExpansionFlag,1,1,fp) == 0)goto end;
	if(pmd->englishExpansionFlag != 1)goto end;
	
	//英名ヘッダ読み込み
	fread(pmd->englishHeader.model_name_eg,1,20,fp);
	fread(pmd->englishHeader.comment_eg,1,256,fp);

	//英ボーン名読み込み
	if(pmd->boneNum)
	{
		pmd->englishBoneName = (sPmdEnglishBoneName*)malloc(sizeof(sPmdEnglishBoneName) * pmd->boneNum);
		if(!pmd->englishBoneName)goto err;
		for(i = 0;i < pmd->boneNum;++i)
			fread(pmd->englishBoneName[i].name,20,1,fp);
	}

	//英スキン名読み込み
	if(pmd->skinNum >= 2)
	{
		pmd->englishSkinName = (sPmdEnglishSkinName*)malloc(sizeof(sPmdEnglishSkinName) * (pmd->skinNum - 1));
		if(!pmd->englishSkinName)goto err;
		for(i = 0;(int)i < pmd->skinNum - 1;++i)
			fread(pmd->englishSkinName[i].name,20,1,fp);
	}

	//英ボーン枠用枠名読み出し
	if(pmd->boneDispNameNum)
	{
		pmd->englishBoneDispName = (sPmdEnglishBoneDispName*)malloc(sizeof(sPmdEnglishBoneDispName) * pmd->boneDispNameNum);
		if(!pmd->englishBoneDispName)goto err;
		for(i = 0;i < pmd->boneDispNameNum;++i)
			fread(pmd->englishBoneDispName[i].name,50,1,fp);
	}

	//トゥーンテクスチャファイル名読み込み
	for(i = 0;i < 10;++i)
		fread(pmd->toonFileName[i].name,1,100,fp);

	//剛体データ読み込み
	fread(&pmd->rigidBodyNum,4,1,fp);
	if(pmd->rigidBodyNum)
	{
		pmd->rigidBody = (sPmdRigidBody*)malloc(sizeof(sPmdRigidBody) * pmd->rigidBodyNum);
		if(!pmd->rigidBody)goto err;
		for(i = 0;i < pmd->rigidBodyNum;++i)
		{
			fread(&pmd->rigidBody[i].rigidbody_name,20,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_rel_bone_index,2,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_group_index,1,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_group_target,2,1,fp);
			fread(&pmd->rigidBody[i].shape_type,1,1,fp);
			fread(&pmd->rigidBody[i].shape_w,4,1,fp);
			fread(&pmd->rigidBody[i].shape_h,4,1,fp);
			fread(&pmd->rigidBody[i].shape_d,4,1,fp);
			fread(pmd->rigidBody[i].pos_pos,4,3,fp);
			fread(pmd->rigidBody[i].pos_rot,4,3,fp);
			fread(&pmd->rigidBody[i].rigidbody_weight,4,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_pos_dim,4,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_rot_dim,4,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_recoil,4,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_friction,4,1,fp);
			fread(&pmd->rigidBody[i].rigidbody_type,1,1,fp);
		}
	}

	//ジョイント読み込み
	fread(&pmd->jointNum,4,1,fp);
	if(pmd->jointNum)
	{
		pmd->joint = (sPmdJoint*)malloc(sizeof(sPmdJoint) * pmd->jointNum);
		if(!pmd->joint)goto err;
		for(i = 0;i < pmd->jointNum;++i)
		{
			fread(pmd->joint[i].joint_name,1,20,fp);
			fread(&pmd->joint[i].joint_rigidbody_a,4,1,fp);
			fread(&pmd->joint[i].joint_rigidbody_b,4,1,fp);
			fread(pmd->joint[i].joint_pos,4,3,fp);
			fread(pmd->joint[i].joint_rot,4,3,fp);
			fread(pmd->joint[i].constrain_pos_1,4,3,fp);
			fread(pmd->joint[i].constrain_pos_2,4,3,fp);
			fread(pmd->joint[i].constrain_rot_1,4,3,fp);
			fread(pmd->joint[i].constrain_rot_2,4,3,fp);
			fread(pmd->joint[i].spring_pos,4,3,fp);
			fread(pmd->joint[i].spring_rot,4,3,fp);
		}
	}
end:
	//ファイルクローズ
	fclose(fp);
	return pmd;
err:
	fclose(fp);
	PmdDestruct(pmd);
	return NULL;
}

void PmdDestruct(sPmdFile *pmd)
{
	int i;
	free(pmd->vertex);
	free(pmd->face);
	free(pmd->material);
	free(pmd->bone);
	if(pmd->ikData)
	{
		for(i = 0;i < pmd->ikDataNum;++i)
			free(pmd->ikData[i].ik_child_bone_index);
	}
	free(pmd->ikData);
	if(pmd->skin)
	{
		for(i = 0;i < pmd->skinNum;++i)
			free(pmd->skin[i].skin_vert_data);
	}
	free(pmd->skin);
	free(pmd->skinIndex);
	free(pmd->boneDispName);
	free(pmd->boneDispList);
	free(pmd->englishBoneName);
	free(pmd->englishSkinName);
	free(pmd->englishBoneDispName);
	free(pmd->rigidBody);
	free(pmd->joint);
	memset(pmd,0,sizeof(sPmdFile));
	free(pmd);
}

