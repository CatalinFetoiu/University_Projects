
/**
 * Clasa folosita pentru construirea nodurilor corespunzatoare instructiunilor de tip
 * if din programul interpretat. Contine noduri pentru conditia din if si pentru
 * ramurile true si false.
 * @author Fetoiu Catalin-Emil
 *
 */
public class IfNode extends AstNode {

	private final AstNode condition;
	private final AstNode trueBody;
	private final AstNode falseBody;
	
	public IfNode(AstNode condition, AstNode trueBody, AstNode falseBody) {
		this.condition = condition;
		this.trueBody = trueBody;
		this.falseBody = falseBody;
	}
	
	public AstNode getCondition() {
		return condition;
	}
	
	public AstNode getTrueBody() {
		return trueBody;
	}
	
	public AstNode getFalseBody() {
		return falseBody;
	}
	
	public Boolean accept(Visitor v) throws Exception {
		return v.visit(this);
	}
}
