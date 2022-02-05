workspace "GAM450"
location ""
configurations {"Debug", "Release", "RelWithDebInfo"}
platforms {"x64"}
filter {"platforms:x64"}
system "Windows"
architecture "x64"
filter {}

project "GAM450-Engine"
kind "StaticLib"
language "C++"
location ""
targetname("GAM450-Engine")
targetdir "Bin/%{cfg.buildcfg}"
objdir "Obj/%{cfg.buildcfg}"
debugdir ""
systemversion "10.0"
flags {"MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck"}
warnings "Extra"
includedirs {"Include/", "Include/**"}
links {}
libdirs {"Lib/%{cfg.buildcfg}"}
files {"./Engine/**.h", "./Engine/**.hpp", "./Engine/**.cpp", "./Engine/**.inl", "./Engine/**.shader"}
buildoptions {"-std:c++17"}
removefiles {"**Template.cpp"}
disablewarnings {"4505", "4456", "4201", "4100", "4189", "4458", "4819", "4127", "4701", "4291", "6385", "26812", "26819"}
defines {"_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN", "IMGUI_INCLUDE_IMGUI_USER_H"}

filter "configurations:Debug"
defines {"DEBUG", "_DEBUG"}
symbols "On"
filter "configurations:Release"
defines {"NDEBUG"}
optimize "Speed"
filter "configurations:RelWithDebInfo"
defines {"NDEBUG"}
optimize "Speed"
symbols "On"
filter {}

project "GAM450-Client"
kind "WindowedApp"
dependson { "pGAM450-Engine" }
language "C++"
location ""
targetname("GAM450-Client")
targetdir "Bin/%{cfg.buildcfg}"
objdir "Obj/%{cfg.buildcfg}"
debugdir ""
systemversion "10.0"
flags {"MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck"}
warnings "Extra"
includedirs {"Include/", "Include/**", "Engine"}
links {"dxgi", "d3d12", "d3dcompiler", "dxguid", "winmm", "Xinput9_1_0", "GAM450-Engine", "assimp-vc142", "DirectXTex"}
libdirs {"Lib/%{cfg.buildcfg}", "Bin/%{cfg.buildcfg}"}
files {"./Client/**.h", "./Client/**.hpp", "./Client/**.cpp", "./Client/**.inl", "./Resource/**.json"}
buildoptions {"-std:c++17"}
removefiles {"**Template.cpp"}
disablewarnings {"4505", "4456", "4201", "4100", "4189", "4458", "4819", "4127", "4701", "4291", "6385"}
defines {"_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN", "IMGUI_INCLUDE_IMGUI_USER_H"}

filter "configurations:Debug"
defines {"DEBUG", "_DEBUG"}
symbols "On"
filter "configurations:Release"
defines {"NDEBUG"}
optimize "Speed"
filter "configurations:RelWithDebInfo"
defines {"NDEBUG"}
optimize "Speed"
symbols "On"
filter {}

