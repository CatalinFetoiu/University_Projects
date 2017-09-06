
public class ProgramIR {

	private AstNode InterpreterTree;
	private boolean hasReturn;
	private boolean hasScopeErrors;
	
	public ProgramIR(AstNode InterpreterTree, boolean hasReturn,
			boolean hasScopeErrors) {
		
		this.InterpreterTree = InterpreterTree;
		this.hasReturn = hasReturn;
		this.hasScopeErrors = hasScopeErrors;
	}
	
	public AstNode getInterpreterTree() {
		return InterpreterTree;
	}
	
	public boolean hasReturn() {
		return hasReturn;
	}
	
	public boolean hasScopeErrors() {
		return hasScopeErrors;
	}
}
