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
 * @file	scan.h
 * @brief	スキャナが返す値の定義
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2007-11-08
 * @version	$Id: scan.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _SCAN_H_
#define _SCAN_H_

#include <stdio.h>

/** 関数名 or 変数名 */
#define LL_ID			(256)
/** カッコ{ */
#define	LL_BRACE_L		(257)
/** カッコ閉じ} */
#define	LL_BRACE_R		(258)
/** カッコ( */
#define	LL_PARENTHIS_L	(259)
/** カッコ閉じ) */
#define	LL_PARENTHIS_R	(260)
/** セミコロン */
#define LL_EXP_END		(261)

/** コメント中のILCタグ */
#define LL_ILC_COMMENT	(500)

/** 終了 */
#define LL_END			(700)


/**
 * lexの入力元を設定する
 */
void set_lex_input ( FILE* );

/**
 * lexの出力先を設定する
 */
void set_lex_output ( FILE* );

#endif	/* _SCAN_H_ */

