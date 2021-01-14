#pragma once

#include "headers.hpp"
#include "extern.hpp"

namespace Action
{
enum Type : short {
    NOTHING = 0,
    SEARCH = 1,
    EAT = 2,
    DRINK = 3,
    LOG = 4,
    FIELD = 5,
    DAY_END = 6,
    RESCUED = 7,
    TENT = 8,
    SLEEP = 9,
    CANCEL = 10,
    FISH = 11,
    BOAT = 12,
    UNDOCK = 13,
    DOCK = 14,
    PIPE = 15,
    DESTROY = 16,
    SOS_SIGN = 17,
    HOUSE1 = 18,
    HOUSE2 = 19,
    HOUSE3 = 20,
    FIREPLACE = 21,
    LIGHT = 22,
    LOOKOUT = 23,
    TREASURE = 24,
    INTRO = 25,
    SLINGSHOT = 26,
    QUIT = 27,
    RESTART = 28,
    DEATH = 29,
    DAY_RESTART = 30,
};
}

namespace Actions {

void handler(Action::Type event);

// Aktionen
void search(); // Kacheln durchsuchen
void eat(); // Früchte essen
void drink(); // Wasser trinken
void log(); // Bäume fällen
void field(); // Feld anlegen
void day_end(); // Wenn der Tag zu Ende ist
void rescued(); // Wenn man gerettet wird
void tent(); // Zelt bauen
void sleep(); // Schlafen
void cancel(); // Aktion abbrechen
void fish(); // Aktion abbrechen
void boat(); // Boot bauen
void undock(); // Vom Strand aus ins Meer ablegen
void dock(); // Vom Meer an den Strand anlegen
void pipe(); // Bewässerungsrohr bauen
void destroy(); // Objekt abreissen
void sos(); // SOS schreiben
void house1(); // Leiter bauen
void house2(); // Plattform bauen
void house3(); // Baumhaus bauen
void fireplace(); // Feuerstelle aufstapeln
void light(); // Feuerstelle anzuenden
void lookout(); // Ausschau halten
void treasure(); // Nach Schatz graben
void intro(); // Intro aktionen
void slingshot(); // Jagdaktion
void quit(); // Fragt, ob man es wirklich verlassen will
void restart(); // Fragt, ob man das Spiel neu beginnen will
void day_restart(); // Fragt, ob man den Tag neu beginnen will
void death(); // Sterben
}
