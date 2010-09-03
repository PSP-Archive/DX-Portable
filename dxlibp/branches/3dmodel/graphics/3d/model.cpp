extern "C"
{
#include "../../graphics.h"
#include "pmdReader.h" //どんどん変なものが追加されてます
}
#include "light.h"
#include "model.h"
#ifndef DXP_BUILDOPTION_NO3D
extern "C"
{
	void sendCommandi(int cmd, int argument);
	void sendCommandiStall(int cmd, int argument);
	void sendCommandf(int cmd, float argument);
}

void dxpGuModelColor(u32 emissive,u32 ambient,u32 diffuse,u32 specular)
{
	sendCommandi(84, emissive & 0xffffff);
	sendCommandi(86, diffuse & 0xffffff);
	sendCommandi(85, ambient & 0xffffff);
	sendCommandi(88, ambient >> 24);//ここだけ公式に追加
	sendCommandi(87, specular & 0xffffff);
}

void cModel::Draw()
{
	if(!IsAvailable || !Visible)return;


	RenewBoneMatrix();
	GUSTART;
	dxpGraphicsSetup3D(0xffffffff);
	dxpGraphicsData.drawstate = DXP_DRAWSTATE_MODEL;
	for(int fId = 0;fId < Frames->Length;++fId)
	{
		sFrame &frame = Frames->Buf[fId];
		if(!frame.Visible)continue;
		for(int mId = 0;mId < frame.MeshList.size();++mId)
		{
			sMesh& mesh = Meshes->Buf[mId];
			if(!mesh.Visible)continue;
			//マテリアル設定
			//テクスチャの設定も今はいいや
			sceGuDisable(GU_TEXTURE_2D);
			sMaterial &material = Materials->Buf[mesh.MaterialId];
			u32 amb,dif,spc,ems;//この辺あとでフレームやメッシュからの数値を計算するように変更
			amb = material.AmbientColor;
			dif = material.DiffuseColor;
			spc = material.SpecularColor;
			ems = material.EmissiveColor;
			dxpGuModelColor(ems,amb,dif,spc);
			//Bone設定
			for(int mmId = 0;mmId < mesh.MicroMesh->Length;++mmId)
			{
				sMesh::sMicroMesh &mmesh = mesh.MicroMesh->Buf[mmId];
				MATRIX ident = MGetIdent();
				for(int b = 0;b < 4;++b)sceGuBoneMatrix(b,mmesh.BoneId[b] == -1 ? &ident.pspm : &Bones->Buf[mmesh.BoneId[b]].BoneMatrix.pspm);
				sceGumDrawArray(GU_TRIANGLES,SVERTEX_TYPE | GU_TRANSFORM_3D,mmesh.IndexBuffer->Length,mmesh.IndexBuffer->Buf,mmesh.VertexBuffer->Buf);
			}
			
		}
	}
}

cModel::cModel(const char *filename)
{
	IsAvailable = true;
	if(LoadMMD(filename) == 0)return;
	IsAvailable = false;
}

