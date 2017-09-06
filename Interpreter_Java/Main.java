
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws IOException {
		
		HashMap<String, Integer> operators = new HashMap<String, Integer>();
		operators.put("+", 0);
		operators.put("=", 0);
		operators.put("==", 0);
		operators.put("<", 0);
		operators.put("*", 0);
		operators.put(";", 0);
		
		HashMap<String, Integer> instructions = new HashMap<String, Integer>();
		instructions.put("assert", 0);
		instructions.put("return", 0);
		
		Parser parser = new Parser(operators, instructions);
		String expression1 = "[; [= x 10] [; [if [< x 3] [= x [+ x 2]] [= x [* x 2]]] [return x]]]";
		String expression2 = "[; [= y [+ 1 x]] [return y]]";
		
		Scanner input = null;
		FileWriter output = null;
		
		try {
			input = new Scanner(new BufferedReader(new FileReader(args[0])));
			output = new FileWriter(args[1]);
			
			String program = "";
			while(input.hasNextLine()) {
				String lineOfCode = input.nextLine();
				program += lineOfCode;
			}
			
			ProgramIR AbstractRepresentation = parser.createAstFromExpression(program);
			AstNode InterpreterTree = AbstractRepresentation.getInterpreterTree();
			
			AstNodeEvaluator v = new AstNodeEvaluator();
			
			try {
				if(AbstractRepresentation.hasScopeErrors()) {
					output.write("Check failed");
				}
				else if(!AbstractRepresentation.hasReturn()) {
					output.write("Missing return");
				}
				else {
					Object result = InterpreterTree.accept(v);
					Integer aux = (Integer)result;
					output.write(aux.toString());
				}
			}
			catch (AssertException e) { 
				output.write("Assert failed");
			}

			output.close();
			
			//System.out.println(PrintTree.maxHeight(InterpreterTree));
			//PrintTree.printDebug(InterpreterTree);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			input.close();
			//output.close();
		}
	}
}
