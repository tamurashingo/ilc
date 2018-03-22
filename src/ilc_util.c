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
 * @file	ilc_util.c
 * @brief	ilc utility
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-11
 * @version	$Id: ilc_util.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include <string.h>
#include "ilc.h"
#include "ilc_util.h"
#include "ilc_util_local.h"
#include "util.h"

/** RCSID */
static const char rcsid[] = "@(#) $Id: ilc_util.c,v 1.2 2008/05/25 13:22:49 shingo Exp $";


/**
 * ILCデータの初期化
 * @param ILC*
 */
void ilc_init (
	ILC* ilc
)
{
	/**/
	/**/
	/* ILC: ilc_init開始 */

	ilc->file_in  = NULL;
	ilc->file_out = NULL;
	ilc->fpin     = stdin;
	ilc->fpout    = stdout;
	ilc->ilc_func = NULL;

	/* ILC: ilc_init終了 */
}

/**
 * ILCデータの終了処理
 * メモリ解放のみを行い、ファイルポインタのクローズは行わない。
 * @param ILC*
 */
void ilc_end (
	ILC* ilc
)
{
	/**/
	/**/
	/* ILC: ilc_end開始 */

	while ( ilc->ilc_func != NULL ) {
		/* ILC: ilc_funcループ中 */
		while ( ((ILC_FUNC_BODY*)(ilc->ilc_func->body))->ilc_comment != NULL ) {
			/* ILC: ilc_commentループ中 */
			ilccomment_remove( &((ILC_FUNC_BODY*)(ilc->ilc_func->body))->ilc_comment );
		}
		ilcfunc_remove( &(ilc->ilc_func) );
	}


	/* ILC: ilc_end終了 */
}

/**
 * ILC_FUNCの作成
 * @return SLIST* ILC_FUNC_BODY付きのSLIST（初期化済み）
 *                NULL: 作成に失敗
 */
SLIST* ilcfunc_create (
)
{
	/**/
	SLIST* ilc_func;
	ILC_FUNC_BODY* body;
	/**/
	/* ILC: ilcfunc_create開始 */

	ilc_func = slist_create();
	if ( ilc_func != NULL ) {
		/* ILC: bodyの作成 */
		body = ilcfuncbody_create();
		if ( body != NULL ) {
			/* ILC: bodyを設定し、ilcfuncを初期化する */
			ilc_func->body = body;
			ilcfunc_init( ilc_func );
		}
		else {
			/* ILC: bodyの作成に失敗→メモリ解放 */
			xfree( ilc_func );
			ilc_func = NULL;
		}
	}

	/* ILC: ilcfunc_create終了 */
	return ilc_func;
}


/**
 * ILC_FUNCの初期化
 * @param SLIST*
 */
void ilcfunc_init (
	SLIST* ilc_func
)
{
	/**/
	ILC_FUNC_BODY* body;
	/**/
	/* ILC: ilcfunc_init開始 */

	if ( ilc_func != NULL ) {
		/* ILC: NULLチェック */
		ilc_func->next = NULL;
		body = (ILC_FUNC_BODY *)ilc_func->body;
		if ( body != NULL ) {
			/* ILC: bodyがあるときだけbodyを初期化 */
			body->func_name = NULL;
			body->count = 0;
			body->ilc_comment = NULL;
		}
	}

	/* ILC: ilcfunc_init終了 */
}




/**
 * ILC_FUNCリストの末尾に要素を追加する。
 * @param SLIST** 追加先のILC_FUNCリスト
 * @param SLIST*  追加するILC_FUNC要素
 */
void ilcfunc_append (
	SLIST** dst,
	SLIST*  src
)
{
	/**/
	/**/
	/* ILC: ilcfunc_append開始 */

	slist_append( dst, src );

	/* ILC: ilcfunc_append終了 */
}


/**
 * ILC_FUNCリストに登録されている関数名を検索する。
 * @param SLIST* 検索対象のILC_FUNCリスト
 * @param const char* 検索する関数名
 * @return SLIST* 検索対象の関数名が登録されているILC_FUNC要素
 *                NULL : 見つかりませんでした。
 */
SLIST* ilcfunc_search (
	SLIST* ilc_func,
	char* func_name
)
{
	/**/
	SLIST* list;
	/**/
	/* ILC: ilcfunc_search開始 */

	list = slist_search( ilc_func, local_ilcfunc_search, func_name );

	/* ILC: ilcfunc_search終了 */
	return list;
}


/**
 * ILC_FUNCを解放する
 * BODYのILC_COMMENTは解放しないので、事前に解放しておくこと。
 * @param SLIST** 解放するILC_FUNC要素
 */
void ilcfunc_remove (
	SLIST** ilc_func
)
{
	/**/
	/**/
	/* ILC: ilcfunc_remove開始 */

	if ( ilc_func != NULL ) {
		/* ILC: NULLチェック */
		if ( *ilc_func != NULL ) {
			/* ILC: NULLチェック */
			xfree( (*ilc_func)->body );
			slist_remove( ilc_func );
		}
	}

	/* ILC: ilcfune_remove終了 */
}


