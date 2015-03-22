#ifndef ULMBLAS_IMPL_LEVEL1EXTENSIONS_TRLCOPY_TCC
#define ULMBLAS_IMPL_LEVEL1EXTENSIONS_TRLCOPY_TCC 1

#include <ulmblas/impl/level1extensions/trlcopy.h>

namespace ulmBLAS {

template <typename IndexType, typename MX, typename MY>
void
trlcopy(IndexType    m,
        IndexType    n,
        bool         unit,
        MX           *X,
        IndexType    incRowX,
        IndexType    incColX,
        MY           *Y,
        IndexType    incRowY,
        IndexType    incColY)
{
    const IndexType    UnitStride(1);

    if (m<=0 || n<=0) {
        return;
    }

    if (unit) {
        trlcopy(m-1, n-1, false,
                &X[1*incRowX], incRowX, incColX,
                &Y[1*incRowY], incRowY, incColY);
        return;
    }

    if (incRowX==UnitStride && incRowY==UnitStride) {
        const IndexType k = std::min(m, n);
        for (IndexType j=0; j<k; ++j) {
            copy(m-j,
                 &X[j*(incRowX+incColX)], UnitStride,
                 &Y[j*(incRowY+incColY)], UnitStride);
        }
    } else if (incColX==UnitStride && incColY==UnitStride) {
        for (IndexType i=0; i<m; ++i) {
            copy(std::min(i+1,n),
                 &X[i*incRowX], UnitStride,
                 &Y[i*incRowY], UnitStride);
        }
    } else {
        const IndexType k = std::min(m, n);
        for (IndexType j=0; j<k; ++j) {
            for (IndexType i=j; i<m; ++i) {
                Y[i*incRowY+j*incColY] = X[i*incRowX+j*incColX];
            }
        }
    }
}

} // namespace ulmBLAS

#endif // ULMBLAS_IMPL_LEVEL1EXTENSIONS_TRLCOPY_TCC 1