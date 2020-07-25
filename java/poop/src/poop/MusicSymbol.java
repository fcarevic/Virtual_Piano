package poop;

public abstract class MusicSymbol implements Cloneable {
	private Trajanje trajanje;
	private char slovo;
	public MusicSymbol(Trajanje t, char slovo) {
		this.slovo=slovo;
		this.trajanje = t;
	}
	public Trajanje getTrajanje() {return trajanje;}
	public void setTrajanje(Trajanje t) {trajanje= t;}
	public String getSlovo() {return slovo+"";}
	public MusicSymbol clone() {
      try {
		MusicSymbol s= (MusicSymbol) super.clone();
		return s;
	} catch (CloneNotSupportedException e) {
		// TODO Auto-generated catch block
		System.out.println("Ne prihvata cloneable");
		return null;
	}
      
		
		
		
	}
}
