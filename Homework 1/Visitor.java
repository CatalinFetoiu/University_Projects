
/**
 * Interfata ce trebuie implementata de clasele folosite pentru evaluarea
 * arborelui de sintaxa corespunzator unui program.
 * @author Fetoiu Catalin-Emil
 *
 */
public interface Visitor {

	public Integer visit(AddNode node) throws Exception;
	
	public Boolean visit(AssertionNode node) throws Exception;
	
	public Boolean visit(AssignmentNode node) throws Exception;
	
	public Integer visit(ConstantNode node) throws Exception;
	
	public Boolean visit(EqualNode node) throws Exception;
	
	public Boolean visit(ForNode node) throws Exception;
	
	public Boolean visit(IfNode node) throws Exception;
	
	public Boolean visit(LessThanNode node) throws Exception;
	
	public Integer visit(MultiplyNode node) throws Exception;
	
	public Object visit(ProgramNode node) throws Exception;
	
	public Object visit(ReturnNode node) throws Exception;
	
	public Integer visit(VariableNode node) throws Exception; 
}
