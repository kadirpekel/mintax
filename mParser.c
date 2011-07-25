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

#include "mAssert.h"
#include "mToken.h"
#include "mLexer.h"
#include "mParser.h"

static void next(ParserState* state) {
  state->currentToken = lex(state->lexState);
  while(state->currentToken == T_COMMENT
    || state->currentToken == T_SPACE) {
    state->currentToken = lex(state->lexState);
  }
}

static int accept(ParserState* state, int token) {
  if (state->currentToken == token) {
    log("%s", state->lexState->buffer);
    next(state);
    return 1;
  }
  return 0;
}

int expression(ParserState* state);
int lambda(ParserState* state);
int program(ParserState* state);
int statement(ParserState* state);

int expression(ParserState* state) {
  if (accept(state, T_ID)) {
    if (accept(state, T_LPAREN)) {
      while(expression(state) && accept(state, T_COMMA));
      if (!accept(state, T_RPAREN)) {
        return 0;
      }
    }
    return 1;
  } else if(lambda(state) || accept(state, T_NUMBER) || accept(state, T_STRING)) {
    return 1;
  }
  return 0;
}

int lambda(ParserState* state) {
  if(accept(state, T_PIPE)) {
    while(accept(state, T_ID) && accept(state, T_COMMA));
    if (!accept(state, T_PIPE)) {
      return 0;
    }
  }
  if(accept(state, T_LCBRACE)) {
    program(state);
    if(!accept(state, T_RCBRACE)) {
      return 0;
    }
    return 1;
  }
  return 0;
}

int program(ParserState* state) {
  while((statement(state) || expression(state)) && accept(state, T_SEMICOLON));
  return 1;
}

int statement(ParserState* state) {
  if (accept(state, T_COLON) && accept(state, T_ID)) {
    if (!expression(state)) {
      return 0;
    }
    log("statement");
    return 1;
  }
  return 0;
}

void initParserState(ParserState * parserState, LexState * lexState) {
  parserState->lexState = lexState;
  parserState->currentToken = T_NOMATCH;
  parserState->level = 0;
}

void parse(ParserState * state) {
  if(!state->readFirst) {
    next(state);
    state->readFirst = 1;
  }
  program(state);
}
