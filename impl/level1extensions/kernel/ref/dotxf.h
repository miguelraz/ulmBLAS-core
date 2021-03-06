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

#ifndef ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_REF_DOTXF_H
#define ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_REF_DOTXF_H 1

#include <ulmblas/impl/config/fusefactor.h>
#include <type_traits>

namespace ulmBLAS { namespace ref {

template <typename IndexType, typename TX, typename TY, typename Result>
    typename std::enable_if<std::is_integral<IndexType>::value
                      && FuseFactor<decltype(TX(0)*TY(0)+Result(0))>::dotuxf==4,
    void>::type
    dotuxf(IndexType      n,
           const TX       *X,
           IndexType      incRowX,
           IndexType      incColX,
           const TY       *y,
           IndexType      incY,
           Result         *result,
           IndexType      resultInc);

template <typename IndexType, typename TX, typename TY, typename Result>
    typename std::enable_if<std::is_integral<IndexType>::value
                      && FuseFactor<decltype(TX(0)*TY(0)+Result(0))>::dotuxf!=4,
    void>::type
    dotuxf(IndexType      n,
           const TX       *X,
           IndexType      incRowX,
           IndexType      incColX,
           const TY       *y,
           IndexType      incY,
           Result         *result,
           IndexType      resultInc);

template <typename IndexType, typename TX, typename TY, typename Result>
    typename std::enable_if<std::is_integral<IndexType>::value
                      && FuseFactor<decltype(TX(0)*TY(0)+Result(0))>::dotcxf==4,
    void>::type
    dotcxf(IndexType      n,
           const TX       *X,
           IndexType      incRowX,
           IndexType      incColX,
           const TY       *y,
           IndexType      incY,
           Result         *result,
           IndexType      resultInc);

template <typename IndexType, typename TX, typename TY, typename Result>
    typename std::enable_if<std::is_integral<IndexType>::value
                      && FuseFactor<decltype(TX(0)*TY(0)+Result(0))>::dotcxf!=4,
    void>::type
    dotcxf(IndexType      n,
           const TX       *X,
           IndexType      incRowX,
           IndexType      incColX,
           const TY       *y,
           IndexType      incY,
           Result         *result,
           IndexType      resultInc);

} } // namespace ref, ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL1EXTENSIONS_KERNEL_REF_DOTXF_H

#include <ulmblas/impl/level1extensions/kernel/ref/dotxf.tcc>
