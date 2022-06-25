# sdlGUI

An OpenGL UI toolkit with a Lua API written in C++. 

This UI toolkit was developed for a 2D game engine. User interface structures are stored in an XML format and interpreted into interactive game objects. The functionality of different user interface elements is implemented through Lua scripts using an API.  

## Compiling
To compile the source code, it is recommended to use the Code::Blocks project included in this repository. This library has only been tested in Windows environments. 

## Dependencies
- [sdlObjects](https://github.com/antSDL2/sdlObjects)
- [sdlUtility](https://github.com/antSDL2/sdlUtility)
- [antXml](https://github.com/atrapalis/antXml)
- Lua 5.2 binding library (lua52)
- OpenGL 3.2 (glu32, opengl32)
- SDL2 base (SDL2)

## Documentation
Documentation can be found in the [GitHub pages](https://antSDL2.github.io/sdlGUI/) for this project.

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
**Note:** The API call "Application:GetFramerate();" is part of the [sdlApp](https://github.com/antSDL2/sdlApp) library API.
