
/**
 * Clasa este folosita pentru efectuarea operatiei de inmultire intre variabilele
 * unei expresii, conform specificatiilor din enuntul temei. Inmultirea este realizata
 * intre instante ale clasei Object intrucat rezultatul operatiei poate fi atat Integer,
 * Double, cat si String.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class MultiplicationCalculator {

	/**
	 * Realizeaza inmultirea a doua obiecte conform specificatiilor din enuntul
	 * temei, obiectele putand fi de tip Integer, Double sau String. Sunt
	 * folosite String-urile NaN_I si NaN_D pentru rezultatele NaN de tip
	 * Integer, respectiv Double.
	 * @param a operandul stang al inmultirii
	 * @param b operandul drept al inmultirii
	 * @return rezultatul inmultirii
	 */
	public static Object multiply(Object a, Object b) {	

		if(a instanceof String || b instanceof String) {
			if((a.equals("NaN_I") || a.equals("NaN_D")) &&
					(b.equals("NaN_I") || b.equals("NaN_D"))) {
				if(((String)a).compareTo((String)b) <= 0)
					return a;
				return b;
			}
			else if(a.equals("NaN_I") || a.equals("NaN_D")) {
				if(b instanceof String) {
					if(a.equals("NaN_D")) {
						return a;
					}
					else {
						return "";
					}
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
						return "";
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
						str = "";
					}
					else {
						String aux = new String(str);
						for(int i = 0; i < (nr-1); i++)
							str += aux;
					}
					
					return str;
				}
				if(a instanceof Integer && b instanceof String) {
					String str = (String)b;
					Integer nr = (Integer)a;
					if(nr <= 0) {
						str = "";
					}
					else {
						String aux = new String(str);
						for(int i = 0; i < (nr-1); i++)
							str += aux;
					}
					
					return str;
				}
				if(a instanceof String && b instanceof Double) {
					return ((String)a).length() * (Double)b;
				}
				if(a instanceof Double && b instanceof String) {
					return (Double)a * ((String)b).length();
				}
				if(a instanceof String && b instanceof String) {
					return ((String)a).length() * ((String)b).length();
				}
			}
		}
		else {
			if(a instanceof Integer && b instanceof Integer) {
				return (Integer)a * (Integer)b;
			}
			if(a instanceof Integer && b instanceof Double) {
				return (Integer)a * (Double)b;
			}
			if(a instanceof Double && b instanceof Integer) {
				return (Double)a * (Integer)b;
			}
			if(a instanceof Double && b instanceof Double) {
				return (Double)a * (Double)b;
			}
		}
		
		return null;
	}
}
