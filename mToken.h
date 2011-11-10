/******************************************************************************
 * Copyright [2010] [Kadir PEKEL]
 *
 * Licensed under the Apache License, Version 2.0 (the "License"},;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef MTOKEN_H_
#define MTOKEN_H_

#include <stdio.h>

enum {
  // Meta Tokens
  T_NOMATCH,
  T_EOF,
  // Non-Terminal Tokens
  T_NUMBER,
  T_SPACE,
  T_NEWLINE,
  // Terminal Tokens
  T_LPAREN,
  T_RPAREN,
  T_PLUS,
  T_MINUS,
  T_MULTIPLY,
  T_DIVIDE
};

#endif
