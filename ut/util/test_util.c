/**
 * @file	test_util.c
 * @brief	util.cのユニットテスト
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-13
 * @version	$Id: test_util.c,v 1.3 2008/05/31 06:15:21 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include "util.h"
#include "ilc.h"
#include "ILUT.h"


/**
 * slist_createのテスト
 */
ILUT_Test test_slist_create (
)
{
	/**/
	SLIST* list;
	/**/

	list = slist_create();
	ILUT_ASSERT( "SLISTが作成できること", list != NULL );
	ILUT_ASSERT( "SLISTが初期化されていること:next部", list->next == NULL );
	ILUT_ASSERT( "SLISTが初期化されていること:body部", list->body == NULL );

	ILUT_ASSERT( "SLISTに値を設定してSEGVが発生しないこと:next部", list->next = list );
	ILUT_ASSERT( "SLISTに値を設定してSEGVが発生しないこと:body部", list->body = list );

	/* メモリ解放 */
	slist_remove( &list );

	return ILUT_SUCCESS;
}


/**
 * slist_removeのテスト
 */
ILUT_Test test_slist_remove (
)
{
	/**/
	SLIST* list;
	/**/

	list = slist_create();
	if ( list == NULL ) {
		ILUT_FAIL( "メモリ確保エラー" );
	}

	slist_remove( &list );
	ILUT_ASSERT( "確保したリストが解放できること", 1 );

	slist_remove( NULL );
	ILUT_ASSERT( "NULLを渡してもSEGVしないこと", 1 );

	list = NULL;
	slist_remove( &list );
	ILUT_ASSERT( "NULLを渡してもSEGVしないこと", 1 );

	return ILUT_SUCCESS;
}


/**
 * slist_appendのテスト
 */
ILUT_Test test_slist_append (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	/**/

	list1 = slist_create();
	list2 = slist_create();
	list3 = slist_create();
	list4 = slist_create();

	/* リスト作成に失敗していないことが前提条件 */
	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "slist_createに失敗" );
	}

	/* 1要素のあとに1要素を追加できること */
	slist_append( &list1, list2 );
	ILUT_ASSERT( "list1の次がlist2であること",    list1->next       == list2 );
	ILUT_ASSERT( "list1の次の次がNULLであること", list1->next->next == NULL );

	/* 2要素のあとに1要素を追加できること */
	slist_append( &list1, list3 );
	ILUT_ASSERT( "list1の次がlist2であること",        list1->next             == list2 );
	ILUT_ASSERT( "list1の次の次がlist3であること",    list1->next->next       == list3 );
	ILUT_ASSERT( "list1の次の次の次がNULLであること", list1->next->next->next == NULL );

	/* 1要素のあとに3要素を追加できること */
	slist_append( &list4, list1 );
	ILUT_ASSERT( "list4の次がlist1であること",            list4->next                   == list1 );
	ILUT_ASSERT( "list4の次の次がlist2であること",        list4->next->next             == list2 );
	ILUT_ASSERT( "list4の次の次の次がlist3であること",    list4->next->next->next       == list3 );
	ILUT_ASSERT( "list4の次の次の次の次がNULLであること", list4->next->next->next->next == NULL );

	slist_remove( &list1 );
	slist_remove( &list2 );
	slist_remove( &list3 );
	slist_remove( &list4 );

	return ILUT_SUCCESS;
}


/**
 * slist_searchのテストで使用する検索関数
 * 常に1を返すため、リストの先頭がマッチするはず。
 */
int test_search_001 (
	const SLIST* list,
	const void* param
)
{
	/**/
	/**/

	return 1;
}

/**
 * slist_searchのテストで使用する関数
 * 常に0を返すため、何もヒットしないはず。
 */
int test_search_002 (
	const SLIST* list,
	const void* param
	)
{
	/**/
	/**/

	return 0;
}


/**
 * slist_searchのテストで使用する関数
 * list->bodyとparamのアドレスが一致すれば1を返す
 */
int test_search_003 (
	const SLIST* list,
	const void* param
	)
{
	/**/
	/**/
	if ( list->body == param ) {
		return 1;
	}
	else {
		return 0;
	}
}


/**
 * slist_searchのテスト
 */
