
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

/**
 * Clasa este folosita pentru parsarea programului si construirea arborelui
 * sau de sintaxa. Pentru aceasta sunt folosite doua stive, una ce contine
 * parantezele, operatorii si instructiunile din program si o a doua ce contine
 * nodurile din arbore. Cele doua stive sunt updatate conform algoritmului descris de
 * metoda createAstFromExpression. Sunt prezente metode pentru adaugarea diferitelor
 * tipuri de noduri in arbore. La finalul parcurgerii expresiei, stiva cu noduri va
 * contine un singur nod, si anume nodul radacina al arborelui. Parserul poate
 * fi construit in functie de operatorii si instructiunile unui program.
 * @author Fetoiu Catalin-Emil
 *
 */
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
	
	/**
	 * Adauga un nod de tip Operator in arbore. Sunt scoase de pe stiva
	 * cele doua noduri copil ale sale, este creat noul nod, acesta fiind
	 * apoi adaugat inapoi pe stiva.
	 */
	private void addOperatorNodeToTree(String operation) {
		AstNode rightChild = treeNodesStack.pop();
		AstNode leftChild = treeNodesStack.pop();
		treeNodesStack.push(NodeFactory.getInstance()
				.createOperatorNode(operation, leftChild, rightChild));
	}
	
	/**
	 * Adauga un nod de tip Instruction (Assert sau Return) in arbore.
	 * Este scos de pe stiva de noduri nodul sau copil, fiind creat noul
	 * nod si apoi adaugat inapoi pe stiva.
	 */
	private void addInstructionNodeToTree(String instruction) {
		AstNode child = treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance()
				.createInstructionNode(instruction, child));
	}
	
	/**
	 * Adauga un nod de tip For in arbore. Sunt scoase de pe stiva nodurile
	 * corespunzatoare initializarii, incrementarii, conditiei de iesire si
	 * a corpului for-ului, fiind adaugat inapoi pe stiva nodul For creat.
	 */
	private void addForNodeToTree() {
		AstNode forBody = treeNodesStack.pop();
		AstNode increment = treeNodesStack.pop();
		AstNode condition = treeNodesStack.pop();
		AstNode init = treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance().
				createForNode(init, condition, increment, forBody));
	}
	
	/**
	 * Adauga un nod de tip If in arbore. Sunt scoase de pe stiva nodurile
	 * corespunzatoare conditiei si celor doua ramuri, fiind adauagat inapoi
	 * pe stiva nodul If creat.
	 */
	private void addIfNodeToTree() {
		AstNode falseBody =  treeNodesStack.pop();
		AstNode trueBody =  treeNodesStack.pop();
		AstNode condition =  treeNodesStack.pop();
		
		treeNodesStack.push(NodeFactory.getInstance().
				createIfNode(condition, trueBody, falseBody));
	}
	
	/**
	 * Creeaza arborele de sintaxa al unui program dat ca parametru prin
	 * String-ul expression, ce contine reprezentarea programului pe o singura linie.
	 */
	public ProgramIR createAstFromExpression(String expression) {
		
		treeNodesStack = new Stack<AstNode>();
		operatorStack = new Stack<String>();
		
		/** Sunt extrasi tokenii din program */
		ArrayList<String> tokens = ParserUtils.extractTokens(expression);
		
		/** Sunt parcursi tokenii programului */
		for(int index = 0; index < tokens.size(); index++) {	
			
			String currentToken = tokens.get(index);
			switch(currentToken) {
			
			/** Daca se intalneste o paranteza deschisa, aceasta este adaugata
			 * pe stiva de operatori */
			case "[":
			{
				operatorStack.push("[");
				break;
			}
			/** Daca se intalnste o paranteza inchisa, inseamna ca aceasta marcheaza 
			 * sfarsitul unui sub-program, prin urmare instructiunea corespunzatoare 
			 * acestui subprogram este eliminata de pe stiva de operatori si este
			 * adaugat nodul de tipul sau in arbore */
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
				else {
					addIfNodeToTree();
				}
				
				/** Este eliminata de pe stiva paranteza deschisa pereche a celei intalnite */
				operatorStack.pop();
				break;
			}
			/** Daca se intalneste o variabila, un operator sau o constanta. Pentru
			 * variabile si constante, este adaugat nodul corespunzator in arbore, iar
			 * pentru operatori, acestia sunt adaugati pe stiva de operatori */
			default:
			{
				/** Daca token-ul curent este operator binar */
				if(operators.get(currentToken) != null) {
					operatorStack.push(currentToken);
				}
				/** Daca token-ul curent este assert sau return */
				else if(instructions.get(currentToken) != null) {
					operatorStack.push(currentToken);
				}
				/** Daca token-ul curent este for */
				else if(currentToken.equals("for")) {
					operatorStack.push(currentToken);
				}
				/** Daca token-ul curent este if */
				else if(currentToken.equals("if")) {
					operatorStack.push(currentToken);
				}
				/** Daca token-ul curent este o constanta */
				else if(currentToken.matches("[0-9]+")) {
					treeNodesStack.push(NodeFactory.getInstance().
							createConstantNode(Integer.parseInt(currentToken)));
				}
				/** Daca token-ul curent este o variabila */
				else {
					treeNodesStack.push(NodeFactory.getInstance().
							createVariableNode(currentToken));
				}
			}
			}
		}
		
		/** Este returnat un ProgramIR ce contine radacina arborelui de sintaxa */
		return new ProgramIR(treeNodesStack.pop());	
	}
}