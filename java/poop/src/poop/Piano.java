package poop;

import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.stream.Stream;

import javax.sound.midi.MidiUnavailableException;

public class Piano extends Frame {
	
	private long timePressed,timeReleased;
	private FormatterDialog midiDialogFormatter= new FormatterDialog(this, new MidiFormatterNOtNative());
	private FormatterDialog txtDialogFormatter= new FormatterDialog(this, new TextFormatter());
	private SettingDialog settings= new SettingDialog(this);
	private Panel sever= new Panel();
	public Panel centraniPanel= new Panel();
	public Label pianoLabel= new Label("piano", Label.CENTER);
	private Platno platno = new Platno();
	private RealTimePlayer player;
	private boolean KompozicijaIspisujeNOte=false;
	private Composition ucitanakompozicija;
	private Panel jug= new Panel();
	private Button [] nepovisene = new Button[35];
	private Button [] povisene= new Button[25];
	public boolean rucnoSviranje=true;
	private PomocniDijalog dijalog_za_ucitavanje= new PomocniDijalog(this);
	//private static String [] stringNepovisenih = {"C","D","E","F","G","A","B"};
	//private static String [] stringPovisenih = {"C","D","F","G","A"};
	private static char [] stringNepovisenih = {'C','D','E','F','G','A','B'};
	private static char [] stringPovisenih = {'C','D','F','G','A'};
	
	
	public Piano() {
		super("Klavir");
			setSize(1065,500);
		setResizable(false);
		
		addWindowListener(new WindowAdapter() {

			@Override
			public void windowClosing(WindowEvent e) {
				player.prekini();
				dispose();
			}});
		
		try {
			player = new RealTimePlayer(this);
		} catch (MidiUnavailableException e1) {}
	
		
		napraviSever();
		napraviDirke();
		dodajTastaturu();
		napraviMeni();
		napraviCentar();
		
		
		
		setVisible(true);
	}

	public void iscrtajKompoziciju(Composition comp,boolean note) {
		platno.crtajKompoziciju(comp,KompozicijaIspisujeNOte);
		
	}
	
	
	private void napraviMeni() {
		
		MenuBar bar = new MenuBar();
		Menu opcije = new Menu("Opcije");
		Menu reprodukcija= new Menu("Reprodukcija");
		setMenuBar(bar);
		bar.add(opcije);
		bar.add(reprodukcija);
		
	 MenuItem pause= new MenuItem("Pause", new MenuShortcut('P'));
	 MenuItem stop= new MenuItem("Stop", new MenuShortcut('S'));	
	 MenuItem play= new MenuItem("Resume", new MenuShortcut('R'));
	 MenuItem res= new MenuItem("Start", new MenuShortcut('T'));
	 reprodukcija.add(res);	
	 reprodukcija.add(stop);
	 reprodukcija.add(play);
	 reprodukcija.add(pause);
	
		
		reprodukcija.addActionListener(l->{
			if(ucitanakompozicija==null) return;
			switch(l.getActionCommand()) {
			
			case "Pause": player.stani(); break;
			case "Start": player.playComposition(ucitanakompozicija);; break;
			case "Stop": player.prekini(); break;
			case "Resume": player.kreni(); break;
			}
			
		});
		
		
		MenuItem roolback= new MenuItem("Resetuj notnu svesku",new MenuShortcut('R', true));
		opcije.add("Ucitaj kompoziciju");
		opcije.addSeparator();
		opcije.add(roolback);
		opcije.addActionListener(l->{
			switch(l.getActionCommand()) {
			case "Settings":  settings.setVisible(true);break;
			case "Ucitaj kompoziciju": dijalog_za_ucitavanje.setVisible(true); break;
			case "Resetuj notnu svesku" : if(ucitanakompozicija!=null) ucitanakompozicija.resetujRed();  iscrtajKompoziciju(ucitanakompozicija, KompozicijaIspisujeNOte);break;
			}
			
			
		});
		
		
	
		
		Menu formatiranje = new Menu("Formatiranje");
		formatiranje.add("MIDI");
		formatiranje.add("txt");
		formatiranje.addActionListener(l->{
			if(snimaSe) return;
				switch(l.getActionCommand()) {
			case "MIDI":    
				midiDialogFormatter.setVisible(true);     break;
			case "txt" :    txtDialogFormatter.setVisible(true);
				 				break;
			}
		});
		
		opcije.addSeparator();
		opcije.add(formatiranje);
		opcije.addSeparator();
		opcije.add("Settings");
		
		
		
	}
	
