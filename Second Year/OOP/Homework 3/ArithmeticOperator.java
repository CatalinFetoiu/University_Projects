
/**
 * Clasa este folosita pentru a crea obiecte corespunzatoare operatiilor
 * prezente intr-o expresie, obiecte ce sunt folosite in cadrul algoritmului
 * de parsare. Contine campuri ce indica prioritatea operatiei in cadrul unei
 * expresii, precum si valori booleene corespunzatoarea proprietatilor de
 * asociativitate la stanga/dreapta.
 * <p>
 * Este implementata interfata Operator.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class ArithmeticOperator implements Operator {

	private final String name;
	private final int priority;
	private final boolean isLeftAssociative;
	private final boolean isRightAssociative;
	
	/**
	 * Creeaza un operator aritmetic avand numele, prioritatea si proprietatile de
	 * asociativitate specificate.
	 * @param name numele operatiei, poate fi +, -, * sau /
	 * @param priority prioritatea operatiei
	 * @param isLeftAssociative true sau false, indica asociativitatea la stanga
	 * @param isRightAssociative true sau false, indica asociativitatea la dreapta
	 */
	public ArithmeticOperator(String name, int priority, boolean isLeftAssociative,
			boolean isRightAssociative) {
		
		this.name = name;
		this.priority = priority;
		this.isLeftAssociative = isLeftAssociative;
		this.isRightAssociative = isRightAssociative;
	}
	
	public boolean isLeftAssociative() {
		return isLeftAssociative;
	}
	
	public boolean isRightAssociative() {
		return isRightAssociative;
	}
	
	public int getPriority() {
		return priority;
	}
	
	public int comparePriorityTo(Operator operator) {
		return this.priority - ((ArithmeticOperator)operator).getPriority();
	}
	
	public String getName() {
		return name;
	}
}
