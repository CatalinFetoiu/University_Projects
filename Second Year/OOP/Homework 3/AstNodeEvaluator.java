
/**
 * Clasa este folosita pentru a crea obiecte folosite in evaluarea unui arbore
 * de sintaxa. Este implementata interfata Visitor. Este folosit design pattern-ul
 * Singleton intrucat nu este necesara instantierea mai multor obiecte de tip Visitor,
 * toate indeplinand exact acelasi scop. Pentru nodurile de tip operatie, sunt evaluati
 * mai intai subarborii stang si drept, fiind returnat rezultatul operatiei aritmetice
 * curente intre cele doua rezultate. Deoarece in arborele rezultat, nodurile de tip
 * variabila vor fi intotdeauna frunze, vizitarea acestora presupunea doar returnarea
 * valorii stocate in cadrul nodului.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class AstNodeEvaluator implements Visitor {

	private static final AstNodeEvaluator instance = new AstNodeEvaluator();
	
	/**
	 * Constructorul este privat.
	 */
	private AstNodeEvaluator() {
	}
	
	/**
	 * Returneaza instanta privata de Visitor.
	 * @return instanta de Visitor stocata in clasa
	 */
	public static AstNodeEvaluator getInstance() {
		return instance;
	}

	public Object visit(AdditionNode node) {
		return AdditionCalculator.add(node.getLeftChild().accept(this), 
				node.getRightChild().accept(this));
	}
	
	public Object visit(SubstractionNode node) {
		return SubstractionCalculator.substract(node.getLeftChild().accept(this),
				node.getRightChild().accept(this));
	}
	
	public Object visit(MultiplicationNode node) {
		return MultiplicationCalculator.multiply(node.getLeftChild().accept(this),
				node.getRightChild().accept(this));
	}
	
	public Object visit(DivisionNode node) {
		return DivisionCalculator.divide(node.getLeftChild().accept(this),
				node.getRightChild().accept(this));
	}
	
	public Integer visit(IntegerNode node) {
		return node.getValue();
	}
	
	public Double visit(DoubleNode node) {
		return node.getValue();
	}
	
	public String visit(StringNode node) {
		return node.getValue();
	}
}