int cModel::LoadMMD(const char* filename)
{
	return -1;
//	sPmdFile *pmd = PmdPerse(filename);
//	if(pmd == NULL)return -1;
//	//この手の異種データのすりあわせが一番めんどくさい気がします。
//	//頂点
//	//Vertices = new tLinerBuffer<sVertex>(pmd->vertexNum);
//	/*
//		MeshをMicroMeshに分けつつ頂点リストとインデックスバッファを再構築。
//		baseフレーム→マテリアル毎のメッシュ→ボーン毎のマイクロメッシュ（頂点バッファとインデックスバッファを持つ）
//	*/
//	{//baseフレームの作成
//		sFrame frame_;
//		Frames.push_back(frame);
//	}
//
//	for(int i = 0;i < pmd->materialNum;++i)
//	{
//		sMaterial material;
//		material.AmbientColor = (static_cast<int>(pmd->material[i].alpha * 255) & 0xff) << 24;material.AmbientColor 
//			|= ((static_cast<int>(pmd->material[i].mirror_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].mirror_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].mirror_color[0] * 255) & 0xff);
//		material.DiffuseColor = ((static_cast<int>(pmd->material[i].diffuse_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].diffuse_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].diffuse_color[0] * 255) & 0xff);
//		material.EmissiveColor = 0;
//		material.SpecularColor = ((static_cast<int>(pmd->material[i].specular_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].specular_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].specular_color[0] * 255) & 0xff);
//		material.SpecularPower = pmd->material[i].specularity;
//		Materials.push_back(material);
//		sMesh mesh;
//		mesh.FrameId = 0;
//		mesh.MaterialId = i;
//		{//MicroMeshへの切り分け。
//			//参照ボーンのリストアップ
//			
//
//			//グループ分け
//			//頂点リスト、インデックスバッファ再構築
//			//登録
//		}
//		Meshes.push_back(mesh);
//		Frames[0].MeshList.push_back(i);
//	}
//
//
//
////	if(Vertices == NULL);//例外機構は無効になってるのでNULLチェック。何するかは後で考える
//	for(int i = 0;i < pmd->vertexNum;++i)
//	{
//		Vertices->Buf[i].pos[0] = pmd->vertex[i].pos[0];
//		Vertices->Buf[i].pos[1] = pmd->vertex[i].pos[1];
//		Vertices->Buf[i].pos[2] = pmd->vertex[i].pos[2];
//		Vertices->Buf[i].norm[0] = pmd->vertex[i].normal_vec[0];
//		Vertices->Buf[i].norm[1] = pmd->vertex[i].normal_vec[1];
//		Vertices->Buf[i].norm[2] = pmd->vertex[i].normal_vec[2];
//		Vertices->Buf[i].u = pmd->vertex[i].uv[0];
//		Vertices->Buf[i].v = pmd->vertex[i].uv[1];
//		Vertices->Buf[i].weight[0] = pmd->vertex[i].bone_weight / 100.0f;//ほんとは0-1の範囲に収まってるかチェックするんだけど後回し
//		Vertices->Buf[i].weight[1] = 1.0f - (pmd->vertex[i].bone_weight / 100.0f);
//		Vertices->Buf[i].weight[2] = 0.0f;
//		Vertices->Buf[i].weight[3] = 0.0f;
//	}
//	//マテリアル
//	for(int i = 0;i < pmd->materialNum;++i)
//	{
//		sMaterial material;
//		material.AmbientColor = (static_cast<int>(pmd->material[i].alpha * 255) & 0xff) << 24;material.AmbientColor 
//			|= ((static_cast<int>(pmd->material[i].mirror_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].mirror_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].mirror_color[0] * 255) & 0xff);
//		material.DiffuseColor = ((static_cast<int>(pmd->material[i].diffuse_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].diffuse_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].diffuse_color[0] * 255) & 0xff);
//		material.EmissiveColor = 0;
//		material.SpecularColor = ((static_cast<int>(pmd->material[i].specular_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].specular_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].specular_color[0] * 255) & 0xff);
//		material.SpecularPower = pmd->material[i].specularity;
//		Materials.push_back(material);
//	}
//	//フレーム　MMDにフレームなんてあるのだらうか　・・・あ、そういえばメッシュがグループ別になってたっけ
//	//マテリアルの数だけ最初のフレームに突っ込む。
//	int faceCnt = 0;
//	int boneRef[4];
//	for(int i = 0;pmd->material;++i)
//	{
//		boneRef[0] = boneRef[1] = boneRef[2] = boneRef[3] = -1;
//		sMesh mesh;
//		mesh.FrameId = 0;
//		
//		mesh.IndexBuffer = new tLinerBuffer<u16>(pmd->material[i].face_vert_count);
//		for(int j = 0;j < mesh.IndexBuffer->Length / 3;++j)
//		{
//			mesh.IndexBuffer->Buf[j * 3 + 0] = pmd->face[faceCnt].vertexIndex[0];
//			mesh.IndexBuffer->Buf[j * 3 + 1] = pmd->face[faceCnt].vertexIndex[1];
//			mesh.IndexBuffer->Buf[j * 3 + 2] = pmd->face[faceCnt].vertexIndex[2];
////ボーン検出
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 0]].bone_num[0];
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 0]].bone_num[1];
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 1]].bone_num[0];
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 1]].bone_num[1];
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 2]].bone_num[0];
//			pmd->vertex[mesh.IndexBuffer->Buf[j * 3 + 2]].bone_num[1];
////３つ以上のボーンが１つのメッシュで参照されてると、頂点リスト側を変更しないといけないのでめんどっちい。
//			++faceCnt;
//			if(faceCnt >= pmd->faceNum);//えらー
//		}
//		mesh.MaterialId = i;
//		mesh.Visible = true;
//	}
//	//後はスキン種別ごとにフレーム作る。
//
//	//メッシュ　ボーン参照数が多かったらアウト！
//	PmdDestruct(pmd);
//	return 0;
}

