# GameEngine
A data driven game engine written in C++ 11. It supports XML as a scripting language to write games.

### Implemented STL container replacements for:
-	Singly Linked List
-	Vector
-	Hashmap

### Modules
- RTTI: Base class for all foundation classes in the engine. It helps in run time type identification of an object.
- Datum: A runtime dynamic data-type storage class. It currently supports the data types - int, float, string, vec4, mat4x4, Scope (Table) and any RTTI class.
- Scope: Helps in creating a hierarchical data structure of key-value pairs at runtime.
- Factory: A templated factory class which creates objects of a class by specifying the name of the class.
- World: Acts as a container for the entire game. It would hold Sectors.
- Sector: Can be loosely considered as Levels / Maps. Sectors would be present inside of a World.
- Entity: Can be loosely considered as Actors / GameObjects. Entities would be present inside a Sector.
- Action: Can be loosely considered as Game Components. This is a base class for all actions in the game, specialized actions inherit from this class and perform specialized functions (Strategy pattern). Example: AttackAction, HealAction etc.
- ActionList: Contains a list of Actions and ActionLists (Composite pattern).
- ActionCreateAction: Creates a specified Action at runtime (in update loop) using Factory pattern.
- ActionDeleteAction: Deletes a specified Action at runtime (in update loop).
- XmlParseMaster: Reads the Xml script, uses Chain of Responsibility pattern and calls the registered XmlParseHelpers for parsing.
- IXmlParseHelper: Abstract base class which acts as an interface for XmlParseHelpers.
- XmlParseHelpers:
    - XmlParseHelperPrimitives: Helper to parse primitives (int, float, string, vec4, mat4x4).
    - XmlParseHelperTable: Helper to parse Scopes (Tables).
    -	XmlParseHelperIf: Helper to parse if-then-else conditional statements.
    - XmlParseHelperWSE: Helper to parse primitives, World, Sector, Entity, Action, ActionList, CreateAction, DeleteAction, ActionEvent and Reaction
- SharedDataWSE: Helper class which helps to create hierarchical chain of objects forming a part of the update loop. It is shared between XmlParseHelpers while parsing.
- EventPublisher: Publishes specific type of events asynchronously. Events are published if they are expired.
- Event: Templated class inherited from EventPublisher class that helps in creating events of a particular type. Provides the interface to Subscribe() and UnSubscribe() from the specific type of events.
- EventMessage: Is the actual message transmitted between an Event Publisher and an Event Subscriber. This would be present inside the templated Event class.
- EventQueue: Responsible for queuing and updating Event Publishers. EventPublisher publishes specific type of events (Example: Foo type of events). EventQueue contains a list of EventPublishers (Example: Foo type of EventPublisher, Bar type of EventPublisher etc) and tells each EventPublisher to dispatch their events.
- WorldState: Contains the state of the game at any point of time.

The entire code is #### unit tested and documented in #### doxygen format.
