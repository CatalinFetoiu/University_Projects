
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare valorilor
 * constante din programul interpretat. Contine valoarea constantei.
 * @author Fetoiu Catalin-Emil
 *
 */
public class ConstantNode extends AstNode {

	private final int value;
	
	public ConstantNode(int value) {
		this.value = value;
	}
	
	public int getValue() {
		return value;
	}
	
	public Integer accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