void cModel::RenewBoneMatrix()
{
	RenewBoneMatrixHelper(0);

}

void QuaternionToMatrix(MATRIX *m,QUATERNION *q)
{
	m->m[0][0] = 2 * (q->y * q->y + q->z * q->z);	m->m[0][1] = 2 * (q->x * q->y + q->w * q->z);	m->m[0][2] = 2 * (q->x * q->z - q->w * q->y);
	m->m[1][0] = 2 * (q->x * q->y - q->w * q->z);	m->m[1][1] = 2 * (q->x * q->x + q->z * q->z);	m->m[1][2] = 2 * (q->y * q->z + q->w * q->x);
	m->m[2][0] = 2 * (q->x * q->z + q->w * q->y);	m->m[2][1] = 2 * (q->y * q->z - q->w * q->x);	m->m[2][2] = 2 * (q->x * q->x + q->y * q->y);
	m->m[0][0] = 1 - m->m[0][0];	m->m[0][3] = 0;
	m->m[1][1] = 1 - m->m[1][1];	m->m[1][3] = 0;
	m->m[2][2] = 1 - m->m[2][2];	m->m[2][3] = 0;
	m->m[3][0] = 0;m->m[3][1] = 0;m->m[3][2] = 0;m->m[3][3] = 1;
}

void cModel::RenewBoneMatrixHelper(int id)
{
	sBone &bone = Bones->Buf[id];
	MATRIX rot,mv,mvi;
	mvi = mv = MGetIdent();
	mv.m[3][0] = bone.Position.x;mvi.m[3][0] = -bone.Position.x;
	mv.m[3][1] = bone.Position.y;mvi.m[3][1] = -bone.Position.y;
	mv.m[3][2] = bone.Position.z;mvi.m[3][2] = -bone.Position.z;
	QuaternionToMatrix(&rot,&bone.Rotation);
	bone.BoneMatrix = MMult(MMult(mvi,rot),mv);

	if(bone.ParentBoneId != -1)
		bone.BoneMatrix = MMult(bone.BoneMatrix,Bones->Buf[bone.ParentBoneId].BoneMatrix);	
	if(bone.ChildBoneId != -1)
		RenewBoneMatrixHelper(bone.ChildBoneId);
	if(bone.NextBrotherId != -1)
		RenewBoneMatrixHelper(bone.NextBrotherId);
}

