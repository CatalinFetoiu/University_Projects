
/**
 * clasa extinde clasa generica GeometricObject
 * in constructor sunt setate campurile corespunzatoare si este creat
 * dreptunghiul ce incadreaza rombul
 * @author Fetoiu Catalin-Emil
 *
 */
public class Rhombus extends GeometricObject {

	/* sunt retinute coordonatele varfurilor rombului */
	public double dX, dY;
	public double uX, uY;
	public double lX, lY;
	public double rX, rY;
	
	public Rhombus(double dX, double dY, double lX, double lY, double uX, double uY, double rX, double rY, int id) {
		super(lX, dY, rX, uY, id);
		this.dX = dX;
		this.dY = dY;
		this.uX = uX;
		this.uY = uY;
		this.lX = lX;
		this.lY = lY;
		this.rX = rX;
		this.rY = rY;	
	}
	
	/**
	 * metoda verifica daca un punct apartine rombului
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @return - id-ul rombului sau -1 daca punctul nu este continut de romb
	 */
	public int containsPoint(double x, double y) {
		
		/* se verifica daca punctul apartine triunghiului superior sau celui
		   inferior din romb */
		Triangle upper = new Triangle(uX, uY, lX, lY, rX, rY, 1);
		Triangle lower = new Triangle(dX, dY, lX, lY, rX, rY, 1);
		
		if(upper.containsPoint(x, y) == 1 || lower.containsPoint(x, y) == 1)
		{
			return id;
		}
		
		return -1;
	}
}
