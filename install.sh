# Check for passed arguments to the script
if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi

# Check if the first argument is "ubuntu-latest"
if [ $1 == "ubuntu-latest" ]; then
    # Install all the dependencies you nned to build the project
    sudo apt-get update
    echo "Installing libgl-dev"
    sudo apt-get install -y libgl-dev
    echo "Installing libglu1-mesa-dev"
    sudo apt-get install -y libgl1-mesa-dev
    
fi