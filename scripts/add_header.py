import os
import sys
import argparse
from pathlib import Path
import codecs

AUTHOR = "Jose A. Perez de Azpillaga"
LINE_WIDTH = 76

HEADER_TEMPLATE = """/**************************************************************************/
/*  {filename:<69} */
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
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE           */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
"""

def format_header_line(content):
    """Format a header line with correct right-padding.
    
    Total line width is LINE_WIDTH. Prefix '/* ' is 3 chars, suffix ' */' is 3 chars.
    Content area is LINE_WIDTH - 6 chars.
    """
    content_area = LINE_WIDTH - 6
    padding = content_area - len(content)
    if padding < 1:
        padding = 1
    return f"/* {content}{' ' * padding} */"

def format_header(filename):
    """Format the header with proper filename alignment."""
    if len(filename) > 69:
        filename = "..." + filename[-66:]
    
    copyright_content = f"Copyright (c) 2026-present {AUTHOR}"
    copyright_line = format_header_line(copyright_content)
    
    return HEADER_TEMPLATE.format(filename=filename, copyright_line=copyright_line)

def get_project_root():
    """Get the project root directory (parent of scripts folder)."""
    script_path = Path(__file__).resolve()
    return script_path.parent.parent

def detect_header_boundaries(content):
    """Detect the start and end positions of existing header."""
    lines = content.splitlines()
    header_start = None
    header_end = None
    
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped.startswith("/**************************************************************************/"):
            if header_start is None:
                header_start = i
            header_end = i
    
    return header_start, header_end

def has_existing_header(content):
    """Check if the file already has a header."""
    header_start, header_end = detect_header_boundaries(content)
    return header_start is not None and header_end is not None

def should_process_file(filepath, extensions):
    """Determine if a file should be processed."""
    return filepath.suffix.lower() in extensions

def read_file_without_bom(filepath):
    """Read file content, removing BOM if present."""
    try:
        # Read as binary to detect BOM
        with open(filepath, 'rb') as f:
            raw_content = f.read()
        
        # Check for BOM
        has_bom = raw_content.startswith(codecs.BOM_UTF8)
        
        # Decode content, removing BOM if present
        if has_bom:
            content = raw_content.decode('utf-8-sig')  # Automatically removes BOM
        else:
            try:
                content = raw_content.decode('utf-8')
            except UnicodeDecodeError:
                # Try other common encodings
                content = raw_content.decode('latin-1')
        
        return content, has_bom
    except Exception as e:
        raise Exception(f"Failed to read file: {e}")

def write_file_without_bom(filepath, content):
    """Write content to file without BOM."""
    # Ensure content is string
    if isinstance(content, bytes):
        content = content.decode('utf-8')
    
    # Write with UTF-8 encoding without BOM
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(content)

