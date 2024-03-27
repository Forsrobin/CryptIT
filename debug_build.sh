# Id the buuild folder is not present, create it
if [ ! -d "build" ]; then
  mkdir build
fi

conan install . --build=missing --profile=Debug

rm CMakeUserPresets.json