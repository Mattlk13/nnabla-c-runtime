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

#include "arithmetic.h"
#include "shape.h"

// Common algorism for arithmetic calculation between vector and scalar value.
void calc_scalar(rt_function_t *f, float value, float(*calc_func)(float, float)) {
  int out_size = calc_shape_size(f->outputs[0]->shape);
  float *input = f->inputs[0]->data;
  float *output = f->outputs[0]->data;
  int i; // Iterator
  for (i = 0; i < out_size; i++) {
    output[i] = calc_func(input[i], value);
  }
}
