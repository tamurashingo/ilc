##############################################################################
# ILC Makefile.
# @(#) $Id: Makefile,v 1.4 2008/05/31 06:10:32 shingo Exp $
#
#   Copyright (c) 2007-2008, 2017 tamura shingo
##############################################################################

##############################################################################
# Makefile定義
#
######################################
# アプリケーションの作成
######################################
default : .default

######################################
# ユニットテストの実行
######################################
ut : .default .unittest

######################################
# 再構築
######################################
rebuild : .clean .default

######################################
# クリーンアップ
######################################
clean : .clean


##############################################################################
# アプリケーション、ライブラリ
##############################################################################
APP=	ilc
LIB=	libilc.a


##############################################################################
# コンパイルルールの定義
##############################################################################
CC=			gcc
AR=			ar
LINK=		gcc
FLEX=		flex
AWK=		awk

CFLAGS=		-g -Wall
INCLUDES=	-I$(SRCDIR)
LDFLAGS=	-L. -ll -lilc
ARFLAGS=	rcsv
LFLAGS=

SRCDIR=		./src
TESTDIR=	./ut
TOOLDIR=	./tool

OBJS= $(SRCDIR)/main.o \
      $(SRCDIR)/options.o \
      $(SRCDIR)/parser.o \
      $(SRCDIR)/ilc_util.o \
      $(SRCDIR)/scan.o \
      $(SRCDIR)/util.o

.c.o :
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<


##############################################################################
# アプリケーションのルール定義
##############################################################################
.default : $(OBJS) $(LIB)
	$(LINK) -o $(APP) $(OBJS) $(LDFLAGS)

$(SRCDIR)/main.o : $(SRCDIR)/ilc.h $(SRCDIR)/ilc_util.h $(SRCDIR)/parser.h $(SRCDIR)/options.h $(SRCDIR)/version.h
$(SRCDIR)/option.o : $(SRCDIR)/options.h
$(SRCDIR)/parser.o : $(SRCDIR)/ilc_util.h $(SRCDIR)/scan.h $(SRCDIR)/util.h $(SRCDIR)/parser_local.h
$(SRCDIR)/scan.o : $(SRCDIR)/scan.c
$(SRCDIR)/scan.c : $(SRCDIR)/scan.h $(SRCDIR)/scan.l
$(SRCDIR)/util.o : $(SRCDIR)/util.h
$(SRCDIR)/ilc.o : $(SRCDIR)/ilc.h

$(LIB) : $(SRCDIR)/ilc.o
	$(AR) $(ARFLAGS) $@ $(SRCDIR)/ilc.o


