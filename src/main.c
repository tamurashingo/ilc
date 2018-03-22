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
 * @file main.c
 * @brief ILCメイン処理
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-06
 * @version $Id: main.c,v 1.3 2008/05/25 13:37:06 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include <string.h>
#include "ilc.h"
#include "ilc_util.h"
#include "parser.h"
#include "options.h"
#include "version.h"


void usage ()
{
  /* ILC: begin usage() */
  fputs("usage: ilc [options] file\n", stdout);
  fputs("  Options are as follows:\n", stdout);
  fputs("  -h           display this help\n", stdout);
  fputs("  -v           display version info\n", stdout);
  fputs("  -f datafile  coverage data file\n", stdout);
  fputs("  -o outfile   output file\n", stdout);

  /* ILC: end usage() */
}        

void version()
{
  /* ILC: begin version() */
  fprintf( stdout, "This is ilc version %d.%d\n", MAJOR_VERSION, MINOR_VERSION );
  fputs(           "Copyright (C) 2007,2011 tamura.shingo\n", stdout );
  /* ILC: end version() */
}





/**
 * 変換元入力ファイル名をもとに、変換後出力ファイル名を自動決定する。
 * ex) test.c => test_ilc.c
 * @param const char* 変換元入力ファイル名
 * @return            変換後出力ファイル名
 *                    NULL:メモリ確保エラー
 */
char* set_outfile (
	const char* infile
)
{
	/**/
	char* outfile;		/**< 出力ファイル名 */
	char* suf;			/**< 拡張子の位置 */
	/**/
	/* ILC: set_outfile開始 */

	/* 4 は _ilc 付与分 */
	outfile = (char*)xmalloc( strlen(infile) + 4 + 1 );

	if ( outfile != NULL ) {
		/* ILC: ファイル名のバッファを確保できた場合 */

		/* ドットの位置を後ろから検出する */
		suf = strrchr( infile, '.' );

		if ( suf != NULL ) {
			/**/
			const char* src;
			char* dst;
			/**/
			/* ILC: 拡張子のあるファイル名の場合 */
			
			/* ドットの直前までをコピー */
			for ( src = infile, dst = outfile; src != suf; *dst++ = *src++ ) {
				/* ILC: コピー中 */
			}
			
			/* _ilc を付与 */
			strcat( dst, "_ilc" );
			dst += 4;
			
			/* ドット以降をコピー */
			strcat( dst, src );
		}
		else {
			/* ILC: 拡張子のないファイル名の場合 */
			strcpy( outfile, infile );
			strcat( outfile, "_ilc" );
		}
	}

	/* ILC: set_outfile終了 */
	return outfile;
}


/**
 * 初期処理
 * 以下の処理を実施する
 * ・引数の解析
 * ・ILCデータへの入出力ファイルの設定
 * ・カバレッジデータの読み込み
 * @param int    引数の数
 * @param char** 引数のアドレス
 * @param ILC*   ILCデータ
 * @return ILC_SUCCESS:継続可（カバレッジデータファイルの指定有り）
 *         ILC_WARN   :継続可（カバレッジデータファイルの指定無し）
 *         ILC_FAILURE:継続不可
 */
ILC_ERROR init (
	int argc,
	char** argv,
	ILC* ilc
)
{
	/**/
	struct opt opt;
	ILC_ERROR ret;
	/**/
	/* ILC: init開始 */

	/* 引数の解析 */
	memset( &opt, 0, sizeof(opt) );
	parse_option( argc, argv, &opt );

	if ( opt.version == 1 ) {
		/* ILC: -vオプションが指定されたため、バージョン情報を表示 */
		version();
		ret = ILC_FAILURE;
	}
	else if ( opt.in_file == NULL || opt.help == 1 ) {
		/* ILC: 入力ファイルの指定がない、または-hオプションが指定されたため、使い方を表示 */
		usage();
		ret = ILC_FAILURE;
	}
	else {
		/* ILC: 通常の変換処理はこちら */

		if ( opt.out_file == NULL ) {
			/* ILC: 変換後出力ファイル名を自動設定する */
			opt.out_file = set_outfile( opt.in_file );
		}

		ilc->file_in  = opt.in_file;
		ilc->file_out = opt.out_file;
		ilc->ilc_func = NULL;
		ilc->fpin     = fopen( ilc->file_in, "r" );
		ilc->fpout    = fopen( ilc->file_out, "w" );

		if ( ilc->fpin != NULL && ilc->fpout != NULL ) {
			/* ILC: 正常系 ILCカバレッジデータの読み込み */
			ret = ILC_Initialize( opt.ilc_file );
		}
		else {
			/* ILC: 変換元ファイル/変換後ファイルのオープンに失敗 */
			/* オープン中のファイルはfinalizeでクローズするため、ここでは何もしない */
			ret = ILC_FAILURE;
		}
	}

	/* ILC: init終了 */
	return ret;
}


