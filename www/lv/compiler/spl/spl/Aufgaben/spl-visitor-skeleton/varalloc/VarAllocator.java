/*
 * Varalloc.java -- variable allocation
 */


package varalloc;

import java.util.Arrays;
import absyn.*;
import table.*;
import types.*;
import sym.*;

public class VarAllocator {

	public static final int INTBYTESIZE = 4;
	public static final int BOOLBYTESIZE = 4;
	public static final int REFBYTESIZE = 4;

	private Table globalTable;
	private boolean showVarAlloc;

	public void allocVars(Absyn program) {
	}

}

