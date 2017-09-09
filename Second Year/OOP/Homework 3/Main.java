
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.HashMap;
import java.util.Scanner;
import java.io.IOException;

/**
 * Clasa este folosita pentru citirea datelor de intrare, stocarea variabilelor,
 * evaluarea expresiilor si scrierea rezultatelor in fisierul de output.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class Main {

	/**
	 * Sunt citite datele de intrare, in functie de tipul liniei citite fiind stocata
	 * o variabila in tabela de hash unde acestea sunt retinute, sau este evaluata o
	 * anumita expresie. Initial sunt stocati intr-o tabela de hash operatorii +, -,
	 * *, /, cu prioritatile si proprietatile de asociativitate corespunzatoare. Este
	 * creat apoi un parser pe baza acestor operatori. Parserul este folosit pentru
	 * creearea arborilor de sintaxa corespunzatori expresiilor, arborii find apoi
	 * evaluati cu ajutorul instantei de Visitor retinuta in clasa AstNodeEvaluator.
	 * @throws IOException daca nu s-a reusit citirea sau scrierea folosind
	 * fisierele de input/output
	 */
	public static void main(String[] args) throws IOException {
		
		HashMap<String, ArithmeticOperator> operators =
				new HashMap<String, ArithmeticOperator>();
		operators.put("+", new ArithmeticOperator("+", 1, true, true));
		operators.put("-", new ArithmeticOperator("-", 1, true, false));
		operators.put("*", new ArithmeticOperator("*", 2, true, true));
		operators.put("/", new ArithmeticOperator("/", 2, true, false));
		
		Parser parser = new Parser(operators);
		Scanner input = null;
		FileWriter output = null;
		
		try {
			input = new Scanner(new BufferedReader(new FileReader("arbore.in")));
			output = new FileWriter("arbore.out");
			
			HashMap<String, Object> variables = new HashMap<String, Object>();
			
			while(input.hasNextLine()) {
				String lineOfCode = input.nextLine();
				String[] tokens = lineOfCode.split("[ ]+");
				Visitor v = AstNodeEvaluator.getInstance();
				
				if(tokens[0].equals("eval")) {
					String expression = lineOfCode.substring(4, lineOfCode.length() - 1);
					ASTNode InterpreterTree = parser.createAstFromExpression(expression, 
							variables);
					Object result = InterpreterTree.accept(v);
					
					if(result instanceof Double) {
						double formattedResult = (Double)result;
						formattedResult *= 100;
						formattedResult = (double)Math.round(formattedResult);
						formattedResult /= 100;
						
						output.write(formattedResult + "\n");
					}
					else {
						if(result.equals("NaN_I") || result.equals("NaN_D")) {
							output.write("NaN" + "\n");
						}
						else {
							output.write(result + "\n");
						}
					}
				}
				else {
					String variableName = null;
					String variableValue = null;
					
					String[] firstParsing = lineOfCode.split("[ =;]+");
					if(firstParsing[0].equals("string")) {
						variableName = firstParsing[1];
						String[] secondParsing = lineOfCode.split("[\"]+");
						variableValue = secondParsing[1];
					} 
					else {
						variableName = firstParsing[1];
						variableValue = firstParsing[2];
					}
					
					if(tokens[0].equals("int")) {
						variables.put(variableName, Integer.valueOf(variableValue));
					}
					else if(tokens[0].equals("double") || tokens[0].equals("float")) {
						variables.put(variableName, Double.valueOf(variableValue));
					}
					else if(tokens[0].equals("string")) {
						variables.put(variableName, variableValue);
					}
				}
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			input.close();
			output.close();
		}
	}
}
