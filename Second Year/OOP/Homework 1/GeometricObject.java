
/**
 * clasa generica ce este mostenita de clasele corespunzatoare
 * figurilor de tip dreptunghi, cerc, triunghi sau romb
 * constructorul creeaza dreptunghiul ce incadreaza figura geometrica
 * @author Fetoiu Catalin-Emil
 *
 */
public class GeometricObject {

	/* coordonatele dreptunghiului ce incadreaza figura geometrica */
	public double downX;
	public double downY;
	public double upperX;
	public double upperY;
	
	/* id-ul figurii geometrice */
	int id;
	
	public GeometricObject(double downX, double downY, double upperX, double upperY, int id) {
		
		this.downX = downX;
		this.downY = downY;
		this.upperX = upperX;
		this.upperY = upperY;
		this.id = id;
	}
	
	/**
	 * metoda verifica daca dreptunghiurile ce incadreaza doua figuri geometrice
	 * se intersecteaza
	 * @param figure - figura testata pentru coliziune
	 * @return true sau false
	 */
	public boolean contains(GeometricObject figure) {
		
		/* se verifica daca figura primita ca parametru se afla la stanga
		   sau la dreapta figurii din clasa */
		if(downX > figure.upperX || upperX < figure.downX)
			return false;
		/* se verifica daca figura primita ca parametru se afla deasupra
		   sau dedesubtul figurii din clasa */
		if(downY > figure.upperY ||  upperY <  figure.downY)
			return false;
		
		return true;
	}
}
