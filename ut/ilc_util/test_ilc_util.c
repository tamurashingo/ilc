/**
 * @file	test_ilc_util.c
 * @brief	ilc_util.cのユニットテスト
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-18
 * @version	$Id: test_ilc_util.c,v 1.3 2008/05/31 06:15:21 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "ilc_util.h"
#include "ILUT.h"


/* 外部関数の宣言 */
/* util_stub.c */
int  getCreateCount();
void setCreateCount();
int  getRemoveCount();
void setRemoveCount();
/* ilc_stub.c */
int  getAppendCount();
void initAppendCount();


/**
 * ilcfunc_createのユニットテスト
 */
ILUT_Test test_ilcfunc_create (
)
{
	/**/
	SLIST* ilcfunc;
	/**/

	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */
	ilcfunc = ilcfunc_create();
	ILUT_ASSERT( "ILCFUNCが作成できていること",       ilcfunc                                        != NULL );
	ILUT_ASSERT( "bodyが作成できていること",          ilcfunc->body                                  != NULL );
	ILUT_ASSERT( "初期化済みであること：next",        ilcfunc->next                                  == NULL );
	ILUT_ASSERT( "初期化済みであること：func_name",   ((ILC_FUNC_BODY*)(ilcfunc->body))->func_name   == NULL );
	ILUT_ASSERT( "初期化済みであること：count",       ((ILC_FUNC_BODY*)(ilcfunc->body))->count       == 0 );
	ILUT_ASSERT( "初期化済みであること：ilc_comment", ((ILC_FUNC_BODY*)(ilcfunc->body))->ilc_comment == NULL );

	xfree( ilcfunc->body );
	xfree( ilcfunc );

	/* 準正常系：bodyの作成に失敗 */
	setCreateCount( 1 );		/* ilcfunc部の１回しかxmallocできない */
	ilcfunc = ilcfunc_create();
	ILUT_ASSERT( "ILCFUNCが作成できていないこと", ilcfunc == NULL );

	/* 異常系：そもそもilcfuncが作成できない */
	setCreateCount( 0 );
	ilcfunc = ilcfunc_create();
	ILUT_ASSERT( "ILCFUNCが作成できていないこと", ilcfunc == NULL );

	return ILUT_SUCCESS;
}


/**
 * ilcfunc_initのユニットテスト
 */
ILUT_Test test_ilcfunc_init (
)
{
	/**/
	SLIST*         ilcfunc;
	ILC_FUNC_BODY* body;
	/**/

	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */
	ilcfunc = slist_create();
	body = (ILC_FUNC_BODY*) xmalloc( sizeof ( ILC_FUNC_BODY ) );
	ilcfunc->body = body;

	ilcfunc_init( ilcfunc );

	ILUT_ASSERT( "SLIST.nextが初期化されていること",   ilcfunc->next                                  == NULL );
	ILUT_ASSERT( "SLIST.bodyはNULLになっていないこと", ilcfunc->body                                  != NULL );
	ILUT_ASSERT( "func_nameが初期化されていること",    ((ILC_FUNC_BODY*)(ilcfunc->body))->func_name   == NULL );
	ILUT_ASSERT( "countが初期化されていること",        ((ILC_FUNC_BODY*)(ilcfunc->body))->count       == 0 );
	ILUT_ASSERT( "ilc_commentが初期化されていること",  ((ILC_FUNC_BODY*)(ilcfunc->body))->ilc_comment == NULL );


	/* 準正常系確認 */
	/* bodyがNULLの場合の動作確認 */
	ilcfunc->body = NULL;
	ilcfunc->next = ilcfunc;

	/* ilcfunc_bodyが初期化されないことの確認 */
	body->func_name = "aaaa";
	body->count = 9999;
	body->ilc_comment = ilcfunc;

	ilcfunc_init( ilcfunc );

	ILUT_ASSERT( "SLIST.nextが初期化されていること", ilcfunc->next == NULL );
	ILUT_ASSERT( "SLSIT.bodyがNULLのままであること", ilcfunc->body == NULL );
	ILUT_ASSERT( "func_nameが初期化されていないこと", body->func_name != NULL );
	ILUT_ASSERT( "countが初期化されていないこと", body->count != 0 );
	ILUT_ASSERT( "ilc_commentが初期化されていないこと", body->ilc_comment != NULL );


	/* 異常系動作確認 */
	/* NULLの場合の動作確認 */
	ilcfunc_init( NULL );

	ILUT_ASSERT( "SEGVしないこと", 1 );

	xfree( ilcfunc );
	xfree( body );

	return ILUT_SUCCESS;
}


