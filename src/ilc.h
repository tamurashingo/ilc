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
 * @file	ilc.h
 * @brief	ilc
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-03-18
 * @version	$Id: ilc.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _ILC_H_
#define _ILC_H_

/**
 * ILCカバレッジデータ
 *
 */
typedef struct _ilc_data {
	char*		filename;		/**< ファイル名 */
	char**		coverage;		/**< カバレッジデータ */
	long		num;			/**< カバレッジデータの数 */
}
ILC_DATA;

/**
 *
 *
 */
typedef enum _ILC_ERROR {
	ILC_SUCCESS = 0,
	ILC_WARN = 1,
	ILC_FAILURE = 2
}
ILC_ERROR;

/*-
 * カバレッジデータの構造
 * char* coverage[0 ... n]
 *
 * coverage[0] = "0:test.c:test:10"
 * coverage[1] = "0:test.c:test:20"
 * coverage[n] = "......."
 *
 * フラグ、ファイル名、関数名、行数を文字列で持つ。
 *
 */


/**
 * ファイルのILCカバレッジデータをメモリに展開する
 * @param const char* ILCカバレッジデータファイル名
 *                    NULLの場合、またはファイルが存在しない場合は ilc.dat を読み込む
 * @return ILC_ERROR  ILC_SUCCESS: 成功
 *                    ILC_WARN   : 指定したファイルがないため、 ilc.dat を使用（続行可能）
 *                    ILC_FAILURE: メモリ展開に失敗（続行不可）
 *
 */
ILC_ERROR ILC_Initialize ( const char* );

/**
 * メモリのILCカバレッジデータをファイルに書き込む
 *
 */
ILC_ERROR ILC_Finalize ( );

/**
 * カバレッジ検出ポイント通過のフラグをたてる
 * @param const char* チェックポイントに設定してある文字列
 *                    ファイル名:関数名:行数
 */
void __ilc_check ( const char* );


/**
 * ILCカバレッジデータで保持している文字列を検索する
 * 「フラグ:ファイル名:関数名:行数」を保持している中の、
 * 「ファイル名:関数名:行数」を検索対象とする。
 * 検索結果はフラグの位置を返す。
 * @param ILC_DATA*   ILCカバレッジデータ
 * @param const char* 検索対象の文字列(ファイル名:関数名:行数)
 * @return            検索結果(フラグ)
 */
char* ILC_Search( ILC_DATA*, const char* );


/**
 * ILCカバレッジデータに、指定したデータを追加する
 * @param ILC_DATA* ILCカバレッジデータ
 * @param char*     追加するデータ
 * @return ILC_SUCCESS:正常終了
 *         ILC_FAILURE:メモリ確保エラー
 */
ILC_ERROR ILC_Append ( 	ILC_DATA*, 	char* );


/**
 * staticで保持しているILCカバレッジデータのアドレスを得る
 * @return ILC_DATA*
 */
ILC_DATA* ILC_GetILCData ( );


#endif /* _ILC_H_ */

