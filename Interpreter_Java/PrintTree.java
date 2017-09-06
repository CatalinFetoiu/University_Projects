
public class PrintTree {

	public static int maxHeight(AstNode node) {

		int height = 0;

		if (node == null) {
			height = 0;
		} else {
			if (node instanceof OperatorNode) {
				height = 1 + Math.max(
						maxHeight(((OperatorNode) node).getLeftChild()),
						maxHeight(((OperatorNode) node).getRightChild()));
			} else {
				if (node instanceof VariableNode
						|| node instanceof ConstantNode)
					height = 1;
				else {
					if (node instanceof AssertionNode)
						height = 1 + maxHeight(((AssertionNode) node)
								.getCondition());
					else {
						if (node instanceof ReturnNode)
							height = 1 + maxHeight(((ReturnNode) node)
									.getValue());
						else {
							if (node instanceof ForNode) {
								height = 1 + Math.max(Math.max(Math.max(
										maxHeight(((ForNode) node).getForBody()),
										maxHeight(((ForNode) node)
												.getCondition())),
										maxHeight(((ForNode) node)
												.getIncrement())),
										maxHeight(((ForNode) node).getInit()));
							} else {

								height = 1 + Math.max(Math.max(maxHeight(((IfNode) node)
										.getCondition()),
										maxHeight(((IfNode) node)
												.getFalseBody())),maxHeight(((IfNode)node).getTrueBody()));
							}
						}
					}
				}
			}

		}
		return height;
}
	
	public static void printLevel(AstNode node, int currentLevel, int level) {

		if (node == null)
			return;
		
		if (currentLevel < level) {
			if( node instanceof OperatorNode){
				printLevel(((OperatorNode)node).getLeftChild(),currentLevel+1,level);
				printLevel(((OperatorNode)node).getRightChild(),currentLevel +1, level);
			} else {
				if( node instanceof ConstantNode || node instanceof VariableNode)
					return;
				else {
					if( node instanceof AssertionNode){
						printLevel(((AssertionNode)node).getCondition(),currentLevel+1,level);
					} else {
						if( node instanceof ReturnNode){
							printLevel(((ReturnNode)node).getValue(),currentLevel+1,level);
						} else {
							if( node instanceof ForNode){
								printLevel(((ForNode)node).getInit(),currentLevel + 1, level);
								printLevel(((ForNode)node).getCondition(),currentLevel +1, level);
								printLevel(((ForNode)node).getIncrement(),currentLevel +1, level);
								printLevel(((ForNode)node).getForBody(),currentLevel+1,level);
							} else {
								printLevel(((IfNode)node).getCondition(),currentLevel + 1,level);
								printLevel(((IfNode)node).getTrueBody(),currentLevel + 1, level);
								printLevel(((IfNode)node).getFalseBody(),currentLevel + 1, level);
							}
						}
					}
				}
			}
		}
		if (currentLevel == level) {
			if (node instanceof VariableNode) {
				System.out.print(((VariableNode)node).getName()+ " ");
			}
			if (node instanceof ConstantNode) {
				System.out.print(((ConstantNode)node).getValue() + " ");
			}
			if (node instanceof MultiplyNode) {
				System.out.print("* ");
			}
			if (node instanceof AddNode) {
				System.out.print("+ ");
			}
			if (node instanceof ForNode) {
				System.out.print("for ");
			}
			if (node instanceof AssertionNode) {
				System.out.print("assert ");
			}
			if (node instanceof IfNode) {
				System.out.print("if ");
			}
			if (node instanceof AssignmentNode){
				System.out.print("= ");
			}
			if (node instanceof EqualNode){
				System.out.print("== ");
			}
			if (node instanceof LessThanNode){
				System.out.print("< ");
			}
			if (node instanceof ProgramNode){
				System.out.print("; ");
			}
			if (node instanceof ReturnNode){
				System.out.print("return ");
			}
		}
		
		if (currentLevel > level) {
			return;
		}
	}

	public static void printDebug(AstNode root) {

		int height = maxHeight(root);
		for (int i = 0; i < height; i++) {
			printLevel(root, 0, i);
			System.out.print("\n");
		}
	}
	
}
