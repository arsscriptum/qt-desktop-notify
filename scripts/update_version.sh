#!/bin/bash


#+--------------------------------------------------------------------------------+
#|                                                                                |
#|   update_version.sh                                                            |
#|                                                                                |
#+--------------------------------------------------------------------------------+
#|   Guillaume Plante <codegp@icloud.com>                                         |
#|   Code licensed under the GNU GPL v3.0. See the LICENSE file for details.      |
#+--------------------------------------------------------------------------------+


SCRIPT_PATH=$(realpath "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")

tmp_root=$(pushd "$SCRIPT_DIR/.." | awk '{print $1}')
ROOT_DIR=$(eval echo "$tmp_root")
LOGS_DIR="$ROOT_DIR/logs"
SRC_DIR="$ROOT_DIR/src"
LOG_FILE="$LOGS_DIR/build.log"
VERSION_SRC_FILE="$SRC_DIR/version.cpp"
VERSION_FILE=$ROOT_DIR/version.nfo
BUILD_FILE=$ROOT_DIR/build.nfo
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

log_version() {
    if [[ -f "$LOG_FILE" ]]; then
        echo "[$(date)] $1" >> "$LOG_FILE"
    fi
    echo -e " ⚡ ${YELLOW}[version]${NC} $1"
}



# Create version.cpp dynamically
cat <<EOF > "$VERSION_SRC_FILE"
//==============================================================================
//
//  version.cpp
//
//==============================================================================
//  automatically generated on {0}
//==============================================================================


#include <string.h>
#include "version.h"

#ifdef RELEASE_MODE
unsigned int version::major  = {7};
unsigned int version::minor  = {8};
unsigned int version::build  = {9};
std::string  version::sha    = "{5}";
std::string  version::branch = "{6}";
#else
unsigned int version::major  = {1};
unsigned int version::minor  = {2};
unsigned int version::build  = {3};
std::string  version::sha    = "{5}";
std::string  version::branch = "{6}";
#endif // RELEASE_MODE
EOF

latest_tag=$(git tag | tail -n 1)
IFS='.' read -r tag_major tag_minor tag_build <<< "$latest_tag"

# Remove existing build directory
if [ ! -f "$VERSION_FILE" ]; then
    current_version=$(git tag | tail -n 1)
    log_version "current_version $current_version. From TAG"
else
    current_version=$(cat "$VERSION_FILE")
    log_version "current_version $current_version From VersionFile"
fi

# Get current version from version.nfo (assuming the format is major.minor.build)

IFS='.' read -r major minor build <<< "$current_version"

# Increment build number
build=$((build + 1))
debug_version="$major.$minor.$build"
release_version="$tag_major.$tag_minor.$tag_build"
# Write the new version back to the version.nfo file
echo "$debug_version" > "$VERSION_FILE"

# Get Git info
current_branch=$(git branch --show-current)
head_rev=$(git log --format=%h -1)
last_rev=$(git log --format=%h -2 | tail -n 1)

# Write the Git branch and revision information to build.nfo
{
    echo "$current_branch"
    echo "$head_rev"
} > "$BUILD_FILE"

# Define values to replace
DATE=$(date +"%Y-%m-%d %H:%M:%S")

# Replace placeholders with actual values
sed -i -e "s/{0}/$DATE/g" \
       -e "s/{1}/$major/g" \
       -e "s/{2}/$minor/g" \
       -e "s/{3}/$build/g" \
       -e "s/{7}/$tag_major/g" \
       -e "s/{8}/$tag_minor/g" \
       -e "s/{9}/$tag_build/g" \
       -e "s|{5}|$head_rev|g" \
       -e "s|{6}|$current_branch|g" \
       "$VERSION_SRC_FILE"


log_version " ====================================== "
log_version "   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   "
log_version "Generated: $VERSION_SRC_FILE"
log_version "DEBUG Version updated to $debug_version"
log_version "RELEASE Version updated to $release_version"
log_version "Branch and revision info saved."
log_version "Get Exe Version by typing 'sysnoptify -v'"
log_version "   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   ⚠️   "
log_version " ====================================== "
