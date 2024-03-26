# Check for passed arguments to the script
if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi

# Check if the first argument is "ubuntu-latest"
if [ $1 == "ubuntu-latest" ]; then
    # Install all the dependencies you nned to build the project


echo "[settings]
arch=x86_64
build_type=Debug
compiler=gcc
compiler.cppstd=gnu17
compiler.libcxx=libstdc++11
compiler.version=11
os=Linux

[conf]
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True" > ~/.conan/profiles/$1

    echo "Checking the content of the profile file: ~/.conan/profiles/$1"
    cat ~/.conan/profiles/$1

fi