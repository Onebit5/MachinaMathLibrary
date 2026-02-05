#!/usr/bin/env python3
"""
Generate a single-header version of Machina Math Library.
"""

import os
import re
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
SRC_DIR = PROJECT_ROOT / "src"
OUTPUT_FILE = PROJECT_ROOT / "mml.hpp"

HEADER_GUARD = "MML_HPP"

CPP_INCLUDES = [
    "#include <cmath>",
    "#include <cstddef>",
    "#include <cstdint>",
    "#include <type_traits>",
    "#include <algorithm>",
    "#include <array>",
    "#include <vector>",
    "#include <limits>",
    "#include <ostream>",
    "#include <istream>",
]


def find_header_files():
    """Find all .hpp files in src/ directory, excluding mml.hpp."""
    headers = []
    for root, dirs, files in os.walk(SRC_DIR):
        for file in sorted(files):
            if file.endswith(".hpp") and file != "mml.hpp":
                headers.append(Path(root) / file)
    return sorted(headers)


def remove_comments(content):
    """Remove all comments from C++ code."""
    lines = content.split("\n")
    result = []
    in_multiline_comment = False

    for line in lines:
        stripped = line.strip()

        if in_multiline_comment:
            if "*/" in stripped:
                in_multiline_comment = False
            continue

        if stripped.startswith("//"):
            continue

        comment_pos = stripped.find("//")
        if comment_pos != -1:
            line = line[:comment_pos]

        if "/*" in stripped and "*/" in stripped:
            continue
        elif "/*" in stripped:
            in_multiline_comment = True
            continue

        result.append(line)

    return "\n".join(result)


def process_header(content):
    """Process a header file for inclusion in single-header output."""
    lines = content.split("\n")
    processed = []

    for line in lines:
        stripped = line.strip()

        if stripped.startswith("#pragma once"):
            continue

        if stripped.startswith("#ifndef") and ("_HPP" in stripped or "_H" in stripped):
            continue

        if stripped.startswith("#define") and any(
            guard in stripped for guard in ["_HPP", "_H"]
        ):
            continue

        if stripped.startswith("#endif") and ("_HPP" in stripped or "_H" in stripped):
            continue

        if stripped.startswith('#include "'):
            continue

        if stripped.startswith('#include "..'):
            continue

        if stripped.startswith('#include "../'):
            continue

        if stripped.startswith("#include <"):
            continue

        if stripped == "namespace mml {" or stripped == "namespace mml{":
            continue

        if "// namespace mml" in stripped and stripped.startswith("}"):
            continue

        if stripped == "}" and "namespace" in line:
            continue

        processed.append(line)

    return "\n".join(processed)


def get_dependencies(header_path, all_headers):
    """Get list of header files this header depends on."""
    with open(header_path, "r", encoding="utf-8") as f:
        content = f.read()

    matches = re.findall(r'#include\s+["<]([^">]+)[">]', content)
    dependencies = []
    for include in matches:
        if include.startswith('"'):
            continue
        include_name = Path(include).name
        for header in all_headers:
            if header.name == include_name or include.replace("../", "") in str(header):
                if header != header_path:
                    dependencies.append(header)
                break
    return dependencies


def topological_sort(headers):
    """Sort headers by dependency order."""
    headers_set = set(headers)
    sorted_headers = []
    visited = set()
    temp_visited = set()

    def visit(header):
        if header in temp_visited:
            return
        if header in visited:
            return

        temp_visited.add(header)

        deps = get_dependencies(header, headers)
        for dep in deps:
            if dep in headers_set:
                visit(dep)

        temp_visited.remove(header)
        visited.add(header)
        sorted_headers.append(header)

    for header in headers:
        if header not in visited:
            visit(header)

    return sorted_headers


def generate_single_header():
    """Generate the single-header mml.hpp file."""
    headers = find_header_files()

    if not headers:
        print("No header files found in src/")
        return False

    ordered_headers = topological_sort(headers)

    processed_contents = []
    for header_path in ordered_headers:
        relative_path = header_path.relative_to(SRC_DIR)
        print(f"Processing: {relative_path}")

        with open(header_path, "r", encoding="utf-8") as f:
            content = f.read()

        processed = process_header(content)
        no_comments = remove_comments(processed)
        processed_contents.append(no_comments)

    header_content = "\n\n".join(processed_contents)

    cpp_includes_str = "\n".join(CPP_INCLUDES)

    template = f"""#ifndef {HEADER_GUARD}
#define {HEADER_GUARD}

{cpp_includes_str}

namespace mml {{
{header_content}
}} // namespace mml

#endif // {HEADER_GUARD}
"""

    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write(template)

    print(f"\\nGenerated: {OUTPUT_FILE}")
    print(f"Total headers processed: {len(ordered_headers)}")

    return True


if __name__ == "__main__":
    generate_single_header()
