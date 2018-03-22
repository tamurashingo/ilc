/**
 * @file	ILUT.h
 * @brief	ユニットテストツールの各種定義
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-13
 * @version	$Id: ILUT.h,v 1.2 2008/05/31 06:11:48 shingo Exp $
 *
 * Copyright (c) 2008 tamura shingo
 *
 */

#ifndef _ILUT_H_
#define _ILUT_H_

#include <stdio.h>
#include <time.h>

/* テスト定義 */
typedef char* ILUT_Test;


/* Assertion */
#define ILUT_ASSERT( _msg, _test ) do { \
		ILUT_Show( ILUT_MODE_DETAIL, "    ASSERT: " _msg ); \
		if ( !(_test) )  { \
			ILUT_Show( ILUT_MODE_DETAIL, "  ... NG\n" );	\
			return _msg; \
		} \
		ILUT_Show( ILUT_MODE_DETAIL, "  ... OK\n" );	\
	} while ( 0 )

/* テスト関数の正常終了値 */
#define ILUT_SUCCESS ((ILUT_Test)0)

/* テスト関数の異常終了 */
#define ILUT_FAIL(msg) do { return msg; } while ( 0 )


/* テストケース定義 */
typedef struct _ILUT_TestCase {
	ILUT_Test (*func)();		/**< テストを実施する関数 */
	char* func_name;			/**< 関数名 */
	int   result;				/**< テスト結果:初期値 0 */
	char* message;				/**< テスト失敗時のエラーメッセージ:初期値 NULL */
	time_t start;				/**< テスト開始時刻 */
	time_t end;					/**< テスト終了時刻 */
}
ILUT_TestCase;

/* テストケース定義の最終列に設定する */
#define TestCaseEnd { NULL, NULL, 0, NULL, (time_t)-1, (time_t)-1 }

/* テストケース定義の省力化 */
#define DEF_TEST(x) { x, #x, 0, NULL, (time_t)-1, (time_t)-1 }


typedef enum _ILUT_MODE {
	ILUT_MODE_NONE = 0,			/**< 表示しない */
	ILUT_MODE_SHOW = 1,			/**< テストケース名を表示 */
	ILUT_MODE_DETAIL = 2		/**< テストケース＋Assertionを表示 */
}
ILUT_MODE;


/**
 * 定義済みのテストケースを実行する
 * @param ILUT_TestCase* テストケース
 * @return 0:全テストOK  1:テストエラー
 */
int ILUT_RunTest ( ILUT_TestCase* );

/**
 * テスト実施時の表示モード
 * @param ILUT_MODE
 */
void ILUT_SetShowMode ( ILUT_MODE );

/**
 * ログ出力
 * @param ILUT_MODE ログレベル
 * @param char*     出力フォーマット
 * @param ...       引数
 * @see             printf
 */
void ILUT_Show ( ILUT_MODE, const char*, ... );

/**
 * ログ出力先の設定
 * @param FILE* 出力先
 */
void ILUT_SetLogOut ( FILE* );

/**
 * テスト結果の出力
 * @param FILE*                出力先
 * @param const char*          テストケース名
 * @param const ILUT_TestCase* テストケース
 */
void ILUT_ResultOut ( FILE*, const char*, const ILUT_TestCase* );


/*-
 * サンプル
 *
 * ILUT_Test test_add (
 * )
 * {
 *     ILUT_ASSERT( "正の数：3+4=7",      add(3, 4) == 7 );
 *     ILUT_ASSERT( "正負：3+(-4)=-1",    add(3, -4) == -1 );
 *     ......
 *     return ILUT_SUCCESS;
 * }
 *
 * ILUT_Test test_xxx (
 * )
 * {
 *     ......
 *     return ILUT_SUCCESS;
 * }
 *
 * void test_main (
 * )
 * {
 *     static ILUT_TestCase test[] = {
 *         DEF_TEST(test_add),
 *         DEF_TEST(test_xxx),
 *         TestCaseEnd
 *     };
 *
 *     ILUT_RunTest( test );
 * }
 *     
 */


#endif /* _ILUT_H_ */

