add_executable(smart_mirror main.cpp)

# Link the executable to the Speeddemon library. Since the Speeddemon library has
# public include directories we will use those link directories when building
# smart_mirror
#target_link_libraries (smart_mirror LINK_PUBLIC Speeddemon)
