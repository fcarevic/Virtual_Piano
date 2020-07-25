package poop;

public class Note extends MusicSymbol {

	private char visina;
	private int oktava;
	private boolean povisena;
	private int midi_num;
	public Note(Trajanje t,char slovo, char visina, int oktava, boolean povisena, int midi_num) {
		super(t,slovo);
		this.midi_num=midi_num;
		this.oktava=oktava;
		this.povisena=povisena;
		this.visina = visina;
		// TODO Auto-generated constructor stub
	}
	
	public int getMidiNum() {return midi_num;}
	public boolean isPovisena() {return povisena;}
	public char getVisina() {
		return Character.toUpperCase(visina);
	}

	public int getOktava() {
		return oktava;
	}

	public void setPovisena(boolean flag) {
		povisena=flag;
		
	}
	public String toString () {
		Character c;
		if(getTrajanje().getVal()==2)
		 c= Character.toUpperCase(visina);
		else c= Character.toLowerCase(visina);
		return  ""+c+(povisena? "#" : "")+ oktava ;
	}
	public Note clone() {
		Note nota = (Note) super.clone();
		 return nota;
	}
	public boolean equals (Object o ) {
		if(o instanceof Note) {
			Note nota=(Note) o;
			return Character.toUpperCase(visina)==Character.toUpperCase(nota.visina )&& povisena==nota.povisena && oktava==nota.oktava;
			
		} return false;
	}
}
