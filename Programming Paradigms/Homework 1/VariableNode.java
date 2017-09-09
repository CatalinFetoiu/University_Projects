
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare variabilelor
 * din program. Contine un String ce reprezinta numele variabilei.
 * @author Fetoiu Catalin-Emil
 *
 */
public class VariableNode extends AstNode {

	private final String name;
	
	public VariableNode(String name) {
		this.name = name;
	}
	
	public String getName() {
		return name;
	}
	
	public Integer accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
