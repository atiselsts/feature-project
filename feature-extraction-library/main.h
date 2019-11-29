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
 * File: main.h
 * The main header file.
 */


#include <stdint.h>

#ifndef MAIN_H
#define MAIN_H

#define SAMPLING_HZ 20

#ifndef TIME_WINDOW_SIZE
#define TIME_WINDOW_SIZE 128
#endif

#ifndef FREQUENCY_WINDOW_SIZE
#define FREQUENCY_WINDOW_SIZE 128
#endif

#if TIME_WINDOW_SIZE == 32
#pragma message "TIME_WINDOW_SIZE == 32"
#elif TIME_WINDOW_SIZE == 64
#pragma message "TIME_WINDOW_SIZE == 64"
#elif TIME_WINDOW_SIZE == 128
#pragma message "TIME_WINDOW_SIZE == 128"
#endif

// Make the windows to have 50% overlap
#define PERIODIC_COMPUTATION_WINDOW_SIZE (TIME_WINDOW_SIZE / 2)

#define VERY_FAST 0
#define FAST 1
#define MODERATE 2
#define SLOW 3

// If this is set to true, repetitions are not done;
// instead, the output is logged for each.
#ifndef DO_LOG_OUTPUT
#define DO_LOG_OUTPUT 0
#endif

#if DO_LOG_OUTPUT

static const int NUM_REPETITIONS[4] = {
    [VERY_FAST] = 1,
    [FAST] = 1,
    [MODERATE] = 1,
    [SLOW] = 1
};

#else // DO_LOG_OUTPUT

#if CONTIKI_TARGET_SRF06_CC26XX || CONTIKI_TARGET_ZOUL || CONTIKI_TARGET_NRF52DK
static const int NUM_REPETITIONS[4] = {
    [VERY_FAST] = 100,
    [FAST] = 10,
    [MODERATE] = 1,
    [SLOW] = 1
};
#elif CONTIKI_TARGET_Z1
static const int NUM_REPETITIONS[4] = {
    [VERY_FAST] = 10,
    [FAST] = 10,
    [MODERATE] = 1,
    [SLOW] = 1
};
#else
static const int NUM_REPETITIONS[4] = {
    [VERY_FAST] = 1000,
    [FAST] = 1000,
    [MODERATE] = 100,
    [SLOW] = 10
};
#endif
#endif // DO_LOG_OUTPUT

// -----------------------------------------------------------

#if DO_LOG_OUTPUT
#define LOG(...) printk(__VA_ARGS__)
#else
#define LOG(...)
#endif

#if DO_LOG_OUTPUT
#define OUTPUT(x, variable, format) printk(format, x)
#else
#define OUTPUT(x, variable, format) variable = x
#endif

#define OUTPUT_I(x, variable)  OUTPUT(x, variable, "%d ")
#define OUTPUT_IL(x, variable) OUTPUT(x, variable, "%lld ")
#define OUTPUT_F(x, variable)  OUTPUT(x, variable, "%f ")

#define NUM_AXIS 3

// -----------------------------------------------------------

typedef struct {
    int8_t v[NUM_AXIS];
} accel_t;

typedef struct {
    volatile int32_t v[NUM_AXIS];
} result_i_t;

typedef struct {
    volatile float v[NUM_AXIS];
} result_f_t;

// -----------------------------------------------------------

// the total number of samples
#define NSAMPLES ((unsigned int)(sizeof(data) / sizeof(*data)))

// -----------------------------------------------------------

typedef void (*feature_function)(int);

// the result of the accel calculations is stored here
volatile result_i_t result_i;
volatile result_f_t result_f;

// -----------------------------------------------------------


#endif
