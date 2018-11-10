/*
 * Symbol.java -- a preliminary token class
 * (will be supplied by the Cup parser generator)
 */


package parse;


public class Symbol {

  public int sym;
  public int left;
  public int right;
  public Object value;

  public Symbol(int s, int l, int r) {
    sym = s;
    left = l;
    right = r;
    value = null;
  }

  public Symbol(int s, int l, int r, Object v) {
    sym = s;
    left = l;
    right = r;
    value = v;
  }

}
