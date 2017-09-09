
/**
 * Clasa folosita pentru Intermediate Represenation-ul programului ce
 * urmeaza a fi interpretat. Contine arborele de sintaxa obtinut in urma
 * parsarii programului.
 * @author Fetoiu Catalin-Emil
 *
 */
public class ProgramIR {

	private AstNode InterpreterTree;
	
	public ProgramIR(AstNode InterpreterTree) {
		this.InterpreterTree = InterpreterTree;
	}
	
	public AstNode getInterpreterTree() {
		return InterpreterTree;
	}
}
