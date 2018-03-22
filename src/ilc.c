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
 * @file	ilc.c
 * @brief	ilc
 *
 * @author	tamura.shingo (tamura.shingo@gmail.com)
 * @date	2008-03-19
 * @version	$Id: ilc.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ilc.h"

/** RCSID */
static const char rcsid[] = "@(#) $Id: ilc.c,v 1.2 2008/05/25 13:22:49 shingo Exp $";

/* ILCカバレッジデータ */
static ILC_DATA __ilc_data;

/* デフォルトのファイル名 */
#define ILC_FILE_DEFAULT "ilc.dat"

/* ILCカバレッジデータファイルの構造 */
/* フラグ:ファイル名:関数名:行数     */
#define ILC_COVERAGE_DATA "%d:%s:%s:%d\n"

/*
 *
 * static functions
 *
 */
/**
 * 指定したファイルを開き、ilc_deployを呼び出す。
 * @param const char* ファイル名
 * @param ILC_DATA*   ILCカバレッジデータ
 * @return ILC_SUCCESS:メモリ展開完了
 *         ILC_WARN   :ファイルオープンエラー
 *         ILC_FAILURE:メモリ展開失敗
 */
static ILC_ERROR ilc_fopen ( const char*, ILC_DATA* );

/**
 * 1行読み込み、ILCカバレッジデータをメモリに展開する
 * @param FILE*     ファイルポインタ
 * @param ILC_DATA* ILCカバレッジデータ
 * @return ILC_SUCCESS:正常終了
 *         ILC_FAILURE:メモリ展開失敗
 */
static ILC_ERROR ilc_deploy ( FILE*, ILC_DATA* );

/**
 * ファイルより１行(最大LONG_MAX文字)読み込む
 * mallocでメモリを確保するため、文字列を使用しなくなった場合はfreeをすること。
 * @param FILE* ファイルポインタ
 * @param size_t* 読み込んだ文字列の長さ
 * @return 読み込んだ文字列。改行文字(CR/LF)を含まないNULL終端文字列。
 */
static char* __fgetln ( FILE*, size_t* );

/**
 * ファイルに１行書き出す
 * @param FILE*       ファイルポインタ
 * @param const char* 書き出す文字列
 */
static void ilc_fout( FILE*, const char* );

/**
 * ilc_foutの何もしない版
 * @param FILE*
 * @param const char*
 */
static void ilc_fout_null( FILE*, const char* );



/**
 * 指定したファイルを開き、ilc_deployを呼び出す。
 * @param const char* ファイル名
 * @param ILC_DATA*   ILCカバレッジデータ
 * @return ILC_SUCCESS:メモリ展開完了
 *         ILC_WARN   :ファイルオープンエラー
 *         ILC_FAILURE:メモリ展開失敗
 */
static ILC_ERROR ilc_fopen (
	const char* ilc_file,
	ILC_DATA *ilc_data
)
{
	/**/
	ILC_ERROR ret = ILC_FAILURE;
	FILE* fp;
	/**/
	/* ILC: ilc_fopen開始 */

	if ( ilc_file != NULL ) {
		/* ILC: ファイル指定あり */
		fp = fopen( ilc_file, "r" );
		if ( fp != NULL ) {
			/* ILC: ファイルの中身をメモリに展開する */
			ret = ilc_deploy( fp, ilc_data );
			fclose( fp );
		}
		else {
			/* ILC: ファイルオープンエラー */
			ret = ILC_WARN;
		}
	}

	/* ILC: ilc_fopen終了 */
	return ret;
}


/**
 * 1行読み込み、ILCカバレッジデータをメモリに展開する
 * @param FILE*     ファイルポインタ
 * @param ILC_DATA* ILCカバレッジデータ
 * @return ILC_SUCCESS:正常終了
 *         ILC_FAILURE:メモリ展開失敗
 */
static ILC_ERROR ilc_deploy (
	FILE* fp,
	ILC_DATA* ilc_data
)
{
	/**/
	char* str;						/* ファイルより取得した1行 */
	size_t len;						/* 1行の長さ(未使用) */
	ILC_ERROR ret = ILC_SUCCESS;
	/**/
	/* ILC: ilc_deploy開始 */

	while ( feof( fp ) == 0 ) {
		/* ILC: feofは正常終了。それ以外は異常終了 */

		str = __fgetln( fp, &len );
		if ( str != NULL && len != 0 ) {
			/* ILC: ILCカバレッジデータへの追加 */
			
			if ( ILC_Append( ilc_data, str ) == ILC_FAILURE ) {
				/**/
				long ix;
				/**/
				/* ILC: ILCカバレッジデータへのデータ追加に失敗 */
				for ( ix = 0; ix < ilc_data->num; ix++ ) {
					/* ILC: メモリ解放中 */
					free( (ilc_data->coverage)[ix] );
				}
				ret = ILC_FAILURE;
				break;
			}
			
		}
		else if ( len != 0 ) {
			/**/
			long ix;
			/**/
			/* ILC: ファイルからの読み込みに失敗 */
			for ( ix = 0; ix < ilc_data->num; ix++ ) {
				/* ILC: メモリ解放中 */
				free( (ilc_data->coverage)[ix] );
			}
			ret = ILC_FAILURE;
			break;
		}
		/* len == 0 は改行のみの行（e.g. 行末） */
	}

	/* ILC: ilc_deploy終了 */
	return ret;
}


