#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "kernel.h"
#include "tab.h"
#include "print_calib.h"

#define NB_META 31
#define OUTOUT_FILE "output_calibrate.txt"

extern uint64_t rdtsc ();

int main(int argc, char **argv)
{
    unsigned int n, repm;
    char* file_name = NULL;
    if (argc != 3 && argc != 4) 
    {
        fprintf (stderr, "Usage: %s <problem size> <nb repeat> [file name]\n", argv[0]);
        return 1;
    }
    else
    {
        n = atoi(argv[1]);
        repm = atoi(argv[2]);
        file_name = (char*)malloc(256*sizeof(char));
        if (argc == 3)
            strcpy(file_name, OUTOUT_FILE);
        else if (argc == 4)
            strcpy(file_name, argv[3]);
    }

    uint64_t **tdiff = malloc( repm * sizeof(tdiff[0][0]));
    for(unsigned int k = 0 ; k < repm ; k++)
    {
        tdiff[k] = malloc( NB_META * sizeof(tdiff[0]));
    }

    int size = n * n * sizeof(double);
    
    double *a = (double*)malloc(size);
    double *b = (double*)malloc(size);
    double *c = (double*)malloc(size);

    srand(0);
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

    printf("Calibration . . . 0%%");
    for (unsigned int m = 0; m < NB_META; m++)
    {
        for (unsigned int k = 0; k < repm; k++)
        {
            const uint64_t t1 = rdtsc();
            kernel(n, d_a, d_b, d_c);
            const uint64_t t2 = rdtsc();
            tdiff[k][m] = t2 - t1;
        }
        GPUMM_MEMCPY_DtH(a, d_a, size);
        sleep(3);
        printf("\rCalibration . . . %d%%",(m*100)/(NB_META-1));
        fflush(stdout);
    }

    GPUMM_FREE(d_a);
    GPUMM_FREE(d_b);
    GPUMM_FREE(d_c);

    free(a);
    free(b);
    free(c);

    print_calib(repm, tdiff, file_name);
    return EXIT_SUCCESS;
}