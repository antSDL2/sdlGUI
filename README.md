# AtGLui

OpenGL User Interface library developed for a custom 2D video game engine. This library interprets user interfaces stored in an XML format into interactive game objects. The functionality of different user interface elements is implemented through Lua scripts using an API.  

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
- [AtObjects](https://github.com/atrapalis/AtObjects "AtObjects GitHub repository")
- [AtUtility](https://github.com/atrapalis/AtUtility "AtUtility GitHub repository")
- [AtXml](https://github.com/atrapalis/AtXml "AtXml GitHub repository")
- Lua 5.2 binding library (lua52)
- OpenGL 3.2 (glu32, opengl32)
- SDL2 base (SDL2)

## Example

This simple example will output the framerate of the application on the bottom left corner of the screen each time the application completes a process loop:

*Framerate.xml*
```
<?xml version="1.0" encoding="UTF-8" ?>
<Interface Scripts="Framerate.lua" Height="25" Anchor="BOTTOM" Padding="10" Shown="false">
	<Message Name="Message" Anchor="MIDDLE" Font="Arial" Size="12" Text="FPS: 0" ShadowOffset="1" />
</Interface>
```

*Framerate.lua*
```
-- SaveData
Framerate.SaveData = {
	["ShowFPS"] = false
}

--[[ Elements ]]

-- Framerate
function Framerate:OnLoad()
	Interface.LoadAddOnData("Framerate");
	
	-- Message
	if (self.SaveData["ShowFPS"]) then self:Show(); end
end

function Framerate:OnExit()
	Interface.SaveAddOnData("Framerate", self.SaveData);
end

-- Message
function Framerate.Message:OnProcess()
	if self:IsShown() then
		local Text = "FPS: " .. Application:GetFramerate();
		self:SetText(Text);
	end
end
```
**Note:** The API call "Application:GetFramerate();" is part of the [AtApp](https://github.com/atrapalis/AtApp) library API.
