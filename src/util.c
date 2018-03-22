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
 * @file	util.c
 * @brief	共通ユーティリティ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-10-19
 * @version	$Id: util.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

/** RCSID */
static const char rcsid[] ="$Id: util.c,v 1.2 2008/05/25 13:22:49 shingo Exp $";

/**
 * SLISTを作成する
 * @return SLIST* 作成したSLIST
 *                NULL: 作成に失敗
 */
SLIST* slist_create (
)
{
	/**/
	SLIST* list;
	/**/
	/* ILC: slist_create開始 */

	list = (SLIST *) xmalloc( sizeof (SLIST) );
	if ( list != NULL ) {
		/* ILC: 初期化 */
		list->next = NULL;
		list->body = NULL;
	}

	/* ILC: slist_create終了 */
	return list;
}

/**
 * SLISTをリストの末尾に追加する。
 * @param SLIST** 追加先のリスト
 * @param SLIST*  追加するSLIST
 */
void slist_append (
	SLIST** p_dst,
	SLIST*  p_elm
)
{
	/**/
	SLIST* ptr;
	/**/
	/* ILC: slist_append開始 */

	if ( p_dst != NULL ) {
		/* ILC: */
		if ( *p_dst == NULL ) {
			/* ILC: 先頭の要素がない時は、そのまま設定 */
			*p_dst = p_elm;
		}
		else {
			/* ILC: 追加モード */
			ptr = *p_dst;
			while ( ptr->next != NULL ) {
				ptr = ptr->next;
			}
			ptr->next = p_elm;
		}
	}
	/* ILC: slist_append終了 */
	return ;
}

/**
 * リストの要素を検索。
 * @param SLIST* 検索対象となるリスト
 * @param int(*) 検索を実行する関数
 * @param void*  関数に渡す引数
 * @return SLIST* 検索で見つかった要素。NULL：見つかりませんでした。
 */
SLIST* slist_search (
	SLIST*  list,
	int (*func)(const SLIST*, const void*),
	const void* arg
)
{
	/**/
	SLIST* ret = NULL;
	/**/
	/* ILC: slist_search開始 */

	if ( func != NULL ) {
		/* ILC: 検索関数が有効 */
		while ( list != NULL ) {
			/* ILC: リスト検索中 */
			if ( (*func)(list, arg) ) {
				/* ILC: hit! */
				ret = list;
				break;
			}
			else {
				/* ILC: 次の要素 */
				list = list->next;
			}
		}
	}
		
	/* ILC: slist_search終了 */
	return ret;
}

/**
 * リストの要素を削除する。
 * @param SLIST** 削除対象の要素
 */
void slist_remove (
	SLIST** list
)
{
	/**/
	/**/
	/* ILC: slist_remove開始 */
	if ( list != NULL ) {
		/* ILC: */
		if ( *list != NULL ) {
			/* ILC: メモリ解放 */
			SLIST* tmp = *list;
			*list = (*list)->next;
			xfree( tmp );
		}
	}

	/* ILC: slist_remove終了 */
}




/**
 * mallocラッパー。
 * @param size_t mallocのサイズ
 * @return void* mallocが確保したアドレス
 */
void *xmalloc (
	size_t size
)
{
	/**/
	void *ptr;
	/**/
	/* ILC: xmalloc開始 */

	ptr = malloc( size );

	/* ILC: xmalloc終了 */
	return ptr;
}


/**
 * freeのラッパー。
 * @param void* freeするアドレス
 */
void xfree (
	void *ptr
)
{
	/**/
	/**/
	/* ILC: xfree開始 */

	free( ptr );

	/* ILC: xfree終了 */
}

