/*
 * Copyright (C) 2014, The University of Texas at Austin
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

/*
 * Copyright (C) 2014-2015, Michael Lehn
 *
 * ulmBLAS adopted general ideas from BLIS.  Using micro kernels from BLIS
 * only requires minor modifications,
 *
 */

#ifndef ULMBLAS_IMPL_LEVEL1_NRM2_TCC
#define ULMBLAS_IMPL_LEVEL1_NRM2_TCC 1

#include <complex>
#include <cmath>
#include <ulmblas/impl/auxiliary/pow.h>
#include <ulmblas/impl/level1/nrm2.h>

namespace ulmBLAS {

template <typename IndexType, typename VX>
VX
nrm2(IndexType  n,
     const VX   *x,
     IndexType  incX)
{
    VX result;

    nrm2(n, x, incX, result);
    return result;
}

template <typename IndexType, typename VX>
VX
nrm2(IndexType                n,
     const std::complex<VX>   *x,
     IndexType                incX)
{
    VX result;

    nrm2(n, x, incX, result);
    return result;
}

template <typename IndexType, typename VX, typename Result>
void
nrm2(IndexType  n,
     const VX   *x,
     IndexType  incX,
     Result     &result)
{
    using std::abs;
    using std::sqrt;

    const Result  Zero(0), One(1);

    if (n<1) {
        result = Zero;
    } else if (n==1) {
        result = abs(*x);
    } else {
        Result scale = 0;
        Result ssq   = 1;

        for (IndexType i=0; i<n; ++i) {
            if (x[i*incX]!=Zero) {
                Result absXi = abs(x[i*incX]);
                if (scale<absXi) {
                    ssq = One + ssq * pow(scale/absXi, 2);
                    scale = absXi;
                } else {
                    ssq += pow(absXi/scale, 2);
                }
            }
        }
        result = scale*sqrt(ssq);
    }
}

template <typename IndexType, typename VX, typename Result>
void
nrm2(IndexType                n,
     const std::complex<VX>   *x,
     IndexType                incX,
     Result                   &result)
{
    using std::abs;
    using std::imag;
    using std::real;
    using std::pow;

    const Result  Zero(0), One(1);

    if (n<1) {
        result = Zero;
    } else if (n==1) {
        result = abs(*x);
    } else {
        Result scale = 0;
        Result ssq = 1;

        for (IndexType i=0; i<n; ++i) {
            if (real(x[i*incX]) != Zero) {
                Result absXi = abs(real(x[i*incX]));
                if (scale<absXi) {
                    ssq = One + ssq * pow(scale/absXi, 2);
                    scale = absXi;
                } else {
                    ssq += pow(absXi/scale, 2);
                }
            }
            if (imag(x[i*incX]) != Zero) {
                Result absXi = abs(imag(x[i*incX]));
                if (scale<absXi) {
                    ssq = One + ssq * pow(scale/absXi, 2);
                    scale = absXi;
                } else {
                    ssq += pow(absXi/scale, 2);
                }
            }
        }
        result = scale*sqrt(ssq);
    }
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL1_NRM2_TCC 1
