workspace "HazelDev"
architecture "x64"
configurations {"debug", "release", "dist"}
output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "hazel-dev"
location "hazel-dev"
kind "SharedLib"
language "C++"
targetdir("%{prj.name}/build/bin/" .. output_dir .. "/%{prj.name}")
objdir("%{prj.name}/build/obj/" .. output_dir .. "/%{prj.name}")
files {"%{prj.name}/include/**.hpp", "%{prj.name}/src/**.cpp"}
includedirs {"%{prj.name}/include/", "%{prj.name}/vendor/spdlog/include/"}
filter "configurations:Debug"
defines "HAZEL_DEV_DEBUG"
symbols "On"
filter "configurations:Release"
defines "HAZEL_DEV_RELEASE"
optimize "On"
filter "configurations:Dist"
defines "HAZEL_DEV_DIST"
optimize "On"

project "sandbox"
location "sandbox"
kind "ConsoleApp"
language "C++"
targetdir("%{prj.name}/build/bin/" .. output_dir .. "/%{prj.name}")
objdir("%{prj.name}/build/obj/" .. output_dir .. "/%{prj.name}")
files {"%{prj.name}/include/**.hpp", "%{prj.name}/src/**.cpp"}
includedirs {"hazel-dev/include", "%{prj.name}/include/"}
links {"hazel-dev"}
filter "configurations:Debug"
defines "HAZEL_DEV_DEBUG"
symbols "On"
filter "configurations:Release"
defines "HAZEL_DEV_RELEASE"
optimize "On"
filter "configurations:Dist"
defines "HAZEL_DEV_DIST"
optimize "On"
