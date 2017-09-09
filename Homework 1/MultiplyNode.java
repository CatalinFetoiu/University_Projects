
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare operatorilor
 * "*" din cadrul programul interpretat. Contine doua noduri ce reprezinta valorile
 * ce urmeaza a fi inmultite.
 * @author Fetoiu Catalin-Emil
 *
 */
public class MultiplyNode extends OperatorNode {

	public MultiplyNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Integer accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
