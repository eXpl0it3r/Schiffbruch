#pragma once

namespace World
{
    void AddTime(const short hours, const short minutes);                      // Addiert Stunden und Minuten zur TagesZeit	
    void AddResource(const short resourceType, const float count);             // Fügt wassser usw hinzu
    void Generate();                                                           // Kacheln in den Speicher zeichnen
    void Compute(const short minSize, const short maxSize);                    // Zufallslandschaft erzeugen
    void Ocean();                                                              // Mit Meer aufüllen
    void River();                                                              // Erstellt einen Fluß
    bool CheckRiver(const short x, const short y);                             // Für River()
    void Trees(short percent);                                                 // Verteilt die Bäume auf Insel
    void Discover();                                                           // Deckt die Kacheln neben der Spielfigur auf	
    void CheckUsage(const short object);                                       // Prüft ob die richtigen Rohstoffe benutzt wurden (im Inventar)
    bool CheckResource();                                                      // Prüft die Rohstoffe die zum Bau benötigt werden
    void MakeResourceString(const short x, const short y, const short object); // Schreibt in den Rohstring die benötigten Rohstoffe
    void CheckSpecialButton();                                                 // Wechselt die Knöpfe (Stop, Weiter, Ablegen usw.)
    void ChangeBoatRide();                                                     // Ändert die notwendigen Sachen für eine Bootsfahrt (und zurück);
    void FillPipe();                                                           // Überprüft die Röhren, ob sie an Wassernetz angeschlossen sind
    void CheckPipe(const short x, const short y);                              // Rekursive Procedure für FillPipe();
    void Treasure();                                                           // Schatz vergraben und Schatzkarte malen
    void PirateWreck();                                                        // Setzt das Piratenwrack
} // namespace World
