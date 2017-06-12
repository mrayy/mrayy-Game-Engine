 
 
/*
 Project: TELESAR V

 Written by Charith Fernando. 
 Basic function definition for Matrix Algebra

 */

#include "stdafx.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

extern "C"{
#include "clapack/blaswrap.h"
#include "clapack/f2c.h"
#include "clapack/clapack.h"
#include "clapack/fblaswr.h"
}
#define M_PI 3.14159265358979

#ifndef JACK

// Non number check for Float or Double
bool isnan(double var){
    volatile double d = var;
    return d != d;
}

Matrix idmat = 
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};


float clamp(float v, float x, float y)
{
	return min(x, max(v, y));
}

void matmult(Matrix A, Matrix B, Matrix C)
/*
 * Multiply 4x4 matrices:
 *
 *  A = B * C
 *
 * Don't use this if the matrices are homogeneous: use hmatmult instead!
 *
 * A *CAN* point to the same matrix as B or C.
 */
{

    int         i,j,k;
    Matrix      a;

    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            a[i][j] = 0.0;
            for (k=0; k<4; k++) {
                a[i][j] += B[i][k] * C[k][j];
            }
        }
    }

    cpmatrix(A,a);

}



void hmatmult(Matrix A, Matrix B, Matrix C)
/*
 * Homogeneous transformation multiplication:
 *
 * A = B * C
 *
 * This routine is optimized for homogeneous transformations. It does
 * *NOT* work on arbitrary 4x4 matrices.
 *
 * John J. Craig Book convension, written by Charith
 *
 * A *CAN* point to the same matrix as B or C.
 */
{

    register float	*a,*b,*c,*bp,*cp;
    register float	*bmax,*cmax,*cpmax;
    register float	*b32,*c00,*c03;
    Matrix Bt,Ct;

    if (A == B) {
	cpmatrix(Bt,B);
	bmax = &Bt[3][0];
	b = &Bt[0][0];
	b32 = &Bt[3][2];
    } else {
	bmax = &B[3][0];
	b = &B[0][0];
	b32 = &B[3][2];
    }

    if (A == C) {
	cpmatrix(Ct,C);
	c00 = &Ct[0][0];
	c03 = &Ct[0][3];
    } else {
	c00 = &C[0][0];
	c03 = &C[0][3];
    }

    a = (float *) &A[0][0];

    while (b < bmax) {
	c = c00;
	cmax = c03;
	while (c < cmax) {
	    cp = c;
	    cpmax = c + 8;
	    bp = b;
	    *a = (*bp++) * (*cp);
	    do {
		cp += 4;
		*a += *bp++ * (*cp);
	    } while (cp < cpmax);
	    a++;
	    c++;
	}
	b += 4;
	*a++;
    }

	*a++ = 0.0;
	*a++ = 0.0;
	*a++ = 0.0;
	*a++ = 1.0;

	A[0][3] = B[0][3] + B[0][0]*C[0][3] + B[0][1]*C[1][3] + B[0][2]*C[2][3]; 
	A[1][3] = B[1][3] + B[1][0]*C[0][3] + B[1][1]*C[1][3] + B[1][2]*C[2][3]; 
	A[2][3] = B[2][3] + B[2][0]*C[0][3] + B[2][1]*C[1][3] + B[2][2]*C[2][3]; 

}


void htranspose(Matrix N, Matrix M)
/*
 * Homogeneous transpose Matrix:
 * John J. Craig Book convension, written by Charith
*/
{
    register float *n,*m,*nmax,*C;
    
    nmax = &N[3][3];
    n = &N[0][0];
    C = &M[0][0];

    while (n < nmax){
		m = C;
		*n++ = *m;
		m += 4;
		*n++ = *m;
		m += 4;
		*n++ = *m;
		m += 4;
		*n++ = *m;
		C++;
    }

}

