# Process Security Scanner & Analyzer (CLI)

A lightweight, high-performance Linux process inspection tool written in C. It parses the '/proc' virtual filesystem to display process metrics, user ownership, command-line arguments, and security tests (detecting suspicious or unknown dynamic library preloads).

## Features

- **Proc Filesystem Parser:** Directly extracts process metadata from `/proc/[PID]/` (`status`, `cmdline`, `maps`, `environ`).
- **Security Tests:**
    - Flags suspicious shared libraries loaded from volatile memory locations (`/tmp`, `/dev/shm`).
    - Detects unknown preloaded libraries (`LD_PRELOAD`).
- **Multiple Output Formats:**
    - **Standard Text:** Human-readable formatted text.
    - **NDJSON:** Newline Delimited JSON export for SIEMs, `jq`, or log pipeline processing.
- **Filtering Options:**
    - Filter processes by specific system users.
    - Display only flagged/suspicious processes.
- **Custom File Export:** Redirect scanner output to a designated file path instead of `stdout`.

## File Structure
```text
Task Manager CLI
├── Makefile            
├── README.md
├── compile_flags.txt
├── include
│   ├── process         
│   │   ├── analyze.h   # Security test functions
│   │   ├── data.h      # ProcessData structure & lifecycle
│   │   ├── parser.h    # /proc file parsing functions
│   │   └── print.h     # Text and NDJSON output formatters
│   ├── procScanner.h   # /proc directory traversal orchestrator
│   └── stringUtils.h   # Helper string manipulation
└── src
    ├── process
    │   ├── analyze.c
    │   ├── data.c
    │   ├── parser.c
    │   └── print.c
    ├── main.c
    ├── procScanner.c
    └── stringUtils.c
```

## Building the Project

### Prerequisites
 - GCC or Clang compiler
 - Make
 - Linux-based OS (utilizes ```/proc``` filesystem)

### Compilation
To compile the project simply use:
```bash
make
```
To clean the build directory:
```bash
make clean
```

### Usage
```bash
./build/taskManagerCLI [OPTIONS]
```
#### Options
| Flag | Long Option | Description |
|------|-------------|-------------|
|```-h```|```--help```|Shows the help message|
|```-s```|```--suspicious```|Display only processes with suspicious/unknown libraries|
|```-j```|```--json```|Output result in NDJSON format|
|```-u <user>```|```--user <name>```|Filter processes owned by a specific username|
|```-o <file>```|```--output <file>```|Save scanner output to a file instead of stdout|

### Examples
1. Scan all processes and display standard text output:
```bash
./build/taskManagerCLI
```
2. Scan processes owned by ```root``` and export to JSON:
```bash
./build/taskManagerCLI -u root -j
```
3. Scan for suspicious processes only and write NDJSON to a report file:
```bash
./build/taskManagerCLI -s -j -o security_report.json
```
4. Parse JSON output using jq:
```bash
./build/taskManagerCLI -j | jq '. | {pid: .pid, name: .name, user: .user}'
```

## License
This project is open-source and available under the MIT License.
