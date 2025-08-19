#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="$SCRIPT_DIR/venv"

# Create virtual environment if it doesn't exist
if [ ! -d "$VENV_DIR" ]; then
    echo "Creating Python virtual environment..."
    python3 -m venv "$VENV_DIR"
fi

# Activate virtual environment
echo "Activating virtual environment..."
source "$VENV_DIR/bin/activate"

# Upgrade pip
echo "Upgrading pip..."
pip install --upgrade pip

# Install dependencies
echo "Installing dependencies..."
pip install -r "$SCRIPT_DIR/requirements.txt"

echo "Generating types..."
rm -rf interfaces && mkdir interfaces
rm -rf out && mkdir out
git clone https://github.com/ros2/rcl_interfaces.git interfaces/rcl_interfaces
git clone https://github.com/ros2/unique_identifier_msgs interfaces/unique_identifier_msgs
git clone https://github.com/ros2/common_interfaces.git interfaces/common_interfaces
git clone https://github.com/ros2/example_interfaces interfaces/example_interfaces
./generate_type_header.py --packages-dir interfaces --output-dir out

cp out/generated_types.h ../../examples/example_types.h
