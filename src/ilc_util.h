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
 * @file	ilc_util.h
 * @brief	ilc utility
 *
 * @author	Tamura Shingo (tamura.shingo@gmail.com)
 * @date	2007-11-10
 * @version	$Id: ilc_util.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _ILC_UTIL_H_
#define _ILC_UTIL_H_

#include "ilc.h"
#include "util.h"

/** カバレッジ検出ポイントに埋め込む文字列 */
#define COVERAGECODE "*/ __ilc_check( \"%s:%s:%d\" ); /*"

/*-
 * データ構造
 * ILC         : 処理対象ファイルのすべての情報を束ねる。
 *             : ILC_FUNCのリストを持つ。
 * ILC_FUNC    : 一つの関数に対する情報を保持する。
 *             : ILC_COMMENTのリストを持つ。
 * ILC_COMMENT : 一つのコメントに対する情報を保持する。
 *
 * [ILC]
 *  |
 *  +--> [ILC_FUNC(SLIST)] --> [ILC_FUNC] --> [ILC_FUNC]
 *        |
 *        +--> [ILC_FUNC_BODY]
 *              |
 *              +--> [ILC_COMMENT(SLIST)] --> [ILC_COMMENT] --> [ILC_COMMENT]
 *                    |
 *                    +--> [ILC_COMMENT_BODY]
 *
 *
 */


/**
 * ILC_FUNC_BODYで定義しているilc_commentのbody部
 */
typedef struct _ILC_COMMENT_BODY {
	unsigned long	line;			/**< ILCコメントが出現した行数 */
}
ILC_COMMENT_BODY;


/**
 * ILC_SRCで定義しているilc_funcのbody部
 */
typedef struct _ILC_FUNC_BODY {
	char*			func_name;		/**< 関数名 */
	unsigned long	count;			/**< ILCコメントの数 */
	SLIST*			ilc_comment;	/**< ILCコメント情報のリスト */
}
ILC_FUNC_BODY;

/**
 * ILCデータ
 * 解析に必要なすべての情報を保持する。
 */
typedef struct _ilc {
	char*			file_in;		/**< 入力元ファイル名 */
	char*			file_out;		/**< 出力先ファイル名 */
	FILE*			fpin;			/**< 入力元 */
	FILE*			fpout;			/**< 出力先 */
	SLIST*   	    ilc_func;		/**< ILC情報 */
}
ILC;


/**
 * ILCデータの初期化
 * @param ILC*
 */
void ilc_init ( ILC* );

/**
 * ILCデータの終了処理
 * メモリ解放のみを行い、ファイルポインタのクローズは行わない。
 * @param ILC*
 */
void ilc_end ( ILC* );

/**
 * ILC_FUNCの作成
 * @return SLIST* ILC_FUNC_BODY付きのSLIST（要初期化）
 *                NULL: 作成に失敗
 */
SLIST* ilcfunc_create ( );

/**
 * ILC_FUNCの初期化
 * @param SLIST*
 */
void ilcfunc_init ( SLIST* );

/**
 * ILC_FUNCリストの末尾に要素を追加する。
 * @param SLIST** 追加先のILC_FUNCリスト
 * @param SLIST*  追加するILC_FUNC要素
 */
void ilcfunc_append ( SLIST**, SLIST* );

/**
 * ILC_FUNCリストに登録されている関数名を検索する。
 * @param SLIST* 検索対象のILC_FUNCリスト
 * @param const char* 検索する関数名
 * @return SLIST* 検索対象の関数名が登録されているILC_FUNC要素
 *                NULL : 見つかりませんでした。
 */
SLIST* ilcfunc_search ( SLIST*, char* );

/**
 * ILC_FUNCを解放する
 * BODYのILC_COMMENTは解放しないので、事前に解放しておくこと。
 * @param SLIST** 解放するILC_FUNC要素
 */
void ilcfunc_remove ( SLIST** );

/**
 * ILC_COMMENTの作成
 * @return SLIST* ILC_COMMENT_BODY付きのSLIST（初期化済み）
 *                NULL: 作成に失敗
 */
SLIST* ilccomment_create ( );

/**
 * ILC_COMMENTの初期化
 * @param SLIST*
 */
void ilccomment_init ( SLIST* );

/**
 * ILC_COMMENTリストの末尾に要素を追加する。
 * @param SLIST** 追加先のILC_COMMENTリスト
 * @param SLIST*  追加するILC_COMMENT要素
 */
void ilccomment_append( SLIST**, SLIST* );

/**
 * ILC_COMMENTを解放する
 * @param SLIST** 解放するILC_COMMENT要素
 */
void ilccomment_remove ( SLIST** );

/**
 * カバレッジ検出ポイントのリスト追加
 * @param SLIST**     ILC_FUNCのリスト
 * @param const char* ILCコメントを検出した関数名
 * @param int         ILCコメントを検出した行
 * @return int  0:正常
 *             -1:異常
 */
int ilc_append_coverage( SLIST**, const char*, int );

/**
 * カバレッジ検出コードの出力
 * @param FILE*       出力先
 * @param const char* ソースファイル名
 * @param const char* 関数名
 * @param const int   検出行
 */
void ilc_put_coverage ( FILE*, const char*, const char*, int );


/**
 * ILCデータをILCカバレッジデータに変換する
 * @param ILC*      変換元のILCデータ
 * @param ILC_DATA* 変換後のILCカバレッジデータ
 * @return  0:正常終了
 *         -1:異常終了（メモリ確保エラー）
 */
int ilc2ilcdata ( ILC*, ILC_DATA* );


#endif /* _ILC_UTIL_H_ */

