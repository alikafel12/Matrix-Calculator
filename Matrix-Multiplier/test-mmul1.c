#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include    "matrix.h"
#include    "unixtimer.h"


int main(int argc, char ** argv)
{
    int     nrow;
    int     ncol;
    int     ncol2;
    int     nthread;

    if (argc == 5) {
        nrow = atoi(argv[1]);
        ncol = atoi(argv[2]);
        ncol2 = atoi(argv[3]);
        nthread = atoi(argv[4]);
        
    }
    else {
        fprintf(stderr, "Usage: %s nrows ncols ncols_2 nthreads\nExiting.\n", argv[0]);
        return -1;
    }

    assert(nrow > 0 && ncol > 0);

    TMatrix *m, *n;

    m = newMatrix(nrow, ncol);
    assert( m != NULL);
    n = newMatrix(ncol, ncol2);
    assert( n != NULL);

    fillMatrix(m);
    fillMatrix(n);

    
    double  time1, time2;

    start_timer();
    
    TMatrix * t1 = mulMatrix(m, n);
    assert(t1 != NULL);
    
    time1=cpu_seconds();
    

    
    start_timer();
    
    TMatrix * t2 = mulMatrix_thread(m, n, nthread);
    assert(t2 != NULL);
    
    time2=cpu_seconds();
    

    int r = compareMatrix(t1, t2);

    freeMatrix(m);
    freeMatrix(n);
    freeMatrix(t1);
    freeMatrix(t2);

    printf("%d %s\n", r, r ? "Do not match." : "Good work!");
    
    printf("time1=%.5f time2=%.5f\n", time1, time2);
    
    return 0;
}
