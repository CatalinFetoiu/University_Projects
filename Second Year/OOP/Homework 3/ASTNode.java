
/**
 * Clasa reprezinta nodul generic al arborelui de sintaxa, fiind mostenita
 * de toate nodurile de tip operatie sau operand. Contine campurile leftChild
 * si rightChild reprezentand copiii nodului, acestea fiind tot de tip ASTNode.
 * <p>
 * Este implementata interfata Visitable.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public abstract class ASTNode implements Visitable { 
	
	private final ASTNode leftChild;
	private final ASTNode rightChild;
	
	/**
	 * Creeaza un nod nou, avand copiii specificati.
	 * @param leftChild copilul stang al nodului
	 * @param rightChild copilul drept al nodului
	 */
	public ASTNode(ASTNode leftChild, ASTNode rightChild) {
		this.leftChild = leftChild;
		this.rightChild = rightChild;
	}
	
	public ASTNode getLeftChild() {
		return leftChild;
	}
	
	public ASTNode getRightChild() {
		return rightChild;
	}
}