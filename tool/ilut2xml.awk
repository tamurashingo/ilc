#!/usr/bin/awk -f
##############################################################################
# @(#) $Id: ilut2xml.awk,v 1.1 2008/05/31 06:09:13 shingo Exp $
#
# ILUTの出力結果をXML形式に変換する。
#
# usage :
#   ilut2xml.awk <ut1.result> <ut2.result> ... > ilut_result.xml
#
#
#   Copyright (c) 2008, 2017 tamura shingo
##############################################################################

# YYYYMMDDHHMMSS -> YYYY/MM/DD HH:MM:SS に変換
function toDateTime( ymdhms ) {
	return sprintf( "%s %s", toDate( ymdhms ),toTime( substr( ymdhms, 9, 6 ) ) );
}

# HHMMSS -> HH:MM:SS に変換
function toTime( hms ) {
	return sprintf( "%s:%s:%s", substr( hms, 1, 2 ),
                                substr( hms, 3, 2 ),
                                substr( hms, 5, 2 ) );
}

# YYYYMMDD -> YYYY/MM/DD に変換
function toDate( ymd ) {
	return sprintf( "%s/%s/%s",
					substr( ymd, 1, 4 ),
                    substr( ymd, 5, 2 ),
                    substr( ymd, 7, 2 ) );
}

BEGIN {
	FS=":"
	print "<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
	print "<?xml-stylesheet href=\"ilut.xsl\" type=\"text/xsl\" ?>"
	print "<unittest>"
}

# コメント行
/^#/ {
	comment = 1;
}

# 通常の行
!/^#/ {
	comment = 0;
}

prev != $1 && comment == 0 {
	if ( initial == 0 ) {
		initial = 1;
	}
	else {
		print "  </testcase>"
	}
	print  "  <testcase>"
	printf "    <name>%s</name>\n", $1
	prev = $1;
}

comment == 0 {
	print  "    <test>"
	printf "      <name>%s</name>\n", $2
	start = toDateTime( $3 );
	printf "      <start>%s</start>\n", start
	end = toDateTime( $4 );
	printf "      <end>%s</end>\n", end
	printf "      <result>%s</result>\n", $5
	printf "      <comment>%s</comment>\n", $6
	print  "    </test>"
}


END {
	if ( initial == 1 ) {
		print "  </testcase>"
	}
	print "</unittest>"
}