void inverthomomatrix(Matrix N, Matrix M)
//*
// * Invert a homogeneous transform.  A homogeneous transform is any
// * combination of rotation and translation, but no scaling or perspective!
// * This does not check where the matrix is homogeneous.
// * 
// * John J. Craig Book convension, written by Charith
// * n = inverse of m
// */
{
    register float *n,*m,*nmax,*C,*m3;
    
    nmax = &N[2][3];
    n = &N[0][0];
    C = &M[0][0];

    while (n < nmax){
		m = C;
		*n++ = *m;
		m += 4;
		*n++ = *m;
		m += 4;
		*n++ = *m;
		*n++;
		C++;
    }

    *n++ = 0.0;
	*n++ = 0.0;
	*n++ = 0.0;
	*n++ = 1.0;

	N[0][3] = -(M[0][0]*M[0][3]+M[1][0]*M[1][3]+M[2][0]*M[2][3]); 
	N[1][3] = -(M[0][1]*M[0][3]+M[1][1]*M[1][3]+M[2][1]*M[2][3]); 
	N[2][3] = -(M[0][2]*M[0][3]+M[1][2]*M[1][3]+M[2][2]*M[2][3]); 

}

// maths functions rad version
void generateRPY(double *mo, double rotX, const double rotY, const double rotZ){
	mo[0] = cos(rotY) * cos(rotZ);
	mo[1] = sin(rotX) * sin(rotY) * cos(rotZ) - cos(rotX) * sin(rotZ);	
	mo[2] = cos(rotX) * sin(rotY) * cos(rotZ) + sin(rotX) * sin(rotZ);	

	mo[3] = cos(rotY) * sin(rotZ);
	mo[4] = sin(rotX) * sin(rotY) * sin(rotZ) + cos(rotX) * cos(rotZ);	
	mo[5] = cos(rotX) * sin(rotY) * sin(rotZ) - sin(rotX) * cos(rotZ);	

	mo[6] = -sin(rotY);
	mo[7] = sin(rotX) * cos(rotY);	
	mo[8] = cos(rotX) * cos(rotY);	

}

// maths functions rad version
void rpy_gen_matrix(Matrix mo, float rotX, float rotY, float rotZ){
	mo[0][0] = cos(rotY) * cos(rotZ);
	mo[0][1] = sin(rotX) * sin(rotY) * cos(rotZ) - cos(rotX) * sin(rotZ);	
	mo[0][2] = cos(rotX) * sin(rotY) * cos(rotZ) + sin(rotX) * sin(rotZ);	
	mo[0][3] = 0;

	mo[1][0] = cos(rotY) * sin(rotZ);
	mo[1][1] = sin(rotX) * sin(rotY) * sin(rotZ) + cos(rotX) * cos(rotZ);	
	mo[1][2] = cos(rotX) * sin(rotY) * sin(rotZ) - sin(rotX) * cos(rotZ);	
	mo[1][3] = 0;

	mo[2][0] = -sin(rotY);
	mo[2][1] = sin(rotX) * cos(rotY);	
	mo[2][2] = cos(rotX) * cos(rotY);	
	mo[2][3] = 0;

	mo[3][0] = 0;
	mo[3][1] = 0;	
	mo[3][2] = 0;	
	mo[3][3] = 1;

}


void generateRx(double *mo, double rotX){
	mo[0] = 1;
	mo[1] = 0;
	mo[2] = 0;

	mo[3] = 0;
	mo[4] = cos(rotX);	
	mo[5] = - sin(rotX);	

	mo[6] = 0;
	mo[7] = sin(rotX);
	mo[8] = cos(rotX);	

}



void MultMatrix3(double *mo, const double *lhs, const double *rhs){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			double e = 0.0;
			for (int k = 0; k < 3; k++)
				e += lhs[3 * i + k] * rhs[3 * k + j];
			mo[3 * i + j] = e;
		}
	}
}




void MultMatrix4(double *mo, const double *lhs, const double *rhs){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			double e = 0.0;
			for (int k = 0; k < 4; k++)
				e += lhs[4 * i + k] * rhs[4 * k + j];
			mo[4 * i + j] = e;
		}
	}
}



// --------------------- Other Functions --------------------------///



void vecmult0(float y[],float x[],Matrix M)
/*
 * 'direction' transformation:
 * y = x * M, with y[3] = 0
 */
{
    register int	i,j;
    float   	Y[3];
    
    for (i=0; i<3; i++) {
        Y[i] = 0;
        for (j=0; j<3; j++) {
            Y[i] += x[j] * M[j][i];
        }
    }
    y[0] = Y[0];
    y[1] = Y[1];
    y[2] = Y[2];
}

