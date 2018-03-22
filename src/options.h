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
 * @file	options.h
 * @brief	オプション関連
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-06
 * @version	$Id: options.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

struct opt {
	char*	ilc_file;		/**< ILCデータファイル名 */
	char*	in_file;		/**< 変換元入力ファイル名 */
	char*	out_file;		/**< 変換後出力ファイル名 */
	int		version;		/**< バージョン情報出力 */
	int		help;			/**< ヘルプ出力 */
};


/**
 * オプションを解析する。
 * @param int         引数の数
 * @param char**      引数へのポインタ
 * @param struct opt* 解析結果を格納する構造体
 */
void parse_option ( int, char**, struct opt* );

#endif /* _OPTIONS_H_ */