/**
 * ILC_COMMENTの作成
 * @return SLIST* ILC_COMMENT_BODY付きのSLIST（初期化済み）
 *                NULL: 作成に失敗
 */
SLIST* ilccomment_create (
)
{
	/**/
	SLIST* ilc_comment;
	ILC_COMMENT_BODY* body;
	/**/
	/* ILC: ilccomment_create開始 */

	ilc_comment = slist_create();
	if ( ilc_comment != NULL ) {
		/* ILC: bodyの作成 */
		body = ilccommentbody_create();
		if ( body != NULL ) {
			/* ILC: bodyを設定し、ilccommentを初期化する */
			ilc_comment->body = body;
			ilccomment_init( ilc_comment );
		}
		else {
			/* ILC: bodyの作成に失敗→メモリ解放 */
			xfree( ilc_comment );
			ilc_comment = NULL;
		}
	}

	/* ILC: ilccomment_create終了 */
	return ilc_comment;
}


/**
 * ILC_COMMENTの初期化
 * @param SLIST*
 */
void ilccomment_init (
	SLIST* ilc_comment
)
{
	/**/
	ILC_COMMENT_BODY* body;
	/**/
	/* ILC: ilccomment_init開始 */

	if ( ilc_comment != NULL ) {
		/* ILC: NULLチェック */
		ilc_comment->next = NULL;
		body = (ILC_COMMENT_BODY *)ilc_comment->body;
		if ( body != NULL ) {
			/* ILC: bodyがあるときだけ初期化 */
			body->line = 0;
		}
	}

	/* ILC: ilccomment_init終了 */
}


/**
 * ILC_COMMENTリストの末尾に要素を追加する。
 * @param SLIST** 追加先のILC_COMMENTリスト
 * @param SLIST*  追加するILC_COMMENT要素
 */
void ilccomment_append (
	SLIST** dst,
	SLIST*  src
)
{
	/**/
	/**/
	/* ILC: ilccomment_append開始 */

	slist_append( dst, src );

	/* ILC: ilccomment_append終了 */
}


/**
 * ILC_COMMENTを解放する
 * @param SLIST** 解放するILC_COMMENT要素
 */
void ilccomment_remove (
	SLIST** ilc_comment
)
{
	/**/
	/**/
	/* ILC: ilccomment_remove開始 */

	if ( ilc_comment != NULL ) {
		/* ILC: NULLチェック */
		if ( *ilc_comment != NULL ) {
			/* ILC: NULLチェック */
			xfree( (*ilc_comment)->body );
			slist_remove( ilc_comment );
		}
	}

	/* ILC: ilccomment_remove終了 */
}


/**
 * ILC_FUNC_BODYの作成
 * @return ILC_FUNC_BODY* 作成したILC_FUNC_BODY
 *                        NULL: 作成に失敗
 */
static ILC_FUNC_BODY* ilcfuncbody_create (
)
{
	/**/
	ILC_FUNC_BODY* body;
	/**/
	/* ILC: ilcfuncbody_create開始 */

	body = (ILC_FUNC_BODY*) xmalloc( sizeof ( ILC_FUNC_BODY ) );

	/* ILC: ilcfuncbody_create終了 */
	return body;
}


/**
 * ILC_COMMENT_BODYの作成
 * @return ILC_COMMENT_BODY* 作成したILC_COMMENT_BODY
 *                           NULL: 作成に失敗
 */
static ILC_COMMENT_BODY* ilccommentbody_create (
	)
{
	/**/
	ILC_COMMENT_BODY* body;
	/**/
	/* ILC: ilccommentbody_create開始 */

	body = (ILC_COMMENT_BODY*) xmalloc( sizeof ( ILC_COMMENT_BODY ) );

	/* ILC: ilccommentbody_create終了 */
	return body;
}


/**
 * ILC_FUNCの検索
 * BODYに登録してある関数名が一致するかを判定する
 * @param const SLIST* 検索対象のILC_FUNC
 * @param const void*  検索する文字列(char*であること)
 * @return int         0:一致せず
 *                     1:一致
 */
static int local_ilcfunc_search (
	const SLIST* ilc_func,
	const void* func_name
)
{
	/**/
	int ret = 0;	/**< 0:一致せず で初期化 */
	/**/
	/* ILC: local_ilcfunc_search開始 */

	if ( ilc_func != NULL && func_name != NULL ) {
		/* ILC: 検索の実施 */

		/* createのときにbodyをチェックしているので、NULLでないはず */
		if ( strcmp(((ILC_FUNC_BODY*)(ilc_func->body))->func_name, (char*)func_name) == 0 ) {
			/* ILC: strcmpで比較し、一致した場合 */
			ret = 1;
		}
	}

	/* ILC: local_ilcfunc_search終了 */
	return ret;
}


/**
 * カバレッジ検出ポイントのリスト追加
 * @param SLIST**     ILC_FUNCのリスト
 * @param const char* ILCコメントを検出した関数名
 * @param int         ILCコメントを検出した行
 * @return int  0:正常
 *             -1:異常
 */
