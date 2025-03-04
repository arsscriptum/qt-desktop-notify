#!/bin/bash


#+--------------------------------------------------------------------------------+
#|                                                                                |
#|   build.sh                                                                  |
#|                                                                                |
#+--------------------------------------------------------------------------------+
#|   Guillaume Plante <codegp@icloud.com>                                         |
#|   Code licensed under the GNU GPL v3.0. See the LICENSE file for details.      |
#+--------------------------------------------------------------------------------+

# variables for colors
WHITE='\033[0;97m'
CYAN='\033[0;36m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color
GREEN='\033[0;32m'


SCRIPT_PATH=$(realpath "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")

tmp_root=$(pushd "$SCRIPT_DIR/.." | awk '{print $1}')
ROOT_DIR=$(eval echo "$tmp_root")
ENV_FILE="$ROOT_DIR/.env"
ROOT_DIRECTORY="$ROOT_DIR"
SCRIPT_DIR="$ROOT_DIR/scripts"
ICONS_DIR="$ROOT_DIR/icons"
BIN_OUT="$BIN_DIR/sysnotify"


YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

log_res() {

    echo -e " ⚡ ${YELLOW}[res]${NC} $1"
}


# Get absolute path and strip trailing slash

REL_PATH=$(basename "$ICONS_DIR")  # Use only the last directory name for prefix

# Define output file
QRC_FILE="resources.qrc"

# Start QRC file
echo '<!DOCTYPE RCC>' > "$QRC_FILE"
echo '<RCC version="1.0">' >> "$QRC_FILE"
REL_PATH=$(realpath --relative-to="$ROOT_DIR" "$ICONS_DIR")
echo "    <qresource prefix=\"\">" >> "$QRC_FILE"
#echo "    <qresource prefix=\"$REL_PATH\">" >> "$QRC_FILE"
log_res "adding prefix \"$REL_PATH\""
# Find all files in the given path and add them to QRC
find "$ICONS_DIR" -type f | while read -r file; do
    REL_FILE_PATH=$(realpath --relative-to="$ROOT_DIR" "$file")
    echo "        <file>$REL_FILE_PATH</file>" >> "$QRC_FILE"
    log_res "adding $REL_FILE_PATH"
done

# Close QRC file
echo "    </qresource>" >> "$QRC_FILE"
echo "</RCC>" >> "$QRC_FILE"

echo "Generated: $QRC_FILE"
