package poop;

public class Pause extends MusicSymbol {

	public Pause(Trajanje t, char slovo) {
		super(t,slovo);
		// TODO Auto-generated constructor stub
	}
	
	public String toString() {
		return getSlovo()+"";
	}

	public Pause clone () {
		return (Pause) super.clone();
	}
	
	
}
