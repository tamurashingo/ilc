/**
 * @file	test_parser.c
 * @brief	parser.cのユニットテスト
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-05-04
 * @version	$Id: test_parser.c,v 1.3 2008/05/31 06:15:21 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <string.h>
#include "ilc_util.h"
#include "scan.h"
#include "parser.h"
#include "ILUT.h"
#include "ilc.h"

/* 外部関数の宣言 */
/* util_stub.c */
int  getRemoveCount();
void setRemoveCount();
void setCreateCount();
/* scan_stub.c */
void setStubData();

/*
 * スタブ用構造体
 */
struct lex_stub {
	int   yylex;		/**< lexで取得した字句に対応したコード */
	char* yytext;		/**< lexで取得した字句そのもの */
	int   yyleng;		/**< yytextの長さ */
};


/**
 * 正常系動作確認
 * いきなりEOFを検出
 * [syntax] -> EOF
 */
ILUT_Test test_parse_001 (
)
{
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ 0, NULL, 0 }
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_001.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setCreateCount( -1 );		/* xmallocの制限無し */
	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること",        ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 空文
 * [syntax] -> LL_EXP_END
 */
ILUT_Test test_parse_002 (
)
{
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_EXP_END, ";",  1 },	/* ; (LL_EXP_END) */
		{ 0,          NULL, 0 }		/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_002.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること",        ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 変数定義
 * [syntax] -> [function_or_variable] -> [ids] -> LL_ID x2 -> LL_EXP_END
 */
