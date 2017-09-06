
public interface Visitor {

	public Integer visit(AddNode node) throws AssertException;
	
	public Boolean visit(AssertionNode node) throws AssertException;
	
	public Boolean visit(AssignmentNode node) throws AssertException;
	
	public Integer visit(ConstantNode node) throws AssertException;
	
	public Boolean visit(EqualNode node) throws AssertException;
	
	public Boolean visit(ForNode node) throws AssertException;
	
	public Boolean visit(IfNode node) throws AssertException;
	
	public Boolean visit(LessThanNode node) throws AssertException;
	
	public Integer visit(MultiplyNode node) throws AssertException;
	
	public Object visit(ProgramNode node) throws AssertException;
	
	public Object visit(ReturnNode node) throws AssertException;
	
	public Integer visit(VariableNode node) throws AssertException; 
}
