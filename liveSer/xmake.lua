target("liveSrv")
    add_deps("groupsock", "UsageEnvironment", "BasicUsageEnvironment", "liveMedia")

    set_kind("binary")
    set_targetdir("../output")
    add_links("liveMedia", "groupsock", "BasicUsageEnvironment", "UsageEnvironment")
    add_files("*.cpp")
    add_syslinks("pthread", "dl")