ILUT_Test test_parse_003 (
)
{
	/*-
	 * int i;
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,      "int", 3 },	/* int (LL_ID) */
		{ LL_ID,      "i",   1 },	/* i   (LL_ID) */
		{ LL_EXP_END, ";",   1 },	/* ;   (LL_EXP_END) */
		{ 0,          NULL,  0 }	/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_003.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること",         ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと",  ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 2 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 関数の宣言
 * [syntax] -> [function_or_variable]-> [ids] -> [function] -> [function_definition] -> LL_EXP_END
 */
ILUT_Test test_parse_004 (
)
{
	/*-
	 * int func004 ( void * );
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",     3 },	/* int     (LL_ID) */
		{ LL_ID,          "func004", 7 },	/* func004 (LL_ID) */
		{ LL_PARENTHIS_L, "(",       1 },	/* (       (LL_PARENTHIS_L) */
		{ LL_ID,          "void",    4 },	/* void    (LL_ID) */
		{ LL_ID,          "*",       1 },	/* *       (LL_ID) */
		{ LL_PARENTHIS_R, ")",       1 },	/* )       (LL_PARENTHIS_R) */
		{ LL_EXP_END,     ";",       1 },	/* ;       (LL_EXP_END) */
		{ 0,              NULL,      0 }	/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_004.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 2 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 構造体を返す関数の宣言
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> LL_EXP_END
 */
ILUT_Test test_parse_005 (
)
{

	/*-
	 * struct st1 func005 ( union un2 arg );
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "struct",  6 },	/* struct  (LL_ID) */
		{ LL_ID,          "st1",     3 },	/* st1     (LL_ID) */
		{ LL_ID,          "func005", 7 },	/* func005 (LL_ID) */
		{ LL_PARENTHIS_L, "(",       1 },	/* (       (LL_PARENTHIS_L) */
		{ LL_ID,          "union",   5 },	/* union   (LL_ID) */
		{ LL_ID,          "un2",     3 },	/* un2     (LL_ID) */
		{ LL_ID,          "arg",     3 },	/* arg     (LL_ID) */
		{ LL_PARENTHIS_R, ")",       1 },	/* )       (LL_PARENTHIS_R) */
		{ LL_EXP_END,     ";",       1 },	/* ;       (LL_EXP_END) */
		{ 0,              NULL,      0 }	/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_005.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 3 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 型定義
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union] -> LL_EXP_END
 */
ILUT_Test test_parse_006 (
)
{
	/*-
	 * typedef struct _point2d {
	 *     int ix;
	 *     int iy;
	 *     struct {
	 *         int r;
	 *         int g;
	 *         int b;
	 *     } col;
	 * }
	 * POINT2D;
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,      "typedef",  7 },	/* typedef  (LL_ID) */
		{ LL_ID,      "struct",   6 },	/* struct   (LL_ID) */
		{ LL_ID,      "_point2d", 8 },	/* _point2d (LL_ID) */
		{ LL_BRACE_L, "{",        1 },	/* {        (LL_BRACE_L) */
		{ LL_ID,      "int",      3 },	/* int      (LL_ID) */
		{ LL_ID,      "ix",       2 },	/* ix       (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_ID,      "int",      3 },	/* int      (LL_ID) */
		{ LL_ID,      "iy",       2 },	/* iy       (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_ID,      "struct",   6 },	/* struct   (LL_ID) */
		{ LL_BRACE_L, "{",        1 },	/* {        (LL_BRACE_L) */
		{ LL_ID,      "int",      3 },	/* int      (LL_ID) */
		{ LL_ID,      "r",        1 },	/* r        (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_ID,      "int",      3 },	/* int      (LL_ID) */
		{ LL_ID,      "g",        1 },	/* g        (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_ID,      "int",      3 },	/* int      (LL_ID) */
		{ LL_ID,      "b",        1 },	/* b        (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_BRACE_R, "}",        1 },	/* }        (LL_BRACE_R) */
		{ LL_ID,      "col",      3 },	/* col      (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ LL_BRACE_R, "}",        1 },	/* }        (LL_BRACE_R) */
		{ LL_ID,      "POINT2D",  7 },	/* POINT2D  (LL_ID) */
		{ LL_EXP_END, ";",        1 },	/* ;        (LL_EXP_END) */
		{ 0,          NULL,       0 }	/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_006.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 3 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 関数定義(ILCタグ無し)
 * [syntax] -> [function_or_variable] -> [ids] -> [function]
 * -> [function_definition] -> [function_imple]
 * -> LL_BRACE_L * LL_BRACE_R
 */
ILUT_Test test_parse_007 (
)
{
	/*-
	 * int add (
	 *     int x,
	 *     int y
	 * )
	 * {
	 *     return x + y;
	 * }
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "add",    3 },	/* add    (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */  /* `+' は字句として認識しない */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ 0,              NULL,     0 } 	/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_007.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 2 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 関数定義(ILCタグ有り)
 * [syntax] -> [function_or_variable] -> [ids] -> [function]
 * -> [function_definition] -> [function_imple]
 * -> LL_BRACE_L LL_ILC_COMMENT * LL_BRACE_R
 */
ILUT_Test test_parse_008 (
)
{
	/*-
	 * int minus (
	 *     int x,
	 *     int y
	 * )
	 * {
	 *     / *  ILC: minus開始  * /    <- 本来はコメント
	 *     return x - y;
	 * }
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "minus",  5 },	/* minus  (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */  /* `+' は字句として認識しない */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ 0,              NULL,     0 } 	/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	SLIST* ilcfunc;
	SLIST* ilccomm;
	/**/

	fout = fopen( "test.dat", "w" );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	ilc.file_in  = "test_parse_008.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = fout;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ilcfunc = ilc.ilc_func;

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていること(アドレス)", ilcfunc != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(関数名)", 
				 strcmp( ((ILC_FUNC_BODY*)(ilcfunc->body))->func_name, "minus" ) == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント数)",
				 ((ILC_FUNC_BODY*)(ilcfunc->body))->count == 1 );

	ilccomm = ((ILC_FUNC_BODY*)(ilcfunc->body))->ilc_comment;

	ILUT_ASSERT( "ilc_funcが登録されていること(コメント)", ilccomm != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント情報)",
				 ((ILC_COMMENT_BODY*)(ilccomm->body))->line == 11 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 26 + test_parse_008.c + minus + 11 => 49 */
	fread( buf, sizeof( char ), 49, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認", strcmp( "*/ __ilc_check( \"test_parse_008.c:minus:11\" ); /*", buf ) == 0 );


	/* 後始末 */
	ilccomment_remove( &ilccomm );
	ilcfunc_remove( &ilcfunc );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * 関数定義２連続(ILCタグ有り)
 * test_parse_008と同様
 */
ILUT_Test test_parse_009 (
)
{
	/*-
	 * int add2( int x ) {
	 *     / *  ILC: add2開始  * /
	 *     / *  ILC: add2終了  * /
	 *    return x + 2;
	 * }
	 * int add3( int x ) {
	 *    / *  ILC: add3開始  * /
	 *    return x + 3;
	 * }
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "add2",   4 },	/* add2   (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */  /* `2' は字句として認識しない */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "add3",   4 },	/* add3   (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */  /* `3' は字句として認識しない */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ 0,              NULL,     0 } 	/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf1[BUFSIZ + 1];
	char buf2[BUFSIZ + 1];
	char buf3[BUFSIZ + 1];
	SLIST* ilcfunc;
	SLIST* ilccomm;
	/**/

	fout = fopen( "test.dat", "w" );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	ilc.file_in  = "test_parse_009.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = fout;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ilcfunc = ilc.ilc_func;

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていること(アドレス) 1/2", ilcfunc != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(関数名) 1/2",
				 strcmp( ((ILC_FUNC_BODY*)(ilcfunc->body))->func_name, "add2" ) == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント数) 1/2",
				 ((ILC_FUNC_BODY*)(ilcfunc->body))->count == 2 );

	ilccomm = ((ILC_FUNC_BODY*)(ilcfunc->body))->ilc_comment;

	ILUT_ASSERT( "ilc_funcが登録されていること(コメント) 1/3", ilccomm != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント情報) 1/3",
				 ((ILC_COMMENT_BODY*)(ilccomm->body))->line == 9 );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント) 2/3", ilccomm->next != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント情報) 2/3",
				 ((ILC_COMMENT_BODY*)(ilccomm->next->body))->line == 10 );

	ILUT_ASSERT( "ilc_funcが登録されていること(アドレス) 2/2", ilcfunc->next != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(関数名) 2/2",
				 strcmp( ((ILC_FUNC_BODY*)(ilcfunc->next->body))->func_name, "add3" ) == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント数) 2/2",
				 ((ILC_FUNC_BODY*)(ilcfunc->next->body))->count == 1 );

	ilccomm = ((ILC_FUNC_BODY*)(ilcfunc->next->body))->ilc_comment;

	ILUT_ASSERT( "ilc_funcが登録されていること(コメント) 3/3", ilccomm != NULL );
	ILUT_ASSERT( "ilc_funcが登録されていること(コメント情報) 3/3",
				 ((ILC_COMMENT_BODY*)(ilccomm->body))->line == 22 );

	memset( buf1, '\0', sizeof( buf1 ) );
	memset( buf2, '\0', sizeof( buf2 ) );
	memset( buf3, '\0', sizeof( buf3 ) );
	fin = fopen( "test.dat", "r" );

	/* 想定する文字数 26 + test_parse_009.c + add2 + 9 => 47 */
	fread( buf1, sizeof( char ), 47, fin );
	/* 想定する文字数 26 + test_parse_009.c + add2 + 10 => 48 */
	fread( buf2, sizeof( char ), 48, fin );
	/* 想定する文字数 26 + test_parse_009.c + add3 + 22 => 48 */
	fread( buf3, sizeof( char ), 48, fin );

	fclose( fin );

	ILUT_ASSERT( "文字列の確認 1/3",
				 strcmp( "*/ __ilc_check( \"test_parse_009.c:add2:9\" ); /*", buf1 ) == 0 );
	ILUT_ASSERT( "文字列の確認 2/3",
				 strcmp( "*/ __ilc_check( \"test_parse_009.c:add2:10\" ); /*", buf2 ) == 0 );
	ILUT_ASSERT( "文字列の確認 3/3",
				 strcmp( "*/ __ilc_check( \"test_parse_009.c:add3:22\" ); /*", buf3 ) == 0 );


	/* 後始末 */
	ilccomment_remove( &(((ILC_FUNC_BODY*)(ilc.ilc_func->body))->ilc_comment) );
	ilccomment_remove( &(((ILC_FUNC_BODY*)(ilc.ilc_func->body))->ilc_comment) );
	ilcfunc_remove( &ilc.ilc_func );
	ilccomment_remove( &(((ILC_FUNC_BODY*)(ilc.ilc_func->body))->ilc_comment) );
	ilcfunc_remove( &ilc.ilc_func );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * function_or_variableにてEOF検出エラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_ornion] -> EOF
 */
