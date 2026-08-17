#!/bin/bash
#
# wikidata-import.sh — import a Wikidata dump into a MillenniumDB database.
#
# Wikidata publishes its data as N-Triples dumps (compressed with bzip2).
# This script streams a dump straight into `mdb import`, so no extra disk
# space is needed beyond the final database (18-110 GB dumps stream fine).
#
# Usage:
#   ./scripts/wikidata-import.sh <db_folder> [--file dump.nt.bz2] [options]
#
# Options:
#   --file <path>           Path to a local Wikidata dump (.nt, .ttl, .bz2)
#   --download              Download the latest Wikidata Truthy dump and
#                           stream it directly (no temp file on disk)
#   --buffer-strings <N>    Passed to mdb import (default: mdb's default)
#   --buffer-tensors <N>    Passed to mdb import (default: mdb's default)
#   --btree-permutations <N> Passed to mdb import (default: mdb's default)
#   --help                  Show this message
#
# Examples:
#   # Import a local truthy dump (compressed or plain)
#   ./scripts/wikidata-import.sh /data/wikidata --file latest-truthy.nt.bz2
#
#   # Download and import in one pass (no temp file)
#   ./scripts/wikidata-import.sh /data/wikidata --download
#
# Exit codes: 0 success, non-zero on failure (set -e).

set -e; set -u; set -o pipefail

BLUE='\033[0;34m'; GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[0;33m'; NC='\033[0m'

info() { echo -e "${BLUE}[wikidata]${NC} $*"; }
ok()   { echo -e "${GREEN}[wikidata]${NC} $*"; }
warn() { echo -e "${YELLOW}[wikidata]${NC} $*"; }
fail() { echo -e "${RED}[wikidata] ERROR:${NC} $*" >&2; exit 1; }

# Repo root, so the script works from anywhere.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Wikidata dumps
WIKIDATA_TRUTHY_URL="https://dumps.wikimedia.org/wikidatawiki/entities/latest-truthy.nt.bz2"

# ---------------------------------------------------------------------------
# locate the mdb binary (prefer Release, fall back to Debug)
# ---------------------------------------------------------------------------
find_mdb() {
    for candidate in \
        "${REPO_ROOT}/build/Release/bin/mdb" \
        "${REPO_ROOT}/build/Debug/bin/mdb"
    do
        if [[ -x "${candidate}" ]]; then
            echo "${candidate}"
            return 0
        fi
    done
    return 1
}

# ---------------------------------------------------------------------------
# arguments
# ---------------------------------------------------------------------------
if [[ $# -lt 1 ]]; then
    sed -n '4,30p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'
    exit 1
fi

if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    sed -n '4,30p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'
    exit 0
fi

DB_FOLDER="$1"
shift

FILE_PATH=""
DO_DOWNLOAD=0
BUFFER_STRINGS=""
BUFFER_TENSORS=""
BTREE_PERMUTATIONS=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --file)          FILE_PATH="$2"; shift ;;
        --download)      DO_DOWNLOAD=1 ;;
        --buffer-strings) BUFFER_STRINGS="$2"; shift ;;
        --buffer-tensors) BUFFER_TENSORS="$2"; shift ;;
        --btree-permutations) BTREE_PERMUTATIONS="$2"; shift ;;
        --help|-h)
            sed -n '4,30p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *) fail "unknown option: $1 (run with --help for usage)" ;;
    esac
    shift
done

if [[ ${DO_DOWNLOAD} -eq 1 && -n "${FILE_PATH}" ]]; then
    fail "--download and --file are mutually exclusive"
fi

if [[ ${DO_DOWNLOAD} -eq 0 && -z "${FILE_PATH}" ]]; then
    fail "specify either --download or --file <dump>"
fi

MDB_BIN="$(find_mdb)" || fail "mdb binary not found — build it first with ./scripts/setup_millenniumdb.sh"

# ---------------------------------------------------------------------------
# pick a decompressor (parallel if available)
# ---------------------------------------------------------------------------
find_decompressor() {
    if command -v lbunzip2 >/dev/null 2>&1; then
        echo "lbunzip2 -c"
    elif command -v bunzip2 >/dev/null 2>&1; then
        echo "bunzip2 -c"
    elif command -v bzcat >/dev/null 2>&1; then
        echo "bzcat"
    else
        return 1
    fi
}

# ---------------------------------------------------------------------------
# build the mdb import invocation
# ---------------------------------------------------------------------------
mdb_import() {
    # $1 = command that produces N-Triples on stdout (streamed)
    local producer="$1"

    local extra_args=()
    if [[ -n "${BUFFER_STRINGS}" ]]; then
        extra_args+=(--buffer-strings "${BUFFER_STRINGS}")
    fi
    if [[ -n "${BUFFER_TENSORS}" ]]; then
        extra_args+=(--buffer-tensors "${BUFFER_TENSORS}")
    fi
    if [[ -n "${BTREE_PERMUTATIONS}" ]]; then
        extra_args+=(--btree-permutations "${BTREE_PERMUTATIONS}")
    fi

    info "Importing Wikidata into ${DB_FOLDER} ..."
    info "Stream: ${producer} | ${MDB_BIN} import ${DB_FOLDER} --format nt ${extra_args[*]:+${extra_args[*]}}"
    echo
    eval "${producer} | '${MDB_BIN}' import '${DB_FOLDER}' --format nt ${extra_args[*]:+"${extra_args[*]}"}"
}

# ---------------------------------------------------------------------------
# run
# ---------------------------------------------------------------------------
if [[ ${DO_DOWNLOAD} -eq 1 ]]; then
    # Stream download → decompress → import, with no temp files.
    if ! command -v curl >/dev/null 2>&1 && ! command -v wget >/dev/null 2>&1; then
        fail "need curl or wget to download the Wikidata dump"
    fi

    DECOMP="$(find_decompressor)" \
        || fail "need lbunzip2, bunzip2 or bzcat to decompress the Wikidata dump"

    if command -v curl >/dev/null 2>&1; then
        info "Downloading Wikidata Truthy from ${WIKIDATA_TRUTHY_URL}"
        mdb_import "curl -L --fail --silent --show-error '${WIKIDATA_TRUTHY_URL}' | ${DECOMP}"
    else
        info "Downloading Wikidata Truthy from ${WIKIDATA_TRUTHY_URL}"
        mdb_import "wget -qO- '${WIKIDATA_TRUTHY_URL}' | ${DECOMP}"
    fi

else
    if [[ ! -f "${FILE_PATH}" ]]; then
        fail "dump file not found: ${FILE_PATH}"
    fi

    case "${FILE_PATH}" in
        *.bz2)
            DECOMP="$(find_decompressor)" \
                || fail "need lbunzip2, bunzip2 or bzcat to decompress ${FILE_PATH}"
            mdb_import "${DECOMP} '${FILE_PATH}'"
            ;;
        *.nt|*.ttl)
            mdb_import "cat '${FILE_PATH}'"
            ;;
        *)
            fail "unrecognized dump extension (expected .nt, .ttl or .bz2): ${FILE_PATH}"
            ;;
    esac
fi

ok "Wikidata import finished."
