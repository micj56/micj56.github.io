/* Semant.java -- semantic checks */

package semant;

import absyn.*;
import sym.Sym;
import table.*;
import types.*;
import varalloc.*;
import java.lang.Class;

/**
 * A SemanticChecker object defines a method "check" for semantic 
 * analysis and symbol table construction for SPL
 * <br>
 * SemanticChecker is a singleton class
 * <br>
 * author: Michael Jäger
 */

public class SemanticChecker {

	static final Type intType = new PrimitiveType("int", VarAllocator.INTBYTESIZE);
	static final Type boolType = new PrimitiveType("boolean", VarAllocator.BOOLBYTESIZE);

	public Table check(Absyn program, boolean showTables) {
           /* Semantische Prüfung */

           /* hier gibts noch was zu tun: das Meiste kann an Visitor-Objekte delegiert werden */
	}

	static void checkClass (Object object, Class<?> expectedClass, String errorMessage, int lineNo)  {
		checkClass(object, expectedClass, errorMessage + " in line " + lineNo);
	}

	static void checkClass (Object object, Class<?> expectedClass, String errorMessage)  {
		if (object.getClass()!=expectedClass)
			throw new RuntimeException(errorMessage);
	}

	private void checkMainProcedure(Table globalTable) {
           /* hier gibts noch was zu tun! */
	}
}