ILUT_Test test_parse_010 (
)
{
	/*-
	 * int<EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID, "int", 3 },		/* int (LL_ID) */
		{ 0,     NULL,  0 }			/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_010.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 1 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_010.c(3)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );

	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_010.c(3)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * functionにてEOF検出エラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> EOF
 */
ILUT_Test test_parse_011 (
)
{
	/*-
	 * int func ( )<EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",  3 },		/* int (LL_ID) */
		{ LL_ID,          "func", 4 },		/* func (LL_ID) */
		{ LL_PARENTHIS_L, "(",    1 },		/* ( (LL_PARENTHIS_L) */
		{ LL_PARENTHIS_R, ")",    1 },		/* ) (LL_PARENTHIS_R) */
		{ 0,              NULL,   0 }		/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_011.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_011.c(6)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_011.c(6)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * function_definitionで解析エラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> <LL_PARENTHIS_R>
 */
ILUT_Test test_parse_012 (
)
{
	/*-
	 * int func );
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",  3 },		/* int  (LL_ID) */
		{ LL_ID,          "func", 4 },		/* func (LL_ID) */
		{ LL_PARENTHIS_R, ")",    1 },		/* )    (LL_PARENTHIS_R) */
		{ LL_EXP_END,     ";",    1 },		/* ;    (LL_EXP_END) */
		{ 0,              NULL,   0 }		/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_012.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(カッコ閉じ:)) in test_parse_012.c(4)^n => 72 */
	fread( buf, sizeof( char ), 72, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(カッコ閉じ:)) in test_parse_012.c(4)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * function_impleでEOFエラー (LL_BRACE_L検出時)
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> [function_imple] -> EOF
 */
ILUT_Test test_parse_013 (
)
{
	/*-
	 * K&R型で定義しようとしてEOF
	 * int func()
	 *     int x <EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",  3 },		/* int (LL_ID) */
		{ LL_ID,          "func", 4 },		/* func (LL_ID) */
		{ LL_PARENTHIS_L, "(",    1 },		/* ( (LL_PARENTHIS_L) */
		{ LL_PARENTHIS_R, ")",    1 },		/* ) (LL_PARENTHIS_R) */
		{ LL_ID,          "int",  3 },		/* int (LL_ID) */
		{ LL_ID,          "x",    1 },		/* x (LL_ID) */
		{ 0,              NULL,   0 }		/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_013.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_013.c(8)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_013.c(8)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * function_impleでEOFエラー (LL_BRACE_R検出時)
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> [function_imple] -> EOF
 */
ILUT_Test test_parse_014 (
)
{
	/*-
	 * int func() { <EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",  3 },		/* int (LL_ID) */
		{ LL_ID,          "func", 4 },		/* func (LL_ID) */
		{ LL_PARENTHIS_L, "(",    1 },		/* ( (LL_PARENTHIS_L) */
		{ LL_PARENTHIS_R, ")",    1 },		/* ) (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",    1 },		/* { (LL_BRACE_L) */
		{ 0,              NULL,   0 }		/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_014.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_014.c(7)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_014.c(7)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 異常系
 * function_impleでメモリ確保エラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> [function_imple] -> mallocエラー
 */
ILUT_Test test_parse_015 (
)
{
	/*-
	 * void func() {
	 *     / *  ILC: func開始  * /
	 *     printf( "hello world\n" );
	 * }
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "void",   4 },	/* void   (LL_ID) */
		{ LL_ID,          "func",   4 },	/* func   (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "printf", 6 },	/* printf (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */	/* 文字列は字句として認識しない */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ 0,              NULL,     0 }		/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_015.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	setCreateCount( 2 );		/* void/funcを読み込んだ2回しかxmallocできない。
								   3回目はilc_append_coverage呼び出し時 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが異常終了すること", ret == 2 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );


	return ILUT_SUCCESS;
}


/**
 * 異常系
 * idsでメモリ確保エラー
 * [syntax] -> [function_or_variable] -> [ids] -> mallocエラー
 */
ILUT_Test test_parse_016 (
)
{
	/*-
	 * extern int value;
	 *            ^^^^^ ここでmallocエラー
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,      "extern", 6 },	/* extern (LL_ID) */
		{ LL_ID,      "int",    3 },	/* int    (LL_ID) */
		{ LL_ID,      "value",  5 },	/* value  (LL_ID) */
		{ LL_EXP_END, ";",      1 },	/* ;      (LL_EXP_END) */
		{ 0,          NULL,     0 }		/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_016.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	setCreateCount( 2 );		/* extern/intを読み込んだ2回しかxmallocできない。 */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが異常終了すること", ret == 2 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );

	/* set_func_name内で2回xfreeしており、parseでさらにxfreeするため、3回xfreeされる。
	 * 3回目はNULLでxfreeされる。NULLをxfreeするのは問題ない。
	 */
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 3 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * function_definitionで ) 検出時にEOFエラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union]
 * -> [function] -> [function_definition] -> <EOF>
 */
ILUT_Test test_parse_017 (
)
{
	/*-
	 * int func( void <EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",  3 },		/* int  (LL_ID) */
		{ LL_ID,          "func", 4 },		/* func (LL_ID) */
		{ LL_PARENTHIS_L, "(",    1 },		/* (    (LL_PARENTHIS_L) */
		{ LL_ID,          "void", 4 },		/* void (LL_ID) */
		{ 0,              NULL,   0 }		/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_017.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	setCreateCount( -1 );		/* xmallocの制限無し */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 2 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_017.c(6)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_017.c(6)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}


/**
 * 正常系動作確認
 * function_impleでの再帰呼び出し
 */
ILUT_Test test_parse_018 (
)
{
	/*-
	 * int max( int x, int y ) {
	 *     if ( x > y ) {
	 *         return x;
	 *     }
	 *     else {
	 *         return y;
	 *     }
	 * }
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "max",    3 },	/* max    (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENHIS_L) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "int",    3 },	/* int    (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_ILC_COMMENT, "ILC:",   4 },	/* ILC:   (LL_ILC_COMMENT) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ID,          "if",     2 },	/* if     (LL_ID) */
		{ LL_PARENTHIS_L, "(",      1 },	/* (      (LL_PARENTHIS_L) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */
		{ LL_PARENTHIS_R, ")",      1 },	/* )      (LL_PARENTHIS_R) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "x",      1 },	/* x      (LL_ID) */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ LL_ID,          "else",   4 },	/* else   (LL_ID) */
		{ LL_BRACE_L,     "{",      1 },	/* {      (LL_BRACE_L) */
		{ LL_ID,          "return", 6 },	/* return (LL_ID) */
		{ LL_ID,          "y",      1 },	/* y      (LL_ID) */
		{ LL_EXP_END,     ";",      1 },	/* ;      (LL_EXP_END) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ LL_BRACE_R,     "}",      1 },	/* }      (LL_BRACE_R) */
		{ 0,              NULL,     0 }		/* EOF */
	};
	int ret;
	/**/

	ilc.file_in  = "test_parse_018.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	setCreateCount( -1 );		/* xmallocの制限無し */

	setStubData( stub );

	ret = parse( &ilc );

	ILUT_ASSERT( "parseが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリ解放が行われていること", getRemoveCount() == 2 );

	return ILUT_SUCCESS;
}


