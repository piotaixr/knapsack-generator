/* 
 * Author: Rémi PIOTAIX <remi.piotaix@gmail.com>
 *
 */

#include "instanceType.h"


#ifdef	__cplusplus

std::ostream& operator<<(std::ostream& os, const instanceType& type) {
    std::string name;
    switch (type) {
        case UNCORRELATED:
            name = "UNCORRELATED";
            break;
        case WEAKLY_CORRELATED:
            name = "WEAKLY_CORRELATED";
            break;
        case STRONGLY_CORRELATED:
            name = "STRONGLY_CORRELATED";
            break;
        case INVERSE_STRONGLY_CORRELATED:
            name = "INVERSE_STRONGLY_CORRELATED";
            break;
        case ALMOST_STRONGLY_CORRELATED:
            name = "ALMOST_STRONGLY_CORRELATED";
            break;
        case SUBSET_SUM:
            name = "SUBSET_SUM";
            break;
        case EVEN_ODD_SUBSET_SUM:
            name = "EVEN_ODD_SUBSET_SUM";
            break;
        case EVEN_ODD_KNAPSASK:
            name = "EVEN_ODD_KNAPSASK";
            break;
        case UNCORRELATED_SIMILAR_WEIGHTS:
            name = "UNCORRELATED_SIMILAR_WEIGHTS";
            break;
        case AVIS_SUBSET_SUM:
            name = "AVIS_SUBSET_SUM";
            break;
        case AVIS_KNAPSACK:
            name = "AVIS_KNAPSACK";
            break;
        case COLLAPSING_KNAPSACK:
            name = "COLLAPSING_KNAPSACK";
            break;
        case BOUNDED_STRONGLY_CORRELATED:
            name = "BOUNDED_STRONGLY_CORRELATED";
            break;
        case NO_SMALL_WEIGHTS:
            name = "NO_SMALL_WEIGHTS";
            break;
        default:
            name = "UNKNOWN";
            break;
    }

    os << name;

    return os;
}
#endif
