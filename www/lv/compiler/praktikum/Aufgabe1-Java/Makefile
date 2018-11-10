#
# Makefile for SPL compiler
#

JAVA = java
JAVAC = javac
JFLEX = ./lib/JFlex.jar

SRCS = main/Main.java \
       parse/Scanner.java parse/sym.java parse/Symbol.java

all:			main/Main.class

tests:			main/Main.class
			@for i in Tests/test??.spl ; do \
			  echo ; \
			  echo -- Compiling $$i -- ; \
			  spl $$i ; \
			done
			@echo

main/Main.class:	$(SRCS)
			$(JAVAC) main/Main.java

parse/Scanner.java:	parse/scanner.jflex
			$(JAVA) -cp $(JFLEX) JFlex.Main parse/scanner.jflex

clean:
			rm -f *~
			rm -f Tests/*~
			rm -f main/*~
			rm -f parse/*~

dist-clean:		clean
			rm -f main/*.class
			rm -f parse/Scanner.java parse/*.class
