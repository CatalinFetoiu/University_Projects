
import java.util.*;

/**
 * clasa retine structura unui nod din QuadTree
 * constructorul primeste ca parametrii centrul regiunii si "raza" acesteia
 * @author Fetoiu Catalin-Emil
 *
 */
public class QuadTreeNode {

	/* regiunea din plan corespunzatoare nodului */
	GeometricObject square;
	
	/* cele 4 sub-regiuni sunt retinute intr-un vector */
	QuadTreeNode children[];
	
	/* campul este folosit pentru a verifica daca un nod este frunza */
	boolean hasChildren;
	
	/* figurile din nod sunt retinute intr-o colectie de tip vector */
	Vector<GeometricObject> figures;
	
	public QuadTreeNode(double centerX, double centerY, double length) {
		
		square = new GeometricObject(centerX - length, centerY - length, centerX + length, centerY + length, 0);
		children = new QuadTreeNode[4];
		
		/* este initializata lista de figuri geometrice din nod */
		hasChildren = false;
		figures = new Vector<GeometricObject>();
	}
	
	/**
	 * metoda imparte regiunea din nod in 4, noile regiuni fiind plasate
	   in nodurile copil
	 * 
	 */
	public void split() {
		
		this.hasChildren = true;
		
		/* sunt determinate coordonatele noilor regiuni */
		double length = this.square.upperX - this.square.downX;
		double centerX = this.square.downX + (length/2);
		double centerY = this.square.downY + (length/2);
		double l = length/4;
		
		/* sunt create cele 4 noduri si regiuni corespunzatoare regiunii initiale */
		this.children[0] = new QuadTreeNode(centerX + l, centerY + l, l);
		this.children[1] = new QuadTreeNode(centerX - l, centerY + l, l);
		this.children[2] = new QuadTreeNode(centerX - l, centerY - l, l);
		this.children[3] = new QuadTreeNode(centerX + l, centerY - l, l);
	}
}
