
/**
 * Interfata ce trebuie implmentata de toate nodurile din arborele de sintaxa
 * pentru a putea fi vizitate si evaluate de catre o variabila de tip Visitor.
 * @author Fetoiu Catalin-Emil
 *
 */
public interface Visitable {

	public Object accept(Visitor v) throws Exception;
}
