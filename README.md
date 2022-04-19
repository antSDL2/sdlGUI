# AtGLui

Graphical User Interface library containing extensive class definitions and implementations required for a video game engine written in C++.

Contains the following classes:
- **Element:** Base class used to describe a user-interface element.
- **Bind:** Used to implement a keybind function. An example of a keybind is the key-combination 'Alt+F4' to shut down an application.
- **Button:** Used to implement a user-interface button.
- **Dialog:** Used to implement a user-interface dialog, used as a container for other elements.
- **Frame:** Used to implement a generic frame, used as a container for other elements.
- **List:** Used to implement a list of user-interface elements. A list is used to easily display and position objects in certain arrangements. 
- **Message:** Used to implement and render Text messages within the application.
- **Paragraph:** Used to implement and position a series of text messages into a paragraph.
- **Slider:** Used to implement a slider user-interface element.
- **StatusBar:** Used to implement a status-bar user-interface element.
- **State:** Manager class used to load and manipulate different user-interface states (e.g. different menus and dialogs).
- **Lua API:** Extensive API for many of the classes defined in the library. Allows run-time interaction and manipulation of objects handled by the library using external scripts written in Lua.
