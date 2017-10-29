local DebugFocus = nil;

-- Functions
function AtGUIEditor.SetFocusFromTextboxes(Index, Name)
	local Element = Element.New(Index .. "." .. Name);
	Element:Focus(true);
end

--[[ Event Handlers ]]
function AtGUIEditor.OnDebugFocusChanged(DebugFocusID)
	if (DebugFocusID ~= "") then
		DebugFocus = Element.New(DebugFocusID);
	else
		DebugFocus = nil;
	end
	
	AtGUIEditor.DebugFocusDialog:Update();
end

--[[ Register Event Handlers ]]
Interface.RegisterEventHandler("DebugFocusChanged", AtGUIEditor, AtGUIEditor.OnDebugFocusChanged); 

-- [[ Elements ]]
function AtGUIEditor:OnLoad()
	Interface.SetDebug(0, true);
	Interface.SetDebug(2, true);
end

-- DebugFocusDialog
function AtGUIEditor.DebugFocusDialog:Update()
	if (DebugFocus ~= nil) then
		self:Show();
	else
		DebugFocus = Element.New("");
		self:Hide();
	end

	AtGUIEditor.FocusIndex:SetText(DebugFocus:GetIndex());
	AtGUIEditor.FocusName:SetText(DebugFocus:GetName());
	
	local Parent = Element.New(DebugFocus:GetParent());
	AtGUIEditor.FocusParent:SetText(Parent:GetName());
	
	AtGUIEditor.FocusPositionX:SetText(DebugFocus:X(2));
	AtGUIEditor.FocusPositionY:SetText(DebugFocus:Y(2));
	AtGUIEditor.FocusAnchorX:SetText(DebugFocus:GetAnchor(1));
	AtGUIEditor.FocusAnchorY:SetText(DebugFocus:GetAnchor(2));
	AtGUIEditor.FocusWidth:SetText(DebugFocus:Width());
	AtGUIEditor.FocusHeight:SetText(DebugFocus:Height());

	AtGUIEditor.FocusMovable:SetChecked(DebugFocus:IsMovable());
	AtGUIEditor.FocusResizable:SetChecked(DebugFocus:IsResizable());
	AtGUIEditor.FocusInteractive:SetChecked(DebugFocus:IsInteractive());
	AtGUIEditor.FocusShown:SetChecked(DebugFocus:IsShown(0));
	AtGUIEditor.FocusAllowOtherInput:SetChecked(DebugFocus:AllowsOtherInput());
	AtGUIEditor.FocusDisabled:SetChecked(DebugFocus:IsDisabled());
	AtGUIEditor.FocusContainer:SetChecked(DebugFocus:IsContainer());
	AtGUIEditor.FocusClipsContents:SetChecked(DebugFocus:ClipsContents());
	AtGUIEditor.FocusSharesInput:SetChecked(DebugFocus:SharesInput());
	AtGUIEditor.FocusCopiesInput:SetChecked(DebugFocus:CopiesInput());
	
	AtGUIEditor.FocusClass:SetText(DebugFocus:GetClass());
	
	if (DebugFocus.ID == "") then DebugFocus = nil; end
end

-- FocusIndex
function AtGUIEditor.FocusIndex:OnClick()
	AtGUIEditor.SetFocusFromTextboxes(self:GetText(), self:GetText());
end

-- FocusParent
function AtGUIEditor.FocusParent:OnClick()
	AtGUIEditor.SetFocusFromTextboxes(AtGUIEditor.FocusIndex:GetText(), self:GetText());
end

-- FocusShown
function AtGUIEditor.FocusShown:OnClick()
	if (self:IsChecked()) then
		DebugFocus:Show();
	else
		DebugFocus:Hide();
	end
end

-- FocusAllowOtherInput
function AtGUIEditor.FocusAllowOtherInput:OnClick()
	DebugFocus:EnableOtherInput(AtGUIEditor.FocusAllowOtherInput:IsChecked());
end

-- FocusDisabled
function AtGUIEditor.FocusDisabled:OnClick()
	DebugFocus:SetDisabled(self:IsChecked());
end

-- FocusMovable
function AtGUIEditor.FocusMovable:OnClick()
	DebugFocus:EnableMoving(self:IsChecked());
end

-- FocusResizable
function AtGUIEditor.FocusResizable:OnClick()
	DebugFocus:EnableResizing(self:IsChecked());
end

-- FocusInteractive
function AtGUIEditor.FocusInteractive:OnClick()
	DebugFocus:EnableInteraction(self:IsChecked());
end

-- FocusDelete
function AtGUIEditor.FocusDelete:OnClick()
	if (DebugFocus ~= nil) then
		DebugFocus:Remove();
	end
end

-- FocusDeselect
function AtGUIEditor.FocusDeselect:OnClick()
	if (DebugFocus ~= nil) then
		DebugFocus:Release(true);
	end
end

-- FocusContainer
function AtGUIEditor.FocusContainer:OnClick()
	DebugFocus:SetContainer(self:IsChecked());
end

-- FocusClipsContents
function AtGUIEditor.FocusClipsContents:OnClick()
	DebugFocus:SetClipContents(self:IsChecked());
end

-- FocusSharesInput
function AtGUIEditor.FocusSharesInput:OnClick()
	DebugFocus:SetShareInput(self:IsChecked());
end

-- FocusCopiesInput
function AtGUIEditor.FocusCopiesInput:OnClick()
	DebugFocus:SetCopyInput(self:IsChecked());
end

-- DeleteFocusObjectBind
function AtGUIEditor.DeleteFocusObjectBind:OnPress()
	AtGUIEditor.FocusDelete:OnClick();
end

-- StopApplicationBind
function AtGUIEditor.StopApplicationBind:OnPress()
	Application.Stop();
end

-- ReloadUIButton
function AtGUIEditor.ReloadUIButton:OnClick()
	Interface.ReloadState();
end
