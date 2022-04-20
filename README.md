# AtGLui

Graphical User Interface library containing class definitions and implementations used in a video game engine written in C++.

## Classes
- **Element:** Base class used to describe a user-interface element.
- **Bind:** Used to implement a keybind function. An example of a keybind is the key-combination 'Alt+F4' to shut down an application.
- **Button:** Used to implement a user-interface button.
- **Dialog:** Used to implement a user-interface dialog, used as a container for other elements.
- **Frame:** Used to implement a generic frame, used as a container for other elements.
- **List:** Used to implement a list of user-interface elements. A list may be used to easily display and position objects in certain arrangements. 
- **Message:** Used to implement and render Text messages within the application.
- **Paragraph:** Used to implement and position a series of text messages into a paragraph.
- **Slider:** Used to implement a slider user-interface element.
- **StatusBar:** Used to implement a status-bar user-interface element.
- **State:** Manager class used to load and manipulate different user-interface states (e.g. different menus and dialogs).
- **API:** API exposing many of the classes and methods defined in the library to a Lua environment. Allows run-time interaction and manipulation of objects created by the library using scripts written in Lua.

## Dependencies
- Lua 5.2 binding library (lua52)
- OpenGL 3.2 (glu32, opengl32)
- SDL2 base (SDL2)
- [AtXml](https://github.com/atrapalis/AtXml "AtXml GitHub repository")
- [AtUtility](https://github.com/atrapalis/AtUtility "AtUtility GitHub repository")
- [AtObjects](https://github.com/atrapalis/AtObjects "AtObjects GitHub repository")
