# Id the buuild folder is not present, create it
if [ ! -d "build" ]; then
  mkdir build
fi

conan install . --build=missing --profile=default 
cmake --preset vs2022-rel
cmake --build --preset vs2022-rel

rm CMakeUserPresets.json