# Id the buuild folder is not present, create it
if [ ! -d "build" ]; then
  mkdir build
fi

if [ $# -eq 0 ]; then
    echo "No arguments supplied, pass either rel or deb"
    exit 1
fi

if [ $1 == "rel" ]; then
    conan install . --build=missing --profile=default 
    rm CMakeUserPresets.json
fi

if [ $1 == "deb" ]; then
    conan install . --build=missing --profile=Debug
    rm CMakeUserPresets.json
fi

