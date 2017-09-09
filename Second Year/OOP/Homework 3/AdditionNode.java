
/**
 * Clasa este folosita pentru a construi nodurile corespunzatoare operatiei
 * de adunare din arborele de sintaxa, fiind o extensie a clasei OperatorNode.
 * <p>
 * Este implementata interfata Visitable.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class AdditionNode extends OperatorNode {

	public AdditionNode(ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
	}
	
	/* (non-Javadoc)
	 * @see Visitable#accept(Visitor)
	 */
	public Object accept(Visitor v) {
		return v.visit(this);
	}
}
