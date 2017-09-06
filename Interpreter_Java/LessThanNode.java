
public class LessThanNode extends OperatorNode {

	public LessThanNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
