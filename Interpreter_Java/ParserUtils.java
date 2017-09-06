
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


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
