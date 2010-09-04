extern "C"
{
#include "../../graphics.h"
#include "pmdReader.h" //どんどん変なものが追加されてます
}

#include "light.h"
#include "model.h"
#include <algorithm>
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
				for(int b = 0;b < 8;++b)sceGuBoneMatrix(b,mmesh.BoneId[b] == -1 ? &ident.pspm : &Bones->Buf[mmesh.BoneId[b]].BoneMatrix.pspm);
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

struct INT8
{
	int x,y,z,w,a,b,c,d;
	INT8()
	{
		w = z = y = x = -1;
		d = c = b = a = -1;
	}
	bool operator ==(const INT8& r){if(x != r.x ||y != r.y ||z != r.z ||w != r.w || a != r.a || b != r.b ||c != r.c||d != r.d)return false;return true;}
	void Sort()
	{
		while(x < y || y < z || z < w || w < a || a < b || b < c || c < d)
		{
			if(x < y){int t = x;x = y;y = t;}
			if(y < z){int t = y;y = z;z = t;}
			if(z < w){int t = z;z = w;w = t;}
			if(w < a){int t = w;w = a;a = t;}
			if(a < b){int t = a;a = b;b = t;}
			if(b < c){int t = b;b = c;c = t;}
			if(c < d){int t = c;c = d;d = t;}
		}
	}
};

int PmdFaceToINT8(INT8 &faceref,const sPmdFile &pmd, const sPmdFace &pface)
{
	faceref.w = faceref.z = faceref.y = faceref.x = -1;
	faceref.d = faceref.c = faceref.b = faceref.a = -1;
	for(int i = 0;i < 3;++i)
	{
		for(int j = 0;j < 2;++j)
		{
			int a = pmd.vertex[pface.vertexIndex[i]].bone_num[j];
			if(faceref.x != a && faceref.y != a && faceref.z != a && faceref.w != a && faceref.a != a && faceref.b != a && faceref.c != a && faceref.d != a )
			{
				if(faceref.x == -1)faceref.x = a;
				else if(faceref.y == -1)faceref.y = a;
				else if(faceref.z == -1)faceref.z = a;
				else if(faceref.w == -1)faceref.w = a;
				else if(faceref.a == -1)faceref.a = a;
				else if(faceref.b == -1)faceref.b = a;
				else if(faceref.c == -1)faceref.c = a;
				else if(faceref.d == -1)faceref.d = a;
				else return -1;
			}
		}
	}
	faceref.Sort();
	return 0;
}

