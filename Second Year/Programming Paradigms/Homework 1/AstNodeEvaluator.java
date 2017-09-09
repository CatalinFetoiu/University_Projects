
import java.util.HashMap;

/**
 * Clasa folosita pentru evaluarea arborelui de sintaxa corespunzator
 * programului interpretat. Contine metode pentru evaluarea fiecarui
 * tip de nod. Valorile variabilelor pe parcursul executiei programului
 * sunt retinute intr-o tabela de hash ce este updatata la fiecare pas.
 * Sunt prezente si metodele hasReturn si assertFailed.
 * @author Fetoiu Catalin-Emil
 *
 */
public class AstNodeEvaluator implements Visitor {

	private HashMap<String, Integer> variables;
	private boolean assertFailed;
	private boolean hasReturn;
	
	public AstNodeEvaluator() {
		variables = new HashMap<String, Integer>();
		assertFailed = false;
		hasReturn = false;
	}
	
	public boolean assertFailed() {
		return assertFailed;
	}
	
	public boolean hasReturn() {
		return hasReturn;
	}
	
	/**
	 * Evaluarea unui nod de tip "+" presupune evaluarea celor doi copii
	 * ai sai si returnarea sumei valorilor obtinute.
	 */
	public Integer visit(AddNode node) throws Exception {
		return (Integer)node.getLeftChild().accept(this) 
				+ (Integer)node.getRightChild().accept(this);
	}
	
	/**
	 * Evaluarea unui nod de tip Assert presupune evaluarea nodului
	 * conditie asociat acestuia si verificarea rezulatului intors.
	 * In cazul in care rezultatul evaluarii conditiei este false,
	 * este activat flagul assertFailed.
	 */
	public Boolean visit(AssertionNode node) throws Exception {
		Boolean result = (Boolean)node.getCondition().accept(this);
		if(result == false) {
			assertFailed = true;
		}
		return result;
	}

	/**
	 * Pentru un nod de tip "=", este mai intai evaluat copilul sau drept,
	 * rezultatul fiind asignat variabilei retinute in copilul sau stang.
	 * Asignarea se face modificand valoarea cheii corespunzatoare din mapa
	 * ce contine variabilele.	 
	 * */
	public Boolean visit(AssignmentNode node) throws Exception {
		variables.put(((VariableNode)node.getLeftChild()).getName(), 
				(Integer)node.getRightChild().accept(this));
		return true;
	}
	
	/**
	 * Pentru un nod de tip Constant, este returnata valorea retinuta in
	 * acesta.
	 */
	public Integer visit(ConstantNode node) throws Exception {
		return node.getValue();
	}
	
	/**
	 * Pentru un nod de tip "==", sunt evaluate cele doua noduri copii ai
	 * acestuia, fiind apoi verificata egalitatea rezultatelor.
	 */
	public Boolean visit(EqualNode node) throws Exception {
		return (Integer)node.getLeftChild().accept(this) ==
				(Integer)node.getRightChild().accept(this);
	}
	
	/**
	 * Pentru un nod de tip For, este mai intai evaluat nodul corespunzator
	 * initializarii. Apoi, este folosita o bucla while in care este evaluat
	 * nodul ce contine corpul for-ului, apoi nodul corespunzator incrementarii,
	 * dupa aceea fiind evaluat nodul conditie. In cazul in care rezultatul intors
	 * de evaluarea nodului conditie este false, se iese din bucla.
	 */
	public Boolean visit(ForNode node) throws Exception {
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
	
	/**
	 * Pentru un nod de tip If, este evaluat nodul sau conditie, iar in
	 * functie de rezultatul intors este evaluat nodul corespunzator ramurii
	 * if sau nodul corespunzator ramurii else.
	 */
	public Boolean visit(IfNode node) throws Exception {
		Boolean check = (Boolean)node.getCondition().accept(this);
		if(check) {
			node.getTrueBody().accept(this);
		}
		else {
			node.getFalseBody().accept(this);
		}
		return true;
	}
	
	/**
	 * Pentru un nod de tip "<", sunt evaluate cele doua noduri copii ai
	 * acestuia, fiind apoi verificata inegalitatea rezultatelor.
	 */
	public Boolean visit(LessThanNode node) throws Exception {
		return (Integer)node.getLeftChild().accept(this) <
				(Integer)node.getRightChild().accept(this);
	}
	
	/**
	 * Evaluarea unui nod de tip "*" presupune evaluarea celor doi copii
	 * ai sai si returnarea produsului valorilor obtinute.
	 */
	public Integer visit(MultiplyNode node) throws Exception {
		return (Integer)node.getLeftChild().accept(this) 
				* (Integer)node.getRightChild().accept(this);
	}
	
	/**
	 * Pentru un nod de tip Program, sunt evaluate in ordine nodurile
	 * corespunzatoare celor doua sub-programe ale sale, fiind returnat
	 * rezulatul intors de al doilea sub-program (intrucat ordinea interpretarii
	 * este de la stanga la dreapta).
	 */
	public Object visit(ProgramNode node) throws Exception {
		node.getLeftChild().accept(this);
		return node.getRightChild().accept(this);
	}
	
	/**
	 * Pentru un nod de tip Return, este returnat rezultatul evaluarii
	 * nodului sau copil si este activat flagul hasReturn.
	 */
	public Object visit(ReturnNode node) throws Exception {
		hasReturn = true;
		return node.getValue().accept(this);
	}
	
	/**
	 * Pentru un nod de tip Variable, se verifica daca numele variablei este
	 * continut de tabela de hash si este returnata valoarea sa in caz afirmativ.
	 * Altfel, este aruncata o exceptie de tip ScopeException, ce indica faptul
	 * ca interpretarea programului se opreste.
	 */
	public Integer visit(VariableNode node) throws Exception {
		if(variables.get(node.getName()) == null) {
			throw new ScopeException();
		}
		return variables.get(node.getName());
	}
}