ILUT_Test test_slist_search (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	char* ptr = "string";
	/**/
	list1 = slist_create();
	list2 = slist_create();
	list3 = slist_create();
	list4 = slist_create();

	/* リスト作成に失敗していないことが前提条件 */
	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "slist_createに失敗" );
	}
	/* list1->list2->list3->list4 につなげる */
	slist_append( &list1, list2 );
	slist_append( &list2, list3 );
	slist_append( &list3, list4 );
	/* list3のbodyとlist4のbodyにptrを設定する */
	list3->body = ptr;
	list4->body = ptr;

	ILUT_ASSERT( "list1を返すこと", list1 == slist_search( list1, test_search_001, ptr ) );
	ILUT_ASSERT( "NULLを返すこと",  NULL  == slist_search( list1, test_search_002, ptr ) );

	/* 必ず最初にマッチした要素を返すこと */
	ILUT_ASSERT( "list3を返すこと 1/3", list3 == slist_search( list1, test_search_003, ptr ) );
	ILUT_ASSERT( "list3を返すこと 2/3", list3 == slist_search( list1, test_search_003, ptr ) );
	ILUT_ASSERT( "list3を返すこと 3/3", list3 == slist_search( list1, test_search_003, ptr ) );

	/* list3のbodyを途中で操作した時の確認 */
	list3->body = NULL;
	ILUT_ASSERT( "list4を返すこと",     list4 == slist_search( list1, test_search_003, ptr ) );

	ILUT_ASSERT( "第1引数にNULLを渡してもSEGVしないこと：NULLを返すこと",
				 NULL == slist_search( NULL, test_search_001, ptr ) );

	ILUT_ASSERT( "第2引数にNULLを渡してもSEGVしないこと：NULLを返すこと", 
				 NULL == slist_search( list1, NULL, ptr ) );

	/* 第3引数は検索関数に渡す引数である。
	   検索関数のつくりによってslist_searchの戻り値が違ってしまうため、
	   戻り値が正しいかどうかは、ここでは精査しない
	*/
	ILUT_ASSERT( "第3引数にNULLを渡してもSEGVしないこと", 1 );
	slist_search( list1, test_search_001, NULL );


	slist_remove( &list1 );
	slist_remove( &list2 );
	slist_remove( &list3 );
	slist_remove( &list4 );

	return ILUT_SUCCESS;
}


/**
 * xmalloc/xfreeのテスト
 */
ILUT_Test test_xmalloc_xfree (
	)
{
	/**/
	char*   ptr_char;
	short*  ptr_short;
	int*    ptr_int;
	long*   ptr_long;
	float*  ptr_float;
	double* ptr_double;
	/**/

	ptr_char   = (char*)  xmalloc( sizeof(char) );
	ptr_short  = (short*) xmalloc( sizeof(short) );
	ptr_int    = (int*)   xmalloc( sizeof(int) );
	ptr_long   = (long*)  xmalloc( sizeof(long) );
	ptr_float  = (float*) xmalloc( sizeof(float) );
	ptr_double = (double*)xmalloc( sizeof(double) );

	/* マシンの状態に依存する試験 */
	ILUT_ASSERT( "メモリ確保が行えること(char)",   ptr_char   != NULL );
	ILUT_ASSERT( "メモリ確保が行えること(short)",  ptr_short  != NULL );
	ILUT_ASSERT( "メモリ確保が行えること(int)",    ptr_int    != NULL );
	ILUT_ASSERT( "メモリ確保が行えること(long)",   ptr_long   != NULL );
	ILUT_ASSERT( "メモリ確保が行えること(float)",  ptr_float  != NULL );
	ILUT_ASSERT( "メモリ確保が行えること(double)", ptr_double != NULL );


	xfree( ptr_char );
	ILUT_ASSERT( "メモリ解放が行えること(char)",   1 );
	xfree( ptr_short );
	ILUT_ASSERT( "メモリ解放が行えること(short)",  1 );
	xfree( ptr_int );
	ILUT_ASSERT( "メモリ解放が行えること(int)",    1 );
	xfree( ptr_long );
	ILUT_ASSERT( "メモリ解放が行えること(long)",   1 );
	xfree( ptr_float);
	ILUT_ASSERT( "メモリ解放が行えること(float)",  1 );
	xfree( ptr_double );
	ILUT_ASSERT( "メモリ解放が行えること(double)", 1 );
	xfree( NULL );
	ILUT_ASSERT( "NULLを渡してもSEGVしないこと",   1 );

	return ILUT_SUCCESS;
}



int main (
	int argc,
	char** argv
)
{
	/**/
	static ILUT_TestCase test[] = {
		DEF_TEST(test_slist_create),
		DEF_TEST(test_slist_remove),
		DEF_TEST(test_slist_append),
		DEF_TEST(test_slist_search),
		DEF_TEST(test_xmalloc_xfree),
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
		printf( "カバレッジデータの読み込みに失敗" );
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
	ILUT_ResultOut( out, "util", test );
	fclose( out );

	ILC_Finalize();

	return ret;
}
