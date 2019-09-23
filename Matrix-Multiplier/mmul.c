#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"


// TODO
// Add any needed auxiliary data types and/or functions here


/* Return the product of two matrices.
 * Return NULL if anything is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
    
   // TODO
struct arg_struct { // add column needs two args
    int cur_thread;
    int num_threads;
    TMatrix *m;
    TMatrix *n;
    TMatrix *t;
};

void *add_rows(void *arguments){ // dirty work function, adds values in rows
    struct arg_struct *args = (struct arg_struct *)arguments;
    for (unsigned int i = args->cur_thread; i < args->m->nrow; i = i + args->num_threads)  {
        for (unsigned int j = 0; j < args->n->ncol; j++) {
            TElement sum = (TElement)0;
            for (unsigned int k = 0; k < args->m->ncol; k++)
                sum += args->m->data[i][k] * args->n->data[k][j];
            args->t->data[i][j] = sum;
        }
    }
    
    pthread_exit(NULL);
    return 0;
}
 
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n, int num_threads)
{
   // TODO
   int i = 0;
   
   // check if arguments are correct
   if (     m == NULL || n == NULL
          || m->ncol != n->nrow   )
        return NULL;
    
    TMatrix * t = newMatrix(m->nrow, n->ncol); // new matrix
    if (t == NULL) // check to see if t is empty before starting
        return t;
        
    // create threads
    struct arg_struct args[num_threads];
    pthread_t threads[num_threads]; // stores all our threads
    for (i = 0; i < num_threads; i++){ 
        args[i].cur_thread = i;
        args[i].num_threads = num_threads;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;
        pthread_create(&threads[i], NULL, add_rows, (void*)&args[i]);
    }
    
    // join the threads wait for them to complete and do our dirty work
    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }
        
    // now t should have all added values, return t
   return t;
   pthread_exit(NULL); 
   
}
