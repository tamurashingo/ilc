/**
 * @file	ilc_stub.c
 * @brief	コンパイルするためだけのilc.cのスタブ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-05-05
 * @version	$Id: ilc_stub.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <stdio.h>
#include "ilc.h"


/**
 * ILC_Searchのスタブ
 */
char* ILC_Search (
	ILC_DATA* ilc_data,
	const char* str
)
{
	return NULL;
}


/**
 * ILC_Appendのスタブ
 */
ILC_ERROR ILC_Append (
	ILC_DATA* ilc_data,
	char* data
)
{
	return ILC_SUCCESS;
}


