/*
 * Main.java -- SPL compiler
 */


package main;

import java.io.*;
import parse.*;


class Main {

  static void help() {
    /* show some help how to use the program */
    System.out.print("Usage: spl [options] <input file>\n");
    System.out.print("Options:\n");
    System.out.print("  --help           show this help\n");
  }

  public static void main(String[] args) {
    String inFileName = null;
    for (int i = 0; i < args.length; i++) {
      if (args[i].charAt(0) == '-') {
        /* option */
        if (args[i].equals("--help")) {
          help();
          System.exit(0);
        } else {
          System.out.println(
            "**** Error: unrecognized option '" +
            args[i] + "'; try 'spl --help'"
          );
          System.exit(1);
        }
      } else {
        /* file */
        if (inFileName != null) {
          System.out.println("**** Error: more than one input file");
          System.exit(1);
        }
        inFileName = args[i];
      }
    }
    if (inFileName == null) {
      System.out.println("**** Error: no input file");
      System.exit(1);
    }
    try {
      FileInputStream source = new FileInputStream(inFileName);
      Scanner scanner = new Scanner(source);
      Symbol token;
      do {
        token = scanner.next_token();
        scanner.showToken(token);
      } while (token.sym != sym.EOF);
    } catch (FileNotFoundException e) {
      System.out.println("**** Error: cannot open input file '" +
                         inFileName + "'");
      System.exit(1);
    } catch (IOException e) {
      System.out.println("**** Error: IO error on input file '" +
                         inFileName + "'");
      System.exit(1);
    } catch (Exception e) {
      System.out.println("**** Error: " + e.getMessage());
      System.exit(1);
    }
  }

}
