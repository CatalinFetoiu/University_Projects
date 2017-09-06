
import java.util.HashMap;

public class AstNodeEvaluator implements Visitor {

	private HashMap<String, Integer> variables;
	private boolean hasReturn;
	
	public AstNodeEvaluator() {
		variables = new HashMap<String, Integer>();
		hasReturn = false;
	}
	
	public boolean getHasReturn() {
		return hasReturn;
	}
	
	public Integer visit(AddNode node) throws AssertException {
		return (Integer)node.getLeftChild().accept(this) 
				+ (Integer)node.getRightChild().accept(this);
	}
	
	public Boolean visit(AssertionNode node) throws AssertException {
		Boolean result = (Boolean)node.getCondition().accept(this);
		if(result == true) {
			return result;
		}
		else {
			throw new AssertException();
		}
	}

	public Boolean visit(AssignmentNode node) throws AssertException {
		variables.put(((VariableNode)node.getLeftChild()).getName(), 
				(Integer)node.getRightChild().accept(this));
		return true;
	}
	
	public Integer visit(ConstantNode node) throws AssertException {
		return node.getValue();
	}
	
	public Boolean visit(EqualNode node) throws AssertException {
		return (Integer)node.getLeftChild().accept(this) ==
				(Integer)node.getRightChild().accept(this);
	}
	
	public Boolean visit(ForNode node) throws AssertException {
		node.getInit().accept(this);
		
		while(true) {
			node.getForBody().accept(this);
			node.getIncrement().accept(this);
			Boolean check = (Boolean)node.getCondition().accept(this);
			if(!check) {
				break;
			}
		}
		return true;
	}
	
	public Boolean visit(IfNode node) throws AssertException {
		Boolean check = (Boolean)node.getCondition().accept(this);
		if(check) {
			node.getTrueBody().accept(this);
		}
		else {
			node.getFalseBody().accept(this);
		}
		return true;
	}
	
	public Boolean visit(LessThanNode node) throws AssertException {
		return (Integer)node.getLeftChild().accept(this) <
				(Integer)node.getRightChild().accept(this);
	}
	
	public Integer visit(MultiplyNode node) throws AssertException {
		return (Integer)node.getLeftChild().accept(this) 
				* (Integer)node.getRightChild().accept(this);
	}
	
	public Object visit(ProgramNode node) throws AssertException {
		node.getLeftChild().accept(this);
		return node.getRightChild().accept(this);
	}
	
	public Object visit(ReturnNode node) throws AssertException {
		hasReturn = true;
		return node.getValue().accept(this);
	}
	
	public Integer visit(VariableNode node) throws AssertException {
		return variables.get(node.getName());
	}
}
