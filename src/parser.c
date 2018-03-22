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
 * @file	parser.c
 * @brief	構文解析
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-10
 * @version	$Id: parser.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

/*-
 * おおざっぱなBNF
 * LL_ILC_COMMENTはどこでも受け取る可能性があるが、
 * <function_imple>以外では処理をしない。
 *
 * <syntax> ::= LL_EXP_END
 *            | <function_or_variable>
 *
 * <function_or_variable> ::= <ids> LL_EXP_END    -> variable
 *                          | <ids> <function>
 *
 * <function> ::= <function_definition> LL_EXP_END
 *              | <function_definition> <function_imple>
 *
 * <function_definition> ::= LL_PARENTHIS_L LL_ID* LL_PARENTHIS_R
 *
 * ** ここで LL_ILC_COMMENT が出た場合に、特殊処理を行う **
 * <function_imple> ::= LL_BRACE_L * LL_BRACE_R
 *
 * ** 最後の LL_ID を関数名とみなす **
 * <ids> ::= LL_ID* <struct_or_union>
 * 
 * <struct_or_union> LL_BRACE_L * LL_BRACE_R
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "ilc_util.h"
#include "scan.h"
#include "util.h"
#include "parser_local.h"

/** RCSID */
static const char rcsid[] = "$Id: parser.c,v 1.2 2008/05/25 13:22:49 shingo Exp $";


/*
 * 外部定義
 */
extern int yylex();
extern int yylineno;
extern char* yytext;
extern int yyleng;


/*
 * 内部定義
 */
static jmp_buf jbuf;


/**
 * 構文解析を実施する
 * @param ILC* ILCデータ
 * @return 0:正常
 *         1:構文エラー
 *         2:メモリエラー
 */
int parse (
	ILC*	ilc
)
{

	/**/
	int token;
	int exception = EXP_START;
	PARSE_DATA pdata;
	int breakflag = 0;		/* 0:処理中 0以外:異常発生 */
	/**/

	/* PARSE_DATA初期化 */
	pdata.file_name = ilc->file_in;
	pdata.func_name = NULL;
	pdata.addflag = 0;
	pdata.ilc_func  = ilc->ilc_func;
	pdata.fpout = ilc->fpout;

	/* parse準備 */
	set_lex_input( ilc->fpin );
	set_lex_output( ilc->fpout );
	yylineno = 1;


	/* ILC: parse開始 */

	while ( breakflag == 0 && (token = get_token()) != 0 ) {
		/* ILC: C++でいう try */
		exception = setjmp( jbuf );
		switch ( exception ) {
		case EXP_START:
			/* ILC: 構文解析の開始 */
			token = syntax( token, &pdata );
			break;
		case EXP_FAILURE:
			/* ILC: 構文解析で異常 */
			breakflag = -1;
			break;
		case EXP_ALLOC:
			/* ILC: メモリ不足 */
			breakflag = -1;
			break;
		default:
			/* ILC: ありえない異常 */
			breakflag = -1;
			break;
		}
		/* 関数名のリセット */
		/* addflagに応じてメモリ解放を行う */
		if ( pdata.addflag == 0 ) {
			/* ILC: ILC_FUNCに登録していないため、メモリを解放する */
			xfree( pdata.func_name );
		}
		pdata.func_name = NULL;
	}

	ilc->ilc_func = pdata.ilc_func;

	/* ILC: parse終了 */
	return exception;
}

/**
 * <syntax> :: = LL_EXP_END
 *             | <function_or_variable>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int syntax (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	/**/
	/* ILC: syntax開始 */

	token = skip_ilc_comment( token, pdata );

	switch ( token ) {
	case 0:
		/* ILC: ここでのEOFは正常 */
		break;
	case LL_EXP_END:
		/* ILC: 空の文 */
		break;
	default:
		/* ILC: */
		token = function_or_variable( token, pdata );
		break;
	}

	/* ILC: syntax終了 */
	return token;
}

/**
 * <function_or_variable> :: = <ids> LL_EXP_END
 *                           | <ids> <function>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_or_variable (
	int token,
	PARSE_DATA *pdata
)
{
	/**/
	/**/

	/* ILC: function_or_variable開始 */

	token = skip_ilc_comment( token, pdata );

	/* idsに処理を移す */
	token = ids( token, pdata );

	token = skip_ilc_comment( token, pdata );

	/* 後続の処理を決定 */
	switch ( token ) {
	case 0:
		/* ILC: EOFは解析エラー */
		parse_error( token, pdata );
		break;
	case LL_EXP_END:
		/* ILC: 変数の定義で終了 */
		break;
	default:
		/* ILC: 関数の定義に遷移 */
		token = function( token, pdata );
		break;
	}

	/* ILC: function_or_variable終了 */
	return token;
}

