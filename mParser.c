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
  while(state->currentToken == T_SPACE) {
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
int term(ParserState* state);
int factor(ParserState* state);

int expression (ParserState* state) {
    if (term(state)) {
        while((accept(state, T_PLUS) || accept(state, T_MINUS)) && term(state));
        return 1;
    }
    return 0;
}

int term (ParserState* state) {
    if (factor(state)) {
        while((accept(state, T_MULTIPLY) || accept(state, T_DIVIDE)) && factor(state));
        return 1;
    }
    return 0;
}

int factor (ParserState* state) {
    return accept(state, T_NUMBER) || (accept(state, T_LPAREN) && expression(state) && accept(state, T_RPAREN));
}

int program(ParserState* state) {
  while(expression(state) && accept(state, T_NEWLINE));
  return 1;
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
