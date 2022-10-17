# Schiffbruch

![Schiffbruch](http://i.imgur.com/Urexwg6.png)

This is a modern port of the rather old German game *Schiffbruch* written by [Dirk Plate](http://www.dplate.de/) back in 1999. When I was younger, I've played it a lot and liked it very much. Unfortunately it doesn't run nicely on newer computers, thus I'm creating this port. Getting a working game however is not the only goal, instead I'm quite interested in turning the one file with over 10k lines into a modern C++ code base.

## Authors

* Dirk Plate (@dplate) - Author of the original game
* Lukas Dürrenberger (@eXpl0it3r) - Author of this port

## License

[![Creative Commons License](https://i.creativecommons.org/l/by/4.0/88x31.png)](http://creativecommons.org/licenses/by/4.0/)  
[Schiffbruch](https://github.com/eXpl0it3r/Schiffbruch) by [Lukas Dürrenberger](https://github.com/eXpl0it3r/Schiffbruch) is based on a work of [Dirk Plate](http://www.dplate.de/) and licensed under the [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).

## Downloads

- [Port Game](http://github.com/eXpl0it3r/Schiffbruch/releases/latest)
- [Port SDK](http://github.com/eXpl0it3r/Schiffbruch/archive/master.zip)
- [Original Game](http://www.heikoplate.de/dP-Software/zips/schiffbruch.exe)
- [Original SDK](http://www.heikoplate.de/dP-Software/zips/schiffbruchsdk.zip)

## Links

- [Original Website](http://www.dplate.de/)
- [My Development Blog](http://dev.my-gate.net/)


----


# Original Remarks

*Schiffbruch* is a mix of building, strategy and adventure and gets played with a two-dimensional view. The game deals with the consequences of a ship wreckage. You're stranded on a desert island and have to survive. In order to do so you need to explore the island, find food, build a shelter and try to get attention, so you get found.

## Controls

*Schiffbruch* is played entirely with the mouse. In the main window you see the island on which you're stranded. A click onto a landscape tile, creates a route to that point marked in red. Another click lets the character walk there. By holding the right mouse button it's possible to scroll.  
On the right side is a control panel: If you hover over a button with the mouse, you get to see what the button does on the status bar on the bottom. The three heads of liquid in the middle need to be watched at all time, because if food and water drop below 50%, the player's health will start to decrease. Once it reaches 0%, you'll die. Objects in the inventory may be combined by drag & drop. Way down on the bottom you get to see the daytime. Since it's an island on the equator, it will get dark at 6 pm and bright at 6 am. At night you'll always be sleeping.  
Every morning and every time you exit, the game will automatically be saved to the disk, as long as the player's health is above 10% and it's possible to write to the application directory. The saved game will be loaded direct when starting the game the next time and thus it's possible to continue playing.

## Special Thanks To

- Matthias Buchetics, who constantly had to test an half finished application.
- Tobias Tappel, who made the same thing, but less regularly.
- Sigrid Plate, who made the same thing as well, but even less regularly.
- John Lonningdal, who doesn't know, that I've 'stolen' his path finding algorithm.
- Heiko Plate, who made the music and can calculate in the binary system.
- Gisela Plate, who got to solve math problems.
