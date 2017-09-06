
public class AssertionNode extends AstNode {

	private final AstNode condition;
	
	public AssertionNode(AstNode condition) {
		this.condition = condition;
	}
	
	public AstNode getCondition() {
		return condition;
	}
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
