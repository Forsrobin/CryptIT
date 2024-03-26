# Id the buuild folder is not present, create it
if [ ! -d "build" ]; then
  mkdir build
fi

conan install . --build=missing -s build_type=Debug 
cmake --preset vs2022-deb
cmake --build --preset vs2022-deb
ctest --preset vs2022-deb

rm CMakeUserPresets.json