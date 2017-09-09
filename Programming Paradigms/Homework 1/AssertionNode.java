
/**
 * Clasa este folosita pentru construirea nodurilor corespunzatoare instructiunilor
 * assert din cadrul programul interpretat. Contine un nod ce reprezinta conditia
 * verificata de assert.
 * @author Fetoiu Catalin-Emil
 *
 */
public class AssertionNode extends AstNode {

	private final AstNode condition;
	
	public AssertionNode(AstNode condition) {
		this.condition = condition;
	}
	
	public AstNode getCondition() {
		return condition;
	}
	
	public Boolean accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
