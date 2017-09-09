
/**
 * Interfata contine metodele folosite de catre un operator aritmetic, utile
 * in cadrul algoritmului de parsare a expresiei si de constructie a arborelui.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public interface Operator {

	/**
	 * Verifica daca un operator este asociativ la stanga.
	 * @return true sau false
	 */
	public boolean isLeftAssociative();
	
	/**
	 * Verifica daca un operator este asociativ la dreapta.
	 * @return true sau false
	 */
	public boolean isRightAssociative();
	
	/**
	 * Compara prioritatea operatorului curent cu cea a operatorului specificat
	 * ca parametru.
	 * @param operator operatorul aritmetic fata de care se realizeaza compararea
	 * @return un numar mai mic/mai mare ca 0 daca prioritatea operatorului curent
	 * este mai mica/mai mare decat cea a operatorului dat ca parametru si 0 in caz
	 * de egalitate
	 */
	public int comparePriorityTo(Operator operator);
}
