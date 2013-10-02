/* 
 * File:   pisinger.h
 * Author: nell
 *
 * Created on 27 mai 2013, 15:46
 */

#ifndef PISINGER_H
#define	PISINGER_H

#include "instanceType.h"

#ifdef	__cplusplus
extern "C" {
#endif

    int callPisinger(int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, char* generatorPath);


#ifdef	__cplusplus
}
#endif

#endif	/* PISINGER_H */

