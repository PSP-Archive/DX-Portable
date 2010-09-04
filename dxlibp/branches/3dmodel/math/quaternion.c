#include "../dxlibp.h"
#include <math.h>
QUATERNION QGetIdent()
{
	QUATERNION result = {0,0,0,1};
	return result;
}

void QMult_ptr(QUATERNION *result,const QUATERNION *q1,const QUATERNION *q2)
{
	result->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
	result->y = q1->w * q2->y + q1->y * q2->w + q1->x * q2->z - q1->z * q2->x;
	result->z = q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x;
	result->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
}

QUATERNION QMult(QUATERNION q1,QUATERNION q2)
{
	QUATERNION result;
	QMult_ptr(&result,&q1,&q2);
	return result;
}

QUATERNION QGetRotX(float angle)
{
	QUATERNION result = {sinf(angle / 2),0,0,cosf(angle / 2)};
	return result;
}

QUATERNION QGetRotY(float angle)
{
	QUATERNION result = {0,sinf(angle / 2),0,cosf(angle / 2)};
	return result;
}

QUATERNION QGetRotZ(float angle)
{
	QUATERNION result = {0,0,sinf(angle / 2),cosf(angle / 2)};
	return result;
}


QUATERNION QNorm(QUATERNION q)
{
	float qn_1 = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	q.x *= qn_1;
	q.y *= qn_1;
	q.z *= qn_1;
	q.w *= qn_1;
	return q;
}