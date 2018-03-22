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
 * @file	ilc_util_local.h
 * @brief	
 *
 * @author	Tamura Shingo (tamura.shingo@gmail.com)
 * @date	2008-01-20
 * @version	$Id: ilc_util_local.h,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#ifndef _ILC_UTIL_LOCAL_H_
#define _ILC_UTIL_LOCAL_H_

#include "ilc_util.h"

/**
 * ILC_FUNC_BODYの作成
 * @return ILC_FUNC_BODY* 作成したILC_FUNC_BODY
 *                        NULL: 作成に失敗
 */
static ILC_FUNC_BODY* ilcfuncbody_create ( );

/**
 * ILC_COMMENT_BODYの作成
 * @return ILC_COMMENT_BODY* 作成したILC_COMMENT_BODY
 *                           NULL: 作成に失敗
 */
static ILC_COMMENT_BODY* ilccommentbody_create ( );

/**
 * ILC_FUNCの検索
 * BODYに登録してある関数名が一致するかを判定する
 * (util::slist_searchに渡す関数)
 * @param const SLIST* 検索対象のILC_FUNC
 * @param const void*  検索する文字列(char*であること)
 * @return int         0:一致せず
 *                     1:一致
 */
static int local_ilcfunc_search ( const SLIST*, const void* );


#endif /* _ILC_UTIL_LOCAL_H_ */


