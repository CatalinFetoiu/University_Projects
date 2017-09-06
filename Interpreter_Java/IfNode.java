
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
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
