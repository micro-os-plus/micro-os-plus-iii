#!/bin/zsh
# -----------------------------------------------------------------------------
# Build the release test configurations and hash the loadable contents of
# every test binary, to prove that a refactor does not change the generated
# code. Run it before and after the change and `diff` the two output files.
#
# Usage: hash-builds.sh <tests-folder> <out-file>
#
# Notes:
# - Cortex-M: hashes `arm-none-eabi-objcopy -O binary` (no debug info).
# - Native (macOS): hashes `otool -t` and the `__cstring` section.
# - cmsis-os-validator embeds __TIME__, so its hash always changes; compare
#   its images with `cmp -l` and expect only a few differing bytes.
# -----------------------------------------------------------------------------

setopt null_glob

T=$1
OUT=$2
if [[ -z "$T" || -z "$OUT" ]]; then
  echo "Usage: $0 <tests-folder> <out-file>" >&2
  exit 1
fi

TMP=$(mktemp -t hash-builds)
: > "$OUT"

for c in synthetic-posix-cmake-clang-release \
  qemu-cortex-m0-cmake-gcc-release qemu-cortex-m3-cmake-gcc-release \
  qemu-cortex-m4f-cmake-gcc-release qemu-cortex-m7f-cmake-gcc-release; do
  (cd "$T" && xpm run build --config $c > /dev/null 2>&1) \
    || { echo "$c BUILD-FAIL" >> "$OUT"; continue; }
  for f in "$T"/build/$c/platform-bin/*-test "$T"/build/$c/platform-bin/*-test.elf; do
    [[ -f "$f" ]] || continue
    if [[ $c == native* ]]; then
      h=$(otool -t "$f" | tail -n +2 | shasum -a 256 | cut -c1-16)
      h2=$(otool -s __TEXT __cstring -V "$f" 2> /dev/null | shasum -a 256 | cut -c1-16)
    else
      "$T"/build/$c/xpacks/.bin/arm-none-eabi-objcopy -O binary "$f" "$TMP" 2> /dev/null
      h=$(shasum -a 256 "$TMP" | cut -c1-16)
      h2=-
    fi
    echo "$c $(basename "$f") $h $h2" >> "$OUT"
  done
done

rm -f "$TMP"
