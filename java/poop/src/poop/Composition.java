package poop;

import java.util.Queue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;

public class Composition {
	private LinkedList<MusicSymbol> lista= new LinkedList<>();
	private Queue<MusicSymbol> red= new LinkedList<MusicSymbol>();
	public Composition() {}
	
	public void ucitaj(String path) {
		lista.clear();
		
		File file = new File(path);
		try {
			BufferedReader br = new BufferedReader(new FileReader(file));
			Stream<String> str= br.lines();
			Pattern regex= Pattern.compile("([^\\[]*)\\[?([^\\]]*)\\]?");
			
			str.forEach(l->{
				//System.out.println("usao");
				Matcher match=regex.matcher(l);
				while(match.find()) {
					String s=null;
					String s2=null;
					 try{s = match.group(1);
						for(char c: s.toCharArray()) {
							 MusicSymbol simbol = MidiMap.mapa.get(c).clone();
							// System.out.println(simbol+" " + c);
							 ;
							 lista.add(simbol); red.add(simbol);
							}} catch (IndexOutOfBoundsException e) {}
					//System.out.println("PRVA GRUPA ::"+s);
					try {s2 = match.group(2);
					//System.out.println("DRUGA GRUPA ::"+s2);
					if(s2.matches(".* .*")) {
						for(char c: s2.toCharArray()) {
							if(c==' ') continue; 
								Note simbol = (Note)MidiMap.mapa.get(c).clone();
							   simbol.setTrajanje(Trajanje.OSMINA);
							// System.out.println(simbol+" " + c);
							 ;
							 lista.add(simbol);
							 red.add(simbol);
						
					}}
					else if(!s2.isEmpty())
					{
					Akord akord = new Akord();
					for(char c: s2.toCharArray()) {
						 MusicSymbol simbol = MidiMap.mapa.get(c).clone();
						 akord.dodaj(simbol);
						}
					lista.add(akord);
					red.add(akord);
					}}catch (IndexOutOfBoundsException e) { }
					if(s==null && s2==null) break;
				}
			});
			br.close();
		} catch (FileNotFoundException e) {
			System.out.println("Fajl kompozicije nije pronadjen");
		} catch (IOException e) {}
		
	}
	
	public void dodajSimbol(MusicSymbol s) {
		lista.add(s);
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (MusicSymbol s : lista) {
			sb.append(s.toString());
		}
		return sb.toString();
	}
	
	public void crtaj(Graphics g,int visina,int sirina,boolean flag) {
		
		int br=0;
		int x=0;
		
		synchronized (red) { //jer i nit RealTimePlayer-a  menja red
			
		for(MusicSymbol simbol:red) {
			Color zelena= null,crvena=null;
			if(simbol instanceof Pause) {zelena = Color.GREEN.darker(); crvena=Color.RED.darker();}
			else {zelena = Color.GREEN; crvena=Color.RED;}
			
			int newx=0;
			if(simbol.getTrajanje().getVal()!=2) { g.setColor(zelena);  newx=x+sirina/20;}
			else {g.setColor(crvena); newx=x+sirina/10;}
			g.fillRect(x, 0, newx-x, visina);
			int offset= (simbol instanceof Note? newx - (newx - x)/2 : newx - 4*(newx - x)/5);
			if(!(simbol instanceof Pause)) {
				g.setColor(Color.BLACK);
			     if(simbol instanceof Akord){
			    	 Akord akord= (Akord)simbol;
			    	 int offset2=  (visina-20) /  akord.getLista().size();
			    	 int oldy=20;
			    	 for(MusicSymbol s : akord.getLista()) {
			    		 
			    		 if(flag)g.drawString(s.toString(), offset+4, oldy);
							else g.drawString(s.getSlovo(), offset+4, oldy);
			    	 oldy+=offset2;	 
			    	}
			      
			    	 
			     }
			     else 
				{
				if(flag)g.drawString(simbol.toString(), offset, visina/2);
				else g.drawString(simbol.getSlovo(), offset, visina/2);}
			}
				br++;
			g.fillRect(br*sirina/10, visina+1, 4, visina);
			x=newx;
			if(br==11) break;
			
		}}
		
		
	}
	synchronized public void resetujRed() {
		red= (Queue<MusicSymbol>) lista.clone();
		
	}
	public Object[] getNizSimbola() {
		return lista.toArray();
	}

	public Queue<MusicSymbol> getQueue() {
		// TODO Auto-generated method stub
		return red;
	}
	
	
	
	
	
	
	/*public static void main (String [] args) {
		MidiMap.ucitajMapu("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\map.csv");
		Composition c= new Composition();
		c.ucitaj("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\input\\ode.txt");
		System.out.println(c);
		
		
		
		
		
		
	}*/
	

}
