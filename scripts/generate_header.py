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

AUTHOR = "Jose A. Perez de Azpillaga"
LINE_WIDTH = 76
HEADER_GUARD = "MML_HPP"

def format_header_line(content):
    content_area = LINE_WIDTH - 6
    padding = content_area - len(content)
    if padding < 1:
        padding = 1
    return f"/* {content}{' ' * padding} */"

CPP_INCLUDES = """#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <limits>
#include <ostream>
#include <istream>
#include <functional>
#include <numeric>
#include <random>

#if defined(__SSE2__) || defined(__AVX__)
#include <emmintrin.h>
#endif
#if defined(__SSE4_1__) || defined(__AVX__)
#include <smmintrin.h>
#endif
#if defined(__AVX__)
#include <immintrin.h>
#endif"""


def find_header_files():
    headers = []
    for root, dirs, files in os.walk(SRC_DIR):
        for file in sorted(files):
            if file.endswith(".hpp") and file != "mml.hpp":
                headers.append(Path(root) / file)
    return sorted(headers)


def remove_comments(content):
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
    lines = content.split("\n")
    processed = []

    for line in lines:
        stripped = line.strip()

        if stripped.startswith("#pragma once"):
            continue
        if stripped.startswith("#ifndef") and ("_HPP" in stripped or "_H" in stripped):
            continue
        if stripped.startswith("#define") and ("_HPP" in stripped or "_H" in stripped):
            continue
        if stripped.startswith("#endif") and ("_HPP" in stripped or "_H" in stripped):
            continue

        if stripped.startswith("#include"):
            continue

        if stripped == "namespace mml {" or stripped == "namespace mml{":
            continue

        if stripped.startswith("} //namespace mml") or stripped.startswith(
            "}//namespace mml"
        ):
            continue

        if stripped == "}" and "namespace mml" in line:
            continue

        if re.match(r"^namespace \w+ \{$", stripped):
            processed.append(line)
            continue

        if re.match(r"^} \/\/ namespace \w+$", stripped) or re.match(
            r"^}// namespace \w+$", stripped
        ):
            processed.append(line)
            continue

        processed.append(line)

    return "\n".join(processed)


def get_dependencies(header_path, all_headers):
    with open(header_path, "r", encoding="utf-8") as f:
        content = f.read()

    matches = re.findall(r'#include\s+["<]([^">]+)[">]', content)
    dependencies = []
    for include in matches:
        if include.startswith('"'):
            continue
        include_name = Path(include).name
        for header in all_headers:
            if header.name == include_name or include.replace("../", "") in str(
                header
            ).replace("\\", "/"):
                if header != header_path:
                    dependencies.append(header)
                break
    return dependencies


def topological_sort(headers):
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

    copyright_line = format_header_line(f"Copyright (c) 2026-present {AUTHOR}")

    license_text = f"""/**************************************************************************/
/*  mml.hpp — Machina Math Library (single-header)                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                         MACHINA MATH LIBRARY                           */
/**************************************************************************/
{copyright_line}
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation, to use, copy, modify, merge, publish,              */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/"""

    template = f"""{license_text}
#ifndef {HEADER_GUARD}
#define {HEADER_GUARD}

{CPP_INCLUDES}

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
