
/**
 * Clasa este folosita pentru a construi nodurile din arborele de sintaxa
 * corespunzatoare variabilelor, fiind extinsa de nodurile de tip Int, Double,
 * respectiv String. Totodata reprezinta o extensie a clasei ASTNode.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public abstract class VariableNode extends ASTNode {

	public VariableNode(ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
	}
}
