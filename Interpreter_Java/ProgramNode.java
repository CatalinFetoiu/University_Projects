
public class ProgramNode extends OperatorNode {
	
	public ProgramNode(AstNode leftChild, AstNode rightChild) {
		super(leftChild, rightChild);
	}
	
	public Object accept(Visitor v) throws AssertException {
		return v.visit(this);
	}
}
