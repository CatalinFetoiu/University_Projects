
/**
 * clasa extinde clasa generica GeometricObject
 * in constructor sunt setate campurile corespunzatoare si este creat
 * dreptunghiul ce incadreaza triunghiul
 * @author Fetoiu Catalin-Emil
 *
 */
public class Triangle extends GeometricObject {

	/* sunt retinute coordonatele varfurilor triunghiului */
	public double x1, y1;
	public double x2, y2;
	public double x3, y3;
	
	public Triangle(double x1, double y1, double x2, double y2, double x3, double y3, int id) {
		
		super(Math.min(Math.min(x1, x2), x3), Math.min(Math.min(y1, y2), y3),
			Math.max(Math.max(x1, x2), x3), Math.max(Math.max(y1, y2), y3), id);
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
		this.x3 = x3;
		this.y3 = y3;
	}
	
	/**
	 * metoda returneaza aria unui triunghi
	 * @param x1 - coordonata de pe axa Ox a primului varf
	 * @param y1 - coordonata de pe axa Oy a primului varf
	 * @param x2 - coordonata de pe axa Ox a celui de-al doilea varf
	 * @param y2 - coordonata de pe axa Oy a celui de-al doilea varf
	 * @param x3 - coordonata de pe axa Ox a celui de-al treilea varf
	 * @param y3 - coordonata de pe axa Oy a celui de-al treilea varf
	 * @return - aria triunghiului
	 */
	public double area(double x1, double y1, double x2, double y2, double x3, double y3) {
		
		return Math.abs(x1 * (y2-y3) + x2 * (y3-y1) + x3 * (y1-y2));
	}
	
	/**
	 * metoda verifica daca un punct apartine triunghiului
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @return - id-ul triunghiului sau -1 daca punctul nu este continut de triunghi
	 */
	public int containsPoint(double x, double y) {
		
		/* se verifica daca suma ariilor celor trei triunghiuri formate de
		   punct si varfuri este egala cu aria triunghiului initial */
		double ABC = area(x1, y1, x2, y2, x3, y3);
		double ABP = area(x1, y1, x2, y2, x, y);
		double APC = area(x1, y1, x, y, x3, y3);
		double PBC = area(x, y, x2, y2, x3, y3);
		
		if(ABP + APC + PBC == ABC)
			return id;
		else
			return -1;
	}
}
