-- Solutions
workspace "Sandbox"
    
    architecture "x86_64"
    startproject "Executable"        -- Set startup project
    toolset "v143"               -- Toolset v143 = Visual Studio 2022

    configurations
    {
        "Editor-Release",
        "Editor-Debug",
        "Game-Release"
    }

    platforms
    {
        "x64",
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
IncludeDir["spdlog"]        = "%{wks.location}/dependencies/spdlog"
IncludeDir["stb"]           = "%{wks.location}/dependencies/stb"
IncludeDir["FreeType"]      = "%{wks.location}/dependencies/FreeType/**"
IncludeDir["FMOD"]          = "%{wks.location}/dependencies/FMOD/include"
IncludeDir["RapidJSON"]     = "%{wks.location}/dependencies/RapidJSON/include"
IncludeDir["Mono"]          = "%{wks.location}/dependencies/Mono/include/mono-2.0"
IncludeDir["FFmpeg"]        = "%{wks.location}/dependencies/FFmpeg/include"
IncludeDir["RTTRLib"]       = "%{wks.location}/dependencies/RTTR/include"
IncludeDir["RTTRLibD"]      = "%{wks.location}/dependencies/RTTR/includeDebug"
IncludeDir["EnTT"]          = "%{wks.location}/dependencies/EnTT/src/entt"

-- Add own defines as needed after the function
-- Common ReleaseLib Config Settings
function CommonReleaseLibConfigSettings()
    kind "StaticLib" 
    staticruntime "on"
    language "C++"

    runtime "Release" -- uses the release Runtime Library
    optimize "On"
    architecture "x86_64"
end

-- Common DebugLib Config Settings
function CommonDebugLibConfigSettings()
    kind "StaticLib" 
    staticruntime "on"
    language "C++"

    runtime "Debug" -- uses the debug Runtime Library
    symbols "On"
    architecture "x86_64"
end


-- Projects 
group "Dependencies"
-- Dependencies that are not here are header-only files
    project "GLFW"
        location "dependencies/GLFW"
        targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
        objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

        files
        {
            "%{prj.location}/include/**.h",
            "%{prj.location}/src/**.h",
            "%{prj.location}/src/**.c"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            } 


        filter "configurations:Editor-Release or configurations:Game-Release"
            CommonReleaseLibConfigSettings()
        
        filter "configurations:Editor-Debug"
            CommonDebugLibConfigSettings()

    project "Glad"
        location "dependencies/Glad"
        targetdir ("bin/" .. outputDir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "/%{prj.name}")
    
        files
        {
            "%{wks.location}/dependencies/Glad/include/glad/glad.h",
            "%{wks.location}/dependencies/Glad/include/KHR/khrplatform.h",
            "%{wks.location}/dependencies/Glad/src/glad.c"    
        }
    
        includedirs
        {
            "%{prj.location}/include"
        }
    
        filter "system:windows"
            systemversion "latest"
    
        filter "configurations:Editor-Release or configurations:Game-Release"
            CommonReleaseLibConfigSettings()
        
        filter "configurations:Editor-Debug"
            CommonDebugLibConfigSettings()
        
    project "ImGui"
        location "dependencies/ImGui"
        kind "StaticLib"
        staticruntime "on"

        language "C++"
        cppdialect "C++17"

        targetdir ("bin/" .. outputDir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

        includedirs
        {
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.spdlog}",
            "%{IncludeDir.stb}",
            "%{IncludeDir.FreeType}",
            "%{IncludeDir.FMOD}",
            "%{IncludeDir.RapidJSON}",
        }

        files
        {
            "%{prj.location}/imconfig.h",
            "%{prj.location}/imgui.h",
            "%{prj.location}/imgui.cpp",
            "%{prj.location}/imgui_draw.cpp",
            "%{prj.location}/imgui_internal.h",
            "%{prj.location}/imgui_widgets.cpp",
            "%{prj.location}/imgui_tables.cpp",
            "%{prj.location}/imstb_rectpack.h",
            "%{prj.location}/imstb_textedit.h",
            "%{prj.location}/imstb_truetype.h",
            "%{prj.location}/imgui_demo.cpp",
            "%{prj.location}/imgui_impl_opengl3.cpp",
            "%{prj.location}/imgui_impl_opengl3.h",
            "%{prj.location}/imgui_impl_opengl3_loader.h",
            "%{prj.location}/imgui_impl_glfw.cpp",
            "%{prj.location}/imgui_impl_glfw.h",
            "%{prj.location}/implot.h",
            "%{prj.location}/implot_internal.h",
            "%{prj.location}/implot.cpp",
            "%{prj.location}/implot_demo.cpp",
            "%{prj.location}/implot_items.cpp",
            "%{prj.location}/ImSequencer.cpp",
            "%{prj.location}/ImSequencer.h",
            "%{prj.location}/ImCurveEdit.cpp",
            "%{prj.location}/ImCurveEdit.h",
            "%{prj.location}/ImGradient.cpp",
            "%{prj.location}/ImGradient.h",
            "%{prj.location}/GraphEditor.cpp",
            "%{prj.location}/GraphEditor.h",
            "%{prj.location}/ImApp.h",
            "%{prj.location}/ImGuizmo.cpp",
            "%{prj.location}/ImGuizmo.h",
            "%{prj.location}/imgui_neo_sequencer.h",
            "%{prj.location}/imgui_neo_sequencer.cpp",
            "%{prj.location}/imgui_neo_internal.h",
            "%{prj.location}/imgui_neo_internal.cpp"
    
        }

        filter "system:windows"
            systemversion "latest"
            
        filter "configurations:Editor-Release or configurations:Game-Release"
            CommonReleaseLibConfigSettings()
        
        filter "configurations:Editor-Debug"
            CommonDebugLibConfigSettings()

group ""

group "Engine"
    include "Engine"
group ""

group "Editor"
    include "Editor"
group ""

group "Executable"
    include "Executable"
group ""


--[[
Premake Troubleshooting
1. Check for missing links between projects
2. Check for missing dependencies between projects
3. Check if all affected files are edited properly
4. Check if the file exists within the directory
5. Delete .vs and bins and run again
]]