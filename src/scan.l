%{
/*-
 * The MIT License (MIT)
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file	scan.l
 * @brief	字句解析
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-06
 * @version	$Id: scan.l,v 1.1 2008/05/25 13:14:47 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include "scan.h"

/** RCSID */
static const char rcsid[] = "$Id: scan.l,v 1.1 2008/05/25 13:14:47 shingo Exp $";

static FILE *fout;

static void init_streams(void) __attribute__((constructor));
static void init_streams(void) {
	fout = stdout;
}


void yyerror( char *message ) {
	printf("error(%d): %s\n", yylineno, message);
}


%}

ID	[[:alpha:]_][[:alnum:]_]*
%x	STRING
%x	COMMENT

%option yylineno

%%

	/* 英数字は関数名とみなす */
{ID}	{
			fprintf(fout, "%s", yytext);
			return LL_ID;
		}

"{"		{
			fprintf(fout, "%s", yytext);
			return LL_BRACE_L;
		}
"}"		{
			fprintf(fout, "%s", yytext);
			return LL_BRACE_R;
		}
";"		{
			fprintf(fout, "%s", yytext);
			return LL_EXP_END;
		}
"("		{
			fprintf(fout, "%s", yytext);
			return LL_PARENTHIS_L;
		}
")"		{
			fprintf(fout, "%s", yytext);
			return LL_PARENTHIS_R;
		}

	/* 文字列の処理 */
"\""	{
			fprintf(fout, "%s", yytext);
	BEGIN(STRING);
		}
<STRING><<EOF>>		{
	yyerror("EOF in string");
	}
<STRING>\"		{
			fprintf(fout, "%s", yytext);
	BEGIN(INITIAL);
				}

"/*"			{
			fprintf(fout, "%s", yytext);
	BEGIN(COMMENT);
				}
<COMMENT>"ILC:"	{
			fprintf(fout, "%s", yytext);
	return LL_ILC_COMMENT;
				}
<COMMENT><<EOF>>	{
	yyerror("EOF in comment");
					}
<COMMENT>"*/"	{
			fprintf(fout, "%s", yytext);
	BEGIN(INITIAL);
				}


	/* 行コメントは置換対象外 */
"//.*$"		{
	fprintf(fout, "%s", yytext);
}


	/* プリプロセッサ用 */
^#.*^n			{
	fprintf(fout, "%s", yytext);
}

	/* 改行文字 */
\n				{
	fprintf(fout, "%s", yytext);
}

				
	/* その他のすべての文字 */
.					{
	fprintf(fout, "%s", yytext);
}

%%

void set_lex_input (
	FILE *fp
)
{
	/**/
	/**/

	yyin = fp;
}

void set_lex_output (
	FILE *fp
)
{
	/**/
	/**/

	fout = fp;
	yyout = fp;
}



