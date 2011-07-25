/******************************************************************************
 * Copyright [2010] [Kadir PEKEL]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>

#include "mAssert.h"
#include "mToken.h"
#include "mLexer.h"

// Lexical Analyzer

void initLexState(LexState * lexState, FILE * in) {
  lexState->stream = in;
  lexState->previousChar = '\0';
  lexState->currentChar = '\0';
  lexState->bufferLen = 0;
  lexState->bufferCursor = -1;
  lexState->readFirst = 0;
}

static void next(LexState * state) {
  state->previousChar = state->currentChar;
  state->currentChar = fgetc(state->stream);
}

static void accept(LexState * state) {
  state->buffer[state->bufferLen++] = state->currentChar;
  next(state);
}

static void yield(LexState * state) {
  state->buffer[state->bufferLen] = '\0';
  state->bufferLen = 0;
}

int lex(LexState * state) {
  int tokenType = T_NOMATCH;
  // Read first byte
  if(!state->readFirst) {
    state->currentChar = fgetc(state->stream);
    state->readFirst = 1;
  }
  // Match eof
  if (state->currentChar == EOF) {
    strcpy(state->buffer, "<eof>");
    tokenType = T_EOF;
  }
  // Match non-terminal
  if (tokenType == T_NOMATCH) {
    if (isalpha(state->currentChar)) {
      accept(state);
      while (isalpha(state->currentChar)
             || isdigit(state->currentChar) || state->currentChar == '_') {
        accept(state);
      };
      tokenType = T_ID;
    } else if (isdigit(state->currentChar)) {
      accept(state);
      while (isdigit(state->currentChar) || state->currentChar == '.') {
        accept(state);
      };
      tokenType = T_NUMBER;
    } else if (isspace(state->currentChar)) {
      while (isspace(state->currentChar)) {
        accept(state);
      }
      tokenType = T_SPACE;
    } else if (state->currentChar == '"') {
      next(state);
      while (state->currentChar != '"') {
        accept(state);
      }
      if (state->currentChar == '"') {
        next(state);
        tokenType = T_STRING;
      } 
    } else if (state->currentChar == '(') {
      accept(state);
      tokenType = T_LPAREN;
    } else if (state->currentChar == ')') {
      accept(state);
      tokenType = T_RPAREN;
    } else if (state->currentChar == '{') {
      accept(state);
      tokenType = T_LCBRACE;
    } else if (state->currentChar == '}') {
      accept(state);
      tokenType = T_RCBRACE;
    } else if (state->currentChar == ';') {
      accept(state);
      tokenType = T_SEMICOLON;
    } else if (state->currentChar == '|') {
      accept(state);
      tokenType = T_PIPE;
    } else if (state->currentChar == ',') {
      accept(state);
      tokenType = T_COMMA;
    } else if (state->currentChar == ':') {
      accept(state);
      tokenType = T_COLON;
    } else if (state->currentChar == '#') {
      accept(state);
      while (state->currentChar != '\n') {
        accept(state);
      }
      tokenType = T_COMMENT;
    }
  }
  if (tokenType != T_NOMATCH) {
    yield(state);
  } else {
    strcpy(state->buffer, "<nomatch>");
  }
  return tokenType;
}
