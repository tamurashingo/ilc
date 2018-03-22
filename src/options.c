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
 * @file	options.c
 * @brief	オプション関連
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-06
 * @version	$Id: options.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <unistd.h>
#include "options.h"

static const char* options_str = "f:o:hv";


/**
 * オプションを解析する。
 * @param int 引数の数
 * @param char** 引数へのポインタ
 * @param struct opt* 解析結果を格納する構造体
 */
void parse_option (
	int argc,
	char **argv,
	struct opt* opt	/* OUT */
)
{
	/**/
	int ch;
	extern char* optarg;
	extern int optind;
	extern int opterr;
	extern int optreset;
	/**/
	/* ILC: parse_option開始 */

	/* getopt初期化 */
	optreset = 1;
	optind   = 1;

	while ( (ch = getopt(argc, argv, options_str)) != -1 ) {
		/* ILC: オプション解析 */
		switch ( ch ) {
		case 'f':
			/* ILC: ILCデータファイルの指定 */
			opt->ilc_file = optarg;
			break;
		case 'o':
			/* ILC: 変換後ファイルの指定 */
			opt->out_file = optarg;
			break;
		case 'v':
			/* ILC: バージョン情報出力 */
			opt->version = 1;
			break;
		case 'h':
			/* ILC: ヘルプ */
			/* FALLTHROUGH */
		default:
			opt->help = 1;
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc > 0 ) {
		/* ILC: 入力ファイル名の設定 */
		opt->in_file = argv[0];
	}

	/* ILC: parse_option終了 */
}
