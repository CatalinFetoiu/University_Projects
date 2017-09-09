
/**
 * Clasa este folosita pentru efectuarea operatiei de adunare intre variabilele
 * unei expresii, conform specificatiilor din enuntul temei. Adunarea este realizata
 * intre instante ale clasei Object intrucat rezultatul operatiei poate fi atat Integer,
 * Double, cat si String.
 * 
 * @author Fetoiu Catalin-Emil
 *
 */
public class AdditionCalculator {

	/**
	 * Realizeaza adunarea a doua obiecte conform specificatiilor din enuntul
	 * temei, obiectele putand fi de tip Integer, Double sau String. Sunt
	 * folosite String-urile NaN_I si NaN_D pentru rezultatele NaN de tip
	 * Integer, respectiv Double.
	 * @param a operandul stang al sumei
	 * @param b operandul drept al sumei
	 * @return rezultatul adunarii
	 */
	public static Object add(Object a, Object b) {

		if(a instanceof String || b instanceof String) {
			if((a.equals("NaN_I") || a.equals("NaN_D")) && 
					(b.equals("NaN_I") || b.equals("NaN_D"))) {
				if(((String)a).compareTo((String)b) <= 0)
					return a;
				return b;
			}
			else if(a.equals("NaN_I") || a.equals("NaN_D")) {
				if(b instanceof String) {
					return "NaN" + (String)b;
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
					return (String)a + "NaN";
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
					return (String)a + (Integer)b;
				}
				if(a instanceof String && b instanceof Double) {
					Double aux = (Double)b;
					aux *= 100;
					aux = (double)Math.round(aux);
					aux /= 100;
					return (String)a + aux;
				}
				if(a instanceof Integer && b instanceof String) {
					return (Integer)a + (String)b;
				}
				if(a instanceof Double && b instanceof String) {
					double aux = (Double)a;
					aux *= 100;
					aux = (double)Math.round(aux);
					aux /= 100;
					return aux + (String)b;
				}
				if(a instanceof String && b instanceof String) {
					return (String)a + (String)b;
				}
			}
		}
		else {
			if(a instanceof Integer && b instanceof Integer) {
				return (Integer)a + (Integer)b;
			}
			if(a instanceof Integer && b instanceof Double) {
				return (Integer)a + (Double)b;
			}
			if(a instanceof Double && b instanceof Integer) {
				return (Double)a + (Integer)b;
			}
			if(a instanceof Double && b instanceof Double) {
				return (Double)a + (Double)b;
			}
		}
		
		return null;
	}
}
