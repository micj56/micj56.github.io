package semant;

import table.*;
import types.*;
import absyn.*;

class ProcedureBodyChecker {

	void check(Absyn program, Table globalTable) {
		program.accept(new CheckVisitor(globalTable));
	}

	private class CheckVisitor extends DoNothingVisitor {

		public void visit(ProcDec procDec) {

		}

		public void visit(CompStm node) {

		}

	}
}
