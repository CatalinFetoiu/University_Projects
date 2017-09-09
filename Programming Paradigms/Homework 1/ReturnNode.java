
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare instructiunilor
 * return din cadrul programul interpretat. Contine un nod ce reprezinta valoarea
 * ce trebuie returnata de catre program.
 * @author Fetoiu Catalin-Emil
 *
 */
public class ReturnNode extends AstNode {

	private final AstNode value;
	
	public ReturnNode(AstNode value) {
		this.value = value;
	}
	
	public AstNode getValue() {
		return value;
	}
	
	public Object accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
