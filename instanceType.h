/* 
 * File:   instanceType.h
 * Author: nell
 *
 * Created on 28 mai 2013, 20:08
 */

#ifndef INSTANCETYPE_H
#define	INSTANCETYPE_H

typedef enum {
    UNCORRELATED = 1,
    WEAKLY_CORRELATED = 2,
    STRONGLY_CORRELATED = 3,
    INVERSE_STRONGLY_CORRELATED = 4,
    ALMOST_STRONGLY_CORRELATED = 5,
    SUBSET_SUM = 6,
    EVEN_ODD_SUBSET_SUM = 7,
    EVEN_ODD_KNAPSASK = 8,
    UNCORRELATED_SIMILAR_WEIGHTS = 9,
    AVIS_SUBSET_SUM = 11,
    AVIS_KNAPSACK = 12,
    COLLAPSING_KNAPSACK = 13,
    BOUNDED_STRONGLY_CORRELATED = 14,
    NO_SMALL_WEIGHTS = 15
} instanceType;

#ifdef	__cplusplus
#include <ostream>
std::ostream& operator<<(std::ostream& os, const instanceType& type);
#endif


#endif	/* INSTANCETYPE_H */

