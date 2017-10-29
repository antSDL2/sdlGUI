--[[ Global ]]

-- SaveData
Essentials.SaveData = {
	["Display"] = 0,
	["WindowMode"] = WM_FULLSCREEN,
	["WindowedResolution"] = "800x600",
	["FullscreenResolution"] = Application.GetWidth(WM_BORDERLESS).."x"..Application.GetHeight(WM_BORDERLESS),
	["VSync"] = 0,
}

Interface.LoadCursor("Cursor02");

LeftPanel = nil;
RightPanel = nil;

--[[ Event Handlers ]]
function Essentials.OnResolutionChange()
	if (Interface) then Interface.SetResolution(Application.GetWidth() .. "x" .. Application.GetHeight()); end
	if (World) then World.SetResolution(Application.GetWidth() .. "x" .. Application.GetHeight()); end
	if (SettingsUI) then SettingsUI:Update("Video"); end
end

function Essentials.OnTargetChange()
	local TargetID = World.GetTargetID();
	
	if (TargetID == "") then
		Interface.LoadCursor("Cursor02");
	else
		local TargetObject = Creature.New(TargetID);
		if (TargetObject:IsHostile() == 1) then
			Interface.LoadCursor("Cursor07");
		else
			Interface.LoadCursor("Cursor04");
		end
	end
end

--[[ Register Events ]]
Application.RegisterEventHandler("ResolutionChanged", Essentials, Essentials.OnResolutionChange);

-- for k, v in pairs(_G) do print(k, v) end -- globals check
-- for k, v in pairs(GlobalEvents) do print(k, v) end -- GlobalEvents check

--[[ Elements ]]
-- Essentials
function Essentials:OnExit()
	self:Update();
	Interface.SaveAddOnData("Essentials", self.SaveData);
end

function Essentials:OnLoad()
	Interface.LoadAddOnData("Essentials");
	
	-- WindowMode
	Application.SetWindowMode(self.SaveData["WindowMode"]);

	-- Resolution
	Application.SetResolution(self.SaveData["FullscreenResolution"], WM_FULLSCREEN);
	Application.SetResolution(self.SaveData["WindowedResolution"], WM_WINDOWED);
	
	-- VSync
	Application.SetVSync(self.SaveData["VSync"]);
	
	-- Apply
	Application.ApplyVideoMode();
	
	if (Interface) then Interface.SetResolution(Application.GetWidth() .. "x" .. Application.GetHeight()); end
end

function Essentials:Update()
	self.SaveData["WindowMode"] = Application.GetWindowMode();
	self.SaveData["FullscreenResolution"] = Application.GetWidth(WM_FULLSCREEN) .. "x" .. Application.GetHeight(WM_FULLSCREEN);
	self.SaveData["WindowedResolution"] = Application.GetWidth(WM_WINDOWED) .. "x" .. Application.GetHeight(WM_WINDOWED);
	self.SaveData["VSync"] = Application.GetVSync();
end