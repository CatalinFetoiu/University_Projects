
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare operatorilor
 * "+" din cadrul programul interpretat. Contine doua noduri ce reprezinta valorile
 * ce urmeaza a fi adunate.
 * @author Fetoiu Catalin-Emil
 *
 */
public class AddNode extends OperatorNode {
	
	public AddNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Integer accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
