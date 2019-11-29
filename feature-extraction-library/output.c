/*
 * Copyright (c) 2019, Institute of Electronics and Computer Science (EDI)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: Atis Elsts
 */

/*
 * File: output.c
 * An application that prints the results of applying the different features on the test data.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#ifdef CONFIG_ARCH
#include <zephyr.h>
#include <misc/printk.h>
#include "qsort.h"
#else
#include "adaptation.h"
#endif

#define DO_LOG_OUTPUT 1
#include "main.h"

// -----------------------------------------------------------

// the input data
const accel_t data[] =
{
#include "sample-data/00001.c"
    ,
#include "sample-data/00002.c"
    ,
#include "sample-data/00003.c"
    ,
#include "sample-data/00004.c"
    ,
#include "sample-data/00005.c"
    ,
#include "sample-data/00007.c"
};

// -----------------------------------------------------------

#include "features-time-basic.c"
#include "features-time-sort.c"
#include "features-time-advanced.c"

// -----------------------------------------------------------

typedef struct {
    const char *name;
    feature_function f;
} test_t;

// -----------------------------------------------------------

void test(const test_t *t)
{
    int i, axis;
    s64_t start, delta;

    start = k_uptime_get();
    LOG("Start feature: %s\n", t->name);
    // iterate for each axis
    for (axis = 0; axis < NUM_AXIS; ++axis) {
        t->f(axis);
    }
}

// -----------------------------------------------------------

const test_t tests[] =
{
    // Mean + energy + std combination
    { "mean", feature_mean },
    { "energy", feature_energy },
    { "std", feature_std },

    // Correlation + std combination
    { "correlation", feature_correlation },

    // Entropy
    { "entropy", feature_entropy },

    // Sorting-related functions
    { "min", feature_min },
    { "max", feature_max },
    { "median", feature_median },
    { "q25", feature_q25 },
    { "q75", feature_q75 },
    { "iqr", feature_iqr },

    // SMA (sum of absolute values)
    //{ "sma", feature_sma },
};

// -----------------------------------------------------------

void do_tests(void)
{
    int i;

    printk("Starting tests, ARCH=%s F_CPU=%d MHz\n",
           CONFIG_ARCH, (int)(CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC / 1000000));

    for (i = 0; i < sizeof(tests) / sizeof(*tests); ++i) {
        test(&tests[i]);
    }

    printk("Done!\n");
}

// -----------------------------------------------------------

int main(void)
{
    do_tests();
}


// -----------------------------------------------------------
