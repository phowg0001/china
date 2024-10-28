getgenv().SpinbotEnabled = false
getgenv().SpinSpeed = 1200
getgenv().animationId = 3152378852 -- Lay
getgenv().flossAnimationId = 10714340543 -- Floss
getgenv().animationKeybind = Enum.KeyCode.N
getgenv().flossKeybind = Enum.KeyCode.V
getgenv().spinKeybind = Enum.KeyCode.Y
getgenv().lmgAmmoEnabled = false
getgenv().layAnimationEnabled = false
getgenv().flossAnimationEnabled = false

local spinning = false
local player = game.Players.LocalPlayer
local character = player.Character or player.CharacterAdded:Wait()
local humanoid = character:WaitForChild("Humanoid")
local runService = game:GetService("RunService")
local userInputService = game:GetService("UserInputService")
local rpgtp = CFrame.new(112.43256378173828, -26.750036239624023, -263.3774108886719)
local lmgtp = CFrame.new(-618.670166015625, 23.245738983154297, -297.5761413574219)
local revtp = CFrame.new(-640.4768676757812, 21.7499942779541, -125.49468994140625)
local dbtp = CFrame.new(-1044.3682861328125, 21.7499942779541, -262.5482482910156)
local higharmortp = CFrame.new(-257.03668212890625, 21.799951553344727, -78.68620300292969)
local tactp = CFrame.new(478.3451232910156, 48.070491790771484, -620.615478515625)
local dbcartp = CFrame.new(35.87136459350586, 25.3799991607666, -842.5587768554688)

local function loadAnimation(humanoid, animationId)
    local animation = Instance.new("Animation")
    animation.AnimationId = "rbxassetid://" .. tostring(animationId)
    local animationTrack = humanoid:LoadAnimation(animation)
    return animationTrack
end

local layAnimationTrack = loadAnimation(humanoid, getgenv().animationId)
local flossAnimationTrack = loadAnimation(humanoid, getgenv().flossAnimationId)
local isPlayingLay = false
local isPlayingFloss = false

local function toggleAnimation(animationTrack, isPlaying)
    if isPlaying then
        animationTrack:Stop()
    else
        animationTrack:Play()
    end
    return not isPlaying
end


player.CharacterAdded:Connect(function(newCharacter)
    character = newCharacter
    humanoid = character:WaitForChild("Humanoid")
    layAnimationTrack = loadAnimation(humanoid, getgenv().animationId)
    flossAnimationTrack = loadAnimation(humanoid, getgenv().flossAnimationId)
end)

-- Keybinds for toggling animations
userInputService.InputBegan:Connect(function(input, gameProcessed)
    if not gameProcessed then
        if input.KeyCode == getgenv().animationKeybind and getgenv().layAnimationEnabled then
            isPlayingLay = toggleAnimation(layAnimationTrack, isPlayingLay)
        elseif input.KeyCode == getgenv().flossKeybind and getgenv().flossAnimationEnabled then
            isPlayingFloss = toggleAnimation(flossAnimationTrack, isPlayingFloss)
        end
    end
end)

local uiLoader = loadstring(game:HttpGet('https://raw.githubusercontent.com/topitbopit/dollarware/main/library.lua'))
local ui = uiLoader({
    theme = 'cherry',
    smoothDragging = true
})

local window = ui.newWindow({
    text = 'china.cc',
    size = Vector2.new(500, 360),
    resize = true
})

local mainMenu = window:addMenu({
    text = 'Main Menu'
})

local togglesSection = mainMenu:addSection({
    text = 'Toggles',
    showMinButton = true
})

local actionsSection = mainMenu:addSection({
    text = 'Actions',
    showMinButton = true
})

local tpsSection = mainMenu:addSection({
    text = 'TPs',
    showMinButton = true
})

local spinToggle = togglesSection:addToggle({
    text = 'Spinbot (Y)',
    state = getgenv().SpinbotEnabled
}):bindToEvent('onToggle', function(newState)
    getgenv().SpinbotEnabled = newState
    ui.notify({
        title = 'Spinbot',
        message = 'Spinbot is now ' .. (newState and 'enabled' or 'disabled'),
        duration = 3
    })
end)

