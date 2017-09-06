
public class EqualNode extends OperatorNode {

	public EqualNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Boolean accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
