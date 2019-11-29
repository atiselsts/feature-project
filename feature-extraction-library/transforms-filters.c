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
 * File: transforms-filters.c
 * Transforms (such as jerk) and filters (such as median).
 * Features can be calculated on the result of these.
 */

// -----------------------------------------------------------

void transform_jerk(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES - 1; i++) {
        int j = data[i + 1].v[axis] - data[i].v[axis];
        OUTPUT_I(j, result_i.v[axis]);
        LOG("\n");
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_magnitude_sq(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES; i++) {
        int x = data[i].v[0];
        int y = data[i].v[1];
        int z = data[i].v[2];

        x *= x;
        y *= y;
        z *= z;

        unsigned r = x + y + z;
        OUTPUT_I(r, result_i.v[axis]);
        LOG("\n");
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_magnitude(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES; i++) {
        int x = data[i].v[0];
        int y = data[i].v[1];
        int z = data[i].v[2];

        x *= x;
        y *= y;
        z *= z;

        OUTPUT_F(sqrtf(x + y + z), result_f.v[axis]);
        LOG("\n");
    }
    OUTPUT_F(0.0, result_f.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_l1norm(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES; i++) {
        int x = data[i].v[0];
        int y = data[i].v[1];
        int z = data[i].v[2];

        unsigned r = abs(x) + abs(y) + abs(z);
        OUTPUT_I(r, result_i.v[axis]);
        LOG("\n");
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_jerk_magnitude_sq(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES - 1; i++) {
        int x = data[i + 1].v[0] - data[i].v[0];
        int y = data[i + 1].v[1] - data[i].v[1];
        int z = data[i + 1].v[2] - data[i].v[2];

        x *= x;
        y *= y;
        z *= z;

        OUTPUT_I(x + y + z, result_i.v[axis]);
        LOG("\n");
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_jerk_magnitude(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES - 1; i++) {
        int x = data[i + 1].v[0] - data[i].v[0];
        int y = data[i + 1].v[1] - data[i].v[1];
        int z = data[i + 1].v[2] - data[i].v[2];

        x *= x;
        y *= y;
        z *= z;

        OUTPUT_F(sqrtf(x + y + z), result_f.v[axis]);
        LOG("\n");
    }
    OUTPUT_F(0.0, result_f.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

void transform_jerk_l1norm(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    for (i = 0; i < NSAMPLES - 1; i++) {
        int x = data[i + 1].v[0] - data[i].v[0];
        int y = data[i + 1].v[1] - data[i].v[1];
        int z = data[i + 1].v[2] - data[i].v[2];

        unsigned r = abs(x) + abs(y) + abs(z);

        OUTPUT_I(r, result_i.v[axis]);
        LOG("\n");
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------

static inline int median(int a, int b, int c)
{
    if (a > b) {
        if (b > c) {
            return b; // a, b, c
        }
        if (a > c) {
            return c; // a, c, b
        }
        return a; // c, a, b
    }
    else { // a <= b
        if (a > c) {
            return a; // b, a, c
        }
        if (b > c) {
            return c; // b, c, a
        }
        return b; // c, b, a
    }
}

void filter_median(int axis)
{
    int i;
    LOG("axis=%d\n", axis);
    int prev = data[0].v[axis];
    int curr = data[1].v[axis];
    OUTPUT_I(0, result_i.v[axis]);
    for (i = 2; i < NSAMPLES; i++) {
        int nxt = data[i].v[axis];
        int m = median(prev, curr, nxt);
        OUTPUT_I(m, result_i.v[axis]);
        LOG("\n");

        prev = curr;
        curr = nxt;
    }
    OUTPUT_I(0, result_i.v[axis]);
    LOG("\n");
}

// -----------------------------------------------------------