/**
 * 準正常系
 * struct_or_unionでEOFエラー
 * [syntax] -> [function_or_variable] -> [ids] -> [struct_or_union] -> <EOF>
 */
ILUT_Test test_parse_019 (
)
{
	/*-
	 * union { <EOF>
	 */
	/**/
	ILC ilc;
	static struct lex_stub stub[] = {
		{ LL_ID,      "union", 4 },			/* union (LL_ID) */
		{ LL_BRACE_L, "{",     1 },			/* {     (LL_BRACE_L) */
		{ 0,          NULL,    0 }			/* EOF */
	};
	int ret;
	FILE* fin;
	FILE* fout;
	char buf[BUFSIZ + 1];
	/**/

	ilc.file_in  = "test_parse_019.c";
	ilc.ilc_func = NULL;
	ilc.fpout    = NULL;

	fout = freopen( "test.dat", "w", stderr );
	if ( fout == NULL ) {
		ILUT_FAIL( "書き込みファイルの作成に失敗" );
	}

	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	setCreateCount( -1 );		/* xmallocの制限無し */

	setStubData( stub );

	ret = parse( &ilc );

	fclose( fout );

	ILUT_ASSERT( "parseが異常終了すること", ret == 1 );
	ILUT_ASSERT( "ilc_funcが登録されていないこと", ilc.ilc_func == NULL );
	ILUT_ASSERT( "メモリが解放されていること", getRemoveCount() == 1 );

	memset( buf, '\0', sizeof( buf ) );
	fin = fopen( "test.dat", "r" );
	/* 想定する文字数 予期しないトークンが出現しました。(EOF:NULL) in test_parse_019.c(4)\n => 70 */
	fread( buf, sizeof( char ), 70, fin );
	fclose( fin );
	ILUT_ASSERT( "文字列の確認",
				 strcmp( "予期しないトークンが出現しました。(EOF:NULL) in test_parse_019.c(4)\n", buf ) == 0 );

	return ILUT_SUCCESS;
}