	private void napraviCentar() {
		Panel p= new Panel();
		p.setLayout(new GridLayout(2, 1));
		Checkbox slovaDirke= new Checkbox("Note na dirkama"); slovaDirke.addItemListener(l->{ 
			if(slovaDirke.getState()) ispisi_slova_na_dirkama(true); else ispisi_slova_na_dirkama(false);});
		Checkbox noteSlova = new Checkbox("Note na notnoj svesci");
		noteSlova.addItemListener(l->{
			 KompozicijaIspisujeNOte=noteSlova.getState();
			  iscrtajKompoziciju(ucitanakompozicija,false);
		});
		Button pocniSnimanje= new Button("Pocni snimanje");
		Button zavrsiSnimanje= new Button("Zavrsi snimanje");
		zavrsiSnimanje.setEnabled(false);
	pocniSnimanje.addActionListener(l->{
		zavrsiSnimanje.setEnabled(true);
		pocniSnimanje.setEnabled(false);
		pocniSnimanje();
		
	});
	
	zavrsiSnimanje.addActionListener(l->{
		if(snimaSe) {prikaziSnimljeno(); pocniSnimanje.setEnabled(true); zavrsiSnimanje.setEnabled(false);}
	});
	Panel p2 = new Panel();
	p2.add(slovaDirke);
	p2.add(pocniSnimanje);
	p2.add(zavrsiSnimanje);
	p2.add(noteSlova);
	p.add(p2);
	p.add(centraniPanel);
	
	pianoLabel.setFont(new Font("serif", Font.ITALIC, 35));
	centraniPanel.setBackground(Color.black);
	pianoLabel.setForeground(Color.white);
	centraniPanel.add(pianoLabel);	
	add(p);	
	}
	private void napraviDirke() {
		int offset=getWidth()/35;
		int oldx=0;
		int j=0;
		jug.setLayout(null);
		jug.setSize(getWidth(),getHeight()/3);
		jug.setBackground(Color.YELLOW);
		System.out.println("usao");
		
	  for(int i =0; i< 35;i++) {
			nepovisene[i]= new Button();
			nepovisene[i].setBackground(Color.white);
			nepovisene[i].setSize(offset, getHeight()/3);
			nepovisene[i].setBounds(oldx,0, offset, getHeight()/3);
			nepovisene[i].setActionCommand(MidiMap.listaNepovisenih.get(i).toString());
			nepovisene[i].addActionListener(l->{
				 MusicSymbol s = MidiMap.mapa.get(l.getActionCommand().charAt(0));
				 if(s!=null) {
					 rucnoSviranje=false;
					 player.playSymbol(s); snimi(s, l.getWhen());}
				
			});
			
			oldx+=offset;
			if(i%7!=2 && i%7 !=6) {
				povisene[j]=new Button();
				povisene[j].setBackground(Color.black);
				povisene[j].setSize(offset/2,getHeight()/6);
				povisene[j].setBounds(oldx-offset/4,0,offset/2,getHeight()/6);
				povisene[j].setActionCommand(MidiMap.listaPovisenih.get(j).toString());
				povisene[j].addActionListener(l->{
					 MusicSymbol s = MidiMap.mapa.get(l.getActionCommand().charAt(0));
					 if(s!=null) {
						 rucnoSviranje=false;
						 player.playSymbol(s); snimi(s,l.getWhen());}
					
				});
				jug.add(povisene[j]);
				j++;
			}
			jug.add(nepovisene[i]);
		}
	  
	 
		
		add(jug,BorderLayout.SOUTH);
		
	}
	private void napraviSever() {
		add(sever,BorderLayout.NORTH);
		((Canvas)platno).setSize(getWidth(),getHeight()/4);
		sever.add(platno);
		
	}
	
	
	private void dodajTastaturu() {
		
		platno.addKeyListener(new KeyAdapter() {
			

			@Override
			public void keyPressed(KeyEvent e) {
				timePressed=e.getWhen();
				rucnoSviranje=true;
				char c=e.getKeyChar();
				MusicSymbol s = MidiMap.mapa.get(c);
				if(s!=null) {
					player.playSymbol(s); snimi(s, e.getWhen());
					
					osvezi(s);}
			 
			 
			}

			@Override
			public void keyReleased(KeyEvent e) {
				// TODO Auto-generated method stub
				timeReleased=e.getWhen();
				System.out.println("Proteklo "+ (timeReleased- timePressed) );
			//	char c=e.getKeyChar();
			//	MusicSymbol s = MidiMap.mapa.get(c);
				//if(s!=null) {player.playSymbol(s); snimi(s, e.getWhen());}
				
				osvezi(new Pause(Trajanje.CETVRITNA, '|'));
			}

		/*	@Override
			public void keyTyped(KeyEvent e) {
				//System.out.println("usao u key event");
				char c=e.getKeyChar();
				MusicSymbol s = MidiMap.mapa.get(c);
				if(s!=null) {player.playSymbol(s); snimi(s, e.getWhen());}
				
			}*/
			
		});
	}
	
	
	
