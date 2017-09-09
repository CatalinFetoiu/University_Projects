
/**
 * Interfata este implementata de catre clasele folosite pentru evaluarea unui
 * arbore de sintaxa, continand metode corespunzatoarea vizitarii fiecarui tip de
 * nod, mai exact cele corespunzatoare operatiilor si variabilelor.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public interface Visitor {
	
	/**
	 * Evalueaza subarborele unui nod de tip +.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Object visit(AdditionNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip -.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Object visit(SubstractionNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip *.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Object visit(MultiplicationNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip /.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Object visit(DivisionNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip Integer.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Integer visit(IntegerNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip Double.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public Double visit(DoubleNode node);
	
	/**
	 * Evalueaza subarborele unui nod de tip String.
	 * @param node nodul ce trebuie evaluat
	 * @return rezultatul evaluarii subarborelui
	 */
	public String visit(StringNode node);
}