/**
 * <function> ::= <function_definition> LL_EXP_END
 *              | <function_definition> <functiton_imple>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	/**/

	/* ILC: function開始 */

	token = skip_ilc_comment( token, pdata );

	token = function_definition( token, pdata );

	switch ( token ) {
	case 0:
		/* ILC: EOFは解析エラー */
		parse_error( token, pdata );
		break;
	case LL_EXP_END:
		/* ILC: 関数定義のみで終了 */
		break;
	default:
		/* ILC: 関数の中身を処理 */
		token = function_imple( token, pdata );
		break;
	}

	/* ILC: function終了 */
	return token;
}


/**
 * <function_definition> ::= LL_PARENTHIS_L LL_ID* LL_PARENTHIS_R
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_definition (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	/**/

	/* ILC: function_definition開始 */

	token = skip_ilc_comment( token, pdata );

	switch ( token ) {
	case 0:
		/* ILC: EOFは解析エラー : 現在のパーサではこのエラーは発生しない */
		parse_error( token, pdata );
		break;
	case LL_PARENTHIS_L:
		/* ILC: 左かっこ検出 */
		/* LL_PARENTHIS_R が出現するまで読み込み続ける */
		/* いろいろと面倒なため、LL_IDであるかのチェックは行わない */
		while ( (token = get_token()) != LL_PARENTHIS_R ) {
			/* ILC: 右かっこ検出中 */
			if ( token == 0 ) {
				/* ILC: EOFが検出されたので解析エラー */
				parse_error( token, pdata );
			}
		}
		token = get_token();
		break;
	default:
		/* ILC: 関数定義なのに左かっこがないので解析エラー */
		parse_error( token, pdata );
		break;
	}

	/* ILC: function_definition終了 */
	return token;
}


/**
 * <function_imple> ::= LL_BRACE_L * LL_BRACE_R
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int function_imple (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	/**/

	/* ILC: function_imple開始 */

	token = skip_ilc_comment( token, pdata );

	/* K&Rタイプの宣言に対応するため、 LL_BRACE_L が出るまで読み続ける */
	while ( token != LL_BRACE_L ) {
		if ( token == 0 ) {
			/* ILC: EOFは解析エラー */
			parse_error( token, pdata );
		}
		token = get_token();
	}


	/* 先ほど検出した LL_BRACE_L に対応した LL_BRACE_R を検出するまでループ */
	while ( (token = get_token()) != LL_BRACE_R ) {
		switch ( token ) {
		case 0:
			/* ILC: EOFは解析エラー */
			parse_error( token, pdata );
			break;
		case LL_BRACE_L:
			/* ILC: 左ブレースの検出 */
			token = function_imple( token, pdata );
			break;
		case LL_ILC_COMMENT:
			/* ILC: ILCコメントを検出！ */
			{
				/**/
				int ret;
				/**/
				/* カバレッジ検出ポイントをリストに追加 */
				ret = ilc_append_coverage( &(pdata->ilc_func), pdata->func_name, yylineno );
				if ( ret == 0 ) {
					/* ILC: リストへの追加に成功 */
					pdata->addflag = 1;
				}
				else {
					/* ILC: リストへの追加に失敗 */
					longjmp( jbuf, EXP_ALLOC );
				}
			}

			/* カバレッジ検出ポイントをコードに付与 */
			ilc_put_coverage( pdata->fpout, pdata->file_name, pdata->func_name, yylineno );
			break;
		default:
			/* ILC: その他の token を検出 */
			break;
		}
	}

	/* ILC: function_imple終了 */
	return token;
}


/**
 * <ids> ::= LL_ID* <struct>
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int ids (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	int ret = 0;
	/**/

	/* ILC: ids開始 */

	while ( token == LL_ID || token == LL_ILC_COMMENT ) {
		/* ILC: IDを検出中 */
		if ( token == LL_ID ) {
			/* ILC: 関数名の定義 */
			ret = set_func_name( yytext, yyleng, pdata );
			if ( ret != 0 ) {
				/* ILC: メモリ不足 */
				longjmp( jbuf, EXP_ALLOC );
			}
		}

		token = get_token();
	}

	token = struct_or_union( token, pdata );

	/* ILC: ids終了 */
	return token;
}


/**
 * <struct_or_union> ::= LL_BRACE_L <struct_or_union>* * ( | LL_BRACE_R LL_ID)
 * @param int 上位から渡された token
 * @param PARSE_DATA*
 * @return パース終了後に読み込んだ token
 */
