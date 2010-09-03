#include "../../graphics.h"
#include "light.h"

DXPLIGHT::DXPLIGHT()
{
	Type = GU_POINTLIGHT;
	Component = GU_DIFFUSE_AND_SPECULAR;
	Position.z = Position.y = Position.x = 0.0f;
	Direction.z = Direction.y = Direction.x = 0.0f;
	Attenuation[0] = 1.0f;
	Attenuation[1] = 0.0f;
	Attenuation[2] = 0.0f;
	AmbientColor = 0xffffffff;
	DiffuseColor = 0xffffffff;
	SpecularColor = 0xffffffff;
	SpotlightExponent = 0;
	SpotlightAngle = 0.392699081f;//pi/8
}

int DXPLIGHT::Update(int id,const DXPLIGHT *param)
{
	if(id < 0 || id >= 4)return -1;
	if(param == NULL)
	{
		sceGuDisable(GU_LIGHT0 + id);
		return 0;
	}
	switch(param->Type)
	{
	case GU_POINTLIGHT:
		break;
	case GU_DIRECTIONAL:
		sceGuLightSpot(id,&param->Direction,0,0);
		break;
	case GU_SPOTLIGHT:
		sceGuLightSpot(id,&param->Direction,param->SpotlightExponent,param->SpotlightAngle);
		break;
	default:
		return -1;
	}
	sceGuLight(id,param->Type,param->Component,&param->Position);
	sceGuLightAtt(id,param->Attenuation[0],param->Attenuation[1],param->Attenuation[2]);
	sceGuLightColor(id,GU_AMBIENT,param->AmbientColor);
	sceGuLightColor(id,GU_DIFFUSE,param->DiffuseColor);
	sceGuLightColor(id,GU_SPECULAR,param->SpecularColor);
	sceGuEnable(GU_LIGHT0 + id);
	return 0;
}