void vecmult(float y[],float x[],Matrix M)
/*
 * point transformation:
 * y = x * M, with y[3] = 1
 */
{
    register	int i,j;
    float   	Y[3];
    
    for (i=0; i<3; i++) {
        Y[i] = M[3][i];
        for (j=0; j<3; j++) {
            Y[i] += x[j] * M[j][i];
        }
    }
    y[0] = Y[0];
    y[1] = Y[1];
    y[2] = Y[2];
}


void axisangletomatrix(Matrix m, float axis[], float theta)
/*
 * Construct a matrix which is a rotation around 'axis' of 'theta' radians.
 * This routine is tuned for speed. It takes advantages of special cases
 * like the coordinate axes.
 */
{
    register float	s,v,c;
    register float	*p;
    register float	a01,a02,a12,a0s,a1s,a2s,a01v,a02v,a12v;
    
    c = cos(theta);
    s = sin(theta);
    v = 1.0 - c;
    
    p = (float *) m;
	
    if (axis[0] == 0.0 && axis[1] == 0.0) {
	if (axis[2] < 0) {
	    s = -s;
	}
	/*
	 * z rotation
	 */
	*p++ = c;
	*p++ = s;
	*p++ = 0.0;
	*p++ = 0.0;
	
	*p++ = - s;
	*p++ = c;
	*p++ = 0.0;
	*p++ = 0.0;
	
	*p++ = 0.0;
	*p++ = 0.0;
	*p++ = 1.0;
    } else if (axis[0] == 0.0 && axis[2] == 0.0) {
	if (axis[1] < 0) {
	    s = -s;
	}
	/*
	 * y rotation
	 */
	*p++ = c;
	*p++ = 0.0;
	*p++ = - s;
	*p++ = 0.0;
	
	*p++ = 0.0;
	*p++ = 1.0;
	*p++ = 0.0;
	*p++ = 0.0;
	
	*p++ = s;
	*p++ = 0.0;
	*p++ = c;
    } else if (axis[1] == 0.0 && axis[2] == 0.0) {
	if (axis[0] < 0) {
	    s = -s;
	}
	/*
	 * x rotation
	 */
	*p++ = 1.0;
	*p++ = 0.0;
	*p++ = 0.0;
	*p++ = 0.0;
	
	*p++ = 0.0;
	*p++ = c;
	*p++ = s;
	*p++ = 0.0;
	
	*p++ = 0.0;
	*p++ = -s;
	*p++ = c;
    } else {	
	a01 = axis[0] * axis[1];
	a02 = axis[0] * axis[2];
	a12 = axis[1] * axis[2];
	a0s = axis[0] * s;
	a1s = axis[1] * s;
	a2s = axis[2] * s;
	a01v = a01 * v;
	a02v = a02 * v;
	a12v = a12 * v;
	
	*p++ = axis[0] * axis[0] * v + c;
	*p++ = a01v + a2s;
	*p++ = a02v - a1s;
	*p++ = 0.0;
	
	*p++ = a01v - a2s;
	*p++ = axis[1] * axis[1] * v + c;
	*p++ = a12v + a0s;
	*p++ = 0.0;
	
	*p++ = a02v + a1s;
	*p++ = a12v - a0s;
	*p++ = axis[2] * axis[2] * v + c;
    }
    *p++ = 0.0;
	
    *p++ = 0.0;
    *p++ = 0.0;
    *p++ = 0.0;
    *p++ = 1.0;
}    

#endif

//
// Construct a general rotation matrix given an angle and axis
//
void rotation_axis_to_matrix(float axis[3], float angle, Matrix R)
{
    float cos_a, sin_a;
    float s1, s2, s3;
    
    cos_a = cos(angle);
    sin_a = sin(angle);
    
    // Assume axis is normalized 

#if 0
    // float normal[3];
    // cpvector(normal,axis);
    // unitize(normal);

    // s1 = normal[0];
    // s2 = normal[1];
    // s3 = normal[2];
#else

    s1 = axis[0];
    s2 = axis[1];
    s3 = axis[2];
    
#endif
    float s1s1 = s1*s1;
    float s1s2 = s1*s2;
    float s1s3 = s1*s3; 
    float s2s2 = s2*s2;
    float s2s3 = s2*s3; 
    float s3s3 = s3*s3; 
    float cos_as1s2 = cos_a * s1s2;
    float cos_as1s3 = cos_a * s1s3;
    float cos_as2s3 = cos_a * s2s3;
    float s1sin_a = s1*sin_a;
    float s2sin_a = s2*sin_a;
    float s3sin_a = s3*sin_a;
    float *r = (float *)R;

    *r++ = s1s1 + cos_a * (1 - s1s1);
    *r++ = s1s2 - cos_as1s2 + s3sin_a;
    *r++ = s1s3 - cos_as1s3 - s2sin_a;
    *r++ = 0;

    *r++ = s1s2 - cos_as1s2 - s3sin_a;
    *r++ = s2s2 + cos_a * (1 - s2s2);
    *r++ = s2s3 - cos_as2s3 + s1sin_a;
    *r++ = 0;

    *r++ = s1s3 - cos_as1s3 + s2sin_a;
    *r++ = s2s3 - cos_as2s3 - s1sin_a;
    *r++ = s3s3 + cos_a * (1 - s3s3);   
    *r++ = 0;

    *r++ = 0;
    *r++ = 0;
    *r++ = 0;
    *r = 1;

}

