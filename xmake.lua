set_project("media")
set_xmakever("2.3.5")
set_version("0.0.1", {build = "%Y%m%d%H%M"})

add_cxflags("-DSOCKLEN_T=socklen_t", "-D_LARGEFILE_SOURCE=1", "-D_FILE_OFFSET_BITS=64", "-DNO_OPENSSL=1")
add_rules("mode.release", "mode.debug")

add_includedirs(".", "./UsageEnvironment/include", "./groupsock/include", "./liveMedia/include", "./BasicUsageEnvironment/include")

includes("UsageEnvironment")
includes("BasicUsageEnvironment")
includes("groupsock")
includes("liveMedia")
includes("mediaServer")
includes("liveSer")
includes("cacheBuf")