/**
 * ilcfunc_appendのユニットテスト
 */
ILUT_Test test_ilcfunc_append (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */

	list1 = ilcfunc_create();
	list2 = ilcfunc_create();
	list3 = ilcfunc_create();
	list4 = ilcfunc_create();

	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "ilcfunc_createに失敗" );
	}

	/* 1要素のあとに1要素を追加できること */
	ilcfunc_append( &list1, list2 );
	ILUT_ASSERT( "list1の次がlist2であること",    list1->next       == list2 );
	ILUT_ASSERT( "list1の次の次がNULLであること", list1->next->next == NULL );

	/* 2要素のあとに1要素を追加できること */
	ilcfunc_append( &list1, list3 );
	ILUT_ASSERT( "list1の次がlist2であること",        list1->next             == list2 );
	ILUT_ASSERT( "list1の次の次がlist3であること",    list1->next->next       == list3 );
	ILUT_ASSERT( "list1の次の次の次がNULLであること", list1->next->next->next == NULL );

	/* 1要素の後に3要素を追加できること */
	ilcfunc_append( &list4, list1 );
	ILUT_ASSERT( "list4の次がlist1であること",            list4->next                   == list1 );
	ILUT_ASSERT( "list4の次の次がlist2であること",        list4->next->next             == list2 );
	ILUT_ASSERT( "list4の次の次の次がlist3であること",    list4->next->next->next       == list3 );
	ILUT_ASSERT( "list4の次の次の次の次がNULLであること", list4->next->next->next->next == NULL );

	/* bodyもfreeする必要がある */
 	xfree( list1->body );
	xfree( list1 );
	xfree( list2->body );
	xfree( list2 );
	xfree( list3->body );
	xfree( list3 );
	xfree( list4->body );
	xfree( list4 );
	
	return ILUT_SUCCESS;
}


/**
 * ilcfunc_searchのユニットテスト
 */
ILUT_Test test_ilcfunc_search (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */

	list1 = ilcfunc_create();
	list2 = ilcfunc_create();
	list3 = ilcfunc_create();
	list4 = ilcfunc_create();

	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "ilcfunc_createに失敗" );
	}

	((ILC_FUNC_BODY*)(list1->body))->func_name = "list1";
	((ILC_FUNC_BODY*)(list2->body))->func_name = "list2";
	((ILC_FUNC_BODY*)(list3->body))->func_name = "list3";
	((ILC_FUNC_BODY*)(list4->body))->func_name = "list4";
	ilcfunc_append( &list1, list2 );
	ilcfunc_append( &list1, list3 );
	ilcfunc_append( &list1, list4 );

	/*-
	 * list1 -> list2 -> list3 -> list4 -> NULL
	 * の順に初期化
	 */

	/* 以下、試験実施 */
	/* 正常系動作確認 */
	ILUT_ASSERT( "list1を返すこと", ilcfunc_search( list1, "list1" ) == list1 );
	ILUT_ASSERT( "list2を返すこと", ilcfunc_search( list1, "list2" ) == list2 );
	ILUT_ASSERT( "list3を返すこと", ilcfunc_search( list1, "list3" ) == list3 );
	ILUT_ASSERT( "list4を返すこと", ilcfunc_search( list1, "list4" ) == list4 );
	ILUT_ASSERT( "一致しない場合はNULLを返すこと", ilcfunc_search( list1, "list5" ) == NULL );

	/* 準正常系動作確認 */
	ILUT_ASSERT( "第1引数がNULLの場合、SEGVしないこと", ilcfunc_search( NULL, "list1" ) == NULL );
	ILUT_ASSERT( "第2引数がNULLの場合、SEGVしないこと", ilcfunc_search( list1, NULL ) == NULL );
	ILUT_ASSERT( "第2引数が空文字の場合、SEGVしないこと", ilcfunc_search( list1, "" ) == NULL );

	/* bodyもfreeする必要がある */
	xfree( list1->body );
	xfree( list1 );
	xfree( list2->body );
	xfree( list2 );
	xfree( list3->body );
	xfree( list3 );
	xfree( list4->body );
	xfree( list4 );

	return ILUT_SUCCESS;
}


/**
 * ilcfunc_removeのユニットテスト
 */