//
// p = Projection of u onto v
// 
void project(float p[3], const float u[3], const float v[3])
{
    float vnorm[3];

    cpvector(vnorm, v);
    unitize(vnorm);
    vecscalarmult(p, vnorm, DOT(u,vnorm));
}


//
// p = Projection of u onto plane whose normal is n
//
void project_plane(float p[3], float u[3], float n[3])
{
    float un[3];

    project(un, u, n);
    vecsub(p, u, un);
}


//
// Return the angle between two vectors u,v about the axis n
//
float angle_between_vectors(float u[3], float v[3], float n[3])
{
#if 0
    float temp[3]; 
    float up[3];
    float vp[3];

    cpvector(up, u);
    cpvector(vp, v);
    unitize(up);
    unitize(vp);

    crossproduct(temp, up, vp);
    float mag = DOT(temp,n);

    // Vectors are parallel at 0 or 180 
    if (mag*mag < 1e-8)
    {
	if (DOT(up,vp) < 0)
	    return M_PI;
	else
	    return 0;
    }

    int sign = (mag > 0) ? 1 : -1;
    float t = DOT(up,vp); 
    if (t > 1.0)
	t = 1.0;
    else if (t < -1.0)
	t = -1.0;
    return sign*acos(t);
#else

    float up[3];
    float vp[3]; 
    float uv[3];

    project_plane(up, u, n);
    project_plane(vp, v, n);
    crossproduct(uv, up, vp); 
    return atan2(DOT(n, uv), DOT(up, vp)); 

#endif
} 


//
// Print 4x4 homogeneous matrix 
//
void print_matrix(Matrix M)
{
    for (int i = 0; i < 4; i++)
    {
	for (int j = 0; j < 4; j++)
	    printf(" %lf ", M[i][j]);
	printf("\n");
    }
}


//
// Print vector
//
void print_vector(float v[3])
{
    printf(" %lf %lf %lf \n", *v, v[1], v[2]);
}


//
// Find a vector n normal to v
//

void find_normal_vector(float v[3], float n[3])
{
    int   num_zero;
    float min, temp;
    int   min_i;

    min_i    = 0;
    min      = fabs(v[0]);
    num_zero = (min < 1e-8);

    temp = fabs(v[1]);
    if (temp < 1e-8)
	num_zero++; 
    if (temp < min)
    {
	min = temp;
	min_i = 1;
    }

    temp = fabs(v[2]);
    if (temp < 1e-8)
	num_zero++; 
    if (temp < min)
    {
	min = temp;
	min_i = 2;
    }

    n[0] = n[1] = n[2] = 0.0;

    switch (num_zero)
    {
    case 3:
	// Vector is zero so there is no soln
	break;
    case 2:
	// Vector has only one nonzero component. Set any of the
	// other to 1.0 and 0 to the others
	n[min_i] = 1.0; 
	break;

	// Vector has at least two nonzero components
    case 1:
    default:
	if (min_i == 0)
	{
	    n[1] = -v[2];
	    n[2] = v[1];
	}
	else if (min_i == 1)
	{
	    n[0] = -v[2];
	    n[2] = v[0];
	}
	else
	{
	    n[0] = -v[1];
	    n[1] = v[0];
	}
	unitize(n);
	break;
    }

}




