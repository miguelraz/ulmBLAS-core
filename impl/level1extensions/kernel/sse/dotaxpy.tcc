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

#ifndef ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_SSE_DOTAXPY_TCC
#define ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_SSE_DOTAXPY_TCC 1

#include <immintrin.h>
#include <ulmblas/impl/auxiliary/isaligned.h>
#include <ulmblas/impl/level1extensions/kernel/ref/dotaxpy.h>
#include <ulmblas/impl/level1extensions/kernel/sse/dotaxpy.h>

namespace ulmBLAS { namespace sse {

//
//  Fuse the computations:
//  (1) $z \leftarrow z + \alpha x$
//  (2) $\rho = x^T y$
//
//  Arguments $x$, $x^T$ or $y$ can be conjugated in the computation
//

//
// ----------------
// Double Precision
// ----------------
//

template <typename IndexType>
void
dotaxpy(IndexType      n,
        bool           conjX,
        bool           conjXt,
        bool           conjY,
        const double   &alpha,
        const double   *x,
        IndexType      incX,
        const double   *y,
        IndexType      incY,
        double         *z,
        IndexType      incZ,
        double         &rho)
{
    if (n<=0) {
        rho = 0;
        return;
    }

    if (incX!=1 || incY!=1 || incZ!=1 || conjX || conjXt || conjY) {
        ref::dotaxpy(n, conjX, conjXt, conjY, alpha, x, incX, y, incY,
                     z, incZ, rho);
        return;
    }

    bool xAligned = isAligned(x, 16);
    bool yAligned = isAligned(y, 16);
    bool zAligned = isAligned(z, 16);

    double rho_ = 0;

    if (!xAligned && !yAligned && !zAligned) {
        z[0] += alpha*x[0];
        rho_  = x[0]*y[0];
        ++x;
        ++y;
        ++z;
        --n;
        xAligned = yAligned = zAligned = true;
    }
    if (xAligned && yAligned && zAligned) {
        IndexType nb = n / 4;
        IndexType nl = n % 4;

        __m128d x12, x34;
        __m128d y12, y34;
        __m128d z12, z34;
        __m128d alpha11;
        __m128d rho12, rho34;

        double  rho12_[2], rho34_[2];

        alpha11 = _mm_loaddup_pd(&alpha);
        rho12   = _mm_setzero_pd();
        rho34   = _mm_setzero_pd();

        for (IndexType i=0; i<nb; ++i) {
            x12 = _mm_load_pd(x);
            y12 = _mm_load_pd(y);
            z12 = _mm_load_pd(z);

            x34 = _mm_load_pd(x+2);
            y34 = _mm_load_pd(y+2);
            z34 = _mm_load_pd(z+2);

            y12 = y12 * x12;
            x12 = alpha11 * x12;
            z12 = z12 + x12;

            _mm_store_pd(z, z12);
            rho12 = rho12 + y12;

            y34 = y34 * x34;
            x34 = alpha11 * x34;
            z34 = z34 + x34;

            _mm_store_pd(z+2, z34);
            rho34 = rho34 + y34;

            x += 4;
            y += 4;
            z += 4;
        }
        _mm_store_pd(rho12_, rho12);
        _mm_store_pd(rho34_, rho34);

        rho = rho_ + rho12_[0] + rho12_[1] + rho34_[0] + rho34_[1];

        for (IndexType i=0; i<nl; ++i) {
            z[i] += alpha*x[i];
            rho  += x[i] * y[i];
        }
    } else {
        rho = 0;
        ref::dotaxpy(n, false, false, false, alpha, x, incX, y, incY,
                     z, incZ, rho);
        rho += rho_;
    }
}

} } // namespace sse, ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_SSE_DOTAXPY_TCC 1
