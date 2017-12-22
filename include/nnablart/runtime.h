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

#ifndef H_RUNTIME_H_
#define H_RUNTIME_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup Runtime ランタイム
/// @brief @ref NetworkDefinitions
/// を用いて順伝播型ニューラルネットワーク計算を実施します。
///
/// 以下のAPIを提供しています。
/// - @ref rt_initialize_context()
///   - @copydoc rt_initialize_context()
/// - @ref rt_free_context()
///   - @copydoc rt_free_context()
/// - @ref rt_num_of_input()
///   - @copydoc rt_num_of_input()
/// - @ref rt_input_size()
///   - @copydoc rt_input_size()
/// - @ref rt_input_dimension()
///   - @copydoc rt_input_dimension()
/// - @ref rt_input_shape()
///   - @copydoc rt_input_shape()
/// - @ref rt_num_of_output()
///   - @copydoc rt_num_of_output()
/// - @ref rt_output_size()
///   - @copydoc rt_output_size()
/// - @ref rt_output_dimension()
///   - @copydoc rt_output_dimension()
/// - @ref rt_output_shape()
///   - @copydoc rt_output_shape()
/// - @ref rt_forward()
///   - @copydoc rt_forward()
///
/// - 処理の流れ
///   処理の流れのシーケンスを示します。具体的な実装例は@ref simple_runtime.c
///   や@ref reference_runtime.c にあります。
///   - 初期化
/// @li @ref rt_initialize_context() で実行時コンテキストを初期化
/// @li @ref rt_num_of_input(), @ref rt_input_size(), @ref
/// rt_num_of_output(), @ref rt_output_size()
/// により、入出力バッファのサイズを取得してバッファを確保する
/// @msc
///   UserApplication,Runtime;
///   UserApplication=>Runtime [label="rt_initialize_context(&context,
///   network)", URL="@ref rt_initialize_context()"];
///   UserApplication<<Runtime [label="(Context stored into context)" ];
///   UserApplication=>Runtime [label="rt_num_of_input(context)", URL="@ref
///   rt_num_of_input()"];
///   UserApplication<<Runtime [label="num_of_input" ];
///   --- [label="LOOP: index"];
///   UserApplication=>Runtime [label="rt_input_size(context, index)",
///   URL="@ref rt_input_size()"];
///   UserApplication<<Runtime [label="input_size[index]" ];
///   --- [label="LOOP_END: index"];
///   UserApplication=>UserApplication [label="(allocate input buffer)" ];
///   UserApplication=>Runtime [label="rt_num_of_output(context)",
///   URL="@ref rt_num_of_output()"];
///   UserApplication<<Runtime [label="num_of_output" ];
///   --- [label="LOOP: index"];
///   UserApplication=>Runtime [label="rt_output_size(context)", URL="@ref
///   rt_output_size()"];
///   UserApplication<<Runtime [label="output_size" ];
///   --- [label="LOOP_END: index"];
///   UserApplication=>UserApplication [label="(allocate output buffer)" ];
/// @endmsc
///   - 実行
/// @li 初期化時に確保した入力バッファに入力データを格納
/// @li @ref rt_forward() を実行して順伝播ネットワーク計算を実施
/// @li 出力データバッファに格納されている結果を取得
/// @msc
///   UserApplication,Runtime;
///   UserApplication=>UserApplication [label="(Copy input data into input
///   buffer)" ];
///   UserApplication=>Runtime [label="rt_forward(context, input_buffer,
///   output_buffer)", URL="@ref rt_forward()"];
///   Runtime=>Runtime [label="(Execute feedforward neural network)" ];
///   UserApplication<<Runtime [label="(Output stored into output_buffer)" ];
/// @endmsc
/// @{

/// @file
/// @brief @ref Runtime のAPIヘッダファイル

/// @brief Errors in @ref Runtime.
typedef enum {
  RT_ERROR_VERSION_UNMATCH = -899, ///< 899
  RT_ERROR_ALLOCATE_CONTEXT,       ///<
  RT_ERROR_INVALID_BUFFER_INDEX,   ///<
  RT_ERROR_INIT_VARIABLE,          ///<
  RT_ERROR_UNKNOWN_FUNCTION,       ///<
  RT_ERROR_NOERROR = 0             ///< 0
} rt_error_enum_t;

