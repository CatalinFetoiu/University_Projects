
public class MultiplyNode extends OperatorNode {

	public MultiplyNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Integer accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
