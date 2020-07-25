package poop;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Dialog;
import java.awt.Label;
import java.awt.TextField;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class FormatterDialog extends Dialog {
	private Formatter formater;
	private TextField polje = new TextField(10);
	 private Piano owner;
	  private String file_name;
	  private Button ucitaj= new Button("export");
		public FormatterDialog(Piano owner, Formatter formater) {
			super(owner, true);
			setTitle("Export..");
			this.formater=formater;
			this.owner=owner;
			setSize(500,150);
			addWindowListener(new WindowAdapter() {

				@Override
				public void windowClosing(WindowEvent e) {
					setVisible(false);
				}
				
			});
			
			add(new Label("Uneti ime izlaznog fajla sa ekstenzijom"), BorderLayout.NORTH);
			add(polje);
			add(ucitaj, BorderLayout.SOUTH);
			ucitaj.addActionListener(l->{
			  formater.format(owner.getUcitanaKompozicija(), polje.getText());
			    
			});
			
			// TODO Auto-generated constructor stub
		}


}
