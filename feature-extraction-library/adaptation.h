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
 * File: adaptation.h
 * Provide an adaptation layer for functionality provided by the different OS
 */

#include <sys/time.h>
#include <stdio.h>
#include <stdio.h>
#include <math.h>

#define printk printf

typedef long long int s64_t;

#if CONTIKI_TARGET_SRF06_CC26XX || CONTIKI_TARGET_Z1 || CONTIKI_TARGET_ZOUL || CONTIKI_TARGET_NRF52DK
#define CONTIKI 1
#else
#define CONTIKI 0
#endif

#if CONTIKI
#include "contiki.h"

#define CONFIG_ARCH "sphere"
#define CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC (48u * 1000 * 1000)

static inline s64_t k_uptime_get(void)
{
    unsigned long t = clock_time();
    // convert to milliseconds
    return t * 1000 / CLOCK_SECOND;
}

static inline s64_t k_uptime_delta(s64_t *s)
{
    return k_uptime_get() - *s;
}

#define min(a, b) MIN(a, b)
#define max(a, b) MAX(a, b)

#else /* CONTIKI */

#include <memory.h>
#include <complex.h>

typedef float complex cplx;

static inline s64_t k_uptime_get(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static inline s64_t k_uptime_delta(s64_t *s)
{
    return k_uptime_get() - *s;
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define CONFIG_ARCH "native"
#define CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC 1

#endif /* CONTIKI */
