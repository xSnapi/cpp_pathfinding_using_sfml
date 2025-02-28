workspace "Pathfinding"
    startproject "A star Pathfinding"
    architecture "x64"
    configurations {
        "debug",
        "release"
    }

    filter { "platforms:Win64" }
        system "Windows"

include "Project"
