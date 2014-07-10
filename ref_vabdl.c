/*

Copyright (c) 2009, 2010, 2011 STMicroelectronics
Written by Christophe Lyon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VABDL"
void exec_vabdl (void)
{
  /* Basic test: v4=vabdl(v1,v2), then store the result.  */
#define TEST_VABDL(T1, T2, W, W2, N)					\
  VECT_VAR(vector_res, T1, W2, N) =					\
    vabdl_##T2##W(VECT_VAR(vector1, T1, W, N),				\
		  VECT_VAR(vector2, T1, W, N));				\
  vst1q_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector_res, T1, W2, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
#define DECL_VABDL_VAR_LONG(VAR)		\
  DECL_VARIABLE(VAR, int, 16, 8);		\
  DECL_VARIABLE(VAR, int, 32, 4);		\
  DECL_VARIABLE(VAR, int, 64, 2);		\
  DECL_VARIABLE(VAR, uint, 16, 8);		\
  DECL_VARIABLE(VAR, uint, 32, 4);		\
  DECL_VARIABLE(VAR, uint, 64, 2)

#define DECL_VABDL_VAR_SHORT(VAR)		\
  DECL_VARIABLE(VAR, int, 8, 8);		\
  DECL_VARIABLE(VAR, int, 16, 4);		\
  DECL_VARIABLE(VAR, int, 32, 2);		\
  DECL_VARIABLE(VAR, uint, 8, 8);		\
  DECL_VARIABLE(VAR, uint, 16, 4);		\
  DECL_VARIABLE(VAR, uint, 32, 2)

  DECL_VABDL_VAR_SHORT(vector1);
  DECL_VABDL_VAR_SHORT(vector2);
  DECL_VABDL_VAR_LONG(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_VLOAD(vector1, buffer, , int, s, 8, 8);
  TEST_VLOAD(vector1, buffer, , int, s, 16, 4);
  TEST_VLOAD(vector1, buffer, , int, s, 32, 2);
  TEST_VLOAD(vector1, buffer, , uint, u, 8, 8);
  TEST_VLOAD(vector1, buffer, , uint, u, 16, 4);
  TEST_VLOAD(vector1, buffer, , uint, u, 32, 2);


  /* Choose init value arbitrarily */
  TEST_VDUP(vector2, , int, s, 8, 8, 1);
  TEST_VDUP(vector2, , int, s, 16, 4, -13);
  TEST_VDUP(vector2, , int, s, 32, 2, 8);
  TEST_VDUP(vector2, , uint, u, 8, 8, 1);
  TEST_VDUP(vector2, , uint, u, 16, 4, 13);
  TEST_VDUP(vector2, , uint, u, 32, 2, 8);

  TEST_VABDL(int, s, 8, 16, 8);
  TEST_VABDL(int, s, 16, 32, 4);
  TEST_VABDL(int, s, 32, 64, 2);
  TEST_VABDL(uint, u, 8, 16, 8);
  TEST_VABDL(uint, u, 16, 32, 4);
  TEST_VABDL(uint, u, 32, 64, 2);

  dump_results_hex (TEST_MSG);
}
