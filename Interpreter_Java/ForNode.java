
public class ForNode extends AstNode {

	private final AstNode init;
	private final AstNode condition;
	private final AstNode increment;
	private final AstNode forBody;
	
	public ForNode(AstNode init, AstNode condition, AstNode increment,
			AstNode forBody) {
		
		this.init = init;
		this.condition = condition;
		this.increment = increment;
		this.forBody = forBody;
	}
	
	public AstNode getInit() {
		return init;
	}
	
	public AstNode getCondition() {
		return condition;
	}
	
	public AstNode getIncrement() {
		return increment;
	}
	
	public AstNode getForBody() {
		return forBody;
	}
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
