#include <pspuser.h>
#include <malloc.h>
#include <new>
#include <vector>
#include <map>
class cModel
{
private:
	cModel(const cModel&){}
	const cModel& operator =(const cModel&){}
public://そのうち外すpublic
	template<typename T>struct tLinerBuffer
	{
		const int Length;
		T *Buf;
		tLinerBuffer(int length):Length(length)
		{
			if(length <= 0)
			{
				*(int*)(&Length) = -1;
				Buf = NULL;return;
			}
			Buf = new T[length];
			if(Buf == NULL)*(int*)(&Length) = -1;
		}
		~tLinerBuffer(){delete[] Buf;}
		T& operator[](int index){return Buf[index];}
	private:
		tLinerBuffer(){}
	};

#define SVERTEX_TYPE (GU_WEIGHTS(8) | GU_WEIGHT_32BITF | GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_INDEX_16BIT)
	struct sVertex
	{
		float weight[8];
		float u,v;
		float norm[3];
		float pos[3];
	};

	struct sMaterial
	{
		float SpecularPower;
		u32 EmissiveColor;
		u32 AmbientColor;
		u32 DiffuseColor;
		u32 SpecularColor;
	};

	struct sBone//二分木で管理する予定だったけど、とりあえず配列に突っ込んでから考えることにしたｗ
	{
		int ParentBoneId;//最上位は-1
		int ChildBoneId;//いなければ-1
		int NextBrotherId;//いなければ-1
		VECTOR Position;//const指定でもいいかも
		QUATERNION Rotation;
		MATRIX BoneMatrix;
		sBone()
		{
			NextBrotherId = ChildBoneId = ParentBoneId = -1;
		}
	};

	struct sMesh
	{
		bool Visible;
		int MaterialId;
		int FrameId;
		float AmbientScale;
		float DiffuseScale;
		float SpecularScale;
		float EmissiveScale;
		struct sMicroMesh
		{
			int BoneId[8];
			tLinerBuffer<u16> *IndexBuffer;
			tLinerBuffer<sVertex> *VertexBuffer;
			sMicroMesh()
			{
				BoneId[3] = -1;BoneId[2] = -1;BoneId[1] = -1;BoneId[0] = -1;
				BoneId[7] = -1;BoneId[6] = -1;BoneId[5] = -1;BoneId[4] = -1;
				IndexBuffer = NULL;
				VertexBuffer = NULL;
			}
			~sMicroMesh()
			{
				delete IndexBuffer;
				delete VertexBuffer;
			}
		};

		tLinerBuffer<sMicroMesh> *MicroMesh;
		sMesh()
		{
			Visible = true;
			MaterialId = -1;
			FrameId = -1;
			AmbientScale = DiffuseScale = SpecularScale = EmissiveScale = 1.0f;
			MicroMesh = NULL;
		}
		~sMesh()
		{
			delete MicroMesh;
		}
	};


	struct sFrame
	{
		bool Visible;
		std::vector<int> MeshList;
		float AmbientScale;
		float DiffuseScale;
		float SpecularScale;
		float EmissiveScale;
		sFrame()
		{
			Visible = true;
			EmissiveScale = SpecularScale = DiffuseScale = AmbientScale = 1.0f;
		}
	};

	bool IsAvailable;
	bool Visible;

	ScePspFVector3 Position;
	ScePspFVector4 Rotation;

	MATRIX LocalToWorld;
	tLinerBuffer<sMaterial> *Materials;
	tLinerBuffer<sBone> *Bones;
	tLinerBuffer<sMesh> *Meshes;
	tLinerBuffer<sFrame> *Frames;

	void RenewBoneMatrix();
	void RenewBoneMatrixHelper(int id);
	int LoadMMD(const char* filename);
public:
	void Draw();
	cModel(const char* filename);
	cModel(int i);
	~cModel();
};

class cModelControl
{
	std::map<int,cModel*> ModelMap;
};