ILUT_Test test_ilcfunc_remove (
)
{
	/**/
	SLIST* func;
	SLIST* comm;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */
	func = ilcfunc_create();
	comm = ilccomment_create();

	((ILC_FUNC_BODY*)(func->body))->ilc_comment = comm;

	/* 正常系 */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	ilcfunc_remove( &func );

	/* ILC_FUNCとBODY部のみが解放され、ILC_COMMENTは解放されないこと */
	ILUT_ASSERT( "メモリ解放されていること", getRemoveCount() == 2 );


	/* 準正常系 : ILC_FUNCのアドレスがNULLでも落ちないこと */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	func = NULL;
	ilcfunc_remove( &func );

	ILUT_ASSERT( "メモリが解放されないこと", getRemoveCount() == 0 );


	/* 準正常系 : NULLを渡しても落ちないこと */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	ilcfunc_remove( NULL );

	ILUT_ASSERT( "メモリが解放されないこと", getRemoveCount() == 0 );

	ilccomment_remove( &comm );


	return ILUT_SUCCESS;
}


/**
 * ilccomment_createのユニットテスト
 */
ILUT_Test test_ilccomment_create (
	)
{
	/**/
	SLIST* ilccomment;
	/**/

	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */
	ilccomment = ilccomment_create();
	ILUT_ASSERT( "ILCCOMMENTが作成できていること", ilccomment                                    != NULL );
	ILUT_ASSERT( "bodyが作成できていること",       ilccomment->body                              != NULL );
	ILUT_ASSERT( "初期化済みであること：next",     ilccomment->next                              == NULL );
	ILUT_ASSERT( "初期化済みであること：line",     ((ILC_COMMENT_BODY*)(ilccomment->body))->line == 0 );

	xfree( ilccomment->body );
	xfree( ilccomment );

	/* 準正常系：bodyの作成に失敗 */
	setCreateCount( 1 );		/* ilccomment部の１回しかxmallocできない */
	ilccomment = ilccomment_create();
	ILUT_ASSERT( "ILCCOMMENTが作成できていないこと", ilccomment == NULL );

	/* 異常系：そもそもilccommentが作成できない */
	setCreateCount( 0 );
	ilccomment = ilccomment_create();
	ILUT_ASSERT( "ILCCOMMENTが作成できていないこと", ilccomment == NULL );

	return ILUT_SUCCESS;
}


/**
 * ilccomment_initのユニットテスト
 */
ILUT_Test test_ilccomment_init (
)
{
	/**/
	SLIST*            ilccomment;
	ILC_COMMENT_BODY* body;
	/**/

	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */
	ilccomment = slist_create();
	body = (ILC_COMMENT_BODY*) xmalloc( sizeof ( ILC_COMMENT_BODY ) );
	ilccomment->body = body;

	ilccomment_init( ilccomment );

	ILUT_ASSERT( "SLIST.nextが初期化されていること",   ilccomment->next                               == NULL );
	ILUT_ASSERT( "SLIST.bodyはNULLになっていないこと", ilccomment->body                               != NULL );
	ILUT_ASSERT( "lineが初期化されていること",        ((ILC_COMMENT_BODY*)(ilccomment->body))->line == 0 );

	/* 準正常系確認 */
	/* bodyがNULLの場合の動作確認 */
	ilccomment->body = NULL;
	ilccomment->next = ilccomment;

	/* ilccomment_bodyが初期化されないことの確認 */
	body->line = 999;

	ilccomment_init( ilccomment );

	ILUT_ASSERT( "SLIST.nextが初期化されていること", ilccomment->next == NULL );
	ILUT_ASSERT( "SLIST.bodyがNULLのままであること", ilccomment->body == NULL );
	ILUT_ASSERT( "lineが初期化されていないこと", body->line != 0 );

	/* 異常系動作確認 */
	/* NULLの場合の動作確認 */
	ilccomment_init( NULL );

	ILUT_ASSERT( "SEGVしないこと", 1 );

	xfree( ilccomment );
	xfree( body );

	return ILUT_SUCCESS;
}


/**
 * ilccomment_appendのユニットテスト
 */
