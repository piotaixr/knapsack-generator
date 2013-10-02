/* 
 * Author: Rémi PIOTAIX <remi.piotaix@gmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pisinger.h"

int callPisinger(int numItems, int coeffRange, instanceType type, int instanceNumber, int serie, char* generatorPath) {
    pid_t ch_id = fork();
    if (ch_id == -1) {
        return EXIT_FAILURE;
    } else if (ch_id == 0) {
        freopen("/dev/null", "w", stdout);
        char argnumItems[16];
        char argcoeffRange[16];
        char argtype[16];
        char arginstanceNumber[16];
        char argserie[16];

        sprintf(argnumItems, "%d", numItems);
        sprintf(argcoeffRange, "%d", coeffRange);
        sprintf(argtype, "%d", type);
        sprintf(arginstanceNumber, "%d", instanceNumber);
        sprintf(argserie, "%d", serie);


        char* argv[7] = {generatorPath, argnumItems, argcoeffRange, argtype, arginstanceNumber, argserie, NULL};
        execvp(argv[0], argv);
    } else {
        waitpid(ch_id, NULL, 0);
    }

    return 1;
}