local checkKOToggle = false
local koConnection

local function startMonitoringKOs()
    if koConnection then
        koConnection:Disconnect()
    end

    koConnection = runService.RenderStepped:Connect(function()
        if not checkKOToggle then
            return
        end

        local playerEffects = workspace.Players:FindFirstChild(player.Name)
            and workspace.Players[player.Name]:FindFirstChild("BodyEffects")
        local koValue = playerEffects and playerEffects:FindFirstChild("K.O")

        if koValue and koValue.Value then
            local character = player.Character or player.CharacterAdded:Wait()
            local humanoid = character:FindFirstChild("Humanoid")
            if humanoid then
                humanoid:ChangeState(Enum.HumanoidStateType.Dead)
                task.wait(0.5)
            end
        end
    end)
end

local function toggleAntiStomp(newState)
    checkKOToggle = newState

    if checkKOToggle then
        startMonitoringKOs()
        ui.notify({
            title = 'Anti Stomp',
            message = 'Anti stomp is now enabled.',
            duration = 5
        })

        player.CharacterAdded:Connect(function()
            task.wait(0.5)
            if checkKOToggle then
                startMonitoringKOs()
            end
        end)
    else
        if koConnection then
            koConnection:Disconnect()
            koConnection = nil
        end
        ui.notify({
            title = 'Anti Stomp',
            message = 'Anti stomp is now disabled.',
            duration = 5
        })
    end
end

togglesSection:addToggle({
    text = 'Anti stomp stuff',
    state = checkKOToggle,
    style = 'large'
}, function(newState)
    toggleAntiStomp(newState)
end)

local lmgToggle = togglesSection:addToggle({
    text = 'LMG Ammo',
    state = getgenv().lmgAmmoEnabled
}):bindToEvent('onToggle', function(newState)
    getgenv().lmgAmmoEnabled = newState
    ui.notify({
        title = 'LMG Ammo Auto Buy',
        message = 'LMG Ammo script is now ' .. (newState and 'enabled' or 'disabled'),
        duration = 3
    })
end)

local layToggle = togglesSection:addToggle({
    text = 'Enable Lay Animation (N)',
    state = getgenv().layAnimationEnabled
}):bindToEvent('onToggle', function(newState)
    getgenv().layAnimationEnabled = newState
    if not newState and isPlayingLay then
        layAnimationTrack:Stop()
        isPlayingLay = false
    end
    ui.notify({
        title = 'Lay Animation',
        message = 'Lay Animation keybind is now ' .. (newState and 'enabled' or 'disabled'),
        duration = 3
    })
end)

local flossToggle = togglesSection:addToggle({
    text = 'Enable Floss Animation (V)',
    state = getgenv().flossAnimationEnabled
}):bindToEvent('onToggle', function(newState)
    getgenv().flossAnimationEnabled = newState
    if not newState and isPlayingFloss then
        flossAnimationTrack:Stop()
        isPlayingFloss = false
    end
    ui.notify({
        title = 'Floss Animation',
        message = 'Floss Animation keybind is now ' .. (newState and 'enabled' or 'disabled'),
        duration = 3
    })
end)

actionsSection:addButton({
    text = 'Force Reset',
    style = 'large'
}, function()
    task.wait(0.1)
    humanoid:ChangeState(Enum.HumanoidStateType.Dead)
    task.wait()
    game.Players.LocalPlayer.Character.Humanoid.Health = 0
end)

