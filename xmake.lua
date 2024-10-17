set_project("PhysX")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_warnings("allextra")

add_requires("glm", "fmt")

target("PhysX")
    set_kind("binary")
    add_files("src/*.cpp")

    add_packages("glm", "fmt")