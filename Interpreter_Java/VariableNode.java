
public class VariableNode extends AstNode {

	private final String name;
	
	public VariableNode(String name) {
		this.name = name;
	}
	
	public String getName() {
		return name;
	}
	
	public Integer accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
