#ifndef ULMBLAS_LEVEL2_TPLMTV_H
#define ULMBLAS_LEVEL2_TPLMTV_H 1

namespace ulmBLAS {

template <typename IndexType, typename TA, typename TX>
    void
    tplmtv(IndexType    n,
           bool         unitDiag,
           const TA     *A,
           TX           *x,
           IndexType    incX);

} // namespace ulmBLAS

#endif // ULMBLAS_LEVEL2_TPLMTV_H

#include <ulmblas/level2/tplmtv.tcc>
