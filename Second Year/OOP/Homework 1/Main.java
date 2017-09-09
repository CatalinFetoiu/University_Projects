
import java.util.Scanner;
import java.io.*;

/**
 * clasa este folosita pentru citirea input-ului si afisarea
 * output-ului corespunzator
 * @author Fetoiu Catalin-Emil
 *
 */
public class Main {

	/* campurile in care se realizeaza citirea coordonatelor */
	double x1, y1, x2, y2, x3, y3, x4, y4, radius, x, y;
	
	/**
	 * metoda citeste coordonatele unui punct
	 * @param input - indica fisierul din care se efectueaza citirea
	 */
	public void ReadPoint(Scanner input) {
		
		x = input.nextDouble();
		y = input.nextDouble();
	}
	
	/**
	 * metoda citeste coordonatele unui dreptunghi
	 * @param input - indica fisierul din care se efectueaza citirea
	 */
	public void ReadRectangle(Scanner input) {
		
		x1 = input.nextDouble();
		y1 = input.nextDouble();
		x2 = input.nextDouble();
		y2 = input.nextDouble();
	}
	
	/**
	 * metoda citeste coordonatele unui triunghi
	 * @param input - indica fisierul din care se efectueaza citirea
	 */
	public void ReadTriangle(Scanner input) {
		
		x1 = input.nextDouble();
		y1 = input.nextDouble();
		x2 = input.nextDouble();
		y2 = input.nextDouble();
		x3 = input.nextDouble();
		y3 = input.nextDouble();
	}
	
	/**
	 * metoda citeste coordonatele centrului unui cerc si raza acestuia
	 * @param input - indica fisierul din care se efectueaza citirea
	 */
	public void ReadCircle(Scanner input) {
		
		radius = input.nextDouble();
		x = input.nextDouble();
		y = input.nextDouble();
	}
	
	/**
	 * metoda citeste coordonatele unui romb
	 * @param input - indica fisierul din care se efectueaza citirea
	 */
	public void ReadRhombus(Scanner input) {
		
		x1 = input.nextDouble();
		y1 = input.nextDouble();
		x2 = input.nextDouble();
		y2 = input.nextDouble();
		x3 = input.nextDouble();
		y3 = input.nextDouble();
		x4 = input.nextDouble();
		y4 = input.nextDouble();
	}
	
	/**
	 * metoda citeste toate operatiile din fisierul de intrare si afiseaza
	 * output-ul corespunzator fiecareia dintre acestea
	 * @throws IOException - este aruncata exceptia in cazul in care nu
	 * se reuseste citirea
	 */
	public void Solution() throws IOException {
		
		/* sunt deschise pentru citire, respectiv scriere fisierele de input si output */
		Scanner input = new Scanner(new BufferedReader(new FileReader("quadtree.in")));
		FileWriter writer = new FileWriter("quadtree.out");
		
		/* sunt citite coordonatele regiunii initiale */
		ReadRectangle(input);
		
		double length = (x2 - x1)/2;
		x = x1 + length;
		y = y1 + length;
		
		/* este initializat arborele */
		QuadTree q = new QuadTree(x, y, length);

		while(input.hasNextInt())
		{
			/* este citita operatia curenta */
			int operation = input.nextInt();
			
			/* in cazul inserarii sunt citite coordonatele figurilor, acestea fiind
			   inserate in arbore si in tabela de hash a acestuia */
			if(operation == 11)
			{
				int type = input.nextInt();
				int id = input.nextInt();
				
				if(type == 1)
				{
					ReadRectangle(input);
					q.mi.put(id, new Rectangle(x1, y1, x2, y2, id));
					q.insert(q.root, new Rectangle(x1, y1, x2, y2, id));
				}
				if(type == 2)
				{
					ReadTriangle(input);
					q.mi.put(id, new Triangle(x1, y1, x2, y2, x3, y3, id));
					q.insert(q.root, new Triangle(x1, y1, x2, y2, x3, y3, id));
				}
				if(type == 3)
				{
					ReadCircle(input);
					q.mi.put(id, new Circle(x, y, radius, id));
					q.insert(q.root, new Circle(x, y, radius, id));
				}
				if(type == 4)
				{
					ReadRhombus(input);
					q.mi.put(id, new Rhombus(x1, y1, x2, y2, x3, y3, x4, y4, id));
					q.insert(q.root, new Rhombus(x1, y1, x2, y2, x3, y3, x4, y4, id));
				}
			}
			
			/* in cazul stergerii este citit id-ul si este eliminata din arbore
			   figura corespunzatoare acestui id */
			if(operation == 22)
			{
				int id = input.nextInt();
				q.remove(q.root, q.mi.get(id));
			}
			
			/* pentru gasirea figurilor ce contin un anumit punct este apelata
			   metoda printPoints din clasa CollisionDetector */
			if(operation == 33)
			{
				ReadPoint(input);
				CollisionDetector.printPoints(q, x, y, writer);
			}
			
			/* pentru detectarea coliziunilor este apelata metoda printCollisions
			   din clasa CollisionDetector */
			if(operation == 44)
			{
				int type = input.nextInt();
				
				if(type == 1)
				{
					ReadRectangle(input);
					CollisionDetector.printCollisions(q, new Rectangle(x1, y1, x2, y2, -1), writer);
				}
				if(type == 2)
				{
					ReadTriangle(input);
					CollisionDetector.printCollisions(q, new Triangle(x1, y1, x2, y2, x3, y3, -1), writer);
				}
				if(type == 3)
				{
					ReadCircle(input);
					CollisionDetector.printCollisions(q, new Circle(x, y, radius, -1), writer);
				}
				if(type == 4)
				{
					ReadRhombus(input);
					CollisionDetector.printCollisions(q, new Rhombus(x1, y1, x2, y2, x3, y3, x4, y4, -1), writer);
				}
			}
		}
		
		/* sunt inchise fisierele de input/output */
		input.close();
		writer.flush();
		writer.close();		
	}
	
	/* este citit inputul si sunt afisate id-urile cerute de operatii */
	public static void main(String[] args) throws IOException {
		
		Main solver = new Main();
		solver.Solution();	
	}
}
