
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare programelor
 * interpretate. Contine doua noduri ce reprezinta cele doua sub-programe.
 * @author Fetoiu Catalin-Emil
 *
 */
public class ProgramNode extends OperatorNode {
	
	public ProgramNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Object accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
