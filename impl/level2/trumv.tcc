/*
 * Copyright (C) 2014, The University of Texas at Austin
 * Copyright (C) 2014-2015, Michael Lehn
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of The University of Texas at Austin nor the names
 *    of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ULMBLAS_IMPL_LEVEL2_TRUMV_TCC
#define ULMBLAS_IMPL_LEVEL2_TRUMV_TCC 1

#include <ulmblas/impl/auxiliary/conjugate.h>
#include <ulmblas/impl/level1extensions/axpyf.h>
#include <ulmblas/impl/level1extensions/dotxf.h>
#include <ulmblas/impl/level2/gemv.h>
#include <ulmblas/impl/level2/trumv.h>

namespace ulmBLAS {

template <typename IndexType, typename TA, typename TX>
void
trumv_unblk(IndexType    n,
            bool         unitDiag,
            bool         conjA,
            const TA     *A,
            IndexType    incRowA,
            IndexType    incColA,
            TX           *x,
            IndexType    incX)
{
    for (IndexType i=0; i<n; ++i) {
        x[i*incX] = (!unitDiag)
                  ? conjugate(A[i*incRowA+i*incColA], conjA)*x[i*incX]
                  : x[i*incX];
        for (IndexType j=i+1; j<n; ++j) {
            x[i*incX] += conjugate(A[i*incRowA+j*incColA], conjA)*x[j*incX];
        }
    }
}

template <typename IndexType, typename TA, typename TX>
void
trumv(IndexType    n,
      bool         unitDiag,
      bool         conjA,
      const TA     *A,
      IndexType    incRowA,
      IndexType    incColA,
      TX           *x,
      IndexType    incX)
{
    typedef decltype(TA(0)*TX(0))  T;

    const IndexType    UnitStride(1);

    if (incRowA==UnitStride) {
        const IndexType bf = FuseFactor<T>::axpyf;
        const IndexType nb = (n/bf)*bf;
        const IndexType nl = n % bf;

        for (IndexType j=0; j<nb; j+=bf) {
            gemv(j, bf,
                 T(1), conjA,
                 &A[0*UnitStride+j*incColA], UnitStride, incColA,
                 &x[j*incX], incX,
                 T(1),
                 &x[0*incX], incX);

            trumv_unblk(bf, unitDiag, conjA,
                        &A[j*UnitStride+j*incColA], UnitStride, incColA,
                        &x[j*incX], incX);
        }

        if (nl) {
            gemv(n-nl, nl,
                 T(1), conjA,
                 &A[0*UnitStride+(n-nl)*incColA], UnitStride, incColA,
                 &x[(n-nl)*incX], incX,
                 T(1),
                 &x[0*incX], incX);

            trumv_unblk(nl, unitDiag, conjA,
                      &A[(n-nl)*UnitStride+(n-nl)*incColA], UnitStride, incColA,
                      &x[(n-nl)*incX], incX);
        }

    } else if (incColA==UnitStride) {
        const IndexType bf = FuseFactor<T>::dotuxf;
        const IndexType nb = (n/bf)*bf;
        const IndexType nl = n % bf;

        for (IndexType j=0; j<nb; j+=bf) {
            trumv_unblk(bf, unitDiag, conjA,
                        &A[j*incRowA+j*UnitStride], incRowA, UnitStride,
                        &x[j*incX], incX);

            gemv(bf, n-j-bf,
                 T(1), conjA,
                 &A[j*incRowA+(j+bf)*incColA], incRowA, UnitStride,
                 &x[(j+bf)*incX], incX,
                 T(1),
                 &x[j*incX], incX);
        }

        trumv_unblk(nl, unitDiag, conjA,
                    &A[(n-nl)*incRowA+(n-nl)*UnitStride], incRowA, UnitStride,
                    &x[(n-nl)*incX], incX);

    } else {
        trumv_unblk(n, unitDiag, conjA, A, incRowA, incColA, x, incX);
    }
}

template <typename IndexType, typename TA, typename TX>
void
trumv(IndexType    n,
      bool         unitDiag,
      const TA     *A,
      IndexType    incRowA,
      IndexType    incColA,
      TX           *x,
      IndexType    incX)
{
    trumv(n, unitDiag, false, A, incRowA, incColA, x, incX);
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL2_TRUMV_TCC
