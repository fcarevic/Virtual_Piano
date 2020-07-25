package poop;

import javax.sound.midi.MidiChannel;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Synthesizer;

import java.util.Queue;
import java.util.Scanner;
public class RealTimePlayer {
 private static final int DEFAULT_INSTRUMENT = 15;
 public static int PERIOD=300;
 private MidiChannel channel;
private CompositionPlayer compPlayer;
private Piano piano;
 
 
 public RealTimePlayer(Piano piano) throws MidiUnavailableException {
 this(piano,DEFAULT_INSTRUMENT);
 }
 public RealTimePlayer(Piano piano , int instrument) throws
MidiUnavailableException {
 channel = getChannel(instrument);
 this.piano=piano;


 }
 private void play(final int note) {
 channel.noteOn(note, 50);
 }
 private void release(final int note) {
 channel.noteOff(note, 50);
 }
 private void play(final int note, final long length)
throws InterruptedException {
 play(note);
 Thread.sleep(length);
 release(note);
 }
 private static MidiChannel getChannel(int instrument)
throws MidiUnavailableException {
 Synthesizer synthesizer = MidiSystem.getSynthesizer();
 synthesizer.open();
 return synthesizer.getChannels()[instrument];
 }
 
 

 public void prekini() {
		if(compPlayer!=null) compPlayer.prekini(); 	}
 public void stani() {if(compPlayer!=null) compPlayer.stani();}
 public void kreni () {if(compPlayer!=null) compPlayer.kreni();}
public void  playSymbol(MusicSymbol simbol) {
	OneNotePlayer pl= new OneNotePlayer(simbol);
}

public  void  playComposition(Composition kompozicija) {
	this.compPlayer= new CompositionPlayer(kompozicija);
	
}
	
public class CompositionPlayer extends Thread{
	
	private Composition comp;
	private boolean radi=true;
	public CompositionPlayer (Composition komp) { this.comp=komp;start();}
	public void run() {
		piano.setFlagRucnoSviranje(false);
		try {
		     Queue<MusicSymbol> red= comp.getQueue();
			while(!red.isEmpty()) {
				 synchronized (this) {while(!radi) wait();}
				MusicSymbol s = null;
				synchronized (red) {
					 s = red.peek();
				}
				//System.out.println("Svira "+ s);
				
					
				
				svirajSimbol(s);
				
	/*			synchronized (red) {
					  red.poll();
				}*/
				
			}
		
		}catch(InterruptedException r) {}
		comp.resetujRed();  
		piano.setFlagRucnoSviranje(true);
		piano.osvezi(new Pause(Trajanje.CETVRITNA, '|')); 
		compPlayer=null;//da bi vratilo piano neobojen
	}
	
	synchronized public void kreni() {radi=true; notifyAll();piano.setFlagRucnoSviranje(false);}
	public void stani() {radi=false; piano.setFlagRucnoSviranje(true);}
	public void prekini() {radi=false ; interrupt(); piano.setFlagRucnoSviranje(true);}
	
}

	
public class OneNotePlayer extends Thread{
		private MusicSymbol simbol;
		public OneNotePlayer(MusicSymbol simbol) {this.simbol=simbol; start();}
		@Override
		public void run() {
			try {
			svirajSimbol(simbol);
			} catch (InterruptedException e) {
				System.out.println("Interrupted exc u sviranju 1 note");
			}
			if(!piano.rucnoSviranje) piano.osvezi(new Pause(Trajanje.CETVRITNA, '|'));
		}
		
		
		
	
	
	
	
	
	
}// KRAJ ONE NOTE



 

private void svirajSimbol(MusicSymbol simbol) throws InterruptedException {
	if(!piano.rucnoSviranje)piano.osvezi(simbol);
	if(simbol instanceof Pause)  Thread.sleep(simbol.getTrajanje().getVal()*PERIOD);
	else {
		int sleepDuration= simbol.getTrajanje().getVal()*PERIOD;
		if(simbol instanceof Note) {
			Note nota= (Note)simbol;
				play(nota.getMidiNum(),sleepDuration);
			
		}
		else if (simbol instanceof Akord) {
			Akord akord= (Akord)simbol;
			Object[] niz= akord.getNizNota();
			for(Object o : niz) 
			  play(((Note)o).getMidiNum());
			Thread.sleep(2*PERIOD);
			for(Object o : niz) 
				  release(((Note)o).getMidiNum());
		}
	}
}




 /*public static void main(String[] args) throws Exception {
 MidiPlayer player = new MidiPlayer();
 Scanner scanner = new Scanner(System.in);
 int note;
 while (!Thread.currentThread().isInterrupted()) {
 System.out.print("Note (1..127) : ");
 note = scanner.nextInt();
 player.play(note, 200);
 }
 scanner.close();
 }*/
}