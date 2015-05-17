#ifndef ULMBLAS_IMPL_LEVEL3_UKERNEL_UGEMM_TCC
#define ULMBLAS_IMPL_LEVEL3_UKERNEL_UGEMM_TCC 1

#include <ulmblas/impl/auxiliary/printmatrix.h>
#include <ulmblas/impl/config/simd.h>
#include <ulmblas/impl/level1extensions/geaxpy.h>
#include <ulmblas/impl/level1extensions/gescal.h>
#include <ulmblas/impl/level3/ukernel/ugemm.h>
#include <algorithm>

namespace ulmBLAS {

//
//  Buffered variant.  Used for zero padded panels.
//
template <typename IndexType, typename T, typename Beta, typename TC>
void
ugemm(IndexType    mr,
      IndexType    nr,
      IndexType    kc,
      const T      &alpha,
      const T      *A,
      const T      *B,
      const Beta   &beta,
      TC           *C,
      IndexType    incRowC,
      IndexType    incColC,
      const T      *nextA,
      const T      *nextB)
{
    const IndexType MR = BlockSizeUGemm<T>::MR;
    const IndexType NR = BlockSizeUGemm<T>::NR;

    T   C_[MR*NR];

    // make sure there is no NaN in buffer
    std::fill_n(C_, MR*NR, T(0));

    ugemm(kc, alpha, A, B, T(0), C_, IndexType(1), MR, nextA, nextB);
    gescal(mr, nr, beta, C, incRowC, incColC);
    geaxpy(mr, nr, T(1), C_, IndexType(1), MR, C, incRowC, incColC);
}

//
//  Buffered variant.  Used if the result alpha*A*B needs to be upcasted for
//  computing C <- beta*C + (alpha*A*B)
//
template <typename IndexType, typename T, typename Beta, typename TC>
void
ugemm(IndexType   kc,
      const T     &alpha,
      const T     *A,
      const T     *B,
      const Beta  &beta,
      TC          *C,
      IndexType   incRowC,
      IndexType   incColC,
      const T     *nextA,
      const T     *nextB)
{
    const IndexType MR = BlockSizeUGemm<T>::MR;
    const IndexType NR = BlockSizeUGemm<T>::NR;

    ugemm(MR, NR, kc, alpha, A, B, beta, C, incRowC, incColC, nextA, nextB);
}

//
//  Unbuffered variant.
//
template <typename IndexType, typename T>
void
ugemm(IndexType   kc,
      const T     &alpha,
      const T     *A,
      const T     *B,
      const T     &beta,
      T           *C,
      IndexType   incRowC,
      IndexType   incColC,
      const T     *nextA,
      const T     *nextB)
{
    SELECT_UGEMM_KERNEL::ugemm(kc, alpha, A, B, beta, C, incRowC, incColC,
                               nextA, nextB);
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL3_UKERNEL_UGEMM_TCC
