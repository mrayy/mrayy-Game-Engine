
/********************************************************************
	created:	2010/03/24
	created:	24:3:2010   13:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\GenericMatrix.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	GenericMatrix
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	set of methods to deal with basic operations on matrices
*********************************************************************/

#ifndef ___GenericMatrix___
#define ___GenericMatrix___


namespace mray{
namespace math{

class GenericMatrix
{
private:
protected:
	int m_rowsCount;
	int m_colsCount;

	float**m_mat;
	void rebuildMat();
	void deleteMat();
public:
	GenericMatrix();
	GenericMatrix(int r,int c);
	virtual~GenericMatrix();

	float** getMat();
	const float** getMat()const;

	void setRowsCount(int c);
	void setColsCount(int c);
	void setRowsColsCount(int r,int c);

	int getRowsCount()const;
	int getColsCount()const;

	float& operator()(int r,int c);
	float operator()(int r,int c)const;
	GenericMatrix& operator=(const GenericMatrix&o);

	//C=AxB,where A(NxK),B(KxM),C(NxM)
	static bool MultiplyMatricies(const float**A,const float**B,float**C,int N,int K,int M);
	static bool MultiplyMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C);

	//C=A+B,where A(NxM),B(NxM),C(NxM)
	static bool AddMatricies(const float**A,const float**B,float**C,int N,int M);
	static bool AddMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C);

	//C=A-B,where A(NxM),B(NxM),C(NxM)
	static bool SubtractMatricies(const float**A,const float**B,float**C,int N,int M);
	static bool SubtractMatricies(const GenericMatrix&A,const GenericMatrix&B,GenericMatrix&C);

	//C=vxA,where A(NxM),C(NxM)
	static bool MultWithScalar(const float**A,float v,float**C,int N,int M);
	static bool MultWithScalar(const GenericMatrix&A,float v,GenericMatrix&C);

	//C=Inverse(A),where A(NxN),C(NxN)
	//return value:determinant of A
	static float CalcInverse(const float**A,float**C,int N);
	static float CalcInverse(const GenericMatrix&A,GenericMatrix&C);

	//C=Transpose(A),where A(NxM),C(MxN)
	static bool CalcTranspose(const float**A,float**C,int N,int M);
	static bool CalcTranspose(const GenericMatrix&A,GenericMatrix&C);

	//calculate A determinate value
	static float CalcDeterminant(const float**A,int N);
	static float CalcDeterminant(const GenericMatrix&A);

	static bool MakeIdentity(float**A,int N);
	static bool MakeIdentity(GenericMatrix&A);

};

}
}


#endif //___GenericMatrix___
