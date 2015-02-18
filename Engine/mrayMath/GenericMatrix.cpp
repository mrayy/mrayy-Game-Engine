

#include "GenericMatrix.h"
#include <memory>

namespace mray{
namespace math{

GenericMatrix::GenericMatrix():
	m_rowsCount(0),m_colsCount(0),m_mat(0)
{
	m_mat=0;
}
GenericMatrix::GenericMatrix(int r,int c):
	m_rowsCount(0),m_colsCount(0),m_mat(0)
{
	setRowsColsCount(r,c);
}
GenericMatrix::~GenericMatrix()
{
	deleteMat();
}

void GenericMatrix::rebuildMat()
{
	m_mat=new float*[m_rowsCount];
	for(int i=0;i<m_rowsCount;++i)
	{
		m_mat[i]=new float[m_colsCount];
	}
}
void GenericMatrix::deleteMat()
{
	for(int i=0;i<m_rowsCount;++i)
	{
		delete [] m_mat[i];
	}
	delete [] m_mat;
	m_mat=0;
	m_colsCount=0;
	m_rowsCount=0;
}

float** GenericMatrix::getMat()
{
	return m_mat;
}
const float** GenericMatrix::getMat()const
{
	return (const float**)m_mat;
}

void GenericMatrix::setRowsCount(int c)
{
	if(m_rowsCount==c)
		return;
	deleteMat();
	m_rowsCount=c;
	rebuildMat();
}
void GenericMatrix::setColsCount(int c)
{
	if(m_colsCount==c)
		return;
	deleteMat();
	m_colsCount=c;
	rebuildMat();
}
void GenericMatrix::setRowsColsCount(int r,int c)
{
	if(m_colsCount==c && m_rowsCount==r)
		return;
	deleteMat();
	m_rowsCount=r;
	m_colsCount=c;
	rebuildMat();
}

int GenericMatrix::getRowsCount()const
{
	return m_rowsCount;
}
int GenericMatrix::getColsCount()const
{
	return m_colsCount;
}

float& GenericMatrix::operator()(int r,int c)
{
	return m_mat[r][c];
}

float GenericMatrix::operator()(int r,int c)const
{
	return m_mat[r][c];
}

GenericMatrix& GenericMatrix::operator=(const GenericMatrix&o)
{
	setRowsColsCount(o.getRowsCount(),o.getColsCount());
	const float**m=o.getMat();
	for (int i=0;i<m_rowsCount;++i)
	{
		memcpy(m_mat[i],m[i],sizeof(float)*m_colsCount);
	}
	return *this;
}

//C=AxB,where A(NxK),B(KxM),C(NxM)
bool GenericMatrix::MultiplyMatricies(const float**A,const float**B,float**C,int N,int K,int M)
{
	int i,j,l;
	float sum;
	for(i=0;i<N;++i){
		for(j=0;j<M;++j){
			sum=0;
			for (l=0;l<K;++l)
			{
				sum+=A[i][l]*B[l][j];
			}
			C[i][j]=sum;
		}
	}
	return true;
}
bool GenericMatrix::MultiplyMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C)
{
	if(A.getColsCount()!=B.getRowsCount())
		return false;
	C.setRowsColsCount(A.getRowsCount(),B.getColsCount());
	return MultiplyMatricies(A.getMat(),B.getMat(),C.getMat(),A.getRowsCount(),A.getColsCount(),B.getColsCount());
}

//C=A+B,where A(NxM),B(NxM),C(NxM)
bool GenericMatrix::AddMatricies(const float**A,const float**B,float**C,int N,int M)
{
	int i,j;
	for (i=0;i<N;++i)
	{
		for (j=0;j<M;++j)
		{
			C[i][j]=A[i][j]+B[i][j];
		}
	}
	return true;
}
bool GenericMatrix::AddMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C)
{
	if(A.getRowsCount()!=B.getRowsCount() ||
		A.getColsCount()!=B.getColsCount())
		return false;
	C.setRowsColsCount(A.getRowsCount(),A.getColsCount());
	return AddMatricies(A.getMat(),B.getMat(),C.getMat(),A.getRowsCount(),B.getColsCount());
}

//C=A-B,where A(NxM),B(NxM),C(NxM)
bool GenericMatrix::SubtractMatricies(const float**A,const float**B,float**C,int N,int M)
{
	int i,j;
	for (i=0;i<N;++i)
	{
		for (j=0;j<M;++j)
		{
			C[i][j]=A[i][j]-B[i][j];
		}
	}
	return true;
}
bool GenericMatrix::SubtractMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C)
{
	if(A.getRowsCount()!=B.getRowsCount() ||
		A.getColsCount()!=B.getColsCount())
		return false;
	C.setRowsColsCount(A.getRowsCount(),A.getColsCount());
	return SubtractMatricies(A.getMat(),B.getMat(),C.getMat(),A.getRowsCount(),B.getColsCount());
}

float GenericMatrix::CalcInverse(const float**A,float**C,int N)
{
	return 0;
}
float GenericMatrix::CalcInverse(const GenericMatrix&A,GenericMatrix&C)
{
	if(A.getRowsCount()!=A.getColsCount())
		return 0;
	C.setRowsColsCount(A.getColsCount(),A.getRowsCount());
	return CalcInverse(A.getMat(),C.getMat(),A.getRowsCount());
}

bool GenericMatrix::MultWithScalar(const float**A,float v,float**C,int N,int M)
{
	int i,j;
	for (i=0;i<N;++i)
	{
		for (j=0;j<M;++j)
		{
			C[i][j]=v*A[i][j];
		}
	}
	return true;
}
bool GenericMatrix::MultWithScalar(const GenericMatrix&A,float v,GenericMatrix&C)
{
	C.setRowsColsCount(A.getRowsCount(),A.getColsCount());
	return MultWithScalar(A.getMat(),v,C.getMat(),A.getRowsCount(),A.getColsCount());
}

//C=Transpose(A),where A(NxM),C(NxM)
bool GenericMatrix::CalcTranspose(const float**A,float**C,int N,int M)
{
	int i,j;
	for (i=0;i<N;++i)
	{
		for (j=0;j<M;++j)
		{
			C[i][j]=A[j][i];
		}
	}
	return true;
}
bool GenericMatrix::CalcTranspose(const GenericMatrix&A,GenericMatrix&C)
{
	C.setRowsColsCount(A.getColsCount(),A.getRowsCount());
	return CalcTranspose(A.getMat(),C.getMat(),A.getRowsCount(),A.getColsCount());
}

//calculate A determinate value
float GenericMatrix::CalcDeterminant(const float**A,int N)
{
	return 0;
}
float GenericMatrix::CalcDeterminant(const GenericMatrix&A)
{
	if(A.getRowsCount()!=A.getColsCount())
		return 0;
	return CalcDeterminant(A.getMat(),A.getRowsCount());
}


bool GenericMatrix::MakeIdentity(float**A,int N)
{
	int i,j;
	for (i=0;i<N;++i)
	{
		for (j=0;j<N;++j)
		{
			A[i][j]=(i==j)? 1.0f : 0.0f;
		}
	}
	return true;
}
bool GenericMatrix::MakeIdentity(GenericMatrix&A)
{
	if(A.getRowsCount()!=A.getColsCount())
		return false;

	return MakeIdentity(A.getMat(),A.getRowsCount());
}

}
}