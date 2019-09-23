#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"


// TODO
// Add any needed auxiliary data types and/or functions here


/* Return the sum of two matrices.
 * Return NULL if anything is wrong.
 *
 * Similar to addMatrix, but with multi-threading.
 */
struct arg_struct { // add column needs two args
    int cur_thread;
    int num_threads;
    TMatrix *m;
    TMatrix *n;
    TMatrix *t;
};

void *add_column(void *arguments){ // dirty work function, adds values in cols
    struct arg_struct *args = (struct arg_struct *)arguments;
    for(int i = args->cur_thread; i < args->n->ncol; i = i + args->num_threads){
        for(int j = 0; j < args->n->nrow; j++){
            args->t->data[j][i] = args->n->data[j][i] + args->m->data[j][i];
        }
    }
    pthread_exit(NULL);
    return 0;
}
 
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n, int num_threads)
{
   // TODO
   int i = 0;
   
   // check if arguments are correct
   if (    m == NULL || n == NULL
         || m->nrow != n->nrow || m->ncol != n->ncol ) // args are correct?
        return NULL;
    
    TMatrix * t = newMatrix(m->nrow, m->ncol); // new matrix
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
        pthread_create(&threads[i], NULL, add_column, (void*)&args[i]);
    }
    
    // join the threads wait for them to complete and do our dirty work
     for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
     }
        
    // now t should have all added values, return t
   return t;
   pthread_exit(NULL); 
   
}