int cModel::LoadMMD(const char* filename)
{
	sPmdFile *pmd = PmdPerse(filename);	if(pmd == NULL)return -1;
	//必要フレーム数見積もり
	//とりあえずボディーだけいきます。
	Frames = new tLinerBuffer<sFrame>(1);
	//要チェック
	sFrame &frame = Frames->Buf[0];
	for(int i = 0;i < (int)pmd->materialNum;++i)frame.MeshList.push_back(i);

	Materials = new tLinerBuffer<sMaterial>(pmd->materialNum);
	Meshes = new tLinerBuffer<sMesh>(pmd->materialNum);
	//ptr check!
	int faceId = 0;
	for(int i = 0;i < (int)pmd->materialNum;++i)
	{
		sMaterial &material = Materials->Buf[i];
		sMesh &mesh = Meshes->Buf[i];

		material.AmbientColor = (static_cast<int>(pmd->material[i].alpha * 255) & 0xff) << 24;material.AmbientColor 
			|= ((static_cast<int>(pmd->material[i].mirror_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].mirror_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].mirror_color[0] * 255) & 0xff);
		material.DiffuseColor = ((static_cast<int>(pmd->material[i].diffuse_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].diffuse_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].diffuse_color[0] * 255) & 0xff);
		material.EmissiveColor = 0;
		material.SpecularColor = ((static_cast<int>(pmd->material[i].specular_color[2] * 255) & 0xff) << 16) | ((static_cast<int>(pmd->material[i].specular_color[1] * 255) & 0xff) << 8) | (static_cast<int>(pmd->material[i].specular_color[0] * 255) & 0xff);
		material.SpecularPower = pmd->material[i].specularity;

		mesh.FrameId = 0;
		mesh.MaterialId = i;
		//ここからメッシュデータの再構築をします！
		//まずはメッシュに含まれる参照ボーンを調査。ここはSTL使おうかな
		std::vector<INT8> boneref;
		for(int j = faceId;j < faceId + (int)pmd->material[i].face_vert_count / 3;++j)
		{
			INT8 faceref;
			if(PmdFaceToINT8(faceref,*pmd,pmd->face[j]) != 0)
			{
				//PmdDestruct(pmd);
				IsAvailable = false;
				printfDx("ああああああ%d,%d,%d",i,j,faceId);
				//return -1;
			}
			bool exist = false;
			for(std::vector<INT8>::iterator it = boneref.begin(),eit = boneref.end();it != eit;++it)
			{
				if((*it) == faceref){exist = true;break;}
			}
			if(!exist)
			{
				boneref.push_back(faceref);
			}
		}
		mesh.MicroMesh = new tLinerBuffer<sMesh::sMicroMesh>(boneref.size());
		if(mesh.MicroMesh == NULL)
		{
			PmdDestruct(pmd);
			printfDx("l182::i=%d",i);
			return -1;
		}
		//ptr check
		for(int j = 0,jend = boneref.size();j < jend;++j)
		{
			sMesh::sMicroMesh &mmesh = mesh.MicroMesh->Buf[j];
			mmesh.BoneId[0] = boneref[j].x;
			mmesh.BoneId[1] = boneref[j].y;
			mmesh.BoneId[2] = boneref[j].z;
			mmesh.BoneId[3] = boneref[j].w;
			mmesh.BoneId[4] = boneref[j].a;
			mmesh.BoneId[5] = boneref[j].b;
			mmesh.BoneId[6] = boneref[j].c;
			mmesh.BoneId[7] = boneref[j].d;
			std::vector<u16> rawindexbuf;
			for(int k = faceId;k < faceId + pmd->material[i].face_vert_count / 3;++k)
			{
				INT8 faceref;
				PmdFaceToINT8(faceref,*pmd,pmd->face[k]);
				if(boneref[j] == faceref)
				{
					rawindexbuf.push_back(pmd->face[k].vertexIndex[0]);
					rawindexbuf.push_back(pmd->face[k].vertexIndex[1]);
					rawindexbuf.push_back(pmd->face[k].vertexIndex[2]);
				}
			}
			std::vector<u16> sortedindex = rawindexbuf;
			std::sort(sortedindex.begin(),sortedindex.end());
			sortedindex.erase(std::unique(sortedindex.begin(),sortedindex.end()),sortedindex.end());

			mmesh.IndexBuffer = new tLinerBuffer<u16>(rawindexbuf.size());
			mmesh.VertexBuffer = new tLinerBuffer<sVertex>(sortedindex.size());
			//とりま、頂点リストからいこうか
			for(int k = 0,kend = sortedindex.size();k < kend;++k)
			{
				sVertex &v = mmesh.VertexBuffer->Buf[k];
				sPmdVertex &pv = pmd->vertex[(int)sortedindex[k]];
				v.u = pv.uv[0];v.v = pv.uv[1];
				v.pos[0] = pv.pos[0];v.pos[1] = pv.pos[1];v.pos[2] = pv.pos[2];
				v.norm[0] = pv.normal_vec[0];v.norm[1] = pv.normal_vec[1];v.norm[2] = pv.normal_vec[2];
				for(int l = 0;l < 8;++l)
				{
					v.weight[l] = 0.0f;
					if(mmesh.BoneId[l] == pv.bone_num[0])
						v.weight[l] = pv.bone_weight / 100.0f;
					if(mmesh.BoneId[l] == pv.bone_num[1])
						v.weight[l] = 1.0f - pv.bone_weight / 100.0f;
				}
			}
			for(int k = 0,kend = rawindexbuf.size();k < kend;++k)
			{
				int l,lend = sortedindex.size();
				for(l = 0;l < lend;++l)
					if(sortedindex[l] == rawindexbuf[k])break;
				mmesh.IndexBuffer->Buf[k] = l;
			}
		}
		faceId += (int)pmd->material[i].face_vert_count / 3;
	}
	printfDx("%p\n",Meshes->Buf[0].MicroMesh);
	//ボーンの読み込みをここでやる。
	Bones = new tLinerBuffer<sBone>(pmd->boneNum);
	//ptr check
	
	for(int i = 0;i < (int)pmd->boneNum;++i)
	{
		sBone &bone = Bones->Buf[i];
		sPmdBone &pbone = pmd->bone[i];
		bone.Rotation = QGetIdent();
		bone.Position.x = pbone.bone_head_pos[0];
		bone.Position.y = pbone.bone_head_pos[1];
		bone.Position.z = pbone.bone_head_pos[2];
		if(pbone.parent_bone_index != 0xffff)
			bone.ParentBoneId = pbone.parent_bone_index;
		if(pbone.tail_pos_bone_index != 0xffff)
			bone.ChildBoneId = pbone.tail_pos_bone_index;
		//NextBrotherIdは次のループで作る。
	}
	for(int i = 0;i < pmd->boneNum;++i)
	{
		std::vector<int> brothers;
		for(int j = 0;j < pmd->boneNum;++j)
		{
			if(Bones->Buf[j].ParentBoneId == i)brothers.push_back(j);
		}
		if(brothers.size() <= 0)continue;
		for(int j = 0;j < (int)brothers.size() - 1;++j)
			Bones->Buf[brothers[j]].NextBrotherId = brothers[j + 1];
		Bones->Buf[i].ChildBoneId = brothers[0];
	}

	PmdDestruct(pmd);
	IsAvailable = true;
	printfDx("mmd loaded!\n");
	return 0;
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
