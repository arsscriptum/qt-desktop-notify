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
BIN_DIR="$ROOT_DIR/bin"
BIN_OUT="$BIN_DIR/sysnotify"
LOGS_DIR="$ROOT_DIR/logs"
MAKEFILE_FILE="$ROOT_DIR/Makefile"
LOG_FILE="$LOGS_DIR/build.log"
VERSION_SCRIPT_FILE="$SCRIPT_DIR/update_version.sh"
GENERATE_RES_SCRIPT_FILE="$SCRIPT_DIR/generate_qrc.sh"
QRC_FILE="$ROOT_DIR/resources.qrc"
QRC_SRC_FILE="$ROOT_DIR/qrc_resources.cpp"


# =========================================================
# function:     logs functions
# description:  log messages to fils and console
#
# =========================================================
log_ok() {
    if [[ -f "$LOG_FILE" ]]; then
        echo "[$(date)] $1" >> "$LOG_FILE"
    fi
    echo -e " ✔️ ${GREEN}[SUCCESS]${NC} ${CYAN}$1${NC}"
}
log_info() {
    if [[ -f "$LOG_FILE" ]]; then
        echo "[$(date)] $1" >> "$LOG_FILE"
    fi
    echo -e " ⚡ ${CYAN}[log]${NC} $1"
}
log_warn() {
    if [[ -f "$LOG_FILE" ]]; then
        echo "[$(date)] $1" >> "$LOG_FILE"
    fi
    echo -e " ⚠️ ${YELLOW}[warn]${NC} ${WHITE}$1${NC}"
}
log_error() {
    if [[ -f "$LOG_FILE" ]]; then
        echo "[$(date)] $1" >> "$LOG_FILE"
    fi
    echo -e " ❌ ${RED}[error]${NC} ${YELLOW}$1${NC}"
    exit 1
}

pushd "$ROOT_DIR" > /dev/null


# Remove existing build directory
if [ -f "$MAKEFILE_FILE" ]; then
    make clean
fi

"$GENERATE_RES_SCRIPT_FILE"
"$VERSION_SCRIPT_FILE"

RCC_BIN=$(which rcc)

$RCC_BIN "$QRC_FILE" -o "$QRC_SRC_FILE"


# Create new build directory
mkdir -p "$BIN_DIR"
mkdir -p "$LOGS_DIR"


TARGET_TYPE="Release"

if [[ $# -eq 1 && "$1" == "debug" ]]; then
    TARGET_TYPE="Debug"
    BIN_OUT="$BIN_DIR/sysnotify_debug"
fi

# Check if cmake is installed
if ! command -v cmake &> /dev/null; then
    log_error "Error: CMake is not installed. Please install it and try again."
    exit 1
fi

# Remove existing build directory
if [ -d "$BIN_DIR" ]; then
    log_warn "Cleaning existing build directory..."
    rm -rf "$BIN_DIR"
fi


# Generate project files
log_info "Generating project files in $TARGET_TYPE mode..."
cmake -DCMAKE_BUILD_TYPE=$TARGET_TYPE .

if [[ "$TARGET_TYPE" == "Debug" ]]; then
    log_info "Building project in $TARGET_TYPE mode..."
    cmake --build . --target sysnotify_debug
else
    log_info "Building project in $TARGET_TYPE mode..."
    cmake --build . --target sysnotify
fi

# Build the project


if [[ $? -ne 0 ]]; then
    log_error "build failed!"
    exit 1
fi


# Remove existing build directory
if [ ! -f "$BIN_OUT" ]; then
    log_error "build failed, missing $BIN_OUT"
    exit 1
fi

log_ok "Build completed successfully."
popd > /dev/null
