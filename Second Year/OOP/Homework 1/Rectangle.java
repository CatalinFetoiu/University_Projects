
/**
 * clasa extinde clasa generica GeometriObject
 * constructorul apeleaza constructorul din clasa GeometricObject
 * @author Fetoiu Catalin-Emil
 *
 */
public class Rectangle extends GeometricObject {
	
	public Rectangle(double downX, double downY, double upperX, double upperY, int id) {
		super(downX, downY, upperX, upperY, id);
	}
	
	/**
	 * metoda verifica daca un punct apartine drepunghiului
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @return - id-ul dreptunghiului sau -1 daca punctul nu este continut de dreptunghi
	 */
	public int containsPoint(double x, double y) {
		
		if(downX <= x && x <= upperX && downY <= y && y <= upperY)
			return id;
		
		return -1;
	}
}
