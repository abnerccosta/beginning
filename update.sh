if command -v snap &> /dev/null && snap version &> /dev/null; then
    echo "Snap support is enabled. Updating snaps..."
    sudo snap refresh
else
    echo "Snap support is not enabled. Skipping snap update."
fi

if command -v flatpak &> /dev/null && flatpak --version &> /dev/null; then
    echo "Flatpak support is enabled. Updating flatpaks..."
        flatpak update -y
else
    echo "Flatpak support is not enabled. Skipping flatpak update."
fi

echo "Updating APT packages..."
sudo apt-get update && sudo apt-get upgrade -y
