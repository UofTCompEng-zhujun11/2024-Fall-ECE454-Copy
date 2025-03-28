#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include "clock.h"

/*
 * Routines for using the cycle counter
 */

struct timespec timespec_start;
struct timespec timespec_start;

void start_counter() {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timespec_start);
}

double get_counter()
{
    struct timespec timespec_end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timespec_end);
    return (timespec_end.tv_sec - timespec_start.tv_sec) * 1e9 + 
           (timespec_end.tv_nsec - timespec_start.tv_nsec);
}

double ovhd()
{
    /* Do it twice to eliminate cache effects */
    int i;
    double result;

    for (i = 0; i < 2; i++) {
        start_counter();
        result = get_counter();
    }
    return result;
}

/* $begin mhz */
/* Estimate the clock rate by measuring the cycles that elapse */
/* while sleeping for sleeptime seconds */
double mhz_full(int verbose, int sleeptime)
{
    double rate;

    start_counter();
    sleep(sleeptime);
    rate = get_counter() / (1e6*sleeptime);
    if (verbose)
        printf("Processor clock rate ~= %.1f MHz\n", rate);
    return rate;
}
/* $end mhz */

/* Version using a default sleeptime */
double mhz(int verbose)
{
    return mhz_full(verbose, 2);
}

/** Special counters that compensate for timer interrupt overhead */

static double cyc_per_tick = 0.0;

#define NEVENT 100
#define THRESHOLD 1000
#define RECORDTHRESH 3000

/* Attempt to see how much time is used by timer interrupt */
static void callibrate(int verbose)
{
    double oldt;
    struct tms t;
    clock_t oldc;
    int e = 0;

    times(&t);
    oldc = t.tms_utime;
    start_counter();
    oldt = get_counter();
    while (e <NEVENT) {
        double newt = get_counter();

        if (newt-oldt >= THRESHOLD) {
            clock_t newc;
            times(&t);
            newc = t.tms_utime;
            if (newc > oldc) {
                double cpt = (newt-oldt)/(newc-oldc);
                if ((cyc_per_tick == 0.0 || cyc_per_tick > cpt) && cpt > RECORDTHRESH)
                    cyc_per_tick = cpt;
                /*
                  if (verbose)
                  printf("Saw event lasting %.0f cycles and %d ticks.  Ratio = %f\n",
                  newt-oldt, (int) (newc-oldc), cpt);
                */
                e++;
                oldc = newc;
            }
            oldt = newt;
        }
    }
    /* ifdef added by Sanjit - 10/2001 */
#ifdef DEBUG
    if (verbose)
	printf("Setting cyc_per_tick to %f\n", cyc_per_tick);
#endif
}

static clock_t start_tick = 0;

void start_comp_counter()
{
    struct tms t;

    if (cyc_per_tick == 0.0)
        callibrate(1);
    times(&t);
    start_tick = t.tms_utime;
    start_counter();
}

double get_comp_counter()
{
    double time = get_counter();
    double ctime;
    struct tms t;
    clock_t ticks;

    times(&t);
    ticks = t.tms_utime - start_tick;
    ctime = time - ticks*cyc_per_tick;
    /*
      printf("Measured %.0f cycles.  Ticks = %d.  Corrected %.0f cycles\n",
      time, (int) ticks, ctime);
    */
    return ctime;
}
