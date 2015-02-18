

#include "TJacobian.h"

namespace mray{
namespace math{

TJacobian::TJacobian()
{
}
TJacobian::~TJacobian()
{
}
void TJacobian::calcInverse(IJacobian&jaco)
{
	jaco.setRowsColsCount(m_colsCount,m_rowsCount);
	for(int i=0;i<m_rowsCount;++i){
		for(int j=0;j<m_colsCount;++j)
		{
			jaco(j,i)=m_mat[i][j];
		}
	}
}

}
}

