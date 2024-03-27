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
build_type=$2
compiler=gcc
compiler.cppstd=gnu17
compiler.libcxx=libstdc++11
compiler.version=11
os=Linux

[conf]
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True" > ~/.conan2/profiles/$1

    ls -la ~/
    ls -la ~/.conan2/
    ls -la ~/.conan2/profiles/

    echo "Checking the content of the profile file: ~/.conan2/profiles/$1"
    cat ~/.conan2/profiles/$1

fi

# Check if the first argument is "ubuntu-latest"
if [ $1 == "windows-latest" ]; then
    # Install all the dependencies you nned to build the project

    echo "[settings]
arch=x86_64
build_type=$2
compiler=msvc
compiler.cppstd=23
compiler.runtime=dynamic
compiler.version=193
os=Windows

[conf]
tools.system.package_manager:mode=install" > ~/.conan2/profiles/$1

    ls -la ~/
    ls -la ~/.conan2/
    ls -la ~/.conan2/profiles/

    echo "Checking the content of the profile file: ~/.conan2/profiles/$1"
    cat ~/.conan2/profiles/$1

fi