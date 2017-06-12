 
/*
 Project: TELESAR V

 Written by Charith Fernando. 
 Basic function definition for Matrix Algebra

 */

#ifndef _VECTORALGEBRA
#define _VECTORALGEBRA

#include <math.h>
#include <string.h>

#define DTOR(x)  ((x)*M_PI/180.0)
#define DOT4(u,v)  ((u)[0]*(v)[0]+(u)[1]*(v)[1]+(u)[2]*(v)[2]+(u)[3]*((v)[3]))

#ifdef _WINDOWS
#define M_PI            3.14159265358979323846
#endif

#include "defines.h"
#define RAD_TO_DEGREE 57.29577951471995317358
#define DEGREE_TO_RAD 0.017453292519943277777

int DrawBox(double d, double w, double h);
enum myAxis {
	axisX,
	axisY,
	axisZ
};



void MakeMatrix(myAxis axis, double radian, double transX, double transY, double transZ, double matrix[16]);
double vecLength3(double v[3]);
double vecLength3(double dx, double dy, double dz);
void InitMatrix(double matrix[16]);
void MultiplyMatrix(double matA[16], double matB[16], double matC[16]);
void MatrixtoXYZ(double matrix[16], double *a, double *b, double *c);

typedef float Matrix[4][4];
typedef float Quaternion[4]; //w,x,y,z
typedef double Mat[4][4];

extern Matrix idmat; 

// u = v
#define cpmatrix(u,v) memcpy(u,v,sizeof(Matrix))
#define cpvector(u,v) memcpy(u,v,sizeof(float)*3)

bool isnan(double var); 

float clamp(float v, float x, float y);
void matmult(Matrix A, Matrix B, Matrix C);
void hmatmult(Matrix A,Matrix B,Matrix C);
void inverthomomatrix(Matrix N,Matrix M);
void vecmult(float y[],float x[],Matrix M);
void vecmult0(float y[],float x[],Matrix M);
void qtomatrix(Matrix m,const Quaternion& q);
void matrixtoq(Quaternion q, Matrix m);
void axistoq(Quaternion q, float angle, float axis[]);
void eulertoq(float x, float y, float z, Quaternion& q);
void qtoeuler(const Quaternion& q, float& x, float& y, float& z);
void qtoaxis(float *angle, float axis[], Quaternion q);
void vecinterp(float x[], float u[], float v[], float t);
float unitize4(float u[4]);
void get_translation(const Matrix M, float p[3]);
void set_translation(Matrix M, const float p[3]);
void get_translation(const Matrix M, float &x, float &y, float &z);
void set_translation(Matrix M, float x, float y, float z);
void htranspose(Matrix A, Matrix B); 

void generateRPY(double *mo, double rotX, const double rotY, const double rotZ);
void rpy_gen_matrix(Matrix mo, float rotX, float rotY, float rotZ);
void generateRx(double *mo, double rotX);
void MultMatrix3(double *mo, const double *lhs, const double *rhs);
void MultMatrix4(double *mo, const double *lhs, const double *rhs);



void MakeMatrix(myAxis axis, double radian, double transX, double transY, double transZ, double matrix[16]);
double vecLength3(double v[3]);
double vecLength3(double dx, double dy, double dz);
void InitMatrix(double matrix[16]);
void MultiplyMatrix(double matA[16], double matB[16], double matC[16]);
void MatrixtoXYZ(double matrix[16], double *a, double *b, double *c);





inline void vecscalarmult(float u[],float v[],float f)
{
    u[0] = v[0] * f;
    u[1] = v[1] * f;
    u[2] = v[2] * f;
}

inline float unitize(float u[])
{
    float   f,m;
    
    f = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];

    if (f != 0.0)
    {
	m = (float)sqrt(f);

	u[0] = u[0] / m;
	u[1] = u[1] / m;
	u[2] = u[2] / m;
    }
    else
	m = 0.0; 
	
    return(m);
}


void inline crossproduct(float r[3], const float a[3], const float b[3])
{
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}

inline void vecsub(float t[],float u[],float v[])
{
    t[0] = u[0] - v[0];
    t[1] = u[1] - v[1];
    t[2] = u[2] - v[2];
}

inline void vecadd(float t[],float u[],float v[])
{
    t[0] = u[0] + v[0];
    t[1] = u[1] + v[1];
    t[2] = u[2] + v[2];
}


#define DOT(u,v) (u[0]*v[0] + u[1]*v[1] + u[2]*v[2])


#endif



float norm(float v[3]);
float normq(Quaternion q);
void project(float p[3], const float u[3], const float v[3]);
void project_plane(float p[3], float u[3], float n[3]);
float angle_between_vectors(float u[3], float v[3], float n[3]);
void rotation_axis_to_matrix(float axis[3], float angle, Matrix R);
void rotation_principal_axis_to_matrix(char axis, float angle, Matrix R);
void rotation_principal_axis_to_deriv_matrix(char axis, float angle, Matrix m);
void rotation_matrix_to_axis(const Matrix R, float axis[], float &angle);
void find_normal_vector(float v[3], float n[3]); 
void print_matrix(Matrix M);
void print_vector(float v[3]);
void rmatmult(Matrix A,Matrix B,Matrix C);
void invertrmatrix(Matrix N,Matrix M);
void axisangletomatrix(Matrix m, float axis[], float theta); 