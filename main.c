#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lang/parser/lexer.h"
#include "lang/ast.h"
#include "lang/compiler.h"
#include "lang/component/component.h"
#include "lang/parser/parser.h"
#include "lang/reader.h"
#include "lang/symbol/symbol.h"
#include "lang/util/hash.h"

int main() { compile("config.uc3"); }
