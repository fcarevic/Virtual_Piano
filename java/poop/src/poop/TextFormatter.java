package poop;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class TextFormatter implements Formatter{
  public static boolean noteOsmina= false;
	@Override
	public  void format(Composition comp, String file_name) {
		BufferedWriter writer;
		try {
			writer = new BufferedWriter(new FileWriter(file_name));
			comp.resetujRed();
			
			comp.getQueue().stream().forEach(l->{
				try {
				if(l instanceof Note) { Note nota= (Note) l ; 
				if (nota.getTrajanje().getVal()==1) {
					if(!noteOsmina) { noteOsmina=true; writer.append('[' + l.getSlovo()+ ' ');}
					else writer.append(l.getSlovo()+' ');
				}else {
					if(noteOsmina) {writer.append(']'); noteOsmina=false;}
					writer.append(l.getSlovo());
						}
				}
				else {
				if(noteOsmina)
						writer.append(']');
				noteOsmina=false;
				if(l instanceof Akord) writer.append('[');
				writer.append(l.getSlovo());}
				if(l instanceof Akord) writer.append(']');
				}catch (IOException e) { System.out.println("Greska upis u txt lambda fja");}});
			if(noteOsmina) writer.append(']');
			noteOsmina=false;
			writer.close();
		} catch (IOException e) {
		 System.out.println("IOException u TextFormatter-u");
		}
		
		
		
	}

}
