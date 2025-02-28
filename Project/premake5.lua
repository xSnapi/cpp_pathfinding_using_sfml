project "A star Pathfinding"
    kind "consoleapp"
    language "c++"
    cppdialect "c++17"
    staticruntime "on"

    targetdir "bin/%{cfg.buildcfg}"
    objdir    "bin/obj/%{cfg.buildcfg}"

    files {
        "**.cpp",
        "**.hpp",
        "**.h",
        "**.lua",
        "**.dll",
    }

    includedirs {
        "include",
        "../include"
    }

    libdirs {
        "../lib/ext"
    }

    links {
        "opengl32",
        "openal32",
        "Winmm",
        "freetype",
        "gdi32",
        "flac",
        "vorbisfile",
        "vorbis",
        "ogg",
        "ws2_32",
        "legacy_stdio_definitions",
    }

    defines {
        "SFML_STATIC",
    }

    filter "configurations:debug"
        defines { "DEBUG" }

        symbols "on"

        libdirs {
            "../lib/debug",
        }

        links {
            "sfml-graphics-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d",
        }

    filter "configurations:release"
        defines { "NDEBUG" }

        optimize "on"

        libdirs {
            "../lib/release",
        }

        links {
            "sfml-graphics-s",
            "sfml-window-s",
            "sfml-system-s",
            "sfml-audio-s",
            "sfml-network-s",
        }
