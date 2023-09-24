-- Solutions
workspace "LitterBox"
    
    architecture "x86_64"
    startproject "Sandbox"        -- Set startup project
    toolset "v143"               -- Toolset v143 = Visual Studio 2022

    configurations
    {
        "Debug",
        "Release",
    }

    platforms
    {
        "Editor",
        "Executable",
    }

    flags
    {
        "MultiProcessorCompile", -- Enable Multicore Compilation
    }

-- Standard output directory
outputDir = "%{cfg.buildcfg}-%{cfg.platform}"

-- Include directories 
IncludeDir = {}
IncludeDir["GLFW"]          = "%{wks.location}/dependencies/GLFW/include"
IncludeDir["Glad"]          = "%{wks.location}/dependencies/Glad/include"
IncludeDir["glm"]           = "%{wks.location}/dependencies/glm/glm"
IncludeDir["ImGui"]         = "%{wks.location}/dependencies/ImGui"
IncludeDir["spdlog"]        = "%{wks.location}/dependencies/spdlog/include"
IncludeDir["stb"]           = "%{wks.location}/dependencies/stb"
IncludeDir["FreeType"]      = "%{wks.location}/dependencies/FreeType/include"
IncludeDir["FMOD"]          = "%{wks.location}/dependencies/FMOD/core/inc"

-- Projects 
group "Dependencies"
-- Dependencies that are not here are header-only files
    include "dependencies/GLFW"
    include "dependencies/Glad"
    include "dependencies/ImGui"
    include "dependencies/FreeType"
group ""

group "Engine"
    include "LitterBox"
group ""

group "Editor"
    include "Editor"
group ""

group "Sandbox"
    include "Sandbox"
group ""