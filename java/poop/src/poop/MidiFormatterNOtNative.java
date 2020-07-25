package poop;
import java.io.*;
import java.util.*;
import javax.sound.midi.*; // package for all midi classes

public class MidiFormatterNOtNative implements Formatter {
  private int current;
  static final int PERIOD=60;
  ShortMessage mm;
  MidiEvent event;
	@Override
	public void format(Composition comp, String file_name) 
		

	{	    
		try
		{
			current=0;
	//****  Create a new MIDI sequence with 24 ticks per beat  ****
			Sequence s = new Sequence(javax.sound.midi.Sequence.PPQ,24);

	//****  Obtain a MIDI track from the sequence  ****
			Track t = s.createTrack();

	//****  General MIDI sysex -- turn on General MIDI sound set  ****
			byte[] b = {(byte)0xF0, 0x7E, 0x7F, 0x09, 0x01, (byte)0xF7};
			SysexMessage sm = new SysexMessage();
			sm.setMessage(b, 6);
			MidiEvent me = new MidiEvent(sm,(long)0);
			t.add(me);

	//****  set tempo (meta event)  ****
			MetaMessage mt = new MetaMessage();
	        byte[] bt = {0x02, (byte)0x00, 0x00};
			mt.setMessage(0x51 ,bt, 3);
			me = new MidiEvent(mt,(long)0);
			t.add(me);

	//****  set track name (meta event)  ****
			mt = new MetaMessage();
			String TrackName = new String("midifile track");
			mt.setMessage(0x03 ,TrackName.getBytes(), TrackName.length());
			me = new MidiEvent(mt,(long)0);
			t.add(me);

	//****  set omni on  ****
			 mm = new ShortMessage();
			mm.setMessage(0xB0, 0x7D,0x00);
			me = new MidiEvent(mm,(long)0);
			t.add(me);

	//****  set poly on  ****
			mm = new ShortMessage();
			mm.setMessage(0xB0, 0x7F,0x00);
			me = new MidiEvent(mm,(long)0);
			t.add(me);

	//****  set instrument to Piano  ****
			mm = new ShortMessage();
			mm.setMessage(0xC0, 0x00, 0x00);
			me = new MidiEvent(mm,(long)0);
			t.add(me);

		   comp.resetujRed();
		   comp.getQueue().stream().forEach(l->{
			   try {
				  
			   if(l instanceof Note) {
				   Note nota=(Note) l;
				   mm = new ShortMessage();
					mm.setMessage(0x90,nota.getMidiNum(),0x60);
					event = new MidiEvent(mm,(long)current);
					t.add(event);
	   
				   current += l.getTrajanje().getVal()==2 ? PERIOD : PERIOD/2;
				   
				   mm = new ShortMessage();
					mm.setMessage(0x80,nota.getMidiNum(),0x40);
					event = new MidiEvent(mm,(long)current);
					t.add(event);
	   } else if (l instanceof Akord) {
		    Akord akord=(Akord) l;
		    
		    
		    for(MusicSymbol simbol : akord.getLista()) 
		    	if(simbol instanceof Note) {
					   Note nota=(Note) simbol;
					   mm = new ShortMessage();
						mm.setMessage(0x90,nota.getMidiNum(),0x60);
						event = new MidiEvent(mm,(long)current);
						t.add(event);}
		    	current += l.getTrajanje().getVal()==2 ? PERIOD : PERIOD/2;
				
		   for(MusicSymbol simbol : akord.getLista()) 
			    	if(simbol instanceof Note) {
						   Note nota=(Note) simbol;
			    	
		 	   mm = new ShortMessage();
				mm.setMessage(0x80,nota.getMidiNum(),0x40);
				event = new MidiEvent(mm,(long)current);
				t.add(event);
  
			    	}	
		    
		} else current+=l.getTrajanje().getVal()==2? PERIOD:PERIOD/2;
			   
			   
			   
			   
			   
			   
			   
			   }catch (Exception e) {}
		   });
			
			
	

	//****  set end of track (meta event) 19 ticks later  ****
			mt = new MetaMessage();
	        byte[] bet = {}; // empty array
			mt.setMessage(0x2F,bet,0);
			me = new MidiEvent(mt, (long)140);
			t.add(me);

	//****  write the MIDI sequence to a MIDI file  ****
			File f = new File(file_name);
			MidiSystem.write(s,1,f);
		} //try
			catch(Exception e)
		{
			System.out.println("Exception caught " + e.toString());
		} //catch
	    System.out.println("midifile end ");
	  } //main
	

}
