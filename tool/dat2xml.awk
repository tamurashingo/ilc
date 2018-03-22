#!/usr/bin/awk -f
##############################################################################
# @(#) $Id: dat2xml.awk,v 1.2 2008/05/31 06:04:31 shingo Exp $
#
# ilcが出力したカバレッジデータをXML形式に変換する。
#
# usage :
#   dat2xml.awk <ilc1.dat> <ilc2.dat> ... > ilc_report.xml
#
#
#   Copyright (c) 2007-2008, 2017 tamura shingo
##############################################################################

BEGIN {
	FS=":"
	print "<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
	print "<?xml-stylesheet href=\"report.xsl\" type=\"text/xsl\" ?>"
	print "<coverage_report>"
}

{
	if ( $1 == "0" ) {
		flag = "false"
	}
	else {
		flag = "true"
	}
	printf "  <coverage source=\"%s\" function=\"%s\" line=\"%d\" result=\"%s\" />\n", $2, $3, $4, flag
}


END {
	print "</coverage_report>"
}
