#!/usr/bin/env python3
"""
Format C++ files using clang-format.

Usage:
    python scripts/format.py          # Check and report
    python scripts/format.py --fix    # Format files in place
"""

import os
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
SRC_DIR = PROJECT_ROOT / "src"
TESTS_DIR = PROJECT_ROOT / "tests"


def find_files(directory, extensions):
    """Find all files with given extensions."""
    files = []
    for ext in extensions:
        pattern = f"**/*{ext}"
        for path in directory.rglob(pattern):
            if path.is_file():
                files.append(path)
    return sorted(files)


def run_clang_format(file_path, fix=False):
    """Run clang-format on a single file."""
    cmd = ["clang-format", "-style=file", str(file_path)]
    if fix:
        cmd.insert(2, "-i")

    try:
        result = subprocess.run(cmd, capture_output=True, text=True)
        return result.returncode == 0
    except FileNotFoundError:
        print("Error: clang-format not found. Install it first.")
        sys.exit(1)
    except Exception as e:
        print(f"Error formatting {file_path}: {e}")
        return False


def main():
    fix_mode = "--fix" in sys.argv

    extensions = [".hpp", ".h", ".cpp"]
    src_files = find_files(SRC_DIR, extensions)
    test_files = find_files(TESTS_DIR, extensions)
    all_files = src_files + test_files

    if not all_files:
        print("No files found to format.")
        return

    changed = []
    unchanged = []
    errors = []

    for file_path in all_files:
        success = run_clang_format(file_path, fix_mode)
        if success:
            if fix_mode:
                changed.append(file_path)
            else:
                unchanged.append(file_path)
        else:
            errors.append(file_path)

    if fix_mode:
        if changed:
            print(f"Formatted {len(changed)} files:")
            for f in changed:
                print(f"  {f.relative_to(PROJECT_ROOT)}")
        else:
            print("No files needed formatting.")
    else:
        if unchanged:
            print(f"All {len(unchanged)} files are properly formatted.")
        else:
            print("Run with --fix to format files.")

    if errors:
        print(f"\nErrors formatting {len(errors)} files:")
        for f in errors:
            print(f"  {f.relative_to(PROJECT_ROOT)}")

    sys.exit(0 if not errors else 1)


if __name__ == "__main__":
    main()
