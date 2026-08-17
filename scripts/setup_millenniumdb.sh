#!/bin/bash
#
# setup_millenniumdb.sh — one-shot installer for MillenniumDB.
#
# Automates the steps from https://github.com/MillenniumDB/MillenniumDB/wiki/Setup :
#   1. detect the OS and install the system packages MillenniumDB needs
#   2. fetch Boost 1.82 into third_party/ (header-only, no compilation)
#   3. configure and build the project with CMake
#   4. verify the mdb binary works
#
# Usage:
#   ./scripts/setup_millenniumdb.sh [--skip-deps] [--build-type Release|Debug] [--native]
#
# Options:
#   --skip-deps       Skip the system package installation (for containers or
#                     environments where you already installed the deps).
#   --build-type <t>  CMake build type (default: Release).
#   --native          Pass -DMDB_USE_NATIVE_ARCH=ON to CMake, trading binary
#                     portability for peak performance on this machine.
#   --help            Show this message.
#
# Exits 0 on success, non-zero on the first failure (set -e).
#
# This script is meant to be re-run safely: steps that are already done
# (Boost present, build configured) are skipped.

set -e; set -u; set -o pipefail

BLUE='\033[0;34m'; GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[0;33m'; NC='\033[0m'

# ---------------------------------------------------------------------------
# helpers
# ---------------------------------------------------------------------------
info()  { echo -e "${BLUE}[setup]${NC} $*"; }
ok()    { echo -e "${GREEN}[setup]${NC} $*"; }
warn()  { echo -e "${YELLOW}[setup]${NC} $*"; }
fail()  { echo -e "${RED}[setup] ERROR:${NC} $*" >&2; exit 1; }

# Find the repo root (the directory containing CMakeLists.txt), so the script
# works when invoked from anywhere: ./setup.sh, scripts/setup.sh, or via PATH.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${REPO_ROOT}"

# ---------------------------------------------------------------------------
# defaults
# ---------------------------------------------------------------------------
SKIP_DEPS=0
BUILD_TYPE="Release"
CMAKE_EXTRA_ARGS=()

# ---------------------------------------------------------------------------
# parse arguments
# ---------------------------------------------------------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        --skip-deps)   SKIP_DEPS=1 ;;
        --build-type)  BUILD_TYPE="$2"; shift ;;
        --native)      CMAKE_EXTRA_ARGS+=("-DMDB_USE_NATIVE_ARCH=ON") ;;
        --help|-h)
            sed -n '2,28p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *) fail "unknown option: $1 (run with --help for usage)" ;;
    esac
    shift
done

# ---------------------------------------------------------------------------
# 1. system dependencies
# ---------------------------------------------------------------------------
if [[ ${SKIP_DEPS} -eq 0 ]]; then
    info "Installing system dependencies..."

    # macOS (Homebrew)
    if [[ "$(uname)" == "Darwin" ]]; then
        if ! command -v brew >/dev/null 2>&1; then
            fail "Homebrew is required on macOS: https://brew.sh"
        fi
        brew install cmake ncurses openssl@3 icu4c boost

    # Debian / Ubuntu (and derivatives with apt)
    elif command -v apt-get >/dev/null 2>&1; then
        sudo apt-get update
        sudo apt-get install -y \
            git g++ cmake libssl-dev libncurses-dev less \
            python3 python3-venv libicu-dev

    # Fedora / RHEL / CentOS (dnf or yum)
    elif command -v dnf >/dev/null 2>&1 || command -v yum >/dev/null 2>&1; then
        local pkg_manager="dnf"
        command -v dnf >/dev/null 2>&1 || pkg_manager="yum"
        sudo ${pkg_manager} install -y \
            git gcc-c++ cmake openssl-devel ncurses-devel less \
            python3 python3-libs libicu-devel

    # Alpine
    elif command -v apk >/dev/null 2>&1; then
        sudo apk add --no-cache \
            git g++ cmake openssl-dev ncurses-dev less \
            python3 py3-pip icu-dev

    else
        warn "Unsupported package manager — install the dependencies manually."
        warn "See https://github.com/MillenniumDB/MillenniumDB/wiki/Setup"
    fi
    ok "System dependencies ready."
else
    info "Skipping system dependency installation (--skip-deps)."
fi

# ---------------------------------------------------------------------------
# 2. Boost 1.82 (header-only, vendored into third_party)
# ---------------------------------------------------------------------------
BOOST_DIR="third_party/boost_1_82/include"
if [[ -d "${BOOST_DIR}/boost" ]]; then
    info "Boost 1.82 already present at ${BOOST_DIR}, skipping download."
else
    info "Fetching Boost 1.82 (header-only) into third_party/..."
    BOOST_TARBALL="boost_1_82_0.tar.gz"
    BOOST_URL="https://archives.boost.io/release/1.82.0/source/${BOOST_TARBALL}"

    if ! command -v wget >/dev/null 2>&1 && ! command -v curl >/dev/null 2>&1; then
        fail "Need wget or curl to download Boost."
    fi

    if command -v wget >/dev/null 2>&1; then
        wget -q --show-progress "${BOOST_URL}"
    else
        curl -# -L -O "${BOOST_URL}"
    fi

    tar -xf "${BOOST_TARBALL}"
    mkdir -p "${BOOST_DIR}"
    mv "boost_1_82_0/boost" "${BOOST_DIR}/boost"
    rm -r "${BOOST_TARBALL}" "boost_1_82_0"
    ok "Boost 1.82 installed at ${BOOST_DIR}."
fi

# ---------------------------------------------------------------------------
# 3. configure and build
# ---------------------------------------------------------------------------
BUILD_DIR="build/${BUILD_TYPE}"
info "Configuring CMake (${BUILD_TYPE}) in ${BUILD_DIR}/..."
cmake -B "${BUILD_DIR}" \
      -D CMAKE_BUILD_TYPE="${BUILD_TYPE}" \
      "${CMAKE_EXTRA_ARGS[@]+"${CMAKE_EXTRA_ARGS[@]}"}"

info "Building..."
cmake --build "${BUILD_DIR}" -j "$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)"

# ---------------------------------------------------------------------------
# 4. verify
# ---------------------------------------------------------------------------
MDB_BIN="${BUILD_DIR}/bin/mdb"
if [[ ! -x "${MDB_BIN}" ]]; then
    fail "Expected binary not found at ${MDB_BIN}"
fi

info "Verifying ${MDB_BIN}..."
"${MDB_BIN}" 2>&1 | head -3 || true

ok "MillenniumDB installed successfully!"
ok "Binary: ${MDB_BIN}"
ok "Run the server with:  ${MDB_BIN} server --help"