	public void setFlagRucnoSviranje(boolean flag) {rucnoSviranje=flag;}
	
	public void ispisi_slova_na_dirkama(boolean flag) {
		if(!flag) {for(int i=0;i<35;i++ ) {nepovisene[i].setLabel(" "); if (i<25) povisene[i].setLabel(" ");} return ;}
		for(int i =0 ; i< 35;i++) {
			if(i<25) {povisene[i].setLabel(stringPovisenih[i%5]+""); povisene[i].setForeground(Color.white);}
			nepovisene[i].setLabel(stringNepovisenih[i%7]+"");
			
			
		}
		
	}
	public void svirajKompoziciju(Composition kompozicija) {
		rucnoSviranje=false;
		 player.playComposition(kompozicija);
	 }
	public void setUcitanaKompozicija(Composition k) { ucitanakompozicija=k;}
	
	private void  obojDirke(MusicSymbol s) {
		if (!rucnoSviranje || s instanceof Pause)for(int i=0;i<35;i++) {nepovisene[i].setBackground(Color.white); if(i<25) povisene[i].setBackground(Color.black);}
	if( s instanceof Pause) return;
		if(s instanceof Note) {
			Note nota = (Note) s;
			if(nota.isPovisena()) {
			int ind=0; for(;nota.getVisina() != stringPovisenih[ind];ind++ ); 
			  ind+= 5*(nota.getOktava()-2);
			  povisene[ind].setBackground(Color.GREEN);}
			else {
				int ind=0; for(;nota.getVisina() != stringNepovisenih[ind];ind++ ); 
				  ind+= 7*(nota.getOktava()-2);
				  nepovisene[ind].setBackground(Color.GREEN);}
			}
			
		else if(!rucnoSviranje ) {
			Akord akord = (Akord)s;
			akord.getLista().stream().forEach(l->{
				
				Note nota = (Note) l;
				if(nota.isPovisena()) {
				int ind=0; for(;nota.getVisina()!= stringPovisenih[ind];ind++ ); 
				  ind+= 5*(nota.getOktava()-2);
				  povisene[ind].setBackground(Color.GREEN);}
				else {
					int ind=0; for(;nota.getVisina() != stringNepovisenih[ind];ind++ ); 
					  ind+= 7*(nota.getOktava()-2);
					  nepovisene[ind].setBackground(Color.GREEN);}
			});
			
			
			
		}
		
	}
	public void osvezi(MusicSymbol s) {
		System.out.println(s);
		obojDirke(s);
		if(/*rucnoSviranje*/ ucitanakompozicija!=null ) {
		//DODAO OVO ISPOD
			
			if(!ucitanakompozicija.getQueue().isEmpty())
			synchronized (ucitanakompozicija.getQueue()) {
				if(ucitanakompozicija.getQueue().peek().equals(s) || ucitanakompozicija.getQueue().peek() instanceof Pause) {
				//	System.out.println("prosao equalls");
					ucitanakompozicija.getQueue().poll();
					if(ucitanakompozicija.getQueue().isEmpty()) ucitanakompozicija.resetujRed();	
				}	
			}
			
			
		}
		platno.crtajKompoziciju(ucitanakompozicija, KompozicijaIspisujeNOte);
		
	}
	
	
	//////////////////////////////////////////////////////
	//DEO ZA SNIMANJE 
	

