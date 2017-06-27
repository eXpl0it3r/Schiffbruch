#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace World
{
	void AddTime(short h, short m); // Addiert Stunden und Minuten zur TagesZeit	
	void AddResource(short Art, float Anzahl); // Fügt wassser usw hinzu
	void Generate(); // Kacheln in den Speicher zeichnen
	void Compute(short MinGroesse, short MaxGroesse); // Zufallslandschaft erzeugen
	void Meer(); // Mit Meer aufüllen
	void Fluss(); // Erstellt einen Fluß
	bool CheckFluss(short x, short y); // Für FLuss()
	void Baeume(short Prozent); // Verteilt die Bäume auf Insel
	void Entdecken(); // Deckt die Kacheln neben der Spielfigur auf	
	void CheckBenutze(short Objekt); // Prüft ob die richtigen Rohstoffe benutzt wurden (im Inventar)
	bool CheckRohstoff(); // Prüft die Rohstoffe die zum Bau benötigt werden
	void MakeRohString(short x, short y, short Objekt); // Schreibt in den Rohstring die benötigten Rohstoffe
	void CheckSpzButton(); // Wechselt die Knöpfe (Stop,Weiter,Ablegen usw.)
	void ChangeBootsFahrt(); // Ändert die notwendigen Sachen für eine Bootsfahrt (und zurück);
	void FillRohr(); // Überprüft die Röhren, ob sie an Wassernetz angeschlossen sind
	void CheckRohr(short x, short y); // Rekursive Procedure für FillRohr();
	void Schatz(); // Schatz vergraben und Schatzkarte malen
	void Piratenwrack(); // Setzt das Piratenwrack
} // namespace World
