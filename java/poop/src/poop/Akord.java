package poop;

import java.util.LinkedList;

public class Akord extends MusicSymbol {
	private LinkedList<MusicSymbol> niz = new LinkedList<>();
	public Akord() {
		super(Trajanje.CETVRITNA,'~');
		
		// TODO Auto-generated constructor stub
	}

	public void dodaj (MusicSymbol s) {niz.add(s);}
	@Override
	public String getSlovo() {
		
		StringBuilder sb = new StringBuilder();
		
		for(MusicSymbol s:niz)sb.append(s.getSlovo());
	   
		return sb.toString();
		
	}
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("[");
		
		for(MusicSymbol s : niz) sb.append(s.toString());
		
		sb.append("]");
		return sb.toString();
	}
	public Object[] getNizNota() {return niz.toArray();}
	public LinkedList<MusicSymbol> getLista(){return niz;}
	
	public boolean equals (Object o ) {
		if(o instanceof Akord) {
			Akord akord=(Akord) o;
		 boolean flag=false;
		 for(MusicSymbol s : niz) {
			 flag=false;
			 for(MusicSymbol s2 : akord.niz) if(s.equals(s2)) flag=true;
			 if(!flag)return flag;}
		return true;	
		} return false;
	}
}


