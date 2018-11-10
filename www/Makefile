#
# Makefile for SPL compiler
#

JAVA = java
JAVAC = javac
JFLEX = ./lib/JFlex.jar
CUP = ./lib/java-cup-11a.jar
CUP_RT = ./lib/java-cup-11a-runtime.jar

SRC_MAIN = main/Main.java
SRC_PARSE = parse/Scanner.java parse/Parser.java parse/sym.java
SRC_ABSYN = $(wildcard absyn/*.java)
SRC_SYM = sym/Sym.java

SRCS = $(SRC_MAIN) $(SRC_PARSE) $(SRC_ABSYN) $(SRC_SYM)
SRCS2 = $(SRC_PARSE) $(SRC_ABSYN) $(SRC_SYM)
CLASSES = $(patsubst %.java,%.class,$(SRCS2))

all:			main/Main.class

tests:			main/Main.class
			@for i in Tests/test??.spl ; do \
			  echo ; \
			  echo -- Compiling $$i -- ; \
			  awk '{if(FNR==2)print}' $$i ; \
			  spl --absyn $$i ; \
			done
			@echo

main/Main.class:	$(SRC_MAIN) $(CLASSES)
			$(JAVAC) -cp $(CUP_RT):. -sourcepath . main/Main.java

%.class:		%.java
			$(JAVAC) -cp $(CUP_RT):. -sourcepath . $<

parse/Scanner.java:	parse/scanner.jflex
			$(JAVA) -cp $(JFLEX) JFlex.Main parse/scanner.jflex

parse/Parser.java:	parse/parser.cup
			$(JAVA) -cp $(CUP) java_cup.Main \
			  -parser Parser -expect 1 parse/parser.cup
			mv Parser.java parse
			mv sym.java parse

parse/sym.java:		parse/parser.cup
			$(JAVA) -cp $(CUP) java_cup.Main \
			  -parser Parser -expect 1 parse/parser.cup
			mv Parser.java parse
			mv sym.java parse

clean:
			rm -f *~
			rm -f Tests/*~
			rm -f main/*~
			rm -f parse/*~
			rm -f absyn/*~
			rm -f sym/*~

dist-clean:		clean
			rm -f main/*.class
			rm -f parse/Scanner.java parse/Parser.java
			rm -f parse/sym.java parse/*.class
			rm -f absyn/*.class
			rm -f sym/*.class