/**
 * ファイルより1行(最大LONG_MAX文字)読み込む
 * mallocでメモリを確保するため、文字列を使用しなくなった場合はfreeをすること。
 * @param FILE* ファイルポインタ
 * @param size_t* 読み込んだ文字列の長さ
 * @return 読み込んだ文字列。改行文字(CR/LF)を含まないNULL終端文字列。
 *         NULLの場合は、何らかのエラー。
 */
static char* __fgetln (
	FILE* fp,
	size_t* len	/* OUT */
)
{
	/**/
#ifndef LONG_MAX
#define LONG_MAX (2147483647)
#endif /* LONG_MAX */
	char *str;
	unsigned int size = 32;		/* バッファの初期値:32 */
	unsigned int cnt;
	int ch;
	/**/
	/* ILC: __fgetln開始 */

	str = (char*) malloc( size );
 
	if ( str != NULL ) {
		/* ILC: 初期バッファ確保成功 */

		for ( cnt = 0; (ch = fgetc( fp )) != EOF ; cnt++ ) {
			/* ILC: EOFでループ終了。または\r/\nでbreak */

			if ( cnt == size ) {
				/* ILC: バッファサイズの拡大 */
				size *= 2;
				if ( size > LONG_MAX ) {
					/* ILC: 最大読み込み文字数を超えたのでエラー(NULLを返す) */
					free( str );
					str = NULL;
				}
				str = realloc( str, size );
				if ( str == NULL ) {
					/* ILC: メモリが確保できない場合はNULLを返す */
					break;
				}
			}
			if ( ch == '\r' ) {
				/* \rを検出した場合は、もう一文字読み込み改行コードを調べる */
				ch = fgetc( fp );
				if ( ch == EOF || ch == '\n' ) {
					/* EOFなら改行はCRなので、そのままbreak。\nなら改行をCRLFとみなしてbreak */
					break;
				}
				else {
					/* ILC: \n以外は改行はCRなので、読み込んだ文字を戻してbreak */
					ungetc( ch, fp );
					break;
				}
			}
			else if ( ch == '\n' ) {
				/* ILC: LFは無条件にbreak */
				break;
			}
			else {
				/* ILC: 改行文字以外なら、バッファに格納 */
				str[cnt] = (char)ch;
			}
		}
		if ( str != NULL ) {
			/* ILC: NULL終端して、長さを設定して終了 */
			str[cnt] = '\0';
			*len = (size_t)cnt;
		}
	}

	/* ILC: __fgetln終了 */
	return str;
}

/**
 * ファイルに１行書き出す
 * @param FILE*       ファイルポインタ
 * @param const char* 書き出す文字列
 */
static void ilc_fout (
	FILE* fp,
	const char* str
)
{
	/**/
	/**/
	/* ILC: ilc_fout開始 */

	fprintf( fp, "%s\n", str );

	/* ILC: ilc_fout終了 */
	return ;
}


/**
 * ilc_foutの何もしない版
 * @param FILE*
 * @param const char*
 */
static void ilc_fout_null (
	FILE* fp,
	const char* str
)
{
	/**/
	/**/
	/* ILC: ilc_fout_null開始 */
	/* ILC: ilc_fout_null終了 */
	return ;
}



/**
 * ファイルのILCカバレッジデータをメモリに展開する
 * @param const char* ILCカバレッジデータファイル名
 *                    NULLの場合、またはファイルが存在しない場合は ilc.dat を読み込む
 * @return ILC_ERROR  ILC_SUCCESS: 成功
 *                    ILC_WARN   : 指定したファイルがないため、 ilc.dat を使用（続行可能）
 *                    ILC_FAILURE: メモリ展開に失敗（続行不可）
 */
ILC_ERROR ILC_Initialize (
	const char* ilc_file
)
{
	/**/
	ILC_ERROR ret = ILC_FAILURE;
	struct _files {				/* 読み込みファイルとその戻り値のリスト */
		const char* filename;	/*   ILCカバレッジデータファイル名 */
		ILC_ERROR   ret;		/*   ILC_Initializeの戻り値 */
	}
	files[] = {
		{ ilc_file,         ILC_SUCCESS },	/* 指定されたファイル名 */
		{ ILC_FILE_DEFAULT, ILC_WARN    },	/* デフォルトのファイル名 */
		{ NULL,             ILC_FAILURE }	/* エラー */
	};
	struct _files* ptr;
	/**/
	/* ILC: ILC_Initialize開始 */

	/* ファイルがまったく存在しないときのため、デフォルト値を設定しておく */
	__ilc_data.filename = ILC_FILE_DEFAULT;

	for ( ptr = &files[0]; ptr->ret != ILC_FAILURE; ptr++ ) {
		/* ILC: ループ終了条件：ILC_FAILUREを検出 */
		if ( ilc_fopen( ptr->filename, &__ilc_data ) != ILC_FAILURE ) {
			/* ILC: open成功、もしくは新規ファイル */
			ret = ptr->ret;
			__ilc_data.filename = (char*)ptr->filename;
			break;
		}
	}

	/* ILC: ILC_Initialize終了 */
	return ret;
}



