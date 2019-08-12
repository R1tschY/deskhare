# GUI

* View abstraction
* View as plugin
* actions
* settings
* support for categories
* show action icon
* show description in results

# Core

* reuse last query if last query is a prefix/superset of current query
* better score calculation in Evaluator
 * use existing score
* history as internal plugin

## Refactorings

* One search engine for every plugin (Sqlite, std::vector, QString + Regex or ...)


# Plugins

## Windows

* do it
* ideas:
  * applications: start menu
  * recent files
  
## Xdg

* recent files
* start apps detached

## Systemd

* move system commands into it

## FileIndex

* settings dialog
* database in user DATADIR
* index modification/access date
* sort by modification/access date

## Ideas

* SSH
* Putty
* ConEmu
* Systemsteuerung / Aufgaben
* Audio (Mute, Unmute, leiser, lauter)
* Multimedia (Play/Pause, Weiter, Zurück, ...)
* Laufwerk auswerfen / öffnen
* Benutzerordner öffnen / Gtk/Qt-Bookmarks / special Ordner / %APPDATA%
* Standardanwendungen höher bewerten?
* Benutzerdefinierte Aktionen (Commandozeile)


# Featureideen

* Kontexte
  * Dateisystem
    * im Dateisystem navigieren
  * Fenster
    * Hauptmenu-Aktionen suchen
    
* Schnellkomandos
  * erste Buchstaben beschreibt Kategorie:
    * "~" -> Angabe von Dateipfad unter Home-Ordner
    * "/" -> Suche nach Datei
    * "=" -> Berechnung
    * "g " -> Websuche
    * "?" -> Wikipediasuche
    * "*." -> Filter nach Dateiendung
    * "@" -> Kontakte
    * "*" -> Websuche
    * ">" -> Aktionen von Plugins durchsuchen
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

