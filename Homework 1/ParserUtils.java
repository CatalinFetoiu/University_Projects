
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Clasa este folosita pentru eliminarea whitespace-urilor din cadrul programului
 * si extragerea componentelor acestuia. Este prezenta metoda extractTokens, ce returneaza
 * un ArrayList ce contine numele de variabile, constantele, parantezele, operatorii si 
 * instructiunile din program (for, if, assert, return). Extragerea tokenilor se face
 * utilizand un regex.
 * @author Fetoiu Catalin-Emil
 *
 */
public class ParserUtils {

	public static ArrayList<String> extractTokens(String expression) {
		ArrayList<String> tokens = new ArrayList<String>();
		String matchRegex = "[0-9]+|[a-zA-Z]+|\\[|\\]|\\+|\\*|[=]+|<|\\;";
		
		Matcher matcher = Pattern.compile(matchRegex)
		    .matcher(expression);
		while (matcher.find()) {
		  tokens.add(matcher.group());
		}
		
		return tokens;
	}
}