fetch_github_content(cmrc vector-of-bool 2.0.1)
cmrc_add_resource_library(
  wintray_example_resource_library
  NAMESPACE resource_library
  resources/tray_icon.ico
)

add_executable(wintray_example)
set_property(TARGET wintray_example PROPERTY CXX_STANDARD 20)
target_link_libraries(wintray_example PRIVATE wintray wintray_example_resource_library make_string)
discover_sources(wintray_example)
target_compile_definitions(wintray_example PRIVATE MODULE_NAME="WinTray Example")