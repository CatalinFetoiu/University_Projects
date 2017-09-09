
/**
 * clasa extinde clasa generica GeometricObject
 * in constructor sunt setate campurile corespunzatoare si este creat
 * dreptunghiul ce incadreaza cercul
 * @author Fetoiu Catalin-Emil
 *
 */
public class Circle extends GeometricObject {

	/* sunt retinute coordonatele centrului cercului si raza */
	double centerX;
	double centerY;
	double radius;
	
	public Circle(double centerX, double centerY, double radius, int id) {
		
		super(centerX - radius, centerY - radius, centerX + radius, centerY + radius, id);
		this.centerX = centerX;
		this.centerY = centerY;
		this.radius = radius;
	}
	
	/**
	 * metoda verifica daca un punct apartine cercului
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @return - id-ul cercului sau -1 daca punctul nu este continut de cerc
	 */
	public int containsPoint(double x, double y) {
		
		/* se verifica daca distanta de la punct la centrul cercului este mai
		   mica decat raza */
		if(Math.sqrt((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY)) <= radius)
			return id;
		
		return -1;
	}
}