//
// Multiplies only the rotational components of B*C 
// and stores the result into A
//
void rmatmult(Matrix A, Matrix B, Matrix C)
{
    Matrix Temp1;
    Matrix Temp2;

    register float *a = (float *) A; 
    register float *b;
    register float *c; 

    if (A == B)
    {
	cpmatrix(Temp1, B);
	b = (float *) Temp1;
    }
    else
	b = (float *) B; 
    if (A == C)
    {
	cpmatrix(Temp2, C);
	c = (float *) Temp2;
    }
    else
	c = (float *) C; 

    float c11 = *c++; 
    float c12 = *c++; 
    float c13 = *c++; c++;
    float c21 = *c++; 
    float c22 = *c++; 
    float c23 = *c++; c++;
    float c31 = *c++; 
    float c32 = *c++; 
    float c33 = *c++; 
    float v1, v2, v3;

    v1 = *b++; v2 = *b++; v3 = *b++; b++;    
    *a++ = v1*c11 + v2*c21 + v3*c31;
    *a++ = v1*c12 + v2*c22 + v3*c32;
    *a++ = v1*c13 + v2*c23 + v3*c33;
    *a++ = 0;

    v1 = *b++; v2 = *b++; v3 = *b++; b++;    
    *a++ = v1*c11 + v2*c21 + v3*c31;
    *a++ = v1*c12 + v2*c22 + v3*c32;
    *a++ = v1*c13 + v2*c23 + v3*c33;
    *a++ = 0;

    v1 = *b++; v2 = *b++; v3 = *b++;     
    *a++ = v1*c11 + v2*c21 + v3*c31;
    *a++ = v1*c12 + v2*c22 + v3*c32;
    *a++ = v1*c13 + v2*c23 + v3*c33;
    *a++ = 0;

    *a++ = 0;
    *a++ = 0;
    *a++ = 0;
    *a = 1;
}

void invertrmatrix(Matrix N, Matrix M)
/*
 * Invert a rotation matrix 
 * n = inverse of m
 */
{
    register float *n,*m,*nmax,*C;
    
    nmax = &N[2][3];
    n = &N[0][0];
    C = &M[0][0];
    while (n < nmax) {
	m = C;
	*n++ = *m;
	m += 4;
	*n++ = *m;
	m += 4;
	*n++ = *m;
	*n++ = 0.0;
	C++;
    }
    
    *n++ = 0;
    *n++ = 0;
    *n++ = 0;
    *n++ = 1.0;
}

//
// Generate the derivative of a rotation matrix about a principal axis
//
void rotation_principal_axis_to_deriv_matrix(char axis, float angle, Matrix m)
{
    float cos_a, sin_a;
    
    memset(m, 0, sizeof(Matrix));
    cos_a = cos(angle);
    sin_a = sin(angle);
    
    switch (axis)
    {
    case 'x':
    case 'X':
	m[1][1] = -sin_a; m[2][1] = -cos_a;
	m[1][2] = cos_a; m[2][2] = -sin_a;
	break;
	
    case 'y':
    case 'Y':
	m[0][0] = -sin_a;  m[2][0] = cos_a;
	m[0][2] = -cos_a; m[2][2] = -sin_a;
	break;
	
    default:
	m[0][0] = -sin_a; m[1][0] = -cos_a; 
	m[0][1] = cos_a; m[1][1] = -sin_a;  
	break;
    }
}

void rotation_principal_axis_to_matrix(char axis, float angle, Matrix m)
{
    float cos_a, sin_a;
    
    cpmatrix(m, idmat);
    cos_a = cos(angle);
    sin_a = sin(angle);
    
    switch (axis)
    {
    case 'x':
    case 'X':
	m[1][1] = cos_a; m[2][1] = -sin_a;
	m[1][2] = sin_a; m[2][2] = cos_a;
	break;
	
    case 'y':
    case 'Y':
	m[0][0] = cos_a;  m[2][0] = sin_a;
	m[0][2] = -sin_a; m[2][2] = cos_a;
	break;
	
    default:
	m[0][0] = cos_a; m[1][0] = -sin_a; 
	m[0][1] = sin_a; m[1][1] = cos_a;  
	break;
    }
}

