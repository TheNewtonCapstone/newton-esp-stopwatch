FILE(GLOB_RECURSE app_sources src/*.cpp src/*.c)

idf_component_register(SRCS ${app_sources}
                    INCLUDE_DIRS "include"
                    REQUIRES newton_esp_utils
                    PRIV_REQUIRES esp_timer)