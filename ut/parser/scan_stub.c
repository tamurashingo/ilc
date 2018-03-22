/**
 * @file	scan_stub.c
 * @brief	scan.cのスタブ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-05-05
 * @version	$Id: scan_stub.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include "scan.h"

/*
 * 外部公開変数の定義
 */
int   yylineno = 0;
char* yytext = NULL;
int   yyleng = 0;


/*
 * スタブ用構造体
 */
struct lex_stub {
	int   yylex;		/**< lexで取得した字句に対応したコード */
	char* yytext;		/**< lexで取得した字句そのもの */
	int   yyleng;		/**< yytextの長さ */
};


/* ドライバが設定した構造体を保持する変数 */
static struct lex_stub* stub;

/**
 * ドライバ側で設定した構造体を保持する
 * 初期化とかも行っている
 * @param struct lex_stub
 */
void setStubData (
	struct lex_stub* src
)
{
	/**/
	/**/
	stub = src;
}


/**
 * yylexのスタブ
 * ドライバで設定した文字列を返していく
 * yylex()が呼ばれたタイミングで yytext、yyleng の設定を行う
 */
int yylex (
)
{
	/**/
	int _yylex;
	/**/
	yytext = stub->yytext;
	yyleng = stub->yyleng;
	_yylex = stub->yylex;

	yylineno++;
	stub++;

	return _yylex;
}

void set_lex_input( FILE* fp ) {}
void set_lex_output( FILE* fp ) {}

