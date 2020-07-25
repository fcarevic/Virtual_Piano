package poop;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class SettingDialog extends Dialog {
	private Checkbox [] boje;
	private Piano piano;
	private static Color[] colors = { Color.black, Color.green,Color.blue, Color.red};
	private static String[] imeboja= {"black", "green","blue","red"};
	private TextField polje= new TextField(4);
	public SettingDialog(Piano piano) {
		super(piano,true);
		this.piano=piano;
		setTitle("Settings");
		
		dodajBoxove();
		dodajCentar();
		setSize(300,400);
		addWindowListener(new WindowAdapter() {

			@Override
			public void windowClosing(WindowEvent e) {
				setVisible(false);
			}
			
		});
	}
	
	private void dodajBoxove() {
		CheckboxGroup group= new CheckboxGroup();
		boje= new Checkbox[4];
		Panel p = new Panel();
		for(int i=0 ; i<4; i++) {
			if(i==0)
			boje[i]= new Checkbox(imeboja[i], true, group);
			else boje[i]= new Checkbox(imeboja[i], false, group);
			p.add(boje[i]);
			boje[i].addItemListener(l->{
				for(int j=0;j<4;j++) {
					if(boje[j]==(Checkbox)l.getSource()) {
						//System.out.println("usao u promenu boje");
						piano.centraniPanel.setBackground(colors[j]);
						piano.pianoLabel.setBackground(colors[j]);
					 break;
					} 
				}
			});
		}
		add(p,BorderLayout.NORTH);
		p.setBackground(Color.white);
	}
	
	private void dodajCentar() {
		
		Panel p = new Panel();
		p.add(new Label("Tempo"));
		p.add(polje);
		p.add(new Label("ms"));
		Button b = new Button("apply");
		p.add(b);
		b.addActionListener(l->{
			try {
			 int i = Integer.parseInt(polje.getText());
			 RealTimePlayer.PERIOD=i;
			}catch(NumberFormatException e) {System.out.println("Nepostojeci format broja");}
		});
		p.setBackground(Color.white);
		add(p);
		
		
	}

}