##############################################################################
# アプリケーションのクリーンアップ
##############################################################################
.clean :
	rm -rf *~ $(SRCDIR)/*.o $(SRCDIR)/*~ $(APP) $(LIB)
	rm -f $(SRCDIR)/scan.c


##############################################################################
# ユニットテストのルール定義
##############################################################################
INCLUDES=		-I$(SRCDIR) -I$(TESTDIR)
UTILDIR=		$(TESTDIR)/util
OPTDIR=			$(TESTDIR)/options
ILCUTILDIR=		$(TESTDIR)/ilc_util
PARSEDIR=		$(TESTDIR)/parser

.unittest : ut_clean ut_tool ut_util ut_options ut_ilcutil ut_parser
	$(AWK) -f $(TOOLDIR)/dat2xml.awk $(UTILDIR)/util_ilc.dat $(OPTDIR)/options_ilc.dat $(ILCUTILDIR)/ilc_util_ilc.dat $(PARSEDIR)/parser_ilc.dat > ilc_report.xml
	$(AWK) -f $(TOOLDIR)/ilut2xml.awk $(UTILDIR)/util.result $(OPTDIR)/options.result $(ILCUTILDIR)/ilc_util.result $(PARSEDIR)/parser.result > ilut_result.xml
	@echo "All tests successful."


######################################
# ユニットテストのクリーンアップ
######################################
ut_clean :
	rm -f $(TESTDIR)/ILUT.o
	rm -f $(UTILDIR)/util_ilc.o
	rm -f $(UTILDIR)/util_ilc.c
	rm -f $(UTILDIR)/test_util.o
	rm -f $(UTILDIR)/util_ilc.dat
	rm -f $(OPTDIR)/options_ilc.o
	rm -f $(OPTDIR)/options_ilc.c
	rm -f $(OPTDIR)/test_options.o
	rm -f $(OPTDIR)/options_ilc.dat
	rm -f $(ILCUTILDIR)/test_ilc_util.o
	rm -f $(ILCUTILDIR)/util_stub.o
	rm -f $(ILCUTILDIR)/ilc_util_ilc.o
	rm -f $(ILCUTILDIR)/ilc_util_ilc.c
	rm -f $(ILCUTILDIR)/ilc_stub.so
	rm -f $(ILCUTILDIR)/ilc.so
	rm -f $(PARSEDIR)/test_parser.o
	rm -f $(PARSEDIR)/util_stub.o
	rm -f $(PARSEDIR)/scan_stub.o
	rm -f $(PARSEDIR)/parser_ilc.o
	rm -f $(PARSEDIR)/parser_ilc.c
	rm -f $(PARSEDIR)/parser_ilc.dat


######################################
# ユニットテストツールのコンパイル
######################################
ut_tool : $(TESTDIR)/ILUT.o

$(TESTDIR)/ILUT.o : $(TESTDIR)/ILUT.h


######################################
# util.cのユニットテスト
######################################
ut_util : $(UTILDIR)/test_util.o $(UTILDIR)/util_ilc.o
	$(LINK) -o $(UTILDIR)/test_util $(UTILDIR)/test_util.o $(UTILDIR)/util_ilc.o $(TESTDIR)/ILUT.o $(LDFLAGS)
	$(UTILDIR)/test_util  $(UTILDIR)/util_ilc.dat $(UTILDIR)/util.result

$(UTILDIR)/test_util.o : $(SRCDIR)/util.h
$(UTILDIR)/util_ilc.c : $(SRCDIR)/util.c
	./$(APP) -o $@ -f $(UTILDIR)/util_ilc.dat $(SRCDIR)/util.c
$(UTILDIR)/util_ilc.o : $(UTILDIR)/util_ilc.c $(SRCDIR)/util.h


######################################
# options.cのユニットテスト
######################################
ut_options : $(OPTDIR)/test_options.o $(OPTDIR)/options_ilc.o
	$(LINK) -o $(OPTDIR)/test_options $(OPTDIR)/test_options.o $(OPTDIR)/options_ilc.o $(TESTDIR)/ILUT.o $(LDFLAGS)
	$(OPTDIR)/test_options $(OPTDIR)/options_ilc.dat  $(OPTDIR)/options.result

$(OPTDIR)/test_options.o : $(SRCDIR)/options.h
$(OPTDIR)/options_ilc.c : $(SRCDIR)/options.c
	./$(APP) -o $@ -f $(OPTDIR)/options_ilc.dat $(SRCDIR)/options.c
$(OPTDIR)/options_ilc.o : $(OPTDIR)/options_ilc.c $(SRCDIR)/options.h


######################################
# ilc_util.cのユニットテスト
######################################
ut_ilcutil : $(ILCUTILDIR)/test_ilc_util.o $(ILCUTILDIR)/util_stub.o \
             $(ILCUTILDIR)/ilc_util_ilc.o $(ILCUTILDIR)/ilc_stub.so $(ILCUTILDIR)/ilc.so \
             $(TESTDIR)/ILUT.o
	$(LINK) -o $(ILCUTILDIR)/test_ilc_util $(ILCUTILDIR)/test_ilc_util.o $(ILCUTILDIR)/util_stub.o $(ILCUTILDIR)/ilc_util_ilc.o $(ILCUTILDIR)/ilc_stub.so $(ILCUTILDIR)/ilc.so $(TESTDIR)/ILUT.o $(LDFLAGS)
	$(ILCUTILDIR)/test_ilc_util $(ILCUTILDIR)/ilc_util_ilc.dat $(ILCUTILDIR)/ilc_util.result


$(ILCUTILDIR)/test_ilc_util.o : $(SRCDIR)/ilc_util.h
$(ILCUTILDIR)/ilc_util_ilc.c : $(SRCDIR)/ilc_util.c
	./$(APP) -o $@ -f $(ILCUTILDIR)/ilc_util_ilc.dat $(SRCDIR)/ilc_util.c
$(ILCUTILDIR)/ilc_util_ilc.o : $(ILCUTILDIR)/ilc_util_ilc.c $(SRCDIR)/ilc_util.h
$(ILCUTILDIR)/util_stub.o : $(SRCDIR)/util.h

# ILC_Search、ILC_Appendをオーバーライドするため、共有ライブラリ化する
$(ILCUTILDIR)/ilc_stub.so : $(ILCUTILDIR)/ilc_stub.c
	$(CC) $(INCLUDES) -fPIC -shared -o $@ $(ILCUTILDIR)/ilc_stub.c $(ILCUTILDIR)/util_stub.o
$(ILCUTILDIR)/ilc.so : $(SRCDIR)/ilc.c
	$(CC) $(INCLUDES) -fPIC -shared -o $@ $(SRCDIR)/ilc.c


######################################
# parser.cのユニットテスト
######################################
ut_parser : $(PARSEDIR)/test_parser.o $(PARSEDIR)/util_stub.o $(PARSEDIR)/scan_stub.o \
            $(SRCDIR)/ilc_util.o $(PARSEDIR)/parser_ilc.o $(TESTDIR)/ILUT.o
	$(LINK) -o $(PARSEDIR)/test_parser $(PARSEDIR)/test_parser.o $(PARSEDIR)/util_stub.o $(PARSEDIR)/scan_stub.o $(SRCDIR)/ilc_util.o $(PARSEDIR)/parser_ilc.o $(TESTDIR)/ILUT.o $(LDFLAGS)
	$(PARSEDIR)/test_parser $(PARSEDIR)/parser_ilc.dat $(PARSEDIR)/parser.result

$(PARSEDIR)/test_parser.c : $(SRCDIR)/parser.h
$(PARSEDIR)/util_stub.o : $(SRCDIR)/util.h
$(PARSEDIR)/scan_stub.o : $(SRCDIR)/scan.h
$(PARSEDIR)/parser_ilc.c : $(SRCDIR)/parser.c $(SRCDIR)/parser.h
	./$(APP) -o $@ -f $(PARSEDIR)/parser_ilc.dat $(SRCDIR)/parser.c
$(PARSEDIR)/parser_ilc.o : $(PARSEDIR)/parser_ilc.c $(SRCDIR)/parser.h

