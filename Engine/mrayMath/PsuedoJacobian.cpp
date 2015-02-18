
#include "PsuedoJacobian.h"
#include <math.h>
#include <assert.h>

namespace mray{
namespace math{

PsuedoJacobian::PsuedoJacobian()
{
	indxc=0;
	indxr=0;
	ipiv=0;
}


void PsuedoJacobian::gaussj(float **a, int n, float **b, int m,int *indxc, int *indxr, int *ipiv) 
{
	int i = 0,icol = 0,irow = 0,j = 0,k = 0,l = 0,ll = 0;
	float big,dum,pivinv,temp;

#define SWAP(a,b){temp=a;a=b;b=temp;}

	for (j=1;j<=n;j++) ipiv[j]=0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++) {
			if (ipiv[j] != 1) {
				for (k=1;k<=n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					}
				}
			}
		}

		++(ipiv[icol]);
		if (irow != icol) {
			for (l=1;l<=n;l++) 
				SWAP(a[irow][l],a[icol][l])
				for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
		}
		indxr[i]=irow;
		indxc[i]=icol;
		assert (a[icol][icol] != 0.0);
		pivinv=1.0f/a[icol][icol];
		a[icol][icol]=1.0;
		for (l=1;l<=n;l++) a[icol][l] *= pivinv;
		for (l=1;l<=m;l++) b[icol][l] *= pivinv;
		for (ll=1;ll<=n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n;l>=1;l--) {
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}

}

PsuedoJacobian::~PsuedoJacobian()
{
	delete [] indxc;
	delete [] indxr;
	delete [] ipiv;
	indxc=0;
	indxr=0;
	ipiv=0;
}


void PsuedoJacobian::_calcInverse()
{
	int  i, j;


	if (nrCopy.getRowsCount()<m_rowsCount) {
		if (indxc) {
			delete [] indxc;
		}
		if (indxr) {
			delete [] indxr;
		}
		if (ipiv) {
			delete [] ipiv;
		}

		nrCopy.setRowsColsCount(m_rowsCount+1, m_rowsCount+1);

		indxc = new int[m_rowsCount+1];
		indxr = new int[m_rowsCount+1];
		ipiv = new int[m_rowsCount+1];
	}

	float**mat1=nrCopy.getMat();
	float**mat2=JacJJT.getMat();

	for (i=0 ; i < m_rowsCount ; i++) {
		for (j=0 ; j < m_rowsCount ; j++) {
			mat1[i+1][j+1] = mat2[i][j];
		}
	}

	gaussj(mat1, m_rowsCount, 0, 0, indxc, indxr, ipiv);

	JacInverse.setRowsColsCount(m_rowsCount, m_rowsCount);

	mat2=JacInverse.getMat();
	for (i=0 ; i < m_rowsCount ; i++) {
		for (j=0 ; j < m_rowsCount ; j++) {
			mat2[i][j] = mat1[i+1][j+1];
		}
	}
}
void PsuedoJacobian::calcInverse(IJacobian&jaco)
{
	float sum;
	int i,j,k;
	float** mat2;
	float** mat3;
	float** mat4;
	//calculate the inverse as:
	//J^-1=J^T(J.J^T)^-1
	transJaco.setRowsColsCount(m_colsCount,m_rowsCount);
	mat2=transJaco.getMat();
	for(i=0;i<m_rowsCount;++i){
		for(j=0;j<m_colsCount;++j)
		{
			mat2[j][i]=m_mat[i][j];
		}
	}

	JacJJT.setRowsColsCount(m_rowsCount,m_rowsCount);

	for(i=0;i<m_rowsCount;++i){
		for(j=0;j<m_rowsCount;++j)
		{
			sum=0;
			for (k=0;k<m_colsCount;++k)
			{
				sum+=m_mat[i][k]*mat2[k][j];
			}
			JacJJT(i,j)=sum;
		}
	}

	_calcInverse();

	//test
/*
	for(int i=0;i<JacJJT.getRowsCount();++i){
		for(int j=0;j<JacInverse.getColsCount();++j)
		{
			sum=0;
			for (int k=0;k<JacJJT.getColsCount();++k)
			{
				sum+=JacJJT(i,k)*JacInverse(k,j);
			}
		}
	}*/

	//finally..
	jaco.setRowsColsCount(m_colsCount,m_rowsCount);

	mat3=jaco.getMat();
	mat4=JacInverse.getMat();
	for(i=0;i<m_colsCount;++i){
		for(j=0;j<m_rowsCount;++j)
		{
			sum=0;
			for (k=0;k<m_rowsCount;++k)
			{
				sum+=mat2[i][k]*mat4[k][j];
			}
			mat3[i][j]=sum;
		}
	}
}

}
}

