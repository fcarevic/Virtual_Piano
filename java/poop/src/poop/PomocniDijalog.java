package poop;

import java.awt.*;
import java.awt.Button;
import java.awt.Dialog;
import java.awt.Label;
import java.awt.TextField;
import java.awt.Window;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class PomocniDijalog extends Dialog {
 private TextField polje = new TextField(10);
 private Piano owner;
  private String file_name;
  private Button ucitaj= new Button("ucitaj");
	public PomocniDijalog(Piano owner) {
		super(owner, true);
		setTitle("Import..");
		this.owner=owner;
		setSize(500,150);
		addWindowListener(new WindowAdapter() {

			@Override
			public void windowClosing(WindowEvent e) {
				setVisible(false);
			}
			
		});
		
		add(new Label("Uneti ime fajla sa ekstenzijom"), BorderLayout.NORTH);
		add(polje);
		add(ucitaj, BorderLayout.SOUTH);
		ucitaj.addActionListener(l->{
			file_name=polje.getText();
			Composition k= new Composition();
			k.ucitaj(file_name);
			owner.setUcitanaKompozicija(k);
			owner.osvezi(new Pause(Trajanje.CETVRITNA, '|'));
		    
		});
		
		// TODO Auto-generated constructor stub
	}

}
