
/**
 * Clasa este folosita pentru a construi nodurile corespunzatoare variabilelor
 * de tip Integer din arborele de sintaxa, fiind o extensie a clasei VariableNode.
 * Contine campul value de tip int, ce reprezinta valoarea stocata in nod.
 * <p>
 * Este implementata interfata Visitable.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class IntegerNode extends VariableNode {

	private final int value;
	
	/**
	 * Creeaza un nod nou, cu valoarea si copiii stang si drept specificati
	 * @param value valorea stocata in nod.
	 * @param leftChild copilul stang al nodului
	 * @param rightChild copilul drept al nodului
	 */
	public IntegerNode(int value, ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
		this.value = value;
	}
	
	public int getValue() {
		return value;
	}
	
	public Integer accept(Visitor v) {
		return v.visit(this);
	}
}
