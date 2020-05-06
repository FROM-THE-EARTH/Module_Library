#include "math.h"
#include "float.h"
#include "AttitudeEstimator.h"

const float rad_per_deg = M_PI / 180.0;
const Vector gravity(0,0,1);
const Vector north(cosf(52 * rad_per_deg),0,-sinf(52 * rad_per_deg));

static float innerProduct(Vector v1,Vector v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static Vector outerProduct(Vector v1,Vector v2){
	return (Vector){
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x
	};
}

static Quaternion generateCorrecor(Vector world_v,Vector goal_v,float alpha){
		Vector outerP = outerProduct(world_v, goal_v);
		float absolute_outerP = outerP.absolute();
		Vector e = {
			outerP.x / absolute_outerP,
			outerP.y / absolute_outerP,
			outerP.z / absolute_outerP };
		float theata = acosf(innerProduct(world_v, goal_v) / (world_v.absolute() * goal_v.absolute())) * alpha * 0.5f;
		float Sin = sinf(theata);
		return (Quaternion){
			e.x * Sin,
			e.y * Sin,
			e.z * Sin,
			cosf(theata)
		};
}

static Vector toWorld(Vector v,Quaternion q){
	return (Vector){
	 	( q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)*v.x + 2*(q.x*q.y+q.z*q.w)*v.y + 2*(q.x*q.z-q.y*q.w)*v.z,
	 	2*(q.x*q.y-q.z*q.w)*v.x + (-q.x*q.x+q.y*q.y-q.z*q.z+q.w*q.w)*v.y + 2*(q.y*q.z+q.x*q.w)*v.z,
	 	2*(q.x*q.z+q.y*q.w)*v.x + 2*(q.y*q.z-q.x*q.w)*v.y + (-q.x*q.x-q.y*q.y+q.z*q.z+q.w*q.w)*v.z
	};
	// q * v * q^-1
}

static Vector toLocal(Vector v,Quaternion q){
	return (Vector){
		( q.x*q.x-q.y*q.y-q.z*q.z+q.w*q.w)*v.x + 2*(q.x*q.y-q.z*q.w)*v.y + 2*(q.x*q.z+q.y*q.w)*v.z,
		2*(q.x*q.y+q.z*q.w)*v.x + (-q.x*q.x+q.y*q.y-q.z*q.z+q.w*q.w)*v.y + 2*(q.y*q.z-q.x*q.w)*v.z,
		2*(q.x*q.z-q.y*q.w)*v.x + 2*(q.y*q.z+q.x*q.w)*v.y + (-q.x*q.x-q.y*q.y+q.z*q.z+q.w*q.w)*v.z
	};
	Quaternion t = q.multi((q.inverse()).multi({v.x,v.y,v.z,0}));
	return {t.x,t.y,t.z};
}

float Vector::absolute(){
	return sqrtf(x*x + y*y + z*z);
}

Quaternion Quaternion::multi(Quaternion p){
	return (Quaternion){
		p.x * w + p.w * x - p.z * y + p.y * z,
		p.y * w + p.z * x + p.w * y - p.x * z,
		p.z * w - p.y * x + p.x * y + p.w * z,
		p.w * w - p.x * x - p.y * y - p.z * z	};
}

Quaternion Quaternion::inverse(){
	return (Quaternion){
		-x,
		-y,
		-z,
		 w	};
}

void AttitudeEstimator::Update(float GyrX,float GyrY,float GyrZ,float dt){
	//Gyr is AngularVelocity in (deg/s)
	//dt is delta time in seconds
	float sx = sinf(GyrX * rad_per_deg * 0.5f * dt)
	 	, sy = sinf(GyrY * rad_per_deg * 0.5f * dt)
	 	, sz = sinf(GyrZ * rad_per_deg * 0.5f * dt)
	 	, cx = cosf(GyrX * rad_per_deg * 0.5f * dt)
	 	, cy = cosf(GyrY * rad_per_deg * 0.5f * dt)
	 	, cz = cosf(GyrZ * rad_per_deg * 0.5f * dt);
	Quaternion dq = (Quaternion){
						sx*cy*cz + cx*sy*sz,
						cx*sy*cz - sx*cy*sz,
						sx*sy*cz + cx*cy*sz,
					   -sx*sy*sz - cx*cy*cz 	};
	//q = q.multi((Quaternion){sx,  0,  0, cx});
	//q = q.multi((Quaternion){ 0, sy,  0, cy});
	//q = q.multi((Quaternion){ 0,  0, sz, cz});
	q = q.multi(dq);
	time += dt;
}

void AttitudeEstimator::Update(float GyrX,float GyrY,float GyrZ,float AccX,float AccY,float AccZ,float dt){
	Update(GyrX, GyrY, GyrZ, dt);
	Vector acc_world = toWorld((Vector){AccX,AccY,AccZ}, q);
	Quaternion c = generateCorrecor(acc_world, gravity, AccAlpha);
	q = q.multi(c);
}

void AttitudeEstimator::Update(float GyrX,float GyrY,float GyrZ,float AccX,float AccY,float AccZ,float MagX,float MagY,float MagZ,float dt){
	Update(GyrX, GyrY, GyrZ, AccX, AccY, AccZ, dt);

	if(MagX > MagMax.x)MagMax.x = MagX;
	if(MagX < MagMin.x)MagMin.x = MagX;
	if(MagY > MagMax.y)MagMax.y = MagY;
	if(MagY < MagMin.y)MagMin.y = MagY;
	if(MagZ > MagMax.z)MagMax.z = MagZ;
	if(MagZ < MagMin.z)MagMin.z = MagZ;

	Vector mag = (Vector){
			MagX - (MagMax.x + MagMin.x) / 2,
			MagY - (MagMax.y + MagMin.y) / 2,
			MagZ - (MagMax.z + MagMin.z) / 2};
	mag = toWorld(mag, q);
	Quaternion c = generateCorrecor(mag, north, MagAlpha);
	if(time > 60)q = q.multi(c);
}

void AttitudeEstimator::vectorRotateToWorld(float *x,float *y,float *z){
	Vector rotated = toWorld((Vector){*x,*y,*z},q);
	*x = rotated.x;
	*y = rotated.y;
	*z = rotated.z;
}

void AttitudeEstimator::vectorRotateToLocal(float *x,float *y,float *z){
	Vector rotated = toLocal((Vector){*x,*y,*z},q);
	*x = rotated.x;
	*y = rotated.y;
	*z = rotated.z;
}