int main (
	int argc,
	char** argv
)
{
	/**/
	static ILUT_TestCase test[] = {
		DEF_TEST(test_parse_001),
		DEF_TEST(test_parse_002),
		DEF_TEST(test_parse_003),
		DEF_TEST(test_parse_004),
		DEF_TEST(test_parse_005),
		DEF_TEST(test_parse_006),
		DEF_TEST(test_parse_007),
		DEF_TEST(test_parse_008),
		DEF_TEST(test_parse_009),
		DEF_TEST(test_parse_010),
		DEF_TEST(test_parse_011),
		DEF_TEST(test_parse_012),
		DEF_TEST(test_parse_013),
		DEF_TEST(test_parse_014),
		DEF_TEST(test_parse_015),
		DEF_TEST(test_parse_016),
		DEF_TEST(test_parse_017),
		DEF_TEST(test_parse_018),
		DEF_TEST(test_parse_019),
		TestCaseEnd
	};
	int ret;
	FILE* out = NULL;
	/**/

	/*-
	 * 第一引数で指定されたファイルを読み込む。
	 * 指定されていない場合は ilc.dat を読み込む。
	 */
	if ( ILC_Initialize( argv[1] ) != ILC_SUCCESS ) {
		printf( "カバレッジデータの読み込みに失敗\n" );
	}

	ILUT_SetShowMode( ILUT_MODE_DETAIL );
	ret = ILUT_RunTest( test );

	/*-
	 * 第二引数でファイルが指定されてあれば、そちらにテスト結果を出力する。
	 * 指定が無ければ標準出力にテスト結果を出力する。
	 */
	if ( argv[2] != NULL ) {
		out = fopen( argv[2], "w" );
	}
	if ( out == NULL ) {
		out = stdout;
	}
	ILUT_ResultOut( out, "parser", test );
	fclose( out );

	ILC_Finalize();

	return ret;
}