cModel::cModel(int i)
{//テスト用のコンストラクタ。
	{
		Materials = new tLinerBuffer<sMaterial>(1);
		sMaterial &material = Materials->Buf[0];
		material.AmbientColor = 0xff0000ff;
		material.DiffuseColor = 0xff0000ff;
		material.EmissiveColor = 0xff0000ff;
		material.SpecularColor = 0xffffffff;
		material.SpecularPower = 0.5f;
	}
	{
		Bones = new tLinerBuffer<sBone>(1);
		sBone &bone = Bones->Buf[0];
		bone.ChildBoneId = bone.NextBrotherId = bone.ParentBoneId = -1;
		bone.Rotation.x = 0;
		bone.Rotation.y = 0;
		bone.Rotation.z = 0;
		bone.Rotation.w = 1;
		bone.Position.x = 10;
		bone.Position.y = 0;
		bone.Position.z = 0;
	}
	{
		Meshes = new tLinerBuffer<sMesh>(1);
		sMesh &mesh = Meshes->Buf[0];
		mesh.FrameId = 0;
		mesh.MaterialId = 0;
		mesh.MicroMesh = new tLinerBuffer<sMesh::sMicroMesh>(1);
		{
			sMesh::sMicroMesh &mmesh = mesh.MicroMesh->Buf[0];
			mmesh.BoneId[0] = 0;mmesh.BoneId[1] = -1;mmesh.BoneId[2] = -1;mmesh.BoneId[3] = -1;
			mmesh.VertexBuffer = new tLinerBuffer<sVertex>(24);
			mmesh.IndexBuffer = new tLinerBuffer<u16>(132);
			float r2 = 0.7071f;
			for(int i = 0;i < 6;++i)
			{
				{
					sVertex &v = mmesh.VertexBuffer->Buf[i * 4 + 0];
					v.pos[0] = 10 * i;v.pos[1] = 0;v.pos[2] = 0;
					v.weight[0] = i / 5.0f;v.weight[1] = 1.0f - i / 5.0f;v.weight[2] = 0;v.weight[3] = 0;
					v.norm[0] = 0;v.norm[1] = -r2;v.norm[2] = -r2;
					v.u = v.v = 0.0f;
				}
				{
					sVertex &v = mmesh.VertexBuffer->Buf[i * 4 + 1];
					v.pos[0] = 10 * i;v.pos[1] = 10;v.pos[2] = 0;
					v.weight[0] = i / 5.0f;v.weight[1] = 1.0f - i / 5.0f;v.weight[2] = 0;v.weight[3] = 0;
					v.norm[0] = 0;v.norm[1] = r2;v.norm[2] = -r2;
					v.u = v.v = 0.0f;
				}
				{
					sVertex &v = mmesh.VertexBuffer->Buf[i * 4 + 2];
					v.pos[0] = 10 * i;v.pos[1] = 10;v.pos[2] = 10;
					v.weight[0] = i / 5.0f;v.weight[1] = 1.0f - i / 5.0f;v.weight[2] = 0;v.weight[3] = 0;
					v.norm[0] = 0;v.norm[1] = r2;v.norm[2] = r2;
					v.u = v.v = 0.0f;
				}
				{
					sVertex &v = mmesh.VertexBuffer->Buf[i * 4 + 3];
					v.pos[0] = 10 * i;v.pos[1] = 0;v.pos[2] = 10;
					v.weight[0] = i / 5.0f;v.weight[1] = 1.0f - i / 5.0f;v.weight[2] = 0;v.weight[3] = 0;
					v.norm[0] = 0;v.norm[1] = -r2;v.norm[2] = r2;
					v.u = v.v = 0.0f;
				}
			}
			//
			for(int i_ = 0;i_ < 5;++i_)
			{
				{
					u16 *i = mmesh.IndexBuffer->Buf + i_ * 24;
					i[0] = i_ * 4 + 0;
					i[1] = i_ * 4 + 1;
					i[2] = i_ * 4 + 5;
					i[3] = i_ * 4 + 0;
					i[4] = i_ * 4 + 5;
					i[5] = i_ * 4 + 4;
					i[6] = i_ * 4 + 1;
					i[7] = i_ * 4 + 2;
					i[8] = i_ * 4 + 6;
					i[9] = i_ * 4 + 1;
					i[10] = i_ * 4 + 6;
					i[11] = i_ * 4 + 5;
					i[12] = i_ * 4 + 2;
					i[13] = i_ * 4 + 3;
					i[14] = i_ * 4 + 7;
					i[15] = i_ * 4 + 2;
					i[16] = i_ * 4 + 7;
					i[17] = i_ * 4 + 6;
					i[18] = i_ * 4 + 3;
					i[19] = i_ * 4 + 0;
					i[20] = i_ * 4 + 4;
					i[21] = i_ * 4 + 3;
					i[22] = i_ * 4 + 4;
					i[23] = i_ * 4 + 7;
				}
			}
			mmesh.IndexBuffer->Buf[120] = 0;
			mmesh.IndexBuffer->Buf[121] = 1;
			mmesh.IndexBuffer->Buf[122] = 2;
			mmesh.IndexBuffer->Buf[123] = 0;
			mmesh.IndexBuffer->Buf[124] = 2;
			mmesh.IndexBuffer->Buf[125] = 3;

			mmesh.IndexBuffer->Buf[126] = 20;
			mmesh.IndexBuffer->Buf[127] = 21;
			mmesh.IndexBuffer->Buf[128] = 22;
			mmesh.IndexBuffer->Buf[129] = 20;
			mmesh.IndexBuffer->Buf[130] = 22;
			mmesh.IndexBuffer->Buf[131] = 23;
		}
	}
	{
		Frames = new tLinerBuffer<sFrame>(1);
		sFrame &frame = Frames->Buf[0];
		frame.MeshList.push_back(1);
	}
	IsAvailable = true;
	Visible = true;
}

cModel::~cModel()
{
	delete Materials;
	delete Bones;
	delete Meshes;
	delete Frames;
}

int DXPLoadModel_MMD(const char *filename)
{
	
	return -1;
}

#endif