int ilc_append_coverage (
	SLIST** ilc_func,
	const char* func_name,
	int line
)
{
	/**/
	SLIST *p_func = NULL;
	SLIST *p_comment = NULL;
	int ret = -1;					/**< 異常で初期化 */
	/**/
	/* ILC: ilc_append_coverage開始 */

	/* コメントデータの作成 */
	p_comment = ilccomment_create();
	if ( p_comment != NULL ) {
		/* ILC: コメントの設定 */
		((ILC_COMMENT_BODY*)(p_comment->body))->line = line;

		/* 関数がリストに登録済みかを調べる */
		p_func = slist_search( *ilc_func, local_ilcfunc_search, func_name );

		if ( p_func == NULL ) {
			/* ILC: リストに未登録のため、関数情報を作成する */
			p_func = ilcfunc_create();
			if ( p_func != NULL ) {
				/* ILC: 作成した関数情報をリストに登録する */
				((ILC_FUNC_BODY*)(p_func->body))->func_name = (char*)func_name;
				ilcfunc_append( ilc_func, p_func );
			}
			else {
				/* ILC: 関数情報の作成に失敗したため、作成済みのコメント情報を解放する */
				xfree( p_comment );
				p_comment = NULL;
			}
		}

		if ( p_func != NULL ) {
			/* ILC: コメントデータの追加 */
			((ILC_FUNC_BODY*)(p_func->body))->count += 1;
			ilccomment_append( &(((ILC_FUNC_BODY*)(p_func->body))->ilc_comment), p_comment );

			ret = 0;
		}
	}

	/* ILC: ilc_append_coverage終了 */
	return ret;
}


/**
 * カバレッジ検出コードの出力
 * @param FILE*       出力先
 * @param const char* ソースファイル名
 * @param const char* 関数名
 * @param const int   検出行
 */
void ilc_put_coverage (
	FILE* fout,
	const char* src_name,
	const char* func_name,
	const int line
)
{
	/**/
	/**/
	/* ILC: ilc_put_coverage開始 */

	if ( fout != NULL && src_name != NULL && func_name != NULL ) {
		/* ILC: 念のためにNULLポインタをガード */
		fprintf( fout, COVERAGECODE, src_name, func_name, line );
	}

	/* ILC: ilc_put_coverage終了 */
}


/**
 * ILCデータをILCカバレッジデータに変換する
 * @param ILC*      変換元のILCデータ
 * @param ILC_DATA* 変換後のILCカバレッジデータ
 * @return  0:正常終了
 *         -1:異常終了（メモリ確保エラー）
 */
int ilc2ilcdata (
	ILC* ilc,
	ILC_DATA* ilc_data
)
{
	/**/
	SLIST* func;
	SLIST* comment;
	size_t fname_len;
	int ret = 0;
#define FUNC(x) ((ILC_FUNC_BODY*)((x)->body))
#define COMMENT(x) ((ILC_COMMENT_BODY*)((x)->body))
	int breakflg;		/* 0:トップのループを継続 0以外:トップのループを終了 */
	/**/
	/* ILC: ilc2ilcdata開始 */

	fname_len = strlen(ilc->file_in);

	for ( func = ilc->ilc_func, breakflg = 0; func != NULL && breakflg == 0 ; func = func->next ) {
		/**/
		size_t func_len;	/* 関数名の長さ */
		/**/
		/* ILC: 関数名でループ */
		func_len = strlen( FUNC(func)->func_name );

		for ( comment = ((ILC_FUNC_BODY*)(func->body))->ilc_comment;
              comment != NULL;
              comment = comment->next ) {
			/**/
			char *buf;		/* ILCカバレッジデータに登録するためのバッファ */
							/* フラグ:ファイル名:関数名:行数 */
			/**/
			/* ILC: 登録文字列を作成する。ILCカバレッジデータに未登録であれば、登録する。 */
			/*-
			 * 3  : ':' x 3
			 * 1  : フラグサイズ
			 * 10 : 行数の最大桁数(unsigned long)
			 * 1  : '\0'
			 */
			buf = (char*)xmalloc( fname_len + func_len + 3 + 1 + 10 + 1 );
			if ( buf != NULL ) {
				/* ILC: 登録用文字列の作成 */
				sprintf( buf, "0:%s:%s:%ld", ilc->file_in, FUNC(func)->func_name, COMMENT(comment)->line );
				if ( ILC_Search( ilc_data, buf + 2 ) == NULL ) {
					/* ILC: `0:' を飛ばしたものを渡す必要がある */
					/* 未登録なので、ILCカバレッジデータに登録する */
					if ( ILC_Append( ilc_data, buf ) == ILC_FAILURE ) {
						/* ILC: 登録に失敗したため、トップレベルのループを終了 */
						xfree( buf );
						breakflg = 1;
						ret = -1;
						break;
					}
				}
				else {
					/* ILC:登録済みの場合は、使用しないため解放する */
					xfree( buf );
				}
			}
			else {
				/* ILC: 登録に失敗したため、トップレベルのループを終了 */
				breakflg = 1;
				ret = -1;
				break;
			}
		}
	}

	/* ILC: ilc2ilcdata終了 */
	return ret;
}