/**
 * 終了処理
 * @param ILC* ILCデータ
 * @param int  1:ILC_Finalizeを呼ぶ
 *             0:ILC_Finalizeを呼ばない（initでエラーが発生したため）
 * @return  0:正常終了
 *         -1:カバレッジデータの書き込みに失敗
 */
int finalize (
	ILC* ilc,
	int outflag
)
{
	/**/
	int ret = 0;
	/**/
	/* ILC: finalize開始 */

	/* メモリ解放 */
	ilc_end( ilc );

	if ( ilc->fpin != NULL ) {
		/* ILC: 変換元入力ファイルのクローズ */
		fclose( ilc->fpin );
	}

	if ( ilc->fpout != NULL ) {
		/* ILC: 変換後出力ファイルのクローズ */
		fclose( ilc->fpout );
	}

	if ( outflag == 1 ) {
		/* ILC: カバレッジデータを吐き出す */
		if ( ILC_Finalize() != ILC_SUCCESS ) {
			/* ILC: カバレッジデータの出力に失敗 */
			ret = -1;
		}
	}

	/* ILC: finalize終了 */
	return ret;
}


/**
 * メイン処理
 * @param int 引数の数
 * @param char** 引数のアドレス
 * @return  0:正常終了
 *         -1:異常終了
 */
int conv_main (
	int argc,
	char** argv
)
{
	/**/
	int ret = -1;		/* 異常状態で初期化しておく。 */
	ILC ilc;
	int outflag = 0;	/* 終了処理でカバレッジデータを出力しない */
	/**/
	/* ILC: conv_main開始 */

	/* ILCデータの初期化 */
	ilc.file_in  = NULL;
	ilc.file_out = NULL;
	ilc.fpin     = NULL;
	ilc.fpout    = NULL;
	ilc.ilc_func = NULL;

	if ( init( argc, argv, &ilc ) != ILC_FAILURE ) {
		/* ILC: 初期化に成功したので変換処理を行います */

		ret = parse( &ilc );
		switch ( ret ) {
		case 0:
			/* ILC: 正常系動作 */
			if ( ilc2ilcdata( &ilc, ILC_GetILCData() ) == 0 ) {
				/* ILC: 正常終了。終了処理でカバレッジデータを出力する */
				outflag = 1;
				ret = 0;
			}
			else {
				/* ILC: メモりエラー */
				fprintf( stderr, "メモリ確保に失敗しました。\n" );
			}
			break;
		case 1:
			/* ILC: 構文エラー */
			fprintf( stderr, "構文解析に失敗したので中断します。\n" );
			break;
		case 2:
			/* ILC: メモリエラー */
			fprintf( stderr, "メモリ確保に失敗したので中断します。\n" );
			break;
		default:
			/* ILC: ここにはこない */
			fprintf( stderr, "ありえないエラーが発生したので中断します。エラーコード=%d\n", ret );
			break;
		}
	}

	if ( finalize( &ilc, outflag ) != 0 ) {
		/* ILC: カバレッジデータの出力に失敗 */
		fprintf( stderr, "カバレッジデータの書き出しに失敗しました。\n" );
		ret = -1;
	}


	/* ILC: conv_main終了 */
	return ret;
}


int main (
	int argc,
	char** argv
)
{
	/**/
	int ret;
	/**/
	/* ILC: main開始 */

	ret = conv_main( argc, argv );

	/* ILC: main終了 */
	return ret;

}


