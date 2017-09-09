
/**
 * Clasa este folosita pentru efectuarea operatiei de impartire intre variabilele
 * unei expresii, conform specificatiilor din enuntul temei. Impartirea este realizata
 * intre instante ale clasei Object intrucat rezultatul operatiei poate fi atat Integer,
 * Double, cat si String.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class DivisionCalculator {

	/**
	 * Realizeaza impartirea a doua obiecte conform specificatiilor din enuntul
	 * temei, obiectele putand fi de tip Integer, Double sau String. Sunt
	 * folosite String-urile NaN_I si NaN_D pentru rezultatele NaN de tip
	 * Integer, respectiv Double.
	 * @param a operandul stang al impartirii
	 * @param b operandul drept al impartirii
	 * @return rezultatul impartirii
	 */
	public static Object divide(Object a, Object b) {

		if(a instanceof String || b instanceof String) {
			if((a.equals("NaN_I") || a.equals("NaN_D")) &&
					(b.equals("NaN_I") || b.equals("NaN_D"))) {
				if(((String)a).compareTo((String)b) <= 0)
					return a;
				return b;
			}
			else if(a.equals("NaN_I") || a.equals("NaN_D")) {
				if(b instanceof String) {
					return a;
				}
				else {
					if(a.equals("NaN_I") && b instanceof Integer) {
						return "NaN_I";
					}
					else {
						return "NaN_D";
					}
				}
			}
			else if(b.equals("NaN_I") || b.equals("NaN_D")) {
				if(a instanceof String) {
					if(b.equals("NaN_D")) {
						return b;
					}
					else {
						return a;
					}
				}
				else {
					if(a instanceof Integer && b.equals("NaN_I")) {
						return "NaN_I";
					}
					else {
						return "NaN_D";
					}
				}
			}
			else {
				if(a instanceof String && b instanceof Integer) {
					String str = (String)a;
					Integer nr = (Integer)b;
					if(nr <= 0) {
						return str;
					}
					else {
						str = str.substring(0, str.length() / nr);
					}
					
					return str;
				}
				if(a instanceof Integer && b instanceof String) {
					if(((String)b).length() != 0)
						return (Integer)a / ((String)b).length();
					return "NaN_I";
				}
				if(a instanceof String && b instanceof Double) {
					if((Double)b != 0)
						return ((String)a).length() / (Double)b;
					return "NaN_D";
				}
				if(a instanceof Double && b instanceof String) {
					if(((String)b).length() != 0)
						return (Double)a / ((String)b).length();
					return "NaN_D";
				}
				if(a instanceof String && b instanceof String) {
					if(((String)b).length() != 0)
						return ((String)a).length() / ((String)b).length();
					return "NaN_I";
				}
			}
		}
		else {
			if(a instanceof Integer && b instanceof Integer) {
				if((Integer)b == 0)
					return "NaN_I";
				return (Integer)a / (Integer)b;
			}
			if(a instanceof Integer && b instanceof Double) {
				if((Double)b != 0)
					return (Integer)a / (Double)b;
				return "NaN_D";
			}
			if(a instanceof Double && b instanceof Integer) {
				if((Integer)b == 0)
					return "NaN_D";
				return (Double)a / (Integer)b;
			}
			if(a instanceof Double && b instanceof Double) {
				if((Double)b != 0)
					return (Double)a / (Double)b;
				return "NaN_D";
			}
		}
		
		return null;
	}
}
