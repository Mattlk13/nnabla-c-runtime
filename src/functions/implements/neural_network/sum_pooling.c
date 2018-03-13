// Copyright (c) 2017 Sony Corporation. All Rights Reserved.
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

#include <nnablart/functions.h>
#include "../../utilities.h"

#include <math.h>

rt_function_error_t allocate_sum_pooling_local_context(rt_function_t *f) {
  sum_pooling_local_context_t *context = (sum_pooling_local_context_t *)(f->local_context);
  pooling_private_t *private = malloc(sizeof(pooling_private_t));
  rt_function_error_t ret = allocate_pooling(f, (pooling_context_t *)context, private);
  ((sum_pooling_local_context_t *)(f->local_context))->private = (void *)private;
  return ret;
}

rt_function_error_t free_sum_pooling_local_context(rt_function_t *f) {
  pooling_private_t *private =
      (pooling_private_t *)(((sum_pooling_local_context_t *)(f->local_context))
                               ->private);
  return free_pooling(private);
}

rt_function_error_t exec_sum_pooling(rt_function_t *f) {
  sum_pooling_local_context_t *context = (sum_pooling_local_context_t *)(f->local_context);
  pooling_private_t *private =
      (pooling_private_t *)(((sum_pooling_local_context_t *)(f->local_context))
                               ->private);
  return exec_pooling(f, (pooling_context_t *)context, private, calc_sum);
}
