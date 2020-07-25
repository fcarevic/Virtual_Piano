package poop;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.stream.Stream;



public class MidiMap {
	public static HashMap<Character, MusicSymbol> mapa= new HashMap<>();
	public static LinkedList<Character> listaPovisenih= new LinkedList<Character>();
	public static LinkedList<Character> listaNepovisenih= new LinkedList<Character>();
	public static void ucitajMapu(String fileName) {
		File fajl = new File(fileName);
		try {
			BufferedReader br = new BufferedReader(new FileReader(fajl));;
			Stream<String> str = br.lines();
			str.forEach(l->{
				MusicSymbol s=null;
				
				if(s==null) {
					String broj= new String(); 
							broj+=l.charAt(l.length()-2);
							broj+=l.charAt(l.length()-1);
							Character znak = l.matches(".*#.*")? l.charAt(4) : l.charAt(3);
					s= new Note(Trajanje.CETVRITNA,l.charAt(0), l.charAt(2),Integer.parseInt(znak.toString()),l.matches(".*#.*") , Integer.parseInt(broj));
				}
				mapa.put(l.charAt(0), s);
				if(l.matches(".*#.*") ) listaPovisenih.add(l.charAt(0)) ; else listaNepovisenih.add(l.charAt(0));
				
			});
			br.close();
			mapa.put(' ', new Pause(Trajanje.OSMINA,' ')); 
			mapa.put('|',  new Pause(Trajanje.CETVRITNA,'|'));
		
		} catch (FileNotFoundException e) {
			System.out.println("Fajl nije pronadjen");
			
		} catch (IOException e) {
			
		}
	}
		
	public static  void ispisi() {
		mapa.values().stream().forEach(System.out::println);
	} 
		
public static void main (String [] args) {
	MidiMap.ucitajMapu("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\map.csv");
	MidiMap.ispisi();
	
	
	
}
}
