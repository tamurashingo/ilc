/**
 * @file	ILUT.c
 * @brief	ユニットテストツールの実装部
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-13
 * @version	$Id: ILUT.c,v 1.2 2008/05/31 06:11:48 shingo Exp $
 *
 * Copyright (c) 2008 tamura shingo
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include "ILUT.h"

static ILUT_MODE ilut_mode = ILUT_MODE_NONE;
static FILE* fout;


void ILUT_Show (
	ILUT_MODE mode,
	const char* format,
	...
)
{
	/**/
	va_list argptr;
	/**/
	if ( ilut_mode != ILUT_MODE_NONE ) {
		if ( mode <= ilut_mode ) {
			va_start( argptr, format );
			vfprintf( fout, format, argptr );
			va_end( argptr );
		}
	}

}


/**
 * 定義済みのテストケースを実行する
 * @param ILUT_TestCase* テストケース
 * @return 0:全テストOK  1:テストエラー
 */
int ILUT_RunTest (
	ILUT_TestCase* testcase
)
{
	/**/
	ILUT_Test ret;
	int result = 0;
	/**/

	fout = stdout;

	for ( ; testcase->func != NULL; testcase++ ) {
		ILUT_Show( ILUT_MODE_SHOW, "Running test case : %s\n", testcase->func_name );

		testcase->start = time(NULL);

		/* テスト実行 */
		ret = (*(testcase->func))();

		testcase->end = time( NULL );

		if ( ret != NULL ) {
			ILUT_Show( ILUT_MODE_SHOW, "  ... failed : %s\n", (char*)ret );
			testcase->result = 1;
			testcase->message = (char*)ret;
			result = 1;
		}
		else {
			ILUT_Show( ILUT_MODE_SHOW, "  ... passed\n" );
			testcase->result = 0;
			testcase->message = NULL;
		}
	}

	return result;
}

/**
 * テスト実施時の表示モード
 * @param ILUT_MODE
 */
void ILUT_SetShowMode (
	ILUT_MODE mode
)
{
	/**/
	/**/
	ilut_mode = mode;
}

/**
 * ログ出力先の設定
 * @param FILE*
 */
void ILUT_SetLogOut (
	FILE* out
)
{
	/**/
	/**/
	fout = out;
}

/**
 * テスト結果の出力
 * @param FILE*                出力先
 * @param const char*          テストケース名
 * @param const ILUT_TestCase* テストケース
 */
void ILUT_ResultOut(
	FILE* out,
	const char* testcase_name,
	const ILUT_TestCase* testcase
)
{
	/**/
	struct tm *ptime;
	/**/

	fputs( "# テストケース名:テスト名:テスト開始日時:テスト終了日時:結果:エラー時のメッセージ\n", out );
	for ( ; testcase->func != NULL; testcase++ ) {

		/* テストケース名 */
		fputs( testcase_name, out );
		fputs( ":", out );
		/* テスト名 */
		fputs( testcase->func_name, out );
		fputs( ":", out );
		/* テスト開始日時 */
		ptime = localtime( &(testcase->start) );
		fprintf( out, "%04d%02d%02d%02d%02d%02d:",
				 ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday,
				 ptime->tm_hour, ptime->tm_min, ptime->tm_sec );
		/* テスト終了日時 */
		ptime = localtime( &(testcase->end) );
		fprintf( out, "%04d%02d%02d%02d%02d%02d:",
				 ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday,
				 ptime->tm_hour, ptime->tm_min, ptime->tm_sec );
		/* 結果 */
		fputs( testcase->result == 0 ? "success:" : "failure:", out );
		/* エラー時のメッセージ */
		fputs( testcase->message == NULL ? "" : testcase->message, out );
		fputs( "\n", out );

	}

}


