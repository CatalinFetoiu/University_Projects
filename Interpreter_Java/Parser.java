
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

public class Parser {

	private final HashMap<String, Integer> operators;
	private final HashMap<String, Integer> instructions;
	
	private Stack<AstNode> treeNodesStack;
	private Stack<String> operatorStack;
	
	public Parser(HashMap<String, Integer> operators,
			HashMap<String, Integer> instructions) {
		
		this.operators = operators;
		this.instructions = instructions;
	}
	
	private void addOperatorNodeToTree(String operation) {
		AstNode rightChild = treeNodesStack.pop();
		AstNode leftChild = treeNodesStack.pop();
		treeNodesStack.push(NodeFactory.getInstance()
				.createOperatorNode(operation, leftChild, rightChild));
	}
	
	private void addInstructionNodeToTree(String instruction) {
		AstNode child = treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance()
				.createInstructionNode(instruction, child));
	}
	
	private void addForNodeToTree() {
		AstNode forBody = treeNodesStack.pop();
		AstNode increment = treeNodesStack.pop();
		AstNode condition = treeNodesStack.pop();
		AstNode init = treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance().
				createForNode(init, condition, increment, forBody));
	}
	
	private void addIfNodeToTree() {
		AstNode falseBody =  treeNodesStack.pop();
		AstNode trueBody =  treeNodesStack.pop();
		AstNode condition =  treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance().
				createIfNode(condition, trueBody, falseBody));
	}
	
	public ProgramIR createAstFromExpression(String expression) {
		
		boolean hasReturn = false;
		boolean hasScopeErrors = false;
		
		HashMap<String, Integer> variables = new HashMap<String, Integer>();
		treeNodesStack = new Stack<AstNode>();
		operatorStack = new Stack<String>();
		
		ArrayList<String> tokens = ParserUtils.extractTokens(expression);
		
		for(int index = 0; index < tokens.size(); index++) {	
			
			String currentToken = tokens.get(index);
			switch(currentToken) {
			
			case "[":
			{
				operatorStack.push("[");
				break;
			}
			case "]":
			{
				String programElement = operatorStack.pop();
				if(operators.get(programElement) != null) {
					addOperatorNodeToTree(programElement);
				}
				else if(instructions.get(programElement) != null){
					addInstructionNodeToTree(programElement);
				}
				else if(programElement.equals("for")) {
					addForNodeToTree();
				}
				//programElement is if
				else {
					addIfNodeToTree();
				}
				
				operatorStack.pop();
				break;
			}
			default:
			{
				if(operators.get(currentToken) != null) {
					operatorStack.push(currentToken);
				}
				else if(instructions.get(currentToken) != null) {
					operatorStack.push(currentToken);
					if(currentToken.equals("return")) {
						hasReturn = true;
					}
				}
				else if(currentToken.equals("for")) {
					operatorStack.push(currentToken);
				}
				else if(currentToken.equals("if")) {
					operatorStack.push(currentToken);
				}
				else if(currentToken.matches("[0-9]+")) {
					treeNodesStack.push(NodeFactory.getInstance().
							createConstantNode(Integer.parseInt(currentToken)));
				}
				//token is variable
				else {
					treeNodesStack.push(NodeFactory.getInstance().
							createVariableNode(currentToken));
					if(variables.get(currentToken) == null) {
						if(tokens.get(index - 1).equals("=")) {
							boolean ok = true;
							int j = index + 2;
							int count = 1;
							if(tokens.get(index + 1).equals("[")) {
								while(count != 0) {
									if(tokens.get(j).equals("[")) {
										count++;
									}
									else if(tokens.get(j).equals("]")) {
										count--;
									}
									else {
										if(tokens.get(j).equals(currentToken)) {
											ok = false;
											break;
										}
									}
									j++;
								}
							}
							else {
								if(tokens.get(index + 1).equals(currentToken)) {
									ok = false;
								}
							}
							if(ok) {
								variables.put(currentToken, 0);
							}
							else {
								hasScopeErrors = true;
							}

						}
						else {
							hasScopeErrors = true;
						}
					}
				}
			}
			}
			if(hasScopeErrors) {
				break;
			}
		}
			
		return new ProgramIR(treeNodesStack.pop(), hasReturn, hasScopeErrors);	
	}
	
}
