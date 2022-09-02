project "BrainfuckVisualizer"
    language "C++"
    cppdialect "C++17"
    flags "FatalWarnings"

    --defines "_CRT_SECURE_NO_WARNINGS"

    -- gcc* clang* msc*
    filter "toolset:msc*"
        warnings "High"
        externalwarnings "Default" -- Default
        buildoptions { "/sdl" }

    filter { "toolset:gcc* or toolset:clang*" }
        enablewarnings {
            "cast-align",
            "cast-qual",
            "ctor-dtor-privacy",
            "disabled-optimization",
            "format=2",
            "init-self",
            "missing-declarations",
            "missing-include-dirs",
            "old-style-cast",
            "overloaded-virtual",
            "redundant-decls",
            "shadow",
            "sign-conversion",
            "sign-promo",
            "strict-overflow=5",
            "switch-default",
            "undef",
            "uninitialized",
            "unreachable-code",
            "unused",
            "alloca",
            "conversion",
            "deprecated",
            "format-security",
            "null-dereference",
            "stack-protector",
            "vla",
            "shift-overflow"
        }

    filter "toolset:gcc*"
        warnings "Extra"
        externalwarnings "Off"
        linkgroups "on" -- activate position independent linking
        enablewarnings {
            "noexcept",
            "strict-null-sentinel",
            "array-bounds=2",
            "duplicated-branches",
            "duplicated-cond",
            "logical-op",
            "arith-conversion",
            "stringop-overflow=4",
            "implicit-fallthrough=3",
            "trampolines"
        }

    filter "toolset:clang*"
        warnings "Extra"
        externalwarnings "Everything"
        enablewarnings {
            "array-bounds",
            "long-long",
            "implicit-fallthrough", 
        }
    filter {}

    files {
        "**.cpp",
        "**.h"
    }

    includedirs {
        RaylibDir .. "/src",
        RaylibDir .. "/include"
    }

    externalincludedirs {
        RaylibDir .. "/src",
        RaylibDir .. "/include"
    }

    links {
        "raylib",
        "Winmm"
    }

    filter { "configurations:Debug" }
        kind "ConsoleApp"
        floatingpoint "default"

    filter { "configurations:Release" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        floatingpoint "fast"