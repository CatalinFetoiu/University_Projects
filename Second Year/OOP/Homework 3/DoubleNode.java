
/**
 * Clasa este folosita pentru a construi nodurile corespunzatoare variabilelor
 * de tip Double din arborele de sintaxa, fiind o extensie a clasei VariableNode.
 * Contine campul value de tip double, ce reprezinta valoarea stocata in nod.
 * <p>
 * Este implementata interfata Visitable.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class DoubleNode extends VariableNode {

	private final double value;
	
	/**
	 * Creeaza un nod nou, cu valoarea si copiii stang si drept specificati
	 * @param value valorea stocata in nod.
	 * @param leftChild copilul stang al nodului
	 * @param rightChild copilul drept al nodului
	 */
	public DoubleNode(double value, ASTNode leftChild, ASTNode rightChild) {
		super(leftChild, rightChild);
		this.value = value;
	}
	
	public double getValue() {
		return value;
	}
	
	public Double accept(Visitor v) {
		return v.visit(this);
	}
}
