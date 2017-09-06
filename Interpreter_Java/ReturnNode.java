
public class ReturnNode extends AstNode {

	private final AstNode value;
	
	public ReturnNode(AstNode value) {
		this.value = value;
	}
	
	public AstNode getValue() {
		return value;
	}
	
	public Object accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
