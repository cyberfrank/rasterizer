-- premake5.lua

workspace "workspace"
    configurations { "Debug", "Release" }
    language "C"
    flags {
        "FatalWarnings", 
        "MultiProcessorCompile"
    }
    warnings "Extra"
    inlining "Auto"
    editAndContinue "Off"
    targetdir "bin"
    location "bin"
    characterset "MBCS" 

filter "system:windows"
    platforms { "Win64" }
    systemversion("latest")

filter "platforms:Win64"
    defines { 
        "OS_WINDOWS", 
        "_CRT_SECURE_NO_WARNINGS"
    }
    includedirs "src"
    staticruntime "On"
    architecture "x64"
    disablewarnings {
        "4057", -- Slightly different base types.
        "4100", -- Unused formal parameter.
        "4152", -- Conversion from function pointer to void *.
        "4200", -- Zero-sized array. Valid C99.
        "4201", -- Nameless struct/union. Valid C11.
        "4204", -- Non-constant aggregate initializer. Valid C99.
        "4206", -- Translation unit is empty. Might be #ifdefed out.
        "4214", -- Bool bit-fields. Valid C99.
        "4221", -- Pointers to locals in initializers. Valid C99.
        "4702", -- Unreachable code.
    }
    linkoptions { "/ignore:4099" }

filter "configurations:Debug"
    defines { "DEBUG_MODE" }
    symbols "On"

filter "configurations:Release"
    defines { "RELEASE_MODE" }
    optimize "On"

project "rasterizer"
    kind "ConsoleApp"
    targetname "rasterizer"
    files { "src/**.h", "src/**.c" }