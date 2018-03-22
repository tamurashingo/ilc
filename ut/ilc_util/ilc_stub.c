/**
 * @file	ilc_stub.c
 * @brief	ilc.cのスタブ
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-04-29
 * @version	$Id: ilc_stub.c,v 1.2 2008/05/25 13:22:49 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <string.h>
#include "ilc.h"
#include "util.h"

/** ILC_Appendが呼ばれた回数 */
static int AppendCount = 0;


int getAppendCount() { return AppendCount; }
void initAppendCount() { AppendCount = 0; }


/**
 * ILC_Searchのスタブ
 * 本家と同じロジック
 */
char* ILC_Search (
	ILC_DATA* ilc_data,
	const char* str
)
{
	/**/
	long ix;
	char* ilc_id;
	char* ret = NULL;
	/**/

	for ( ix = 0; ix < ilc_data->num; ix++ ) {
		ilc_id = (ilc_data->coverage)[ix];
		if ( strcmp( str, ilc_id + 2 ) == 0 ) {
			ret = ilc_id;
			break;
		}
	}

	return ret;
}


/**
 * ILC_Appendのスタブ
 * ほぼ本家と一緒。
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

	AppendCount++;

	ptr = (char**)xmalloc( sizeof(char*) * (ilc_data->num + 1) );
	if ( ptr != NULL ) {
		if ( ilc_data->coverage != NULL ) {
			memcpy( ptr, ilc_data->coverage, sizeof(char*) * ilc_data->num );
			xfree( ilc_data->coverage );
		}
		ptr[ilc_data->num] = data;
		ilc_data->num++;
		ilc_data->coverage = ptr;
		ret = ILC_SUCCESS;
	}
	else {
		ret = ILC_FAILURE;
	}

	return ret;
}


