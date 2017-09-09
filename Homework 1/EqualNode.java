
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare operatorilor
 * "==" din cadrul programul interpretat. Contine doua noduri ce reprezinta valorile
 * ce urmeaza a fi comparate.
 * @author Fetoiu Catalin-Emil
 *
 */
public class EqualNode extends OperatorNode {

	public EqualNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Boolean accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