def add_or_replace_header_in_file(filepath, dry_run=False, verbose=False, remove_bom=False, refresh_header=False):
    """Add or replace header in a single file with BOM handling."""
    try:
        # Read file content and check for BOM
        content, had_bom = read_file_without_bom(filepath)
        
        # Format new header
        header = format_header(filepath.name)
        
        # Check if already has header
        header_start, header_end = detect_header_boundaries(content)
        has_header = header_start is not None and header_end is not None
        
        actions = []
        new_content = ""
        
        if has_header:
            if refresh_header:
                # Remove existing header and add new one
                lines = content.splitlines()
                
                # Get content after the header
                if header_end + 1 < len(lines):
                    # Skip the header and any empty lines immediately after it
                    after_header_start = header_end + 1
                    while after_header_start < len(lines) and not lines[after_header_start].strip():
                        after_header_start += 1
                    
                    remaining_content = "\n".join(lines[after_header_start:])
                    if remaining_content:
                        # Add newline after header if there's content
                        new_content = header + "\n" + remaining_content
                    else:
                        # No content after header
                        new_content = header
                else:
                    # Header is at the end of file
                    new_content = header
                
                actions.append("replaced header")
            else:
                # Don't replace, just report
                if had_bom and remove_bom and not dry_run:
                    # File has header but also has BOM, remove BOM
                    write_file_without_bom(filepath, content)
                    if verbose:
                        print(f"⚠️  {filepath} - removed BOM from file with existing header")
                    return True, "removed BOM"
                elif had_bom and verbose:
                    print(f"⚠️  {filepath} - has BOM (use --remove-bom to fix)")
                elif verbose:
                    print(f"✅ {filepath} - already has header")
                return False, "already has header"
        else:
            # No existing header, add new one
            # Remove leading whitespace/newlines from original content
            cleaned_content = content.lstrip()
            if cleaned_content:
                new_content = header + "\n" + cleaned_content
            else:
                new_content = header
            actions.append("added header")
        
        # Handle BOM
        if had_bom and remove_bom:
            actions.append("removed BOM")
        
        # Apply changes if not in dry run
        if not dry_run:
            write_file_without_bom(filepath, new_content)
        
        return True, " and ".join(actions)
        
    except PermissionError:
        print(f"❌ Permission denied: {filepath}", file=sys.stderr)
        return False, "permission error"
    except UnicodeDecodeError:
        print(f"❌ Encoding error: {filepath}", file=sys.stderr)
        return False, "encoding error"
    except Exception as e:
        print(f"❌ Error processing {filepath}: {e}", file=sys.stderr)
        return False, str(e)

def fix_bom_only(filepath, dry_run=False, verbose=False):
    """Only remove BOM from file without adding header."""
    try:
        content, had_bom = read_file_without_bom(filepath)
        
        if had_bom:
            if not dry_run:
                write_file_without_bom(filepath, content)
            if verbose or not dry_run:
                print(f"{'➕' if dry_run else '✅'} {filepath} - removed BOM")
            return True
        elif verbose:
            print(f"✅ {filepath} - no BOM found")
        return False
        
    except Exception as e:
        print(f"❌ Error fixing BOM in {filepath}: {e}", file=sys.stderr)
        return False

