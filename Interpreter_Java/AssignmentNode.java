
public class AssignmentNode extends OperatorNode {

	public AssignmentNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
