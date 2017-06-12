
#ifndef ___mCurves___
#define ___mCurves___



namespace mray{
namespace math{

template<class T>
T BSpline(float t,const T& a1,const T& a2,const T& a3,const T& a4){
	float u=t;
	float u_2=u*u;
	float u_3=u_2*u;

	static float i6=1.0f/6.0f;
	//www.blackpawn.com/texts/splines
	return	(a1*(-u_3   + u_2*3 - u*3+1)+
			 a2*(u_3*3  - u_2*6      +4)+
			 a3*(-u_3*3 + u_2*3 + 3*u +1)+
			 a4*( u_3 ) )*i6;
}
template<class T>
T BSplineD(float t,const T& a1,const T& a2,const T& a3,const T& a4){
	float u=t;
	float u_2=u*u;

	static float i2=1.0f/6.0f;

	return	(a1*(- u_2   + u*2-1)+
			 a2*(- u_2*3 - u*4  )+
			 a3*(- u_2*3 + u*2+1)+
			 a4*(  u_2          ) )*i2;
}



template<class T>
T catmullRom(float t,const T& a1,const T& a2,const T& a3,const T& a4){
	float u=t;
	float u_2=u*u;
	float u_3=u_2*u;

	static float i2=1.0f/2.0f;

	return	(a1*(-u_3   + u_2*2 - u*1  )+
			 a2*(u_3*3  - u_2*5      +2)+
			 a3*(-u_3*3 + u_2*4 + u*1  )+
			 a4*( u_3   - u_2          ) )*i2;
}

template<class T>
T catmullRomD(float t,const T& a1,const T& a2,const T& a3,const T& a4){
	float u=t;
	float u_2=u*u;
	float u_3=u_2*u;

	static float i2=1.0f/2.0f;

	return	(a1*(-u_3   - u_2   + u*4-1)+
			 a2*(u_3*9  + u_2*3 - u*10 )+
			 a3*(-u_3*9 - u_2*3 + u*8+1)+
			 a4*( u_3*3 + u_2   - u*2  ) )*i2;
}



template<class T>
T cubicBezier(float t,const T& a1,const T& a2,const T& a3,const T& a4){

	float u=t;
	float u_2=u*u;
	float u_3=u_2*u;


	return	(a1*(-u_3   +u_2*3 - u*3+1)+
			 a2*( u_3*3 -u_2*6 + u*3  )+
			 a3*(-u_3*3 +u_2*3        )+
			 a4*( u_3                 ) );
}

template<class T>
T cubicBezierD(float t,const T& a1,const T& a2,const T& a3,const T& a4){

	float u=t;
	float u_2=u*u;

	return	(a1*(-u_2*3 +u*6 - 3)+
			 a2*( u_2*9 -u*12+3 )+
			 a3*(-u_2*9 +u*6    )+
			 a4*( 3             ) );
}


}
}


#endif


