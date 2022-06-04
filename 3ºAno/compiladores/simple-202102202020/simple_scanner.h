#ifndef __SIMPLESCANNER_H__
#define __SIMPLESCANNER_H__

#undef yyFlexLexer
#define yyFlexLexer simple_scanner_FlexLexer
#include <FlexLexer.h>

typedef simple_scanner_FlexLexer simple_scanner;

#endif
