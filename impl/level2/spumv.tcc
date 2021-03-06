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

#ifndef ULMBLAS_IMPL_LEVEL2_SPUMV_TCC
#define ULMBLAS_IMPL_LEVEL2_SPUMV_TCC 1

#include <ulmblas/impl/level1extensions/dotaxpy.h>
#include <ulmblas/impl/level1/scal.h>
#include <ulmblas/impl/level2/spumv.h>

namespace ulmBLAS {

template <typename IndexType, typename Alpha, typename TA, typename TX,
          typename Beta, typename TY>
void
spumv(IndexType    n,
      const Alpha  &alpha,
      const TA     *A,
      const TX     *x,
      IndexType    incX,
      const Beta   &beta,
      TY           *y,
      IndexType    incY)
{
    typedef decltype(Alpha(0)*TA(0)*TX(0)+Beta(0)*TY(0))  T;

    if (n==0 || (alpha==Alpha(0) && beta==Beta(1))) {
        return;
    }

    scal(n, beta, y, incY);

    if (alpha==Alpha(0)) {
        return;
    }

    for (IndexType j=0; j<n; ++j) {
        T  rho;

        dotaxpy(j, false, false, false,
                alpha*x[j*incX],
                A, IndexType(1),
                x, incX,
                y, incY,
                rho);
        y[j*incY] += alpha*(rho+A[j]*x[j*incX]);
        A += j+1;
    }
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL2_SPUMV_TCC