static int struct_or_union (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	int stack = 0;
	/**/

	/* ILC: struct_or_union開始 */

	token = skip_ilc_comment( token, pdata );

	/* { が出たら struct か union の定義とみなす */
	if ( token == LL_BRACE_L ) {
		/* ILC: LL_BRACEの検出 */
		stack++;
		while ( stack != 0 ) {
			/* ILC: スタックが空になるまでループです */

			token = get_token();
			switch ( token ) {
			case LL_BRACE_L:
				/* ILC: LL_BRACE_Lを検出したのでスタックに積みます */
				stack++;
				break;
			case LL_BRACE_R:
				/* ILC: LL_BRACE_Rを検出したのでスタックから下ろします */
				stack--;
				break;
			case 0:
				/* ILC: EOFは解析エラー */
				parse_error( token, pdata );
				break;
			}
		}
		/* ILC: LL_IDを検出したら、飲み込む。それ以外の字句であれば上位へ返す */
		/* typedef struct _st { ... } ST; という宣言を想定しての動作*/

		token = get_token();
		token = skip_ilc_comment( token, pdata );
		if ( token == LL_ID ) {
			/* ILC: LL_IDだったので次の字句を上位へ返す */
			token = get_token();
		}
	}


	/* ILC: struct_or_union終了 */
	return token;
}


/**
 * 解析異常時のメッセージを出力し、longjmpでトップレベルへ処理を戻す。
 * @param int 異常を検出した token
 * @param PARSE_DATA*
 */
static void parse_error (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	struct tokens {
		int token;
		char* type;
		char* text;
	}
	tokens[] = {
		{ LL_ID,			"関数名 or 変数名",		yytext },
		{ LL_BRACE_L,		"カッコ",				yytext },
		{ LL_BRACE_R,		"カッコ閉じ",			yytext },
		{ LL_PARENTHIS_L,	"カッコ", 				yytext },
		{ LL_PARENTHIS_R,	"カッコ閉じ", 			yytext },
		{ LL_EXP_END,		"セミコロン",			yytext },
		{ LL_ILC_COMMENT,	"コメント中のILCタグ",	yytext },
		{ 0,				"EOF",					"NULL" },
		{ -1,				NULL,					NULL   }
	};
	struct tokens *ptoken;
	/**/

	/* ILC: parse_error開始 */

	for ( ptoken = tokens; ptoken->type != NULL; ptoken++ ) {
		if ( ptoken->token == token ) {
			break;
		}
	}

	fprintf( stderr, "予期しないトークンが出現しました。(%s:%s) in %s(%d)\n"
			 , ptoken->type, ptoken->text, pdata->file_name, yylineno );

	/* ILC: parse_error終了 */
	longjmp( jbuf, EXP_FAILURE );
}

/**
 * LL_ILC_COMMENT を読み飛ばす。
 * @param int チェック対象の token
 * @param PARSE_DATA*
 * @return LL_ILC_COMMENT以外の token
 */
static int skip_ilc_comment (
	int token,
	PARSE_DATA* pdata
)
{
	/**/
	/**/

	/* ILC: skip_ilc_comment開始 */

	while ( token == LL_ILC_COMMENT ) {
		/* ILC: コメント読み飛ばし中 */
		token = get_token();
	}

	/* ILC: skip_ilc_comment終了 */
	return token;
}


/**
 * lex から token を取得するラッパー
 * @return lex から取得した token
 */
static int get_token (
)
{
	/**/
	int retcode;
	/**/

	/* ILC: get_token開始 */

	retcode = yylex();

	/* ILC: get_token終了 */
	return retcode;
}

/**
 * 関数名をバッファに設定する
 * @param char* lexが読み込んだ関数名
 * @param int 関数名のサイズ
 * @param PARSE_DATA*
 * @return 0:正常 -1:メモリ不足エラー
 */
static int set_func_name (
	char*			func_name,
	int				length,
	PARSE_DATA*		pdata
)
{
	/**/
	int ret;
	/**/

	/* ILC: set_func_name開始 */

	if ( pdata->func_name != NULL ) {
		/* ILC: 登録済みの関数名があれば解放する */
		xfree( pdata->func_name );
	}

	pdata->func_name = (char *)xmalloc( (size_t)length + 1 );

	if ( pdata->func_name != NULL ) {
		/* ILC: 関数名を新たに登録する */
		memset( pdata->func_name, 0, (size_t)length + 1 );
		strncpy( pdata->func_name, func_name, (size_t)length );
		ret = 0;
	}
	else {
		/* ILC: メモリ不足エラー */
		ret = -1;
	}

	/* ILC: set_func_name終了 */
	return ret;
}