tpsSection:addButton({
    text = 'Rpg',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = rpgtp
end)

tpsSection:addButton({
    text = 'lmg',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = lmgtp 
end)

tpsSection:addButton({
    text = 'db',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = dbtp 
end)

tpsSection:addButton({
    text = 'rev',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = revtp 
end)

tpsSection:addButton({
    text = 'high armor',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = higharmortp 
end)

tpsSection:addButton({
    text = 'db mil',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = dbcartp 
end)

tpsSection:addButton({
    text = 'tactical',
    style = 'large'
}, function()
    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = tactp 
end)

local scriptsSection = mainMenu:addSection({
    text = 'Scripts',
    showMinButton = true
})

scriptsSection:addButton({
    text = 'azure modded',
    style = 'large'
}, function()
    loadstring(game:HttpGet("https://raw.githubusercontent.com/Actyrn/Scripts/main/AzureModded"))()
    print("--=--")
end)

scriptsSection:addButton({
    text = 'infninite yield',
    style = 'large'
}, function()
    loadstring(game:HttpGet("https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source"))()
    print("--=--")
end)

scriptsSection:addButton({
    text = 'chat spy',
    style = 'large'
}, function()
    loadstring(game:HttpGet("https://raw.githubusercontent.com/phowg0001/ChatSpy/refs/heads/main/Universal"))()
    print("--=--")
end)

scriptsSection:addButton({
    text = 'dodge explosive',
    style = 'large'
}, function()
    local plr = game.Players.LocalPlayer
    local rand = Random.new()
    local detectRadius, teleportRange = 27, 15
    local pos = nil 

    local function isNear(part, char)
        return part and (part.Position - char.PrimaryPart.Position).Magnitude <= detectRadius
    end

    local function teleportSmoothly(char, newPos)
        local root = char:FindFirstChild("HumanoidRootPart")
        if root then
            root.CFrame = CFrame.new(newPos)
        end
    end

    local function startTeleporting(char)
        local origPos = char.PrimaryPart.Position

        while true do
            local launcher = workspace:FindFirstChild("Ignored") and workspace.Ignored:FindFirstChild("Model") and workspace.Ignored.Model:FindFirstChild("Launcher")
            local handle = workspace.Ignored and workspace.Ignored:FindFirstChild("Handle")
            
            if (launcher and isNear(launcher, char)) or (handle and isNear(handle, char)) then
                if not pos then
                    pos = char.PrimaryPart.Position
                end
                local newPos = char.PrimaryPart.Position + Vector3.new(rand:NextNumber(-teleportRange, teleportRange), 15, rand:NextNumber(-teleportRange, teleportRange))
                teleportSmoothly(char, newPos)
            else
                if pos then
                    teleportSmoothly(char, pos)
                    pos = nil
                end
            end

            wait(0.05)
        end
    end

    local function onCharacterAdded(char)
        wait(1)
        startTeleporting(char)
    end

    plr.CharacterAdded:Connect(onCharacterAdded)

    if plr.Character then
        onCharacterAdded(plr.Character)
    end
end)

scriptsSection:addButton({
    text = 'anti mods',
    style = 'large'
}, function()
    local NotifyLibrary = loadstring(game:HttpGet("https://raw.githubusercontent.com/XK5NG/XK5NG.github.io/main/Notify"))()
    local Notify = NotifyLibrary.Notify

    local badhuman = {
        163721789, 15427717, 201454243, 822999, 63794379, 17260230, 28357488, 
        93101606, 8195210, 89473551, 16917269, 85989579, 1553950697, 476537893, 
        155627580, 31163456, 7200829, 25717070, 201454243, 15427717, 63794379, 
        16138978, 60660789, 17260230, 16138978, 1161411094, 9125623, 11319153, 
        34758833, 194109750, 35616559, 1257271138, 28885841, 23558830, 25717070, 
        4255947062, 29242182, 2395613299, 3314981799, 3390225662, 2459178, 
        2846299656, 2967502742, 7001683347, 7312775547, 328566086, 170526279, 
        99356639, 352087139, 6074834798, 2212830051, 3944434729, 5136267958, 
        84570351, 542488819,
    }

    local function checkForBadHumans()
        while wait(3) do  
            for _, player in pairs(game.Players:GetPlayers()) do
                if table.find(badhuman, player.UserId) then
                    Notify({
                        Title = "Mod/Star Detector",
                        Description = player.Name .. " detected.",
                        Duration = 4
                    })
                end
            end
        end
    end

    checkForBadHumans()
    print("moddetector")
end)

ui.notify({
    title = '$Script Loaded$',
    message = 'phowg0001. on discord',
    duration = 5
})

for i = 1, 6 do
    ui.notify({
        title = 'phowg loves maddie', 
        message = 'i love maddie', 
        duration = 3
    })
    wait(0.1)
end
