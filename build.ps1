# Check if the build folder is not present, create it
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Name "build"
}

if ($args.Length -eq 0) {
    Write-Host "No arguments supplied, pass either rel or deb"
    exit 1
}

if ($args[0] -eq "rel") {
    conan install . --build=missing --profile=default
    Remove-Item "CMakeUserPresets.json"
}

if ($args[0] -eq "deb") {
    conan install . --build=missing --profile=Debug
    Remove-Item "CMakeUserPresets.json"
}
