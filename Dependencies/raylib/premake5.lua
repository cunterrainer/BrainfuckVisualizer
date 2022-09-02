project "raylib"
    language "C"
    kind "StaticLib"

    files {
        "src/**.c",
        "src/**.h"
    }

    removefiles {
        "src/external/glfw/**.c",
        "src/external/glfw/**.h"
    }

    includedirs {
        "src/external/glfw/include"
    }


    filter "system:windows"
        defines { "_CRT_SECURE_NO_WARNINGS", "PLATFORM_DESKTOP" }
    filter {}