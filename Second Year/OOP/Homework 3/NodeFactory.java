
/**
 * Clasa foloseste design pattern-ul factory pentru a crea instante ale nodurilor
 * din arborele corespunzator unei expresii, continand metode pentru creearea
 * nodurilor de tip operatie, precum si a nodurilor de tip variabila. Este folosit
 * de asemenea design pattern-ul Singleton intrucat orice instanta a clasei ar indeplini
 * acelasi scop.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class NodeFactory {
	
	private static final NodeFactory instance = new NodeFactory();
	
	/**
	 * Constructorul este privat.
	 */
	private NodeFactory() {	
	}
	
	/**
	 * Returneaza instanta private de NodeFactory.
	 * @return instanta de NodeFactory stocata in clasa
	 */
	public static NodeFactory getInstance() {
		return instance;
	}
	
	/**
	 * Creeaza un nod de tip operatie in functie de numele dat ca parametru, avand
	 * copiii specificati ca parametrii.
	 * @param operatorName tipul operatorului
	 * @param leftChild copilul stang al nodului
	 * @param rightChild copilul drept al nodului
	 * @return nodul de tip operatie corespunzator
	 */
	public ASTNode createOperatorNode(String operatorName, ASTNode leftChild,
			ASTNode rightChild) {
		
		switch(operatorName) {
		
		case "+":
			return new AdditionNode(leftChild, rightChild);
		case "-":
			return new SubstractionNode(leftChild, rightChild);
		case "*":
			return new MultiplicationNode(leftChild, rightChild);
		case "/":
			return new DivisionNode(leftChild, rightChild);
		}
		
		throw new IllegalArgumentException("Operator name " + operatorName
				+ "is not recognized");
	}
	
	/**
	 * Creeaza un nod de tip variabila, in functie de obiectul specificat ca parametru.
	 * Nodul creat este de tip frunza, deoarece in arborele de sintaxa, variabilele sunt
	 * intotdeauna frunze.
	 * @param value obiectul pe baza caruia este creat nodul, acesta poate fi o instanta
	 * de Integer, Double sau String
	 * @return nodul frunza creat
	 */
	public ASTNode createVariableNode(Object value) {
		
		if(value instanceof Integer) {
			return new IntegerNode((Integer)value, null, null);
		}
		if(value instanceof Double) {
			return new DoubleNode((Double)value, null, null);
		}
		if(value instanceof String) {
			return new StringNode((String)value, null, null);
		}
		
		throw new IllegalArgumentException("Value type is not recognized");
	}
}
