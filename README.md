# gd-mod-cpm

geometry dash mod template using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) as a package manager

this also uses [mat-dash](https://github.com/matcool/mat-dash) to make hooking syntax simpler (and mod_main (very nice))

## setup

clone and configure/build with cmake using x86 msvc (too lazy to copy paste the 32 bit clang setup sorry)

### manual setup
if u want to do it in cmd then
```bash
# Configure
cmake -B build -A win32
# Build
cmake --build build --config Release
```

## tip

with how cpm works if you want to avoid recloning the same repo multiple times you can look into [CPM_SOURCE_CACHE](https://github.com/cpm-cmake/CPM.cmake#cpm_source_cache)

## help

for more detailed info for how modding gd works i suggest checking out my [gd-mod-example](https://github.com/matcool/gd-mod-example) repo