//
// To extract axis and angle from R use the formulas (murray, pg 414)
//
//	2 * cos(theta) - 1 = trace(R) 
// and 
//	axis = vector associated with skew symmetric matrix (R-R')/(2*sin(theta)) 
//
//
// By our convention always return 0 <= angle < M_PI
// 
void rotation_matrix_to_axis(const Matrix R, float axis[], float &angle)
{
    const float eps = 1e-7;

    angle = acos((R[0][0] + R[1][1] + R[2][2] - 1) / 2.0);
    

    // Close to identity. Arbitrarily set solution to z axis rotation of 0 
    if (fabs(angle) < eps || fabs(angle - M_PI) < eps)
    {
	angle = 0.0;
	axis[0] = axis[1] = 0.0; axis[2] = 1.0; 
    }
    else
    {
	axis[0] = R[1][2] - R[2][1]; 
	axis[1] = R[2][0] - R[0][2];
	axis[2] = R[0][1] - R[1][0]; 
	unitize(axis);
    }
}

void linterpmatrix(Matrix R,Matrix A,Matrix B,float t)
/*
 * Linearly interpolate matrices.  The rotation submatrix is interpolated
 * by finding the axis of rotation between the two matrix and interpolating
 * the angle of rotation. The translation part is just interpolated.
 *
 * t=0 gives A; t=1 gives B.
 *
 * R = (1-t) * A  + t * B
 */
{
    Matrix      Ainv,T;
    float       angle,axis[3];
    Quaternion  q;

    inverthomomatrix(Ainv,A);
    matmult(T,B,Ainv);
    matrixtoq(q,T);
    qtoaxis(&angle,axis,q);

    angle *= t;

    axistoq(q,angle,axis);
    qtomatrix(T,q);
    matmult(R,T,A);
    vecinterp(R[3],A[3],B[3],t);
}


#define ATAN2(a,b) ((a==0.0)&&(b==0.0) ? 0.0 : atan2(a,b))

#define EPSILON 0.001
#define W q[0]    
#define X q[1]    
#define Y q[2]    
#define Z q[3]    

void
qtomatrix(Matrix m,const Quaternion& q)
/*
 * Convert quaterion to rotation sub-matrix of 'm'.
 * The left column of 'm' gets zeroed, and m[3][3]=1.0, but the 
 * translation part is left unmodified.
 * 
 * m = q
 */
{

#if 0
	float fTx = 2.0f*X;
	float fTy = 2.0f*Y;
	float fTz = 2.0f*Z;
	float fTwx = fTx*W;
	float fTwy = fTy*W;
	float fTwz = fTz*W;
	float fTxx = fTx*X;
	float fTxy = fTy*X;
	float fTxz = fTz*X;
	float fTyy = fTy*Y;
	float fTyz = fTz*Z;
	float fTzz = fTz*Z;

	m[0][0] = 1.0f - (fTyy + fTzz);
	m[0][1] = fTxy - fTwz;
	m[0][2] = fTxz + fTwy;
	m[0][3] = 0.0;

	m[1][0] = fTxy + fTwz;
	m[1][1] = 1.0f - (fTxx + fTzz);
	m[1][2] = fTyz - fTwx;
	m[1][3] = 0.0;

	m[2][0] = fTxz - fTwy;
	m[2][1] = fTyz + fTwx;
	m[2][2] = 1.0f - (fTxx + fTyy);
	m[2][3] = 0.0;

	m[3][3] = 1.0;
#else
    float    x2 = X * X;
    float    y2 = Y * Y;
    float    z2 = Z * Z;
    
    m[0][0] = 1 - 2 * (y2 +  z2);
    m[0][1] = 2 * (X * Y + W * Z);
    m[0][2] = 2 * (X * Z - W * Y);
    m[0][3] = 0.0;
    
    m[1][0] = 2 * (X * Y - W * Z);
    m[1][1] = 1 - 2 * (x2 + z2);
    m[1][2] = 2 * (Y * Z + W * X);
    m[1][3] = 0.0;
    
    m[2][0] = 2 * (X * Z + W * Y);
    m[2][1] = 2 * (Y * Z - W * X);
    m[2][2] = 1 - 2 * (x2 + y2);
    m[2][3] = 0.0;

    m[3][3] = 1.0;
#endif
}

