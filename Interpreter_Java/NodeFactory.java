
import java.util.HashMap;

public class NodeFactory {

	private static final NodeFactory instance = new NodeFactory();
	private final HashMap<String, Operator> operators;
	private final HashMap<String, Instruction> instructions;
	
	private NodeFactory() {	
		operators = new HashMap<String, Operator>();
		operators.put("+", Operator.AddNode);
		operators.put("=", Operator.AssignmentNode);
		operators.put("==", Operator.EqualNode);
		operators.put("<", Operator.LessThanNode);
		operators.put("*", Operator.MultiplyNode);
		operators.put(";", Operator.ProgramNode);
		
		instructions = new HashMap<String, Instruction>();
		instructions.put("assert", Instruction.AssertionNode);
		instructions.put("return", Instruction.ReturnNode);
	}
	
	public static NodeFactory getInstance() {
		return instance;
	}
	
	public enum Operator {
		AddNode, AssignmentNode, EqualNode, LessThanNode, MultiplyNode,
			ProgramNode; 
	}
	
	private enum Instruction {
		AssertionNode, ReturnNode;
	}
	
	public AstNode createOperatorNode(String operation, AstNode leftChild,
			AstNode rightChild) {
		
		Operator op = operators.get(operation);
		
		switch(op) {
		
		case ProgramNode:
			return new ProgramNode(leftChild, rightChild);
		case AddNode:
			return new AddNode(leftChild, rightChild);
		case AssignmentNode:
			return new AssignmentNode(leftChild, rightChild);
		case EqualNode:
			return new EqualNode(leftChild, rightChild);
		case LessThanNode:
			return new LessThanNode(leftChild, rightChild);
		case MultiplyNode:
			return new MultiplyNode(leftChild, rightChild);
		}
		
		throw new IllegalArgumentException();
	}
	
	public AstNode createInstructionNode(String instruction, AstNode child) {
		
		Instruction ins = instructions.get(instruction);
		
		switch(ins) {
		
		case AssertionNode:
			return new AssertionNode(child);
		case ReturnNode:
			return new ReturnNode(child);
		}
		
		throw new IllegalArgumentException();
	}
	
	public AstNode createForNode(AstNode init, AstNode condition, AstNode increment,
			AstNode forBody) {
		
		return new ForNode(init, condition, increment, forBody);
	}
	
	public AstNode createIfNode(AstNode condition, AstNode trueBody, AstNode falseBody) {
		return new IfNode(condition, trueBody, falseBody);
	}
	
	public AstNode createVariableNode(String name) {
		return new VariableNode(name);
	}
	
	public AstNode createConstantNode(int value) {
		return new ConstantNode(value);
	}
}