ILUT_Test test_ilccomment_append (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */

	list1 = ilccomment_create();
	list2 = ilccomment_create();
	list3 = ilccomment_create();
	list4 = ilccomment_create();

	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "ilccomment_createに失敗" );
	}

	/* 1要素の後に1要素を追加できること */
	ilccomment_append( &list1, list2 );
	ILUT_ASSERT( "list1の次がlist2であること",     list1->next       == list2 );
	ILUT_ASSERT( "list1の次の次がNULLであること",  list1->next->next == NULL );

	/* 2要素のあとに1要素を追加できること */
	ilccomment_append( &list1, list3 );
	ILUT_ASSERT( "list1の次がlist2であること",        list1->next             == list2 );
	ILUT_ASSERT( "list1の次の次がlist3であること",    list1->next->next       == list3 );
	ILUT_ASSERT( "list1の次の次の次がNULLであること", list1->next->next->next == NULL );

	/* 1要素の後に3要素を追加できること */
	ilccomment_append( &list4, list1 );
	ILUT_ASSERT( "list4の次がlist1であること",            list4->next                   == list1 );
	ILUT_ASSERT( "list4の次の次がlist2であること",        list4->next->next             == list2 );
	ILUT_ASSERT( "list4の次の次の次がlist3であること",    list4->next->next->next       == list3 );
	ILUT_ASSERT( "list4の次の次の次の次がNULLであること", list4->next->next->next->next == NULL );

	/* bodyもfreeする必要がある */
 	xfree( list1->body );
	xfree( list1 );
	xfree( list2->body );
	xfree( list2 );
	xfree( list3->body );
	xfree( list3 );
	xfree( list4->body );
	xfree( list4 );

	return ILUT_SUCCESS;
}


/**
 * ilccomment_removeのユニットテスト
 */
ILUT_Test test_ilccomment_remove (
)
{
	/**/
	SLIST* list;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */
	list = ilccomment_create();

	/* 正常系 */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	ilccomment_remove( &list );

	ILUT_ASSERT( "メモリ解放されていること", getRemoveCount() == 2 );


	/* 準正常系 : ILC_COMMENTのアドレスがNULLでも落ちないこと */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	list = NULL;
	ilccomment_remove( &list );

	ILUT_ASSERT( "メモリが解放されないこと", getRemoveCount() == 0 );


	/* 準正常系 : NULLを渡しても落ちないこと */
	setRemoveCount( 0 );		/* xfreeの回数を初期化 */
	ilccomment_remove( NULL );

	ILUT_ASSERT( "メモリが解放されないこと", getRemoveCount() == 0 );


	return ILUT_SUCCESS;
}


/**
 * ilc_initのテスト
 */
ILUT_Test test_ilc_init (
)
{
	/**/
	ILC ilc;
	/**/

	ilc_init( &ilc );

	ILUT_ASSERT( "file_in  が NULL  であること", ilc.file_in  == NULL );
	ILUT_ASSERT( "file_out が NULL  であること", ilc.file_out == NULL );
	ILUT_ASSERT( "fpin     が stdin であること", ilc.fpin     == stdin );
	ILUT_ASSERT( "fpout    が stdoutであること", ilc.fpout    == stdout );
	ILUT_ASSERT( "ilc_func が NULL  であること", ilc.ilc_func == NULL );

	return ILUT_SUCCESS;
}

/**
 * ilc_endのテスト
 */
ILUT_Test test_ilc_end (
)
{
	/**/
	ILC ilc;
	SLIST* func1;
	SLIST* func2;
	SLIST* func3;
	SLIST* comm1;
	SLIST* comm2;
	SLIST* comm3;
	SLIST* comm4;
	/**/

	setCreateCount( -1 );		/* xmallocの制限無し */
	func1 = ilcfunc_create();
	func2 = ilcfunc_create();
	func3 = ilcfunc_create();

	comm1 = ilccomment_create();
	comm2 = ilccomment_create();
	comm3 = ilccomment_create();
	comm4 = ilccomment_create();

	((ILC_FUNC_BODY*)(func1->body))->ilc_comment = comm1;
	((ILC_FUNC_BODY*)(func2->body))->ilc_comment = comm2;
	((ILC_FUNC_BODY*)(func3->body))->ilc_comment = comm3;

	ilcfunc_append( &func1, func2 );
	ilcfunc_append( &func1, func3 );

	ilccomment_append( &comm3, comm4 );

	ilc.ilc_func = func1;


	/* 正常系 */
	setRemoveCount(0);		/* xfreeの呼ばれた回数を初期化 */

	ilc_end( &ilc );

	/* SLIST本体とbody、それぞれ７個分 => 14回mallocしている */
	ILUT_ASSERT( "すべてのメモリが解放されていること", getRemoveCount() == 14 );


	/* 準正常系 ilc_commentが存在しなくても解放されること */

	setRemoveCount(0);		/* xfreeの呼ばれた回数を初期化 */
	func1 = ilcfunc_create();
	func2 = ilcfunc_create();
	ilcfunc_append( &func1, func2 );

	ilc.ilc_func = func1;

	ilc_end( &ilc );

	/* SLIST本体とbody、それぞれ２個分 => 4回mallocしている */
	ILUT_ASSERT( "すべてのメモリが解放されていること", getRemoveCount() == 4 );


	/* 準正常系 NULLを渡しても落ちないこと */
	ilc.ilc_func = NULL;
	ilc_end( &ilc );
	ILUT_ASSERT( "SEGVしないこと", 1 );

	return ILUT_SUCCESS;
}


