
/**
 * Clasa este folosita pentru a construi nodurile din arborele de sintaxa
 * corespunzatoare operatiilor, fiind extinsa de nodurile de tip +, -, *, /.
 * Totodata reprezinta o extensie a clasei ASTNode.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public abstract class OperatorNode extends ASTNode {
	
	public OperatorNode(ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
	}
}