void
matrixtoq(Quaternion q,Matrix m)
/*
 * Convert the rotation sub-matrix of `m' to a quaternion.
 *
 * q = m
 */
{
    float    f;

    f = (1.0 + m[0][0] + m[1][1] + m[2][2]) / 4.0;
    if (f > EPSILON) {
        W = sqrt(f);
        X = (m[1][2] - m[2][1]) / (4 * W);
        Y = (m[2][0] - m[0][2]) / (4 * W);
        Z = (m[0][1] - m[1][0]) / (4 * W);
    } else {
        W = 0.0;
        f = - (m[1][1] + m[2][2]) / 2.0;
        if (f > EPSILON) {
            X = sqrt(f);
            Y = m[0][1] / (2 * X);
            Z = m[0][2] / (2 * X);
        } else {
            X = 0.0;
            f = (1 - m[2][2]) / 2.0;
            if (f > EPSILON) {
                Y = sqrt(f);
                Z = m[1][2] / (2 * Y);
            } else {
                Y = 0.0;
                Z = 1.0;
            }
        }
    }
    unitize4(q);
}

void axistoq(Quaternion q,float angle,float axis[])
{
    float    f;

    f = (float)sin(angle/2);
    q[0] = (float)cos(angle/2);
    q[1] = axis[0] * f;
    q[2] = axis[1] * f;
    q[3] = axis[2] * f;
}

void eulertoq(float pitch, float yaw, float roll, Quaternion& q)
{

	float num = (roll * 0.5f)*DEGREE_TO_RAD;
	float num2 = (float)sin((double)num);
	float num3 = (float)cos((double)num);
	float num4 = (pitch * 0.5f)*DEGREE_TO_RAD;
	float num5 = (float)sin((double)num4);
	float num6 = (float)cos((double)num4);
	float num7 = (yaw * 0.5f)*DEGREE_TO_RAD;
	float num8 = (float)sin((double)num7);
	float num9 = (float)cos((double)num7);
	q[0] = num9 * num6 * num3 + num8 * num5 * num2;
	q[1] = num9 * num5 * num3 + num8 * num6 * num2;
	q[2] = num8 * num6 * num3 - num9 * num5 * num2;
	q[3] = num9 * num6 * num2 - num8 * num5 * num3;
	normq(q);
}

void qtoeuler(const Quaternion& q, float& x, float& y, float& z)
{

	double sqw = W*W;
	double sqx = X*X;
	double sqy = Y*Y;
	double sqz = Z*Z;

	// rotation about x-axis

	x = (float)(atan2(2.0 * (Y*Z + X*W), (-sqx - sqy + sqz + sqw)));

	// rotation about y-axis
	y = (float)(asin(clamp(2.0 * (X*Z + Y*W), -1, 1)));

	// rotation about z-axis
	z = (float)(atan2(2.0 * (X*Y + Z*W), (sqx - sqy - sqz + sqw)));
	
}

void qtoaxis(float *angle,float axis[],Quaternion q)
{
    float    f;

    *angle = 2 * ((float)acos(q[0]));
    f = (float)sin(*angle/2);
    if (f > 0) {
        axis[0] = q[1] / f;
        axis[1] = q[2] / f;
        axis[2] = q[3] / f;
    } else {
        axis[0] = axis[1] = axis[2] = 0.0;
    }
}

void vecinterp(float x[],float u[],float v[],float t)
/*
 * x = u + t * (v - u).
 */ 
{
    x[0] = u[0] + t * (v[0] - u[0]);
    x[1] = u[1] + t * (v[1] - u[1]);
    x[2] = u[2] + t * (v[2] - u[2]);
}
float unitize4(float u[4])
/*
 * Same as unitize, except that arguments are 4-vectors.
 * Make u into a unit 4-vector.
 */
{
    float    f;

    f = (float)sqrt(DOT4(u,u));
    if (f > 0) {
        f = 1.0 / f;
        u[0] *= f;
        u[1] *= f;
        u[2] *= f;
        u[3] *= f;
    }
    return(f);
}


// length of a vector
//
float norm(float v[3])
{
    return sqrt(DOT(v,v));
}
float normq(Quaternion q)
{
	float len = q[0] * q[0]
		+ q[1] * q[1]
		+ q[2] * q[2]
		+ q[3] * q[3];
	if (len)
	{
		len = sqrt(len);
		q[0] /= len;
		q[1] /= len;
		q[2] /= len;
		q[3] /= len;
	}
	return len;
}

//
// translation component of a matrix
//

void get_translation(const Matrix M, float p[3])
{
    p[0] = M[3][0];
    p[1] = M[3][1];
    p[2] = M[3][2];
}

