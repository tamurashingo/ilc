/**
 * @file	test_options.c
 * @brief	options.cのユニットテスト
 *
 * @author	tamura shingo (tamura.shingo@gmail.com)
 * @date	2008-05-16
 * @version	$Id: test_options.c,v 1.3 2008/05/31 06:15:21 shingo Exp $
 *
 * Copyright (c) 2007-2008, 2017 tamura shingo
 *
 */

#include <string.h>
#include "options.h"
#include "ILUT.h"
#include "ilc.h"

/**
 * 引数が無いパターン
 */
ILUT_Test test_options_001(
)
{
	/**/
	int   argc = 1;
	char* argv[] = {
		"./test"		/* プログラム名 */
	};
	struct opt opt;
	/**/

	/* 初期化 */
	memset( &opt, 0, sizeof(opt) );

	parse_option( argc, argv, &opt );

	ILUT_ASSERT( "データファイル名が設定されていないこと",     opt.ilc_file == NULL );
	ILUT_ASSERT( "変換後出力ファイル名が設定されていないこと", opt.out_file == NULL );
	ILUT_ASSERT( "バージョン出力情報が設定されていないこと",   opt.version  == 0 );
	ILUT_ASSERT( "ヘルプ出力が設定されていないこと",           opt.help     == 0 );
	ILUT_ASSERT( "変換元入力ファイル名が設定されていないこと", opt.in_file  == NULL );

	return ILUT_SUCCESS;
}


/**
 * オプション指定あり かつ 入力ファイル指定なし
 */
ILUT_Test test_options_002 (
)
{
	/**/
	int argc = 7;
	char* argv[] = {
		"./test",		/* プログラム名 */
		"-f",			/* ILCデータファイルの指定 */
		"ilc.dat",		/* ILCデータファイル名 */
		"-o",			/* 変換後ファイルの指定 */
		"outfile",		/* 変換後ファイル名 */
		"-h",			/* ヘルプの表示 */
		"-v"			/* バージョン情報出力 */
	};
	struct opt opt;
	/**/

	/* 初期化 */
	memset( &opt, 0, sizeof(opt) );

	parse_option( argc, argv, &opt );

	ILUT_ASSERT( "データファイル名が設定されていること",
				 strcmp( "ilc.dat", opt.ilc_file ) == 0 );
	ILUT_ASSERT( "変換後出力ファイル名が設定されていること",
				 strcmp( "outfile", opt.out_file ) == 0 );
	ILUT_ASSERT( "バージョン出力情報が設定されていること",     opt.version == 1 );
	ILUT_ASSERT( "ヘルプ出力が設定されていること",             opt.help    == 1 );
	ILUT_ASSERT( "変換元入力ファイル名が設定されていないこと", opt.in_file == NULL );

	return ILUT_SUCCESS;
}


/**
 * オプション指定なし かつ 入力ファイル指定あり
 */
ILUT_Test test_options_003 (
)
{
	/**/
	int argc = 2;
	char* argv[] = {
		"./test",		/* プログラム名 */
		"infile"		/* 入力ファイル名 */
	};
	struct opt opt;
	/**/

	/* 初期化 */
	memset( &opt, 0, sizeof(opt) );

	parse_option( argc, argv, &opt );

	ILUT_ASSERT( "データファイル名が設定されていないこと",     opt.ilc_file == NULL );
	ILUT_ASSERT( "変換後出力ファイル名が設定されていないこと", opt.out_file == NULL );
	ILUT_ASSERT( "バージョン出力情報が設定されていないこと",   opt.version  == 0 );
	ILUT_ASSERT( "ヘルプ出力が設定されていないこと",           opt.help     == 0 );
	ILUT_ASSERT( "変換元入力ファイル名が設定されていること",
				 strcmp( "infile", opt.in_file )  == 0 );

	return ILUT_SUCCESS;
}


/**
 * オプション指定あり かつ 入力ファイル指定あり
 */
ILUT_Test test_options_004 (
)
{
	/**/
	int argc = 8;
	char* argv[] = {
		"./test",		/* プログラム名 */
		"-f",			/* ILCデータファイルの指定 */
		"ilc.dat",		/* ILCデータファイル名 */
		"-o",			/* 変換後ファイルの指定 */
		"outfile",		/* 変換後ファイル名 */
		"-h",			/* ヘルプの表示 */
		"-v",			/* バージョン情報出力 */
		"infile"		/* 入力ファイル名 */
	};
	struct opt opt;
	/**/

	/* 初期化 */
	memset( &opt, 0, sizeof(opt) );

	parse_option( argc, argv, &opt );

	ILUT_ASSERT( "データファイル名が設定されていること",
				 strcmp( "ilc.dat", opt.ilc_file ) == 0 );
	ILUT_ASSERT( "変換後出力ファイル名が設定されていること",
				 strcmp( "outfile", opt.out_file ) == 0 );
	ILUT_ASSERT( "バージョン出力情報が設定されていること",     opt.version == 1 );
	ILUT_ASSERT( "ヘルプ出力が設定されていること",             opt.help    == 1 );
	ILUT_ASSERT( "変換元入力ファイル名が設定されていること",
				 strcmp( "infile", opt.in_file )  == 0 );

	return ILUT_SUCCESS;
}


/**
 * 入力ファイルの指定が複数
 */
ILUT_Test test_options_005 (
)
{
	/**/
	int argc = 4;
	char* argv[] = {
		"./test",		/* プログラム名 */
		"file1",		/* 入力ファイル名1 */
		"file2",		/* 入力ファイル名2 */
		"file3"			/* 入力ファイル名3 */	
	};
	struct opt opt;
	/**/

	/* 初期化 */
	memset( &opt, 0, sizeof(opt) );

	parse_option( argc, argv, &opt );

	ILUT_ASSERT( "データファイル名が設定されていないこと",     opt.ilc_file == NULL );
	ILUT_ASSERT( "変換後出力ファイル名が設定されていないこと", opt.out_file == NULL );
	ILUT_ASSERT( "バージョン出力情報が設定されていないこと",   opt.version  == 0 );
	ILUT_ASSERT( "ヘルプ出力が設定されていないこと",           opt.help     == 0 );
	ILUT_ASSERT( "変換元入力ファイル名が設定されていること",
				 strcmp( "file1", opt.in_file )  == 0 );

	return ILUT_SUCCESS;
}

int main (
	int argc,
	char** argv
)
{
	/**/
	static ILUT_TestCase test[] = {
		DEF_TEST(test_options_001),
		DEF_TEST(test_options_002),
		DEF_TEST(test_options_003),
		DEF_TEST(test_options_004),
		DEF_TEST(test_options_005),
		TestCaseEnd
	};
	int ret;
	FILE* out = NULL;
	/**/

	/*-
	 * 第一引数で指定されたファイルを読み込む。
	 * 指定されていない場合は ilc.dat を読み込む。
	 */
	if ( ILC_Initialize( argv[1] ) != ILC_SUCCESS ) {
		printf( "カバレッジデータの読み込みに失敗\n" );
	}

	ILUT_SetShowMode( ILUT_MODE_DETAIL );
	ret = ILUT_RunTest( test );

	/*-
	 * 第二引数でファイルが指定されてあれば、そちらにテスト結果を出力する。
	 * 指定が無ければ標準出力にテスト結果を出力する。
	 */
	if ( argv[2] != NULL ) {
		out = fopen( argv[2], "w" );
	}
	if ( out == NULL ) {
		out = stdout;
	}
	ILUT_ResultOut( out, "options", test );
	fclose( out );

	ILC_Finalize();

	return ret;
}