def main():
    """Main function to process files."""
    parser = argparse.ArgumentParser(
        description="Add or refresh license headers to source files with BOM handling",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s                    # Add headers to all files in src/
  %(prog)s --dry-run          # Show what would be changed
  %(prog)s --verbose          # Show detailed output
  %(prog)s --remove-bom       # Remove BOM while adding headers
  %(prog)s --refresh-headers  # Refresh existing headers (remove and re-add)
  %(prog)s --fix-bom-only     # Only remove BOM, don't add headers
  %(prog)s --extensions .cpp,.h,.hpp  # Process specific extensions
  %(prog)s --exclude-dirs build,test  # Exclude directories
        """
    )
    parser.add_argument('--src-dir', default='src',
                       help='Source directory (default: src)')
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be changed without modifying files')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Show detailed output')
    parser.add_argument('--remove-bom', action='store_true',
                       help='Remove UTF-8 BOM when adding headers')
    parser.add_argument('--refresh-headers', action='store_true',
                       help='Refresh existing headers (remove and re-add)')
    parser.add_argument('--fix-bom-only', action='store_true',
                       help='Only remove BOM, don\'t add headers')
    parser.add_argument('--extensions', default='.cpp,.h,.hpp,.c,.cc,.cxx',
                       help='Comma-separated list of extensions to process')
    parser.add_argument('--exclude-dirs', default='',
                       help='Comma-separated list of directories to exclude')
    parser.add_argument('--report-bom', action='store_true',
                       help='Report files with BOM without modifying them')
    
    args = parser.parse_args()
    
    # Get project root relative to script location
    project_root = get_project_root()
    src_dir = project_root / args.src_dir
    
    if not src_dir.exists():
        print(f"❌ Source directory not found: {src_dir}", file=sys.stderr)
        sys.exit(1)
    
    extensions = tuple(ext.strip().lower() for ext in args.extensions.split(','))
    exclude_dirs = set(dir.strip() for dir in args.exclude_dirs.split(',')) if args.exclude_dirs else set()
    
    print(f"📁 Processing: {src_dir}")
    if args.dry_run:
        print("🔍 Dry run mode - no files will be modified")
    if args.fix_bom_only:
        print("🛠️  Fix BOM only mode - will not add headers")
    elif args.refresh_headers:
        print("🔄 Refresh mode - will replace existing headers")
    elif args.remove_bom:
        print("⚠️  BOM removal mode - will remove BOM when adding headers")
    if args.report_bom:
        print("📋 BOM report mode - will only report files with BOM")
    print(f"📄 Extensions: {', '.join(extensions)}")
    if exclude_dirs:
        print(f"🚫 Excluding: {', '.join(exclude_dirs)}")
    print("-" * 50)
    
    processed = 0
    skipped = 0
    errors = 0
    bom_files = 0
    
    for root, dirs, files in os.walk(src_dir):
        # Modify dirs in-place to exclude directories
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            filepath = Path(root) / file
            
            if not should_process_file(filepath, extensions):
                if args.verbose:
                    print(f"⏩ Skipping {filepath.relative_to(project_root)} - wrong extension")
                continue
            
            relative_path = filepath.relative_to(project_root)
            
            try:
                if args.report_bom:
                    # Just report BOM status
                    content, had_bom = read_file_without_bom(filepath)
                    if had_bom:
                        print(f"⚠️  BOM found: {relative_path}")
                        bom_files += 1
                    elif args.verbose:
                        print(f"✅ No BOM: {relative_path}")
                    continue
                
                if args.fix_bom_only:
                    # Fix BOM only mode
                    if fix_bom_only(filepath, args.dry_run, args.verbose):
                        processed += 1
                    else:
                        skipped += 1
                else:
                    # Normal mode: add/replace header with optional BOM removal
                    if args.dry_run:
                        content, had_bom = read_file_without_bom(filepath)
                        header_start, header_end = detect_header_boundaries(content)
                        has_header = header_start is not None and header_end is not None
                        
                        if has_header:
                            if args.refresh_headers:
                                actions = ["replace header"]
                                if had_bom and args.remove_bom:
                                    actions.append("remove BOM")
                                print(f"➕ {relative_path} - would {' and '.join(actions)}")
                                processed += 1
                            else:
                                if had_bom and args.remove_bom:
                                    print(f"➕ {relative_path} - would remove BOM from file with header")
                                    processed += 1
                                elif args.verbose:
                                    print(f"✅ {relative_path} - already has header" + 
                                          (" (has BOM)" if had_bom else ""))
                                    skipped += 1
                        else:
                            actions = ["add header"]
                            if had_bom and args.remove_bom:
                                actions.append("remove BOM")
                            print(f"➕ {relative_path} - would {' and '.join(actions)}")
                            processed += 1
                            
                    else:
                        success, action = add_or_replace_header_in_file(
                            filepath, 
                            dry_run=False, 
                            verbose=args.verbose,
                            remove_bom=args.remove_bom,
                            refresh_header=args.refresh_headers
                        )
                        if success:
                            print(f"✅ {relative_path} - {action}")
                            processed += 1
                        else:
                            if "already has header" not in action:  # Don't count skips as errors
                                errors += 1
                            skipped += 1
                            
            except Exception as e:
                print(f"❌ {relative_path} - error: {e}")
                errors += 1
    
    print("-" * 50)
    print(f"📊 Summary:")
    
    if args.report_bom:
        print(f"   Files with BOM: {bom_files}")
    else:
        print(f"   Processed: {processed} files")
        print(f"   Skipped: {skipped} files")
    
    if errors > 0:
        print(f"   Errors: {errors} files")
    
    if args.dry_run and processed > 0 and not args.report_bom:
        print("\n💡 Run without --dry-run to apply changes")

if __name__ == "__main__":
    main()
