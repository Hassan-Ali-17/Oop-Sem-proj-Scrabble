# Oop-Sem-proj-Scrabble
This project develops a digital Scrabble game using C++ and the SFML library, replicating
the classic word game where players form words on a 15x15 board to score points. The game
aims to provide an accessible, interactive experience with automated rule enforcement, word
validation, and scoring, including bonus tiles. It demonstrates object-oriented programming
(OOP) principles such as inheritance, polymorphism, composition, and aggregation, ensuring
modular and extensible code. Features include player racks, a tile bag, dictionary validation,
and a user-friendly interface with move cancellation and dictionary updates.
-------------------------------------------------------------------------------------------------------------------------

The Scrabble game employs key OOP principles to achieve robust, maintainable code:
• Inheritance: The Tile class inherits from GameEntity (for position management)
and Drawable (for rendering), enabling code reuse and a clear hierarchy.
• Polymorphism: The Drawable interfaces virtual draw method allows polymorphic
rendering, enabling different components (e.g., Tile) to be drawn uniformly.
• Composition: The Board class owns a 15x15 array of Tile objects, managing their
lifecycle. Similarly, Bag and YesNoDialog compose Piece and Button objects,
respectively.
• Aggregation: The Rack class loosely holds Piece objects, which can be reassigned to
the Bag, supporting flexible tile management.