typedef void *rt_context_pointer;

/// @brief @ref nn_network_t をパースして必要なバッファ等を確保した@ref
/// ContextDefinitions を作成する
/// @param[out] context
/// 作成したコンテキストを格納するポインタ。利用が終了したら@ref
/// rt_free_context() で開放する必要があります。
/// @param[in] network ネットワーク
/// @return @ref rt_error_enum_t
rt_error_enum_t rt_initialize_context(rt_context_pointer *context,
                                      nn_network_t *network);

/// @brief コンテキストを開放する
/// コンテキスト内にはレイヤ毎のパラメータやweight,
/// biasなどのバッファがmallocされて保持していますので、この関数を用いて開放する必要があります。
/// @param[in] context 開放したいコンテキスト
/// @return @ref rt_error_enum_t
rt_error_enum_t rt_free_context(rt_context_pointer *context);

/// @brief 入力データの数を返す
/// networkは複数の入力を持つことができますので、入力の数を返します。この関数の返り値を@ref
/// rt_input_size などのindexとして利用します。
/// @param[in] context コンテキスト
/// @return 入力データ数
int rt_num_of_input(rt_context_pointer context);

/// @brief index番目の入力データのサイズを返します。
/// ここで返す数値は全ての次元のデータ数を乗算したものと同じです。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return データサイズ
int rt_input_size(rt_context_pointer context, size_t index);

/// @brief index番目の入力データの次元を返します。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return データサイズ
int rt_input_dimension(rt_context_pointer context, size_t index);

/// @brief index番目, shape_index次元のデータサイズを返します。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @param[in] shape_index 次元を指定
/// @return データサイズ
int rt_input_shape(rt_context_pointer context, size_t index,
                   size_t shape_index);

/// @brief index番目の入力バッファを返します
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return バッファへのポインタ
float *rt_input_buffer(rt_context_pointer context, size_t index);

/// @brief 出力データの数を返す
/// networkは複数の出力を持つことができますので、出力の数を返します。この関数の返り値を@ref
/// rt_input_size などのindexとして利用します。
/// @param[in] context コンテキスト
/// @return 出力データ数
int rt_num_of_output(rt_context_pointer context);

/// @brief index番目の出力データのサイズを返します。
/// ここで返す数値は全ての次元のデータ数を乗算したものと同じです。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return データサイズ
int rt_output_size(rt_context_pointer context, size_t index);

/// @brief index番目の出力データの次元を返します。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return データサイズ
int rt_output_dimension(rt_context_pointer context, size_t index);

/// @brief index番目, shape_index次元のデータサイズを返します。
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @param[in] shape_index 次元を指定
/// @return データサイズ
int rt_output_shape(rt_context_pointer context, size_t index,
                    size_t shape_index);

/// @brief index番目の出力バッファを返します
/// @param[in] context コンテキスト
/// @param[in] index 何番目のデータかを指定する
/// @return バッファへのポインタ
float *rt_output_buffer(rt_context_pointer context, size_t index);

/// @brief フィードフォワード計算を実施する
/// コンテキストに与える入力データを指定してフィードフォワード計算を実施し、結果を出力バッファに格納します。
/// この関数では入出力バッファの確保や開放は行わないので、利用者が確保したバッファを渡し、利用後に開放する責務があります。
/// この関数で実行できるレイヤのリストは別文書にて提供する予定です。
/// @param[in] context コンテキスト
/// @param[in] input @ref rt_num_of_input
/// 個の入力バッファ。それぞれの入力データはフラットなバッファに行列が格納されたものとなります。
/// @param[in] output  @ref rt_num_of_output
/// 個の出力バッファ。それぞれの出力バッファは@ref rt_output_size()
/// の値でmallocされているものとします。
/// @return @ref rt_error_enum_t
rt_error_enum_t rt_forward(rt_context_pointer context, float **input,
                           float **output);

/// @}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // H_RUNTIME_H_
