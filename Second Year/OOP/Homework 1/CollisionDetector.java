
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collections;
import java.util.Vector;

/**
 * clasa este folosita pentru a determina coliziunile intre figuri si totodata
 * pentru determinarea figurilor in care se afla un anumit punct
 * @author Fetoiu Catalin-Emil
 *
 */
public class CollisionDetector {

	/**
	 * metoda cauta in arbore figurile ce contin punctul primit ca parametru, adaugand
	 * id-urile acestora in vectorul found
	 * @param node - nodul din care se incepe cautarea
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @param found - vectorul in care sunt retinute id-urile figurilor
	 */
	public static void findPoint(QuadTreeNode node, double x, double y, Vector<Integer> found) {
			
		/* daca nodul curent este frunza */
		if(node.hasChildren == false)
		{
			/* daca lista de figuri este nevida */
			if(!node.figures.isEmpty())
			{
				/* sunt parcurse figurile din lista si pentru fiecare dintre acestea se
				   testeaza apartenenta punctului cu ajutorul functiilor specifice celor
				   4 tipuri de forme geometrice date */
				for(int i = 0; i < node.figures.size(); i++)
				{
					boolean isContained = false;
					if(node.figures.elementAt(i) instanceof Circle)
					{
						if(((Circle)node.figures.elementAt(i)).containsPoint(x, y) != -1)
						{
							isContained = true;
						}
					}
					if(node.figures.elementAt(i) instanceof Rectangle)
					{
						if(((Rectangle)node.figures.elementAt(i)).containsPoint(x, y) != -1)
						{
							isContained = true;
						}
					}
					if(node.figures.elementAt(i) instanceof Triangle)
					{
						if(((Triangle)node.figures.elementAt(i)).containsPoint(x, y) != -1)
						{
							isContained = true;
						}
					}
					if(node.figures.elementAt(i) instanceof Rhombus)
					{
						if(((Rhombus)node.figures.elementAt(i)).containsPoint(x, y) != -1)
					    {
							isContained = true;
						}
					}
					
					/* daca figura contine punctul dat si id-ul nu se regaseste deja in vector
					   acesta este adaugat */
					if(isContained == true)
					{
						int aux = node.figures.elementAt(i).id;
						if(found.contains(aux) == false)
							found.add(aux);
					}
				}
			}
		}
		/* daca nodul curent nu este frunza */
		else
		{
			/* punctul este cautat in cele 4 sub-cadrane */
			for(int i = 0; i < 4; i++)
			{
				if(node.children[i].square.contains(new Rectangle(x, y, x, y, 0)))
				{
					findPoint(node.children[i], x, y, found);
				}
			}
		}
	}

	/**
	 * metoda afiseaza id-urile figurilor ce contin un anumit punct
	 * @param q - arborele in care se realizeaza cautarea
	 * @param x - coordonata de pe axa Ox a punctului
	 * @param y - coordonata de pe axa Oy a punctului
	 * @param writer - indica fisierul in care se efectueaza scrierea
	 * @throws IOException - este aruncata exceptia in cazul in care nu
	 * se reuseste citirea
	 */
	public static void printPoints(QuadTree q, double x, double y, FileWriter writer) throws IOException {
		
		/* sunt determinate id-urile cerute */
		Vector<Integer> found = new Vector<Integer>();
		findPoint(q.root, x, y, found);
		
		/* id-urile sunt sortate */
		Collections.sort(found);
		
		/* daca nu s-a gasit nicio figura este afisat mesajul "NIL" */
		if(found.size() == 0)
			writer.write("NIL" + "\n");
		/* altfel sunt afisate id-urile sortate */
		else
		{	
			for(int i = 0; i < found.size(); i++)
			{
				if(i < found.size() - 1)
					writer.write(found.elementAt(i) + " ");
				else
					writer.write(found.elementAt(i) + "\n");
			}
		}
	}
	
	/**
	 * metoda cauta in arbore figurile ce se intersecteaza cu figura primita ca
	 * parametru, adaugand id-urile acestora in vectorul found
	 * @param node - nodul din care se incepe cautarea
	 * @param figure - figura ce este cautata
	 * @param found - vectorul in care sunt retinute id-urile figurilor
	 */
	public static void detectCollisions(QuadTreeNode node, GeometricObject figure, Vector<Integer> found) {
			
		/* daca nodul curent este frunza */
		if(node.hasChildren == false)
		{
			/* daca lista de figuri este nevida */
			if(!node.figures.isEmpty())
			{
				/* pentru fiecare figura, daca este detectata o coliziune este
				   adaugat id-ul in vector daca acesta nu a fost adaugat deja */
				for(int i = 0; i < node.figures.size(); i++) 
				{
					if(node.figures.elementAt(i).contains(figure))
					{	
						int aux = node.figures.elementAt(i).id;
						if(found.contains(aux) == false)
							found.add(aux);
					}
				}
			}
		}
		/* daca nodul nu este frunza */
		else
		{
			/* sunt cautate coliziuni cu figurile din cele 4
			   sub-regiuni */
			for(int i = 0; i < 4; i++)
			{
				if(node.children[i].square.contains(figure))
				{
					detectCollisions(node.children[i], figure, found);
				}
			}
		}
	}

	/* functia afiseaza id-urile corespunzatoare coliziunilor */
	/**
	 * metoda afiseaza id-urile corespunzatoare coliziunilor
	 * @param q - arborele in care se realizeaza cautarea
	 * @param figure - figura ale carei coliziuni sunt cautate
	 * @param writer - indica fisierul in care se efectueaza scrierea
	 * @throws IOException - este aruncata exceptia in cazul in care nu
	 * se reuseste citirea
	 */
	public static void printCollisions(QuadTree q, GeometricObject figure, FileWriter writer) throws IOException {
		
		/* sunt determinate id-urile cerute */
		Vector<Integer> found = new Vector<Integer>();
		detectCollisions(q.root, figure, found);
		
		/* id-urile sunt sortate */
		Collections.sort(found);
		
		/* daca nu s-a gasit nicio figura este afisat mesajul "NIL" */
		if(found.size() == 0)
		{
			writer.write("NIL" + "\n");
		}
		/* altfel sunt afisate id-urile sortate */
		else
		{
			for(int i = 0; i < found.size(); i++)
			{
				if(i < found.size() - 1)
					writer.write(found.elementAt(i) + " ");
				else
					writer.write(found.elementAt(i) + "\n");
			}
		}
	}
}