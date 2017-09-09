
import java.util.Stack;
import java.util.HashMap;

/**
 * Clasa este folosita pentru a crea arborele de sintaxa (AST) corespunzator
 * unei expresii, arbore ce urmeaza apoi a fi evaluat cu ajutorul design pattern-ului
 * Visitor. Contine o stiva de noduri, o stiva cu operatori si o tabela de hash ce
 * mapeaza numele unui operator cu o instanta a clasei ArithmeticOperator. Este
 * folosita o varianta putin modificata a algoritmului de parsare Shunting-Yard, in
 * care nu mai este creeata coada de output corespunzatoare formei RPN a expresiei,
 * fiind creeat direct arborele folosind stiva de noduri.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class Parser {

	private Stack<ASTNode> treeNodesStack;
	private Stack<String> operatorStack;
	private final HashMap<String, ArithmeticOperator> operators;
	
	/**
	 * Creeaza un nou parser, potrivit pentru expresii ce contin operatorii din
	 * tabela de hash specificata ca parametru.
	 * @param operators - operatorii din expresiile ce vor fi date ca input parser-ului
	 */
	public Parser(HashMap<String, ArithmeticOperator> operators) {
		this.operators = operators;
		this.operatorStack = new Stack<String>();
	}
	
	/**
	 * Adauga un nod de tip operatie in arbore, copiii stang si drept ai
	 * acestuia fiind cele doua noduri aflate in top-ul stivei.
	 * @param name tipul operatiei
	 */
	private void addNodeToTree(String name) {
		
		ASTNode rightChild = treeNodesStack.pop();
		ASTNode leftChild = treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance()
				.createOperatorNode(name, leftChild, rightChild));
	}
	
	/**
	 * Creeaza arborele de sintaxa corespunzator unei expresii folosind o varianta
	 * modificata a algoritmului Shunting-Yard. Diferenta principala este ca, de fiecare
	 * data cand in algoritmul initial o operatie sau o variabila era introdusa in coada
	 * de output, aici este creat un nod nou si este adaugat pe stiva de noduri. Daca
	 * nodul este de tip variabila acesta este adaugat direct, iar in cazul in care nodul
	 * este de tip operatie, sunt eliminate primele doua noduri din top-ul stivei (acestea
	 * reprezentand cei doi copii ai noului nod), fiind apoi adaugat noul nod pe stiva.
	 * Dupa ce intreaga expresie a fost parcursa, sunt adaugate in arbore nodurile
	 * corespunzatoare operatiilor ramase pe operatorStack. La final este returnat singurul
	 * element ramas pe stiva de noduri, acesta fiind radacina arborelui.
	 * @param expression expresia ce trebuie parsata
	 * @param variables o tabela de hash ce asociaza variabilele cu valorile lor
	 * @return radacina arborelui de sintaxa creat
	 */
	public ASTNode createAstFromExpression(String expression, 
			HashMap<String, Object> variables) {
		
		treeNodesStack = new Stack<ASTNode>();
		
		String[] tokens = ParserUtils.extractOperands(expression);
	
		for(String currentToken : tokens) {
			
			switch(currentToken) {
			
			case " ":
				break;
			case "(":
			{
				operatorStack.push("(");
				break;
			}
			case ")":
			{	
				while(operatorStack.peek().equals("(") == false) {
					
					String name = operatorStack.pop();
					addNodeToTree(name);
				}
				operatorStack.pop();
				break;
			}
			default:
			{
				if(operators.get(currentToken) != null) {
					ArithmeticOperator currentOperator = operators.get(currentToken);
					
					while(!operatorStack.isEmpty()) {
						String nextOperatorName = operatorStack.peek();
						
						if(operators.get(nextOperatorName) == null) {
							break;
						}
						ArithmeticOperator nextOperator = operators.get(nextOperatorName);
						
						if(currentOperator.isLeftAssociative() && 
								currentOperator.comparePriorityTo(nextOperator) <= 0) {
							
							addNodeToTree(nextOperatorName);
							operatorStack.pop();
						}
						else if(currentOperator.isRightAssociative() &&
								currentOperator.comparePriorityTo(nextOperator) < 0) {
							
							addNodeToTree(nextOperatorName);
							operatorStack.pop();
						}
						else {
							break;
						}
					}
					
					operatorStack.push(currentToken);
				}
				else {
					treeNodesStack.push(NodeFactory.getInstance().createVariableNode(
							variables.get(currentToken))); 
															 
				}
			}
			}
		}
		
		while(!operatorStack.isEmpty()) {
			String operatorName = operatorStack.pop();
			addNodeToTree(operatorName);
		}
		
		return treeNodesStack.pop();
	}
}
