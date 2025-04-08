
idf_component_register(SRC_DIRS "." "idfx_src/elecrow" "idfx_src/hardware"
                       INCLUDE_DIRS "." "idfx_include")

lvgl_port_create_c_image("images/esp_logo.png" "images/" "ARGB8888" "NONE")
lvgl_port_add_images(${COMPONENT_LIB} "images/")