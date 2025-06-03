#!/bin/bash

# Exit on error
set -e

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Create output directory if it doesn't exist
mkdir -p "$SCRIPT_DIR/generated"

# Download doxygen-awesome theme if not present
if [ ! -d "$SCRIPT_DIR/doxygen-awesome-css" ]; then
    echo "Downloading doxygen-awesome theme..."
    git clone https://github.com/jothepro/doxygen-awesome-css.git "$SCRIPT_DIR/doxygen-awesome-css"
fi

# Copy theme files
mkdir -p "$SCRIPT_DIR/generated/html"
cp "$SCRIPT_DIR/doxygen-awesome-css/"*.{css,js} "$SCRIPT_DIR/generated/html/" 2>/dev/null || true

# Run doxygen
echo "Generating documentation..."
cd "$SCRIPT_DIR"
doxygen Doxyfile

echo "Documentation generated in $SCRIPT_DIR/generated/html" 