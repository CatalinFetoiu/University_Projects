
/**
 * Clasa generica ce este mostenita de toate nodurile de tip operatori binari.
 * Contine doua noduri copii, corespunzatoare celor doi operanzi.
 * @author Fetoiu Catalin-Emil
 *
 */
public abstract class OperatorNode extends AstNode {
	
	private final AstNode leftChild;
	private final AstNode rightChild;
	
	public OperatorNode(AstNode leftChild, AstNode rightChild) {
		this.leftChild = leftChild;
		this.rightChild = rightChild;
	}
	
	public AstNode getLeftChild() {
		return leftChild;
	}
	
	public AstNode getRightChild() {
		return rightChild;
	}
}
