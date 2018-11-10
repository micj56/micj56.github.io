/*
 * Sym.java -- symbol handling
 */


package sym;


public class Sym {

  private static java.util.Map<String,Sym> dict =
    new java.util.HashMap<String,Sym>();
  private String name;

  private Sym(String n) {
    name = n;
  }

  public String toString() {
    return name;
  }

  public static Sym newSym(String n) {
    String u = n.intern();
    Sym s = dict.get(u);
    if (s == null) {
      s = new Sym(u);
      dict.put(u, s);
    }
    return s;
  }

}
