set(MODULE_NAME "mod-promotion-rewards")
set(MODULE_SRCS
    src/mod-promotion-rewards.cpp
)

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod-promotion-rewards.cpp")

install(FILES
    conf/mod-promotion-rewards.conf.dist
    DESTINATION "${CMAKE_INSTALL_PREFIX}/conf"
)