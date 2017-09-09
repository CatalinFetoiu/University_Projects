
/**
 * Clasa este folosita pentru a construi nodurile corespunzatoare variabilelor
 * de tip String din arborele de sintaxa, fiind o extensie a clasei VariableNode.
 * Contine campul value de tip String, ce reprezinta valoarea stocata in nod.
 * <p>
 * Este implementata interfata Visitable.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class StringNode extends VariableNode {

	private final String value;
	
	/**
	 * Creeaza un nod nou, cu valoarea si copiii stang si drept specificati
	 * @param value valorea stocata in nod.
	 * @param leftChild copilul stang al nodului
	 * @param rightChild copilul drept al nodului
	 */
	public StringNode(String value, ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
		this.value = value;
	}
	
	public String getValue() {
		return value;
	}
	
	public String accept(Visitor v) {
		return v.visit(this);
	} 
}
