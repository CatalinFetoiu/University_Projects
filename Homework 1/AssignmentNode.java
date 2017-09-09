
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare operatorilor
 * "==" din cadrul programul interpretat. Contine doua noduri ce reprezinta
 * valorile verificate de operator.
 * @author Fetoiu Catalin-Emil
 *
 */
public class AssignmentNode extends OperatorNode {

	public AssignmentNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Boolean accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
