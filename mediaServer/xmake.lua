target("mediaServer")
    add_deps("groupsock", "UsageEnvironment", "BasicUsageEnvironment", "liveMedia")

    set_kind("binary")
    add_links("liveMedia", "groupsock", "BasicUsageEnvironment", "UsageEnvironment")
    add_files("*.cpp")
    add_syslinks("pthread", "dl")
    
