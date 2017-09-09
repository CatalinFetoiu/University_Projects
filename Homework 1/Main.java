
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Scanner;

/**
 * Clasa folosita pentru testarea programului si pentru citirea programului
 * ce urmeaza a fi interpretat, constructia arborelui de sintaxa corespunzator
 * si evaluarea acestuia.
 * @author Fetoiu Catain-Emil
 *
 */
public class Main {

	public static void main(String[] args) throws IOException {
		
		/** Este creata o mapa continand operatorii ce pot fi continuti de program */
		HashMap<String, Integer> operators = new HashMap<String, Integer>();
		operators.put("+", 0);
		operators.put("=", 0);
		operators.put("==", 0);
		operators.put("<", 0);
		operators.put("*", 0);
		operators.put(";", 0);
		
		/** Este creata o mapa ce contine instructiunile assert si return. Instructiunile
		 * for si if nu sunt incluse intrucat acestea au alt numar de noduri copil */
		HashMap<String, Integer> instructions = new HashMap<String, Integer>();
		instructions.put("assert", 0);
		instructions.put("return", 0);
		
		/** Este creat un parser corespunzator programelor de tip IMP++ */
		Parser parser = new Parser(operators, instructions);
		
		Scanner input = null;
		FileWriter output = null;
		
		try {
			input = new Scanner(new BufferedReader(new FileReader(args[0])));
			output = new FileWriter(args[1]);
			
			/** Este citit programul linie cu linie, fiind creat un String ce reprezinta
			 * concatenare acestora */
			String program = "";
			while(input.hasNextLine()) {
				String lineOfCode = input.nextLine();
				program += lineOfCode;
			}
			
			/** Programul este parsat si este construit arborele sau de sintaxa */
			ProgramIR AbstractRepresentation = parser.createAstFromExpression(program);
			AstNode InterpreterTree = AbstractRepresentation.getInterpreterTree();
			
			AstNodeEvaluator v = new AstNodeEvaluator();
			
			/** Este evaluat arborele de sintaxa si este afisat rezulatul evaluarii */
			try {
				Object result = InterpreterTree.accept(v);
				if(!v.hasReturn()) {
					output.write("Missing return");
				}
				else if(v.assertFailed()) {
					output.write("Assert failed");
				}
				else {
					output.write((Integer)result + "");
				}
			}
			catch (ScopeException e) {
				output.write("Check failed");
			}
			catch (Exception e) {
				
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