	private Akord akord= new Akord();
	private Composition snimanaKompozicija ;
	private long timeForLastNote=0;
	private boolean snimaSe= false;
	private static final long PERIODVREMENA=RealTimePlayer.PERIOD;
	private void pocniSnimanje() {snimaSe=true; snimanaKompozicija=new Composition();}
	private void prikaziSnimljeno() {  snimi(null,0);snimaSe=false;timeForLastNote=0; ucitanakompozicija=snimanaKompozicija; snimanaKompozicija.resetujRed(); osvezi(new Pause(Trajanje.CETVRITNA,'|'));}
	private void snimi(MusicSymbol s, long time) { // poslednji element
		if(s ==null) {
			if(akord.getLista().size()==1) {
				if(snimaSe) {if(timeReleased-timePressed < PERIODVREMENA)akord.getLista().peek().setTrajanje(Trajanje.OSMINA);
				snimanaKompozicija.dodajSimbol(akord.getLista().poll());}
				}
			else {if(!akord.getLista().isEmpty()) {if(snimaSe)snimanaKompozicija.dodajSimbol(akord); osvezi(akord);}
			   akord=new Akord(); 
			}
			
		}else 
		{
		if(time-timeForLastNote > RealTimePlayer.PERIOD && timeForLastNote !=0) {
			
			Trajanje t=((time-timeForLastNote)/RealTimePlayer.PERIOD >1 ? Trajanje.CETVRITNA:Trajanje.OSMINA);
			char slovo=((time-timeForLastNote)/RealTimePlayer.PERIOD >1 ? '|':' ');
			if(!akord.getLista().isEmpty()) {
				if(akord.getLista().size()==1) {if(snimaSe) {
					if(timeReleased-timePressed < PERIODVREMENA)akord.getLista().peek().setTrajanje(Trajanje.OSMINA);
					 snimanaKompozicija.dodajSimbol(akord.getLista().poll()); } else akord.getLista().poll();}
				else {
				
					if(snimaSe)	snimanaKompozicija.dodajSimbol(akord);
					osvezi(akord);
					akord=new Akord();}}
			if(snimaSe)
			{ //OVO IZBRISI AKO NE TREBA
				//long tt=time-timeForLastNote;
				//while(tt>RealTimePlayer.PERIOD) {
				snimanaKompozicija.dodajSimbol(new Pause(t, slovo));
				//tt-=RealTimePlayer.PERIOD;
				//}
				}}
		if(time-timeForLastNote < PERIODVREMENA) 
			akord.dodaj(s);
		else if(akord.getLista().size()==1) {if(snimaSe) {
			if(timeReleased-timePressed < PERIODVREMENA)akord.getLista().peek().setTrajanje(Trajanje.OSMINA);
			snimanaKompozicija.dodajSimbol(akord.getLista().poll());} else akord.getLista().poll();
		akord.dodaj(s);}
		
		else {if(!akord.getLista().isEmpty()) {if(snimaSe)snimanaKompozicija.dodajSimbol(akord); osvezi(akord);} 
		   akord=new Akord(); akord.dodaj(s);
		   System.out.println("AKORD NOIVE EL" + s.getSlovo());
		}
		
		//System.out.println("Ubacuje se "+ s.toString());
		timeForLastNote=time;
		     
		
		}}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	////////////////////////////////////////////////////////
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
public static void main(String[] args) {
		MidiMap.ucitajMapu("C:\\Users\\CAR\\Desktop\\semestar3\\poop\\13S112POOP_PZ1_1819_prilog\\map.csv");
		Composition c= new Composition();
		c.ucitaj("C:\\Users\\CAR\\Desktop\\semestar3\\poop\\13S112POOP_PZ1_1819_prilog\\input\\ode.txt");
		System.out.println(c);
		Piano p = new Piano();
		p.setUcitanaKompozicija(c);
		p.iscrtajKompoziciju(c,false);
		
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println( "JRE Version :" + System.getProperty( "java.runtime.version" ) );
        System.out.println( "JVM Bit size: " + System.getProperty( "sun.arch.data.model" ) );

		
}

public Composition getUcitanaKompozicija() { return ucitanakompozicija;
	// TODO Auto-generated method stu
}
	 
}
