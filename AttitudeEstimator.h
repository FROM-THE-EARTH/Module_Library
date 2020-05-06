#include "math.h"
#include "float.h"

class Vector
{
	private:
	public:
		float x = 0;
		float y = 0;
		float z = 0;
    Vector();
		Vector(float v1, float v2, float v3){
			x = v1;
			y = v2;
			z = v3;
		}
		float absolute();
};

class Quaternion
{
	private:
	public:
		float x = 0;
		float y = 0;
		float z = 1;
		float w = 0;
    	Quaternion(){};
		Quaternion(float q1, float q2, float q3, float q4){
			x = q1;
			y = q2;
			z = q3;
			w = q4;
		}
		Quaternion multi(Quaternion);
		Quaternion inverse();
};

class AttitudeEstimator
{
	private:
		Quaternion q;
		Vector MagMax = {FLT_MIN,FLT_MIN,FLT_MIN};
		Vector MagMin = {FLT_MAX,FLT_MAX,FLT_MAX};
		float AccAlpha = 0.0001;
		float MagAlpha = 0.0001;
    	float time = 0;
	public:
		void GetQuaternion(float* x,float* y,float* z,float* w){*x = q.x; *y = q.y; *z = q.z; *w = q.w;}
		void SetAccAlpha(float alpha){ AccAlpha = alpha; }
		void SetMagAlpha(float alpha){ MagAlpha = alpha; }
		void GetMagCenter(float *x,float *y, float *z){*x = (MagMax.x + MagMin.x) / 2; *y = (MagMax.y + MagMin.y) / 2; *z = (MagMax.z + MagMin.z) / 2;}

		void Update(float GyrX,float GyrY,float GyrZ, float dt);

		void Update(float GyrX,float GyrY,float GyrZ,
					float AccX,float AccY,float AccZ, float dt);

		void Update(float GyrX,float GyrY,float GyrZ,
					float AccX,float AccY,float AccZ,
					float MagX,float MagY,float MagZ, float dt);
		
		void vectorRotateToWorld(float *x,float *y,float *z);
		void vectorRotateToLocal(float *x,float *y,float *z);
};
