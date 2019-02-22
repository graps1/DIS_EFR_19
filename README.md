# Driver Information System Saison 18/19

Hier kann die Implementation des DIS für die Saison 18/19 gefunden werden. 

## Zielstellung
Grundsätzlich soll...

* mindestens die Funktionalität des DIS aus der letztes Saison bestehen bleiben.
* ausführliches Refactoring vorgenommen- & die Menüführung deutlich strukturierter und einfacher gestalten werden.
* es leicht sein, die Reihenfolge der Menüs zu vertauschen, deren Struktur umzustellen oder diese beliebig zu verschachteln.
* ein "Push"-System entwickelt werden, mit dem am Ende wichtige Ereignisse (Fehler, Grenzwertüberschreitungen etc.) direkt sichtbar gemacht werden.
* verschiedene Parameter von dem DIS aus anpassbar sein.

## Menüführung
Die Menüs sind mit Hilfe einer Baumstruktur erstellt worden. Jedes Menü hat eine Referenz auf das 
erste Untermenü, auf seinen Nachbarn (Menüs der gleichen Rangordnung) und auf das Obermenü. Sonderfälle, z.B. dass kein Untermenü,
Nachbar oder Obermenü vorhanden ist, werden entsprechend behandelt. Zur Navigation gibt es auf dem Lenkrad
drei Tasten, mit denen jeweils entweder einen Nachbar weitergeschalten ("next"), einen Menüebene tiefer betreteten ("enter")
oder wieder eine Menüebene nach oben gegangen werden kann ("return"). 
![Idee](https://github.com/graps1/DIS_EFR_19/blob/master/doc/menu_structure.png)

Dazu wird ein einfacher Pointer genutzt, der immer das aktuelle Menü anzeigt und mit entsprechenden Operationen 
manipuliert werden kann, also dann z.B. auf den Nachbarn oder das Untermenü zeigt. Die "äußersten" Menüs, also die,
die als letztes hinzugefügt worden sind, zeigen mit ihrem Nachbarzeiger immer auf das erste Untermenü des Obermenüs,
wodurch man durch die Menüs "durchrotieren" kann.
