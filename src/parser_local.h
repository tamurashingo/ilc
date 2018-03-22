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
 * @file	parser_local.h
 * @brief	構文解析 内部用の定義
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-11
 * @version	$Id: parser_local.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _PARSER_LOCAL_H_
#define _PARSER_LOCAL_H_

#include "util.h"

/** 構文解析の未実施 (setjmpの戻り値) */
#define EXP_START	(0)
/** 構文解析の異常終了 (setjmpの戻り値) */
#define EXP_FAILURE (1)
/** メモリ不足 (setjmpの戻り値) */
#define EXP_ALLOC	(2)


typedef struct _parse_data {
	char*		file_name;
	char*		func_name;
	int         addflag;	/* ILC_FUNCにfunc_nameを格納したかどうか */
	SLIST*		ilc_func;
	FILE*		fpout;
}
PARSE_DATA;


/**
 * <syntax> ::= LL_EXP_END
 *            | <function_or_variable>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int syntax( int, PARSE_DATA* );

/**
 * <function_or_variable> :: = <ids> LL_EXP_END
 *                           | <ids> <function>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_or_variable( int, PARSE_DATA* );

/**
 * <function> ::= <function_definition> LL_EXP_END
 *              | <function_definition> <functiton_imple>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function( int, PARSE_DATA* );

/**
 * <function_definition> ::= LL_PARENTHIS_L LL_ID* LL_PARENTHIS_R
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_definition( int, PARSE_DATA* );

/**
 * <function_imple> ::= LL_BRACE_L * LL_BRACE_R
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_imple( int, PARSE_DATA* );

/**
 * <ids> ::= LL_ID* <struct>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int ids( int, PARSE_DATA* );

/**
 * <struct_or_union> ::= LL_BRACE_L <struct_or_union>* * LL_BRACE_R
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int struct_or_union( int, PARSE_DATA* );


/**
 * 解析異常時のメッセージを出力し、longjmpでトップレベルへ処理を戻す。
 * @param int 異常を検出した token
 * @param PARSE_DATA*
 */
static void parse_error( int, PARSE_DATA* );

/**
 * LL_ILC_COMMENT を読み飛ばす。
 * @param int チェック対象の token
 * @param PARSE_DATA*
 * @return LL_ILC_COMMENT以外の token
 */
static int skip_ilc_comment( int, PARSE_DATA* );

/**
 * lex から token を取得するラッパー
 * @return lex から取得した token
 */
static int get_token( );

/**
 * 関数名をバッファに設定する
 * @param char* lexが読み込んだ関数名
 * @param int 関数名のサイズ
 * @param PARSE_DATA*
 * @return 0:正常 -1:メモリ不足エラー
 */
static int set_func_name( char*, int, PARSE_DATA* );

#endif /* _PARSER_LOCAL_H_ */


