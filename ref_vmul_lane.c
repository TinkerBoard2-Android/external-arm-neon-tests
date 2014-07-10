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

#define TEST_MSG "VMUL_LANE"
void exec_vmul_lane (void)
{
#define DECL_VMUL(VAR)				\
  DECL_VARIABLE(VAR, int, 16, 4);		\
  DECL_VARIABLE(VAR, int, 32, 2);		\
  DECL_VARIABLE(VAR, uint, 16, 4);		\
  DECL_VARIABLE(VAR, uint, 32, 2);		\
  DECL_VARIABLE(VAR, float, 32, 2);		\
  DECL_VARIABLE(VAR, int, 16, 8);		\
  DECL_VARIABLE(VAR, int, 32, 4);		\
  DECL_VARIABLE(VAR, uint, 16, 8);		\
  DECL_VARIABLE(VAR, uint, 32, 4);		\
  DECL_VARIABLE(VAR, float, 32, 4)

  /* vector_res = vmul_lane(vector,vector2,lane), then store the result.  */
#define TEST_VMUL_LANE(Q, T1, T2, W, N, N2, L)				\
  VECT_VAR(vector_res, T1, W, N) =					\
    vmul##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N),			\
			   VECT_VAR(vector2, T1, W, N2),		\
			   L);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VMUL(vector);
  DECL_VMUL(vector_res);

  DECL_VARIABLE(vector2, int, 16, 4);
  DECL_VARIABLE(vector2, int, 32, 2);
  DECL_VARIABLE(vector2, uint, 16, 4);
  DECL_VARIABLE(vector2, uint, 32, 2);
  DECL_VARIABLE(vector2, float, 32, 2);

  clean_results ();

  /* Initialize vector from pre-initialized values  */
  TEST_VLOAD(vector, buffer, , int, s, 16, 4);
  TEST_VLOAD(vector, buffer, , int, s, 32, 2);
  TEST_VLOAD(vector, buffer, , uint, u, 16, 4);
  TEST_VLOAD(vector, buffer, , uint, u, 32, 2);
  TEST_VLOAD(vector, buffer, , float, f, 32, 2);
  TEST_VLOAD(vector, buffer, q, int, s, 16, 8);
  TEST_VLOAD(vector, buffer, q, int, s, 32, 4);
  TEST_VLOAD(vector, buffer, q, uint, u, 16, 8);
  TEST_VLOAD(vector, buffer, q, uint, u, 32, 4);
  TEST_VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Initialize vector2 */
  TEST_VDUP(vector2, , int, s, 16, 4, 0x4);
  TEST_VDUP(vector2, , int, s, 32, 2, 0x22);
  TEST_VDUP(vector2, , uint, u, 16, 4, 0x444);
  TEST_VDUP(vector2, , uint, u, 32, 2, 0x532);
  TEST_VDUP(vector2, , float, f, 32, 2, 22.8f);

  /* Choose lane arbitrarily */
  TEST_VMUL_LANE(, int, s, 16, 4, 4, 2);
  TEST_VMUL_LANE(, int, s, 32, 2, 2, 1);
  TEST_VMUL_LANE(, uint, u, 16, 4, 4, 2);
  TEST_VMUL_LANE(, uint, u, 32, 2, 2, 1);
  TEST_VMUL_LANE(, float, f, 32, 2, 2, 1);
  TEST_VMUL_LANE(q, int, s, 16, 8, 4, 2);
  TEST_VMUL_LANE(q, int, s, 32, 4, 2, 0);
  TEST_VMUL_LANE(q, uint, u, 16, 8, 4, 2);
  TEST_VMUL_LANE(q, uint, u, 32, 4, 2, 1);
  TEST_VMUL_LANE(q, float, f, 32, 4, 2, 0);

  /* FIXME: only a subset of the result buffers are used, but we
     output all of them */
  dump_results_hex (TEST_MSG);
}
