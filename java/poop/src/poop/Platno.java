package poop;

import java.awt.Canvas;
import java.awt.Graphics;

public class Platno extends Canvas {
	private Composition comp;
	private boolean noteSlova;
	@Override
	public void paint(Graphics g) {
		
		comp.crtaj(g,super.getHeight()/2, super.getWidth(),noteSlova);
	}
	
	public void crtajKompoziciju(Composition comp,boolean note) {
		this.comp=comp;
		noteSlova=note;
		repaint();
	}

	
	
}
