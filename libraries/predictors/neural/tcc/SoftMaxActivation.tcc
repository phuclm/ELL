////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     SoftMaxActivation.tcc (neural)
//  Authors:  Byron Changuion
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// stl
#include <algorithm>
#include <limits>

namespace ell
{
namespace predictors
{
namespace neural
{
    template <typename ElementType>
    void SoftMaxActivation<ElementType>::Apply(math::ColumnVector<ElementType>& input) const
    {
        ElementType maxVal = std::numeric_limits<ElementType>::lowest();
        for (size_t i = 0; i < input.Size(); ++i)
        {
            maxVal = std::max(maxVal, input[i]);
        }

        ElementType sum = 0;
        for(size_t i = 0; i < input.Size(); ++i)
        {
            double eulerVal = std::exp(input[i] - maxVal);
            input[i] = eulerVal;
            sum += eulerVal;
        }

        const ElementType epsilon = 1e-7;
        if (sum < epsilon)
        {
            sum = 1.0;
        }
        
        input.Transform([sum](ElementType value){ return value / sum; });
    }
}
}
}
