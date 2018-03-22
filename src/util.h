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
 * @file	util.h
 * @brief	共通ユーティリティ 外部公開用ヘッダ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-10-19
 * @version	$Id: util.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * 一方向リスト
 */
typedef struct _SLIST {
	struct _SLIST*	next;		/**< 次のELEMENT */
	void*			body;		/**< ELEMENTの中身 */
}
SLIST;

#define SLIST_NEXT(p)	(p->next)
#define SLIST_BODY(p)	(p->body)

/**
 * SLISTを作成する。
 * @return SLIST* 作成したSLIST
 *                NULL: 作成に失敗
 */
SLIST* slist_create( void );

/**
 * SLISTをリストの末尾に追加する。
 * @param SLIST** 追加先のリスト
 * @param SLIST*  追加するLIST
 */
void slist_append( SLIST**, SLIST * );

/**
 * リストの要素を検索。
 * @param SLIST* 検索対象となるリスト
 * @param int(*) 検索を実行する関数
 * @return SLIST* 検索で見つかった要素。
 *                NULL：見つかりませんでした。
 */
SLIST* slist_search( SLIST*, int(*)(const SLIST*, const void*), const void* );

/**
 * リストの要素を削除する。
 * @param SLIST** 削除対象の要素
 */
void slist_remove( SLIST** );


/**
 * mallocのラッパー
 * @param size_t メモリを確保するバイト数
 * @return void * 確保したメモリのアドレス
 */
void* xmalloc( size_t );

/**
 * freeのラッパー
 * @param void * 解放するメモリのアドレス
 */
void xfree( void * );


#endif /* _UTIL_H_ */