/**
 * ilc_append_coverageのユニットテスト
 */
ILUT_Test test_ilc_append_coverage (
)
{
	/**/
	SLIST* list1;
	SLIST* list2;
	SLIST* list3;
	SLIST* list4;
	int    ret;		/**< ilc_append_coverageの戻り値 */
	/**/

	/* 初期処理 */
	setCreateCount( -1 );		/* xmallocの制限無し */

	list1 = ilcfunc_create();
	list2 = ilcfunc_create();
	list3 = ilcfunc_create();
	list4 = ilcfunc_create();

	if ( list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL ) {
		ILUT_FAIL( "ilcfunc_createに失敗" );
	}

	ilcfunc_append( &list1, list2 );
	ilcfunc_append( &list1, list3 );
	ilcfunc_append( &list1, list4 );

	/* 関数名の設定 */
	((ILC_FUNC_BODY*)(list1->body))->func_name = "func1";
	((ILC_FUNC_BODY*)(list2->body))->func_name = "func2";
	((ILC_FUNC_BODY*)(list3->body))->func_name = "func3";
	((ILC_FUNC_BODY*)(list4->body))->func_name = "func4";

	/* 以上、初期処理 */

	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */

	/* 登録済みのfunc2(list2)にコメント行を追加できること */
	{
		/**/
		SLIST* comment;
		/**/
		ret = ilc_append_coverage( &list1, "func2", 10 );
		comment = ((ILC_FUNC_BODY*)(list2->body))->ilc_comment;

		ILUT_ASSERT( "ilc_append_coverageが正常終了すること", ret == 0 );
		ILUT_ASSERT( "list2のコメント数が1であること", ((ILC_FUNC_BODY*)(list2->body))->count == 1 );
		ILUT_ASSERT( "コメントが追加されていること", comment != NULL );
		ILUT_ASSERT( "コメント行数が10であること", ((ILC_COMMENT_BODY*)(comment->body))->line == 10 );
	}

	/* 登録済みのfunc2(list2)にさらにコメント行を追加できること */
	{
		/**/
		SLIST* comment;
		/**/
		ret = ilc_append_coverage( &list1, "func2", 20 );
		comment = ((ILC_FUNC_BODY*)(list2->body))->ilc_comment;

		ILUT_ASSERT( "ilc_append_coverageが正常終了すること", ret == 0 );
		ILUT_ASSERT( "list2のコメント数が2であること", ((ILC_FUNC_BODY*)(list2->body))->count == 2 );
		ILUT_ASSERT( "コメントが追加されていること", comment != NULL );
		ILUT_ASSERT( "コメントが追加されていること", comment->next != NULL );
		ILUT_ASSERT( "追加したコメント行数が20であること", ((ILC_COMMENT_BODY*)(comment->next->body))->line == 20 );
	}

	/* 未登録のfunc5にコメントを登録できること */
	{
		/**/
		SLIST* list5;
		SLIST* comment;
		int    ret;
		/**/
		ret = ilc_append_coverage( &list1, "func5", 30 );
		list5 = list4->next;

		ILUT_ASSERT( "ilc_append_coverageが正常終了すること", ret == 0 );
		ILUT_ASSERT( "list4の次の要素(list5)が作成されていること", list4->next != NULL );

		comment = ((ILC_FUNC_BODY*)(list5->body))->ilc_comment;

		ILUT_ASSERT( "list5の関数名が func5 であること", 
					 strcmp(((ILC_FUNC_BODY*)(list5->body))->func_name, "func5") == 0 );
		ILUT_ASSERT( "list5のコメント数が1であること", ((ILC_FUNC_BODY*)(list5->body))->count == 1 );
		ILUT_ASSERT( "コメントが追加されていること", comment != NULL );
		ILUT_ASSERT( "追加したコメント行数が30であること", ((ILC_COMMENT_BODY*)(comment->body))->line == 30 );
	}

	/* 異常系動作確認 */
	/* コメントデータ作成に失敗 */
	setCreateCount( 0 );
	{
		/**/
		int ret;
		/**/
		/* func1(list1)にデータを追加する */
		ret = ilc_append_coverage( &list1, "func1", 5 );

		ILUT_ASSERT( "ilc_append_coverageが異常終了すること", ret == -1 );
		ILUT_ASSERT( "list1のコメント数が0のままであること", ((ILC_FUNC_BODY*)(list1->body))->count == 0 );
		ILUT_ASSERT( "list1のコメントへのポインタがNULLのままであること",
					 ((ILC_FUNC_BODY*)(list1->body))->ilc_comment == NULL );
		ILUT_ASSERT( "listの要素数に変化がないこと(list5->next == NULL)", list4->next->next == NULL );
	}

	/* 関数情報作成に失敗 */
	setCreateCount( 1 );
	{
		/**/
		int ret;
		/**/
		/* func6(未登録)にデータを追加する */
		ret = ilc_append_coverage( &list1, "func6", 100 );

		ILUT_ASSERT( "ilc_append_coverageが異常終了すること", ret == -1 );
		ILUT_ASSERT( "listの要素数に変化がないこと(list5->next == NULL)", list4->next->next == NULL );
	}


	/* 後処理 */
	xfree( list1->body );
	xfree( list1 );
	xfree( ((ILC_FUNC_BODY*)(list2->body))->ilc_comment->next->body );
	xfree( ((ILC_FUNC_BODY*)(list2->body))->ilc_comment->next );
	xfree( ((ILC_FUNC_BODY*)(list2->body))->ilc_comment->body );
	xfree( ((ILC_FUNC_BODY*)(list2->body))->ilc_comment );
	xfree( list2->body );
	xfree( list2 );

	xfree( list3->body );
	xfree( list3 );

	/* list5を消しておく */
	xfree( ((ILC_FUNC_BODY*)(list4->next->body))->ilc_comment->body );
	xfree( ((ILC_FUNC_BODY*)(list4->next->body))->ilc_comment );
	xfree( list4->next->body );
	xfree( list4->next );

	xfree( list4->body );
	xfree( list4 );

	return ILUT_SUCCESS;
}