void set_translation(Matrix  M, const float p[3])
{
    M[3][0] = p[0];
    M[3][1] = p[1];
    M[3][2] = p[2];
}

void get_translation(const Matrix  M, float &x, float &y, float &z)
{
    x = M[3][0];
    y = M[3][1];
    z = M[3][2];
}

void set_translation(Matrix  M, float x, float y, float z)
{
    M[3][0] = x;
    M[3][1] = y;
    M[3][2] = z;
}

void set_row(Matrix  M, int row, const float v[3])
{
    M[row][0] = v[0];
    M[row][1] = v[1];
    M[row][2] = v[2];
}

void get_row(Matrix  M, int row,  float v[3])
{
    v[0] = M[row][0];
    v[1] = M[row][1];
    v[2] = M[row][2];
}
float vecdist(const float t[], const float t2[])
{
    float t3[3];

    vecsub(t3, (float*)t, (float*)t2);
    return sqrt(DOT(t3,t3));
}





// Torso dedicates


double vecLength3(double v[3])
{
	return (sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
}
double vecLength3(double dx, double dy, double dz)
{
	return (sqrt(dx*dx + dy*dy + dz*dz));
}


void MakeMatrix(myAxis axis, double radian, double transX, double transY, double transZ, double matrix[16])
{
	double c = cos(radian);
	double s = sin(radian);
	switch (axis){
	case axisX:///@bug バラバラな順番にメモリアクセスして読み込み効率が悪いかもしれない
		matrix[0] = 1;	matrix[4] = 0;	matrix[8] = 0;	matrix[12] = transX;
		matrix[1] = 0;	matrix[5] = c;	matrix[9] = -s;	matrix[13] = transY;
		matrix[2] = 0;	matrix[6] = s;	matrix[10] = c;	matrix[14] = transZ;
		matrix[3] = 0;	matrix[7] = 0;	matrix[11] = 0;	matrix[15] = 1;
		break;
	case axisY:
		matrix[0] = c;	matrix[4] = 0;	matrix[8] = s;	matrix[12] = transX;
		matrix[1] = 0;	matrix[5] = 1;	matrix[9] = 0;	matrix[13] = transY;
		matrix[2] = -s;	matrix[6] = 0;	matrix[10] = c;	matrix[14] = transZ;
		matrix[3] = 0;	matrix[7] = 0;	matrix[11] = 0;	matrix[15] = 1;
		break;
	case axisZ:
		matrix[0] = c;	matrix[4] = -s;	matrix[8] = 0;	matrix[12] = transX;
		matrix[1] = s;	matrix[5] = c;	matrix[9] = 0;	matrix[13] = transY;
		matrix[2] = 0;	matrix[6] = 0;	matrix[10] = 1;	matrix[14] = transZ;
		matrix[3] = 0;	matrix[7] = 0;	matrix[11] = 0;	matrix[15] = 1;
		break;
	}
}


void InitMatrix(double matrix[16])
{
	matrix[0] = 1;	matrix[4] = 0;	matrix[8] = 0;	matrix[12] = 0;
	matrix[1] = 0;	matrix[5] = 1;	matrix[9] = 0;	matrix[13] = 0;
	matrix[2] = 0;	matrix[6] = 0;	matrix[10] = 1;	matrix[14] = 0;
	matrix[3] = 0;	matrix[7] = 0;	matrix[11] = 0;	matrix[15] = 1;
}

void MultiplyMatrix(double matA[16], double matB[16], double matC[16])
{
	char transA = 'N', transB = 'N';
	long int m = 4, n = 4, k = 4, lda = 4, ldb = 4, ldc = 4;
	double alpha = 1.0, beta = 0.0;
	dgemm_(&transA, &transB, &m, &n, &k, &alpha, matA, &lda, matB, &ldb, &beta, matC, &ldc);
}


void MatrixtoXYZ(double matrix[16], double *a, double *b, double *c)
{

	*c = atan2(matrix[4], matrix[0]);
	*b = -asin(matrix[8]);
	if (cos(*b) != 0){
		*a = asin((matrix[9] / cos(*b)));
		if (matrix[10]<0)*a = M_PI - *a;
		*c = *c;
		*b = *b;
	}
	else{
		*a = 0;
		*c = atan2(matrix[1], matrix[5]);
	}

}
