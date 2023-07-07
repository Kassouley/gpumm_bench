#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"
extern "C" {
#include "tab.h"
}

#define OUTOUT_FILE "output_check.txt"

#ifdef GPUMM_HANDLE_ENABLE
GPUMM_BLAS_HANDLE handle;
#endif

int main(int argc, char **argv)
{
    int n;
    char* file_name = NULL;
    FILE * output = NULL;

    if (argc != 2 && argc != 3) 
    {
        fprintf (stderr, "Usage: %s <problem size> [file name]\n", argv[0]);
        return 1;
    }
    else
    {
        n = atoll(argv[1]);
        file_name = (char*)malloc(256*sizeof(char));
        if (argc == 2)
            strcpy(file_name, OUTOUT_FILE);
        else if (argc == 3)
            strcpy(file_name, argv[2]);
    }
    
    int size = n * n * sizeof(double);
    
    double *a = (double*)malloc(size);
    double *b = (double*)malloc(size);
    double *c = (double*)malloc(size);

    init_tab2d_random(n, &b);
    init_tab2d_random(n, &c);

    double* d_a;
    double* d_b;
    double* d_c;

	GPUMM_ALLOC(d_a, size);
	GPUMM_ALLOC(d_b, size);
	GPUMM_ALLOC(d_c, size);

    GPUMM_MEMCPY_HtD(d_b, b, size);
    GPUMM_MEMCPY_HtD(d_c, c, size);

    #ifdef GPUMM_HANDLE_ENABLE
    GPUMM_HANDLE_CREATE(handle);
    #endif

    kernel(n, d_a, d_b, d_c);

    #ifdef GPUMM_HANDLE_ENABLE
    GPUMM_HANDLE_DESTROY(handle);
    #endif 
    
    GPUMM_MEMCPY_DtH(a, d_a, size);

    GPUMM_FREE(d_a);
    GPUMM_FREE(d_b);
    GPUMM_FREE(d_c); 

    output = fopen(file_name, "w");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(output, "%.10f ", a[i*n+j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
    
    free(a);
    free(b);
    free(c);
}