/**
 * ilc_put_coverageのユニットテスト
 */
ILUT_Test test_ilc_put_coverage (
)
{
	/**/
	FILE* fout;
	FILE* fin;
	char buf[BUFSIZ + 1];
	int eof;
	/**/


	/* 正常系動作確認 */
	{
		fout = fopen( "test.dat", "w" );
		if ( fout == NULL ) {
			ILUT_FAIL( "書き込みファイルの作成に失敗" );
		}
		ilc_put_coverage( fout, "src001", "func001", 1 );
		fclose( fout );
		
		/* 確認 */
		memset( buf, '\0', sizeof( buf ) );
		fin = fopen( "test.dat", "r" );
		/* 想定する文字数 26 + src001 func001 1 => 40 */
		fread( buf, sizeof( char ), 40, fin );
		eof = feof( fin );
		fclose( fin );
		
		ILUT_ASSERT( "文字列の確認", strcmp( "*/ __ilc_check( \"src001:func001:1\" ); /*", buf ) == 0 );
	}


	/* 2回連続で出力した場合 */
	{
		fout = fopen( "test.dat", "w" );
		if ( fout == NULL ) {
			ILUT_FAIL( "書き込みファイルの作成に失敗" );
		}
		ilc_put_coverage( fout, "src002", "func002", 2 );
		ilc_put_coverage( fout, "src002", "func002", 3 );
		fclose( fout );

		/* 確認 */
		memset( buf, '\0', sizeof( buf ) );
		fin = fopen( "test.dat", "r" );
		/* 想定する文字数 26+26+ src002 func002 2 src002 func002 3 => 80 */
		fread( buf, sizeof( char ), 80, fin );
		eof = feof( fin );
		fclose( fin );

		ILUT_ASSERT( "文字列の確認", strcmp( "*/ __ilc_check( \"src002:func002:2\" ); /**/ __ilc_check( \"src002:func002:3\" ); /*", buf ) == 0 );

	}

	/* 準正常系確認 */
	/* 第一引数がNULL */
	{
		ilc_put_coverage( NULL, "src003", "func003", 3 );
		ILUT_ASSERT( "SEGVしないこと", 1 );
	}

	/* 第二引数がNULL */
	{
		/**/
		struct stat st;
		/**/
		fout = fopen( "test.dat", "w" );
		if ( fout == NULL ) {
			ILUT_FAIL( "書き込みファイルの作成に失敗" );
		}
		ilc_put_coverage( fout, NULL, "func004", 4 );
		fclose( fout );
		ILUT_ASSERT( "SEGVしないこと", 1 );
		stat( "test.dat", &st );
		ILUT_ASSERT( "出力されていないこと", st.st_size == 0 );
	}
	
	/* 第三引数がNULL */
	{
		/**/
		struct stat st;
		/**/
		fout = fopen( "test.dat", "w" );
		if ( fout == NULL ) {
			ILUT_FAIL( "書き込みファイルの作成に失敗" );
		}
		ilc_put_coverage( fout, "func5", NULL, 5 );
		fclose( fout );
		ILUT_ASSERT( "SEGVしないこと", 1 );
		stat( "test.dat", &st );
		ILUT_ASSERT( "出力されていないこと", st.st_size == 0 );
	}

		
	return ILUT_SUCCESS;
}