/**
 * メモリのILCカバレッジデータをファイルに書き込む
 * @return ILC_SUCCESS:正常終了
 *         ILC_WARN   :ファイル書き出し失敗
 *
 */
ILC_ERROR ILC_Finalize (
)
{
	/**/
	FILE* fp;
	void (*p_func)(FILE*, const char*);	/* ファイル書き出し用関数 */
	long ix;							/* ループカウンタ */
	ILC_ERROR ret;
	/**/
	/* ILC: ILC_Finalize開始 */

	fp = fopen( __ilc_data.filename, "w" );
	if ( fp != NULL ) {
		/* ILC: ファイルに書き出す関数。通常はこちら。 */
		p_func = ilc_fout;
		ret = ILC_SUCCESS;
	}
	else {
		/* ILC: NULL OBJECTパターン */
		p_func = ilc_fout_null;
		ret = ILC_WARN;
	}

	for ( ix = 0; ix < __ilc_data.num; ix++ ) {
		/* ILC: ファイルに1行ずつ書き出しながら、メモリ解放 */
		(*p_func)( fp, __ilc_data.coverage[ix] );
		free( __ilc_data.coverage[ix] );
	}
	free( __ilc_data.coverage );


	/* ILC: ILC_Finalize終了 */
	return ret;
}


/**
 * カバレッジ検出ポイント通過のフラグを立てる
 * @param const char* チェックポイントに設定してある文字列
 *                    ファイル名:関数名:行数
 */
void __ilc_check (
	const char* check_str
)
{
	/**/
	char* flag;
	/**/
	/* ILC: __ilc_check開始 */

	flag = ILC_Search( &__ilc_data, check_str );
	if ( flag != NULL ) {
		/* ILC: NULLはありえないが... */
		*flag = '1';
	}

	/* ILC: __ilc_check終了 */
}



/**
 * ILCカバレッジデータで保持している文字列を検索する
 * 「フラグ:ファイル名:関数名:行数」を保持している中の、
 * 「ファイル名:関数名:行数」を検索対象とする。
 * 検索結果はフラグの位置を返す。
 * @param ILC_DATA*   ILCカバレッジデータ
 * @param const char* 検索対象の文字列(ファイル名:関数名:行数)
 * @return            検索結果(フラグ)
 */
char* ILC_Search(
	ILC_DATA* ilc_data,
	const char* str
)
{
	/**/
	long ix;
	char* ilc_id;		/* ILCカバレッジデータに登録してある文字列 */
	char* ret = NULL;
	/**/
	/* ILC: ILC_Search開始 */

	for ( ix = 0; ix < ilc_data->num; ix++ ) {
		/* ILC: 検索開始 */
		ilc_id = (ilc_data->coverage)[ix];
		if ( strcmp( str, ilc_id + 2 ) == 0 ) {
			/* ILC: フラグ + ':' を飛ばし、ファイル名から検索させるため +2 で比較する */
			ret = ilc_id;
			break;
		}
	}

	/* ILC: ILC_Search終了 */
	return ret;
}


/**
 * ILCカバレッジデータに、指定したデータを追加する
 * @param ILC_DATA* ILCカバレッジデータ
 * @param char*     追加するデータ
 * @return ILC_SUCCESS:正常終了
 *         ILC_FAILURE:メモリ確保エラー
 */
ILC_ERROR ILC_Append (
	ILC_DATA* ilc_data,
	char* data
)
{
	/**/
	char** ptr;
	ILC_ERROR ret;
	/**/
	/* ILC: ILC_Append開始 */

	/* ilc_data->coverage を realloc する */
	ptr = (char**)malloc( sizeof(char*) * (ilc_data->num + 1) );
	if ( ptr != NULL ) {
		/* ILC: realloc成功 */
		if ( ilc_data->coverage != NULL ) {
			/* ILC: realloc実施 */
			memcpy( ptr, ilc_data->coverage, sizeof(char*) * ilc_data->num );
			free( ilc_data->coverage );
		}
		ptr[ilc_data->num] = data;
		ilc_data->num++;
		ilc_data->coverage = ptr;
		ret = ILC_SUCCESS;
	}
	else {
		/* ILC: realloc失敗 */
		ret = ILC_FAILURE;
	}

	/* ILC: ILC_Append終了 */
	return ret;
}


/**
 * staticで保持しているILCカバレッジデータのアドレスを得る
 * @return ILC_DATA*
 */
ILC_DATA* ILC_GetILCData (
)
{
	/**/
	/**/
	/* ILC: ILC_GetILCData開始 */

	/* ILC: ILC_GetILCData終了 */
	return &__ilc_data;
}
