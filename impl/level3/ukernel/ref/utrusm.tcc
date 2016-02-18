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

#ifndef ULMBLAS_IMPL_LEVEL3_UKERNEL_REF_UTRUSM_TCC
#define ULMBLAS_IMPL_LEVEL3_UKERNEL_REF_UTRUSM_TCC 1

#include <iostream>
#include <ulmblas/impl/level3/ukernel/ugemm.h>
#include <ulmblas/impl/level1extensions/gecopy.h>
#include <ulmblas/impl/level3/ukernel/ref/utrusm.h>

namespace ulmBLAS { namespace ref {

template <typename IndexType, typename T>
void
utrusm(const T     *A,
       const T     *B,
       T           *C,
       IndexType   incRowC,
       IndexType   incColC)
{
    const IndexType MR = BlockSizeUGemm<T>::MR;
    const IndexType NR = BlockSizeUGemm<T>::NR;

    T   C_[MR*NR];

    for (IndexType i=0; i<MR; ++i) {
        for (IndexType j=0; j<NR; ++j) {
            C_[i+j*MR] = B[i*NR+j];
        }
    }

    A += MR*(MR-1);
    for (IndexType i=MR-1; i>=0; --i) {
        for (IndexType j=0; j<NR; ++j) {
            C_[i+j*MR] *= A[i];
            for (IndexType l=0; l<i; ++l) {
                C_[l+j*MR] -= A[l]*C_[i+j*MR];
            }
        }
        A -= MR;
    }

    gecopy(MR, NR, C_, IndexType(1), MR, C, incRowC, incColC);
}

} } // namespace ref, ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL3_UKERNEL_REF_UTRUSM_TCC 1
