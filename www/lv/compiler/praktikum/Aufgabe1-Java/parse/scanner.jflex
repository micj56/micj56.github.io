/*
 * scanner.jflex -- SPL scanner specification
 */


package parse;


%%


%class Scanner
%public
%line
%column
%type Symbol
%function next_token
%eofval{
  return symbol(sym.EOF);
%eofval}
%eofclose

%{

  private Symbol symbol(int type) {
    return new Symbol(type, yyline + 1, yycolumn + 1);
  }

  private Symbol symbol(int type, Object value) {
    return new Symbol(type, yyline + 1, yycolumn + 1, value);
  }

  public void showToken(Symbol token) {
    String s;
    switch (token.sym) {
      case sym.EOF:
        s = "-- EOF --";
        break;
      default:
        /* this should never happen */
        throw new RuntimeException(
          "unknown token " + token.sym + " in showToken"
        );
    }
    System.out.println(
      "TOKEN = " + s +
      " in line " + token.left +
      ", column " + token.right
    );
  }

%}


%%


.		{
		  throw new RuntimeException(
		    "illegal character 0x" +
		    Integer.toString((int) yytext().charAt(0), 16) +
		    " in line " + (yyline + 1) +
		    ", column " + (yycolumn + 1)
		  );
		}
