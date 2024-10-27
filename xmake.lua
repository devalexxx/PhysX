set_project("PhysX")
set_version("0.0.1")

includes("xmake/**.lua")

add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_warnings("allextra")

add_requires("glm", "fmt")
add_requires("orion")

target("PhysX")
    set_kind("static")

    add_includedirs("include/", { public = true })
    add_headerfiles("include/PhysX/*.h")
    add_files("src/PhysX/*.cpp")

    add_packages("glm", "fmt")

target("Skydiver")
    set_kind("binary")

    add_files("src/Skydiver/*.cpp")

    after_run(function(target)
        import("core.project.task")
        task.run("plot-sd", { path = "$(buildir)/$(os)/$(arch)/$(mode)/asset/skydiver/out.txt" })
    end)

    add_deps("PhysX")
    add_packages("glm", "fmt")

target("Rope")
    set_kind("binary")

    add_files("src/Rope/*.cpp")

    after_run(function(target)
        import("core.project.task")
        task.run("plot-sd", { path = "$(buildir)/$(os)/$(arch)/$(mode)/asset/rope/out.txt" })
    end)

    add_deps("PhysX")
    add_packages("glm", "fmt")

target("Bridge")
    set_kind("binary")

    add_files("src/Bridge/*.cpp")

    add_deps("PhysX")
    add_packages("glm", "fmt", "orion")
    add_rules("@orion/resource")

