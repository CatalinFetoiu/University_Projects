
/**
 * Interfata este implementata de catre nodurile non-abstracte din arborele de
 * sintaxa al unei expresii, continand metoda accept necesara posibilitatii
 * traversarii si evaluarii arborelui prin intermediul unui obiect de tip Visitor.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public interface Visitable {

	/**
	 * Accepta un obiect de tip Visitor, apeland metoda visit a acestuia pe
	 * nodul curent, ceea ce permite evaluarea subarborelui ce il are pe acesta
	 * drept radacina.
	 * @param v obiectul de tip Visitor folosit pentru evaluarea arborelui
	 * @return rezultatul evaluarii subarborelui curent, acesta fiind de tip
	 * Object intrucat expresia contine variabile de tip Integer, Double sau String
	 */
	public Object accept(Visitor v);
}
