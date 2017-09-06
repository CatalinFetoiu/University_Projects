
public class AddNode extends OperatorNode {
	
	public AddNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Integer accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
