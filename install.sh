# Check for passed arguments to the script
if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi

# Check if the first argument is "ubuntu-latest"
if [ $1 == "ubuntu-latest" ]; then
    # Install all the dependencies you nned to build the project
    sudo apt-get update
    echo "Installing build-essential"
    sudo apt-get install -y build-essential
    echo "Installing libgl-dev"
    sudo apt-get install -y libgl-dev
    echo "Installing libglu1-mesa-dev"
    sudo apt-get install -y libgl1-mesa-dev

    # Install all the dependencies you nned to build the project
    echo "Installing libx11-dev"
    sudo apt-get install -y libx11-dev
    echo "Installing libx11-xcb-dev"
    sudo apt-get install -y libx11-xcb-dev
    echo "Installing libfontenc-dev"
    sudo apt-get install -y libfontenc-dev
    echo "Installing libice-dev"
    sudo apt-get install -y libice-dev
    echo "Installing libsm-dev"
    sudo apt-get install -y libsm-dev
    echo "Installing libxau-dev"
    sudo apt-get install -y libxau-dev
    echo "Installing libxaw7-dev"
    sudo apt-get install -y libxaw7-dev
    echo "Installing libxcomposite-dev"
    sudo apt-get install -y libc6-dev
    echo "Installing libxext-dev"
    sudo apt-get install libc++-dev
    
fi