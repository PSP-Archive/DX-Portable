//注意。UpdateするのはsceGuStart-sceGuFinish間である必要があります。

struct DXPLIGHT
{
	int Type;//ライトタイプ
	int Component;//有効成分
	ScePspFVector3 Position;//場所
	ScePspFVector3 Direction;//方向
	float Attenuation[3];//光度＝距離×減衰率＝距離／（A0＋A1×距離＋A2×距離×距離）//初期値は1,0,0
	u32 AmbientColor;//環境光
	u32 DiffuseColor;//拡散光
	u32 SpecularColor;//反射光
	float SpotlightExponent;//スポットライト末端部減衰率
	float SpotlightAngle;//スポットライト角度
	DXPLIGHT();
	static int Update(int id,const DXPLIGHT *param);//NULLが指定されたらライトOFF。IDは0-3
};
