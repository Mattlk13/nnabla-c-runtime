// Copyright (c) 2018 Sony Corporation. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../../../utilities.h"
#include <assert.h>
#include <math.h>
#include <nnablart/functions.h>
#include "convolution_internal.h"

#define X             (0)            //x input
#define WEIGHT        (1)            //weight
#define BIAS          (2)            //bias
#define Y0            (0)            //y0 output
#define ALPHA         (-1)           //ALPHA

// Convolution
rt_function_error_t allocate_convolution_local_context(rt_function_t *f) {
  return allocate_convolution_local_context_common(f, X, WEIGHT, BIAS, ALPHA, Y0);
}

rt_function_error_t free_convolution_local_context(rt_function_t *f) {
  return free_convolution_local_context_common(f);
}

rt_function_error_t exec_convolution(rt_function_t *f) {
  return ((convolution_private_t *)(((convolution_local_context_t*)(f->local_context))
                                   ->private))
      ->exec(f);
}
