
import java.util.Vector;

/**
 * Clasa este folosita de catre clasa Parser, pentru a extrage numele variabilelor
 * operatiile si parantezele din cadrul unei expresii, eliminand spatiile. Este utila
 * in cazul in care variabilele nu sunt delimitate prin spatii de operatii sau paranteze.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class ParserUtils {

	/**
	 * Creeaza un vector de String-uri dintr-o expresie, vector ce contine numele
	 * variabilelor impreuna cu operatiile si parantezele prezente in expresie.
	 * @param expression expresia ce trebuia parsata
	 * @return vectorul ce contine componentele expresiei
	 */
	public static String[] extractOperands(String expression) {
		
		char[] tokens = expression.toCharArray();
		Vector<String> operands = new Vector<String>();
		
		int i = 0;
		while(i < tokens.length) {
			switch(tokens[i]) {
			
			case ' ':
			{
				i++;
				break;
			}
			case '+':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			case '-':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			case '*':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			case '/':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			case '(':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			case ')':
			{
				operands.add(String.valueOf(tokens[i]));
				i++;
				break;
			}
			default:
			{
				String str = "";
				while(i < tokens.length && Character.isLetter(tokens[i]))
				{
					str += tokens[i];
					i++;
				}
				
				operands.add(str);
			}
			}
		}
		
		String[] words = new String[operands.size()];
		for(int index = 0; index < operands.size(); index++) {
			words[index] = operands.elementAt(index);
		}
		
		return words;
	}
}
