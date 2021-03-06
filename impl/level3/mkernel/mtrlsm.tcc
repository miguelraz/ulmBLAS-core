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

#ifndef ULMBLAS_IMPL_LEVEL3_MKERNEL_MTRLSM_TCC
#define ULMBLAS_IMPL_LEVEL3_MKERNEL_MTRLSM_TCC 1

#include <ulmblas/impl/level3/ukernel/ugemm.h>
#include <ulmblas/impl/level3/mkernel/mtrlsm.h>
#include <ulmblas/impl/level3/ukernel/utrlsm.h>

namespace ulmBLAS {

template <typename IndexType, typename T>
void
mtrlsm(IndexType mc, IndexType nc, const T &alpha, const T *A_, T *B_)
{
    const IndexType MR = BlockSize<T>::MR;
    const IndexType NR = BlockSize<T>::NR;

    const IndexType mp = (mc+MR-1) / MR;
    const IndexType np = (nc+NR-1) / NR;

    const T *nextA;
    const T *nextB;

    #if defined(_OPENMP)
    #pragma omp parallel for
    #endif
    for (IndexType j=0; j<np; ++j) {
        nextB = &B_[j*mc*NR];

        IndexType ia = 0;
        for (IndexType i=0; i<mp; ++i) {
            IndexType kc = i*MR;
            nextA        = &A_[(ia+i+1)*MR*MR];

            if (i==mp-1) {
                nextA = A_;
                nextB = &B_[(j+1)*mc*NR];
                if (j==np-1) {
                    nextB = B_;
                }
            }

            ugemm(kc,
                  T(-1), &A_[ia*MR*MR], &B_[j*mc*NR],
                  alpha,
                  &B_[(j*mc+kc)*NR], NR, IndexType(1),
                  nextA, nextB);

            utrlsm(&A_[(ia*MR+kc)*MR], &B_[(j*mc+kc)*NR]);
            ia += i+1;
        }
    }
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL3_MKERNEL_MTRLSM_TCC
