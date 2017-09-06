
public class ConstantNode extends AstNode {

	private final int value;
	
	public ConstantNode(int value) {
		this.value = value;
	}
	
	public int getValue() {
		return value;
	}
	
	public Integer accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
