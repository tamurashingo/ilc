/**
 * @file	util_stub.c
 * @brief	util.cのスタブ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-19
 * @version	$Id: util_stub.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include "util.h"

/** xmallocがNULLを返すまでの回数
 *  0になったときにNULLを返し、以降NULLを返し続ける。
 * -1の場合、常にmallocしたアドレスを返す。
 */
static int CreateCount = -1;

/** xfreeが呼ばれた回数 */
static int RemoveCount = 0;

int getCreateCount() { return CreateCount; }
void setCreateCount(int cnt) { CreateCount = cnt; }

int getRemoveCount() { return RemoveCount; }
void setRemoveCount(int cnt) {RemoveCount = cnt; }


/**
 * slist_createのスタブ
 */
SLIST* slist_create (
)
{
	/**/
	SLIST* list;
	/**/
	
	list = (SLIST*)xmalloc( sizeof (SLIST) );
	if ( list != NULL ) {
		list->next = NULL;
		list->body = NULL;
	}
	return list;
}


/**
 * slist_removeのスタブ
 */
void slist_remove (
	SLIST** list
)
{
	/**/
	/**/

	if ( list != NULL ) {
		if ( *list != NULL ) {
			SLIST* tmp = *list;
			*list = (*list)->next;
			xfree( tmp );
		}
	}
}

void slist_append (
	SLIST** p_dst,
	SLIST*  p_elm
)
{
	/**/
	SLIST* ptr;
	/**/

	if ( p_dst != NULL ) {
		if ( *p_dst == NULL ) {
			*p_dst = p_elm;
		}
		else {
			ptr = *p_dst;
			while ( ptr->next != NULL ) {
				ptr = ptr->next;
			}
			ptr->next = p_elm;
		}
	}
	return ;
}

SLIST* slist_search (
	SLIST* list,
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
 * xmallocのスタブ
 * 動作は本家のものと同様
 * 追加機能として、static変数：CreateCountを参照し、
 * 指定回数以上呼び出されたら、NULLを返す。
 */
void* xmalloc (
	size_t size
)
{
	/**/
	void* ptr;
	/**/

	if ( CreateCount == 0 ) {
		ptr = NULL;
	}
	else {
		/* 0になるまでカウントダウンとする */
		/* 0になったら、0のまま（NULLを返し続ける） */
		if ( CreateCount > 0 ) {
			CreateCount--;
		}

		ptr = malloc( size );
	}

	return ptr;
}

/**
 * xfreeのスタブ
 * 呼ばれた回数を static変数：RemoveCount に記録する。
 */
void xfree (
	void* ptr
)
{
	RemoveCount++;
	free( ptr );
}
