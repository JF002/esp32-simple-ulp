set(COMPONENT_SRCS "ulp_blink.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()

set(ULP_APP_NAME ulp_${COMPONENT_NAME})
set(ULP_S_SOURCES ulp/main-ulp-blink.S)
set(ULP_EXP_DEP_SRCS "main-ulp-blink.c")
include(${IDF_PATH}/components/ulp/component_ulp_common.cmake)