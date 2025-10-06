#include "include/lang/parser/lexer.h"
#include "lang/parser/parser.h"
#include <stdio.h>
#include <string.h>
#include "lang/ast.h"
#include <stdlib.h>
#include "lang/reader.h"
#include "lang/symbol/symbol.h"
#include "lang/util/hash.h"
#include "lang/component/component.h"
#include "lang/compiler.h"


int main()
{
    compile("config.uc3");
}
