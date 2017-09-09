
import java.util.*;

/**
 * clasa retine intregul QuadTree
 * constructorul plaseaza in radacina regiunea initiala din plan si
 * initializeaza tabela de hash
 * @author Fetoiu Catalin-Emil
 *
 */
public class QuadTree {

	/* este retinuta radacina arborelui */
	QuadTreeNode root;
	
	/* este folosita o tabela de hash pentru a mapa id-urile cu figurile
	   geometrice corespunzatoare */
	HashMap<Integer, GeometricObject> mi;
	
	public QuadTree(double centerX, double centerY, double length) {
		
		root = new QuadTreeNode(centerX, centerY, length);
		mi = new HashMap<Integer, GeometricObject>();
	}
	
	/**
	 * metoda insereaza recursiv o figura geometrica in QuadTree
	 * @param node - nodul in care se realizeaza inserarea
	 * @param figure - figura geometrica ce este inserata
	 */
	public void insert(QuadTreeNode node, GeometricObject figure) {
		
		/* daca nodul este frunza */
		if(node.hasChildren == false)
		{
			/* daca figura se afla in regiunea continuta de nod */
			if(node.square.contains(figure)) 
			{	
				/* sunt parcurse figurile din lista si este verificata
				   coliziunea noii figuri cu acestea */
				boolean hasCollision = false;
				for(int i = 0; i < node.figures.size(); i++)
				{
					if(node.figures.elementAt(i).contains(figure))
					{
						hasCollision = true;
						break;
					}
				}
				
				/* daca este detectata cel putin o coliziune sau lista este vida 
				   noua figura este adaugata in lista */
				if(hasCollision || node.figures.isEmpty())
				{
					node.figures.add(figure);
				}
				else
				{
					/* in caz contrar sunt creati cei 4 fii ai nodului */
					node.split();
					
					/* sunt inserate figurile existente in lista in nodurile create */
					for(int i = 0; i < node.figures.size(); i++)
					{
						for(int j = 0; j < 4; j++)
						{
							if(node.children[j].square.contains(node.figures.elementAt(i)))
							{
								insert(node.children[j], node.figures.elementAt(i));
							}
						}
					}
					
					/* lista de figuri este reinitializata */
					node.figures = new Vector<GeometricObject>();
					
					/* este inserata apoi noua figura in cele 4 noduri create */
					for(int i = 0; i < 4; i++)
					{
						if(node.children[i].square.contains(figure))
						{
							insert(node.children[i], figure);
						}
					}
				}
			}
		}
		
		/* daca nodul nu este frunza */
		else
		{
			/* figura este inserata recursiv in nodurile copil daca se afla 
			   in regiunile corespunzatoare acestora */
			for(int i = 0; i < 4; i++)
			{
				if(node.children[i].square.contains(figure))
				{
					insert(node.children[i], figure);
				}
			}	
		}
	}

	/**
	 * metoda sterge recursiv din arbore toate figurile cu un anumit id
	 * @param node - nodul din care este stearsa figura
	 * @param figure - figura geometrica ce este stearsa
	 */
	public void remove(QuadTreeNode node, GeometricObject figure) {
		
		/* daca nodul este frunza */
		if(node.hasChildren == false)
		{
			/* sunt parcurse figurile din lista, fiind stearsa cea cu
			   id-ul egal cu al figurii date ca parametru */
			if(!node.figures.isEmpty())
			{
				for(int i = 0; i < node.figures.size(); i++)
				{
					if(node.figures.elementAt(i).id == figure.id)
					{
						node.figures.remove(i);
						break;
					}
				}
			}
			
		}
		/* daca nodul nu este frunza */
		else
		{
			/* sunt cautate si eliminate figurile cu id-ul corespunzator in
			   cele 4 sub-cadrane */
			for(int i = 0; i < 4; i++)
			{
				if(node.children[i].square.contains(figure))
					remove(node.children[i], figure);
			}
			
			/* dupa stergere se verifica daca toti fiii nodului curent sunt frunze */
			boolean allLeafs = true;
			for(int i = 0; i < 4; i++)
			{
				if(node.children[i].hasChildren == true)
				{
					allLeafs = false;
					break;
				}
			}
				
			if(allLeafs)
			{
				/* sunt adaugate listele nevide intr-un vector */
				Vector<Integer> lists = new Vector<Integer>();
					
				for(int i = 0; i < 4; i++)
				{
					if(!node.children[i].figures.isEmpty())
						lists.add(new Integer(i));
				}
					
				/* se verifica daca listele sunt identice */
				boolean sameList = true;
				for(int i = 1; i < lists.size(); i++)
				{
					if(!node.children[lists.elementAt(i)].figures.equals(node.children[lists.elementAt(0)].figures))
					{
						sameList = false;
						break;
					}
				}
					
				/* daca listele sunt identice sau sunt toate vide acestea sunt mutate in nodul
				   curent si acesta este transformat in frunza */
				if(sameList || lists.isEmpty())
				{
					node.figures = node.children[lists.elementAt(0).intValue()].figures;
					node.hasChildren = false;
				}
			}
		}
	}
}