/**
 * ilc2ilcdataのユニットテスト
 */
ILUT_Test test_ilc2ilcdata (
	)
{
	/**/
	ILC ilc;
	ILC_DATA ilcdata;

	SLIST* func1;
	SLIST* func2;
	SLIST* func3;
	SLIST* comm1;
	SLIST* comm2;
	SLIST* comm3;
	SLIST* comm4;

	int ret;
	/**/

	/* 初期化 */
	/*-
	 * リストについて
	 * func1 ----> func2 ----> func3
	 *  - comm1     - comm2     - comm4
	 *                comm3
	 */

	setCreateCount( -1 );		/* xmallocの制限無し */
	func1 = ilcfunc_create();
	func2 = ilcfunc_create();
	func3 = ilcfunc_create();

	comm1 = ilccomment_create();
	comm2 = ilccomment_create();
	comm3 = ilccomment_create();
	comm4 = ilccomment_create();

	((ILC_FUNC_BODY*)(func1->body))->func_name   = "func1";
	((ILC_FUNC_BODY*)(func1->body))->count       = 1;
	((ILC_FUNC_BODY*)(func1->body))->ilc_comment = comm1;
	((ILC_FUNC_BODY*)(func2->body))->func_name   = "func2";
	((ILC_FUNC_BODY*)(func2->body))->count       = 2;
	((ILC_FUNC_BODY*)(func2->body))->ilc_comment = comm2;	/* ->next = comm3 */
	((ILC_FUNC_BODY*)(func3->body))->func_name   = "func3";
	((ILC_FUNC_BODY*)(func3->body))->count       = 1;
	((ILC_FUNC_BODY*)(func3->body))->ilc_comment = comm4;

	((ILC_COMMENT_BODY*)(comm1->body))->line     = 11;
	((ILC_COMMENT_BODY*)(comm2->body))->line     = 22;
	((ILC_COMMENT_BODY*)(comm3->body))->line     = 33;
	((ILC_COMMENT_BODY*)(comm4->body))->line     = 44;

	ilcfunc_append( &func1, func2 );
	ilcfunc_append( &func1, func3 );

	ilccomment_append( &comm2, comm3 );

	ilc_init( &ilc );
	ilc.file_in = "src001.c";
	ilc.ilc_func = func1;

	ilcdata.num = 0;
	ilcdata.coverage = NULL;


	/* 正常系動作確認 */
	setCreateCount( -1 );		/* xmallocの制限無し */
	initAppendCount();			/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が4であること", getAppendCount() == 4 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が4であること", ilcdata.num == 4 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 1/4",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 2/4",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 3/4",
				 strcmp( "0:src001.c:func2:33", ilcdata.coverage[2] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 4/4",
				 strcmp( "0:src001.c:func3:44", ilcdata.coverage[3] ) == 0 );

	/* 登録済みのデータが登録されないこと */
	initAppendCount();		/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が0であること", getAppendCount() == 0 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が4のままであること", ilcdata.num == 4 );
	ILUT_ASSERT( "登録済みデータが変更されていないこと 1/4",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "登録済みデータが変更されていないこと 2/4",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );
	ILUT_ASSERT( "登録済みデータが変更されていないこと 3/4",
				 strcmp( "0:src001.c:func2:33", ilcdata.coverage[2] ) == 0 );
	ILUT_ASSERT( "登録済みデータが変更されていないこと 4/4",
				 strcmp( "0:src001.c:func3:44", ilcdata.coverage[3] ) == 0 );

	/* 後始末 */
	xfree( ilcdata.coverage[0] );
	xfree( ilcdata.coverage[1] );
	xfree( ilcdata.coverage[2] );
	xfree( ilcdata.coverage[3] );
	xfree( ilcdata.coverage );
	ilcdata.num = 0;
	ilcdata.coverage = NULL;
	
	/* コメントがない関数がある場合 (func3のilc_commentをNULLにする) */
	((ILC_FUNC_BODY*)(func3->body))->ilc_comment = NULL;

	initAppendCount();		/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が3であること", getAppendCount() == 3 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が3であること", ilcdata.num == 3 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 1/3",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 2/3",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 3/3",
				 strcmp( "0:src001.c:func2:33", ilcdata.coverage[2] ) == 0 );

	/* 未登録のデータが登録されること (func3のilc_comment復活) */
	((ILC_FUNC_BODY*)(func3->body))->ilc_comment = comm4;

	initAppendCount();		/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが正常終了すること", ret == 0 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が1であること", getAppendCount() == 1 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が4であること", ilcdata.num == 4 );
	ILUT_ASSERT( "登録済みのデータが変更されていないこと 1/3",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "登録済みのデータが変更されていないこと 2/3",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );
	ILUT_ASSERT( "登録済みのデータが変更されていないこと 3/3",
				 strcmp( "0:src001.c:func2:33", ilcdata.coverage[2] ) == 0 );
	ILUT_ASSERT( "未登録のデータが登録されていること 1/1",
				 strcmp( "0:src001.c:func3:44", ilcdata.coverage[3] ) == 0 );

	/* 後始末 */
	xfree( ilcdata.coverage[0] );
	xfree( ilcdata.coverage[1] );
	xfree( ilcdata.coverage[2] );
	xfree( ilcdata.coverage[3] );
	xfree( ilcdata.coverage );
	ilcdata.num = 0;
	ilcdata.coverage = NULL;


	/* 準正常系：登録用文字列のメモリ確保に失敗 */
	setCreateCount( 4 );		/* xmallocが4回のみ成功。ILCカバレッジデータ２個分
								   (文字列確保、ILC_Append内、文字列確保、ILC_Append内) */
	initAppendCount();			/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが異常終了すること", ret == -1 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が2であること", getAppendCount() == 2 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が2であること", ilcdata.num == 2 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 1/2",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 2/2",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );



	/* 準正常系：ILC_Appendに失敗 */
	setCreateCount( 3 );		/*- xmallocが3回のみ成功(文字列確保)
								 * 文字列確保(登録済み) -> 文字列確保(登録済み) ->
								 * 文字列確保(未登録) -> ILC_Appendで実行される */
	initAppendCount();			/* ILC_Append呼び出し回数の初期化 */
	ret = ilc2ilcdata( &ilc, &ilcdata );

	ILUT_ASSERT( "ilc2ilcdataが異常終了すること", ret == -1 );
	ILUT_ASSERT( "ILC_Append呼び出し回数が1であること", getAppendCount() == 1 );
	ILUT_ASSERT( "ILCカバレッジデータの登録数が2であること", ilcdata.num == 2 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 1/2",
				 strcmp( "0:src001.c:func1:11", ilcdata.coverage[0] ) == 0 );
	ILUT_ASSERT( "ILCカバレッジデータに登録されていること 2/2",
				 strcmp( "0:src001.c:func2:22", ilcdata.coverage[1] ) == 0 );


	/* 終了処理 */
	xfree( func1->body );
	xfree( func1 );
	xfree( func2->body );
	xfree( func2 );
	xfree( func3->body );
	xfree( func3 );
	xfree( comm1->body );
	xfree( comm1 );
	xfree( comm2->body );
	xfree( comm2 );
	xfree( comm3->body );
	xfree( comm3 );
	xfree( comm4->body );
	xfree( comm4 );
	xfree( ilcdata.coverage[0] );
	xfree( ilcdata.coverage[1] );
	xfree( ilcdata.coverage );

	return ILUT_SUCCESS;
}


int main (
	int argc,
	char** argv
)
{
	/**/
	static ILUT_TestCase test[] = {
		DEF_TEST(test_ilcfunc_create),
		DEF_TEST(test_ilcfunc_init),
		DEF_TEST(test_ilcfunc_append),
		DEF_TEST(test_ilcfunc_search),
		DEF_TEST(test_ilcfunc_remove),
		DEF_TEST(test_ilccomment_create),
		DEF_TEST(test_ilccomment_init),
		DEF_TEST(test_ilccomment_append),
		DEF_TEST(test_ilccomment_remove),
		DEF_TEST(test_ilc_init),
		DEF_TEST(test_ilc_end),
		DEF_TEST(test_ilc_append_coverage),
		DEF_TEST(test_ilc_put_coverage),
		DEF_TEST(test_ilc2ilcdata),
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
	ILUT_ResultOut( out, "ilc_util", test );
	fclose( out );

	ILC_Finalize();

	return ret;

}

