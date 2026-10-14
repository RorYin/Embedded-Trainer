# Embedded C Lab

A lightweight, self-paced Embedded C trainer and interview preparation app. The server runs as a simple FastAPI web application (or standalone Windows executable) on `127.0.0.1:8765`, opening a Monaco-based coding environment in your browser.

**No local or backend GCC compiler is required.** Compilation and test execution run entirely inside the client's browser using a WebAssembly-based C compiler powered by [c2wasm](https://github.com/divsmith/c2wasm).

---

## Key Features & Architecture

- **Zero Host Toolchains**: No native GCC, MinGW, Clang, or toolchain configuration needed on host or client.
- **Ultra-Low Resource Footprint**: The backend server only serves static files and JSON endpoints. It can run on low-power servers, Raspberry Pi, or embedded gateways with minimal CPU and RAM usage.
- **100% Client-Side WebAssembly Execution**: User C programs compile and execute within the browser's WebAssembly sandbox with WASI emulation.
- **Instant Test Feedback**: Compilation to WASM takes tens of milliseconds directly in the browser, providing instant test feedback and detailed compilation error line numbers.
- **Load Reference Solution**: Users can view and load the reference solution (`<problem_name>_solution.c`) at any time using the **Load Solution** button next to the Hint panel.
- **Infinite Loop Protection**: Code runs inside a background Web Worker with an automatic execution timeout to prevent browser freezes.
- **Offline Capable**: Bundled with Monaco editor assets and WebAssembly compiler binaries (`compiler.wasm`, `assembler.wasm`).

---

## Curriculum (30 Embedded C Interview Prep Lessons)

The curriculum is adapted from [Amir7698/embedded-interview-prep](https://github.com/Amir7698/embedded-interview-prep) (01 C Fundamentals) and tailored for high-yield firmware and embedded systems engineering concepts:

### 1. Bit Manipulation (`bit_manipulation`)
- **GPIO Register Operations**: Set, clear, toggle, and read pins using bitmasks (`|=`, `&= ~`, `^=`, `&`).
- **CAN Bit Fields**: Encode and decode CAN frame Priority (3 bits) and DLC (4 bits).
- **Endianness Byte Swap**: Implement 16-bit and 32-bit endianness byte swapping without library functions.
- **Counting & Finding Bits**: Implement bit counting (`popcount`) and lowest set bit detection (`x & -x`).
- **USART_CR1 Register Configuration**: Configure word length, parity, and transmitter/receiver control bits.
- **Bitfield Bug Hunt**: Locate and fix precedence, signed bitshift, and masking errors.

### 2. Pointers and Memory (`pointers_and_memory`)
- **Parsing UART Packet Frame**: Extract packet headers, payload length, payload data, and checksum via pointer arithmetic.
- **Memory-Mapped Peripherals**: Simulate register polling with timeout counters and atomic register writes.
- **Safe Type Punning & Byte Copying**: Implement byte-by-byte memory copying (`my_memcpy`) and safe word unpacking avoiding unaligned access faults.
- **Function Pointer Dispatch Table**: Dispatch system event IDs to handler callbacks without `if`/`switch` chains.
- **Const Checksum Calculation**: Compute 8-bit checksums over read-only flash buffers.
- **Pointer Bug Hunt**: Detect and repair dangling pointers, memory leaks, and off-by-one pointer arithmetic.

### 3. Structs, Unions and Bitfields (`structs_unions_bitfields`)
- **Sizeof & Memory Alignment**: Understand alignment rules and struct padding on 32-bit architectures.
- **GPIO Register Struct Layout**: Model hardware register blocks (MODER, ODR, IDR, BSRR) with struct memory offsets.
- **Bitfield Control Register**: Map hardware status, error, and control bits using C bitfields.
- **Sensor Word Union**: Unpack multi-byte sensor frames into temperature and humidity readings.
- **Packed Command Frame**: Serialize fixed-layout telemetry headers and payloads.
- **Struct Header Bug Hunt**: Fix struct packing and natural alignment traps in protocol packets.

### 4. Storage Classes and Type Qualifiers (`volatile_const_static`)
- **Volatile ISR Communication**: Safe flag communication between an interrupt service routine and main loop.
- **Const Lookup Table**: Store mathematical lookup tables (sine/gamma) in flash ROM to conserve RAM.
- **Static Button Debounce**: Encapsulate button debounce state across periodic polling cycles.
- **Const Volatile Timer Register**: Model hardware read-only timer registers (`const volatile uint32_t*`).
- **Static Module-Level PWM Driver**: Implement private module-scoped hardware variables and public driver APIs.
- **LED Blink Bug Hunt**: Fix compiler optimization traps caused by missing `volatile` in delay loops.

### 5. Endianness and Protocols (`endianness`)
- **Endianness Detection**: Programmatically detect system byte order (Little Endian vs. Big Endian).
- **Read Big-Endian & Little-Endian Buffers**: Parse 16-bit and 32-bit values from mixed-endian network payloads.
- **Write Big-Endian & Little-Endian Buffers**: Serialize sensor readings into network byte order.
- **Modbus TCP MBAP Header**: Parse and construct 7-byte Modbus TCP headers (Transaction ID, Length, Unit ID).
- **CAN Intel Little-Endian Signal Extraction**: Extract variable bit-width signals across byte boundaries.
- **Sensor Endianness Bug Hunt**: Fix byte-order transposition bugs in multi-byte pressure/status packets.

---

## Lesson Directory Structure

Each problem follows a descriptive naming format:

```text
lessons/
  section.json               # Defines curriculum sections and ordering
  bit_manipulation/
    gpio-pin-operations/
      gpio_pin_operations.c          # Starter code for the user
      gpio_pin_operations_solution.c # Reference answer (loaded by "Load Solution" button)
      problem_description.json       # Title, description, and hint
      input_and_output.json          # Test cases
      gpio_pin_operations_solved.c   # Automatically saved when tests pass
  progress.json              # Tracks solved problem completion
```

---

## Running from Source

1. Install **Python 3.11** or newer.
2. Clone or open the repository folder:
   ```powershell
   python -m venv .venv
   .\.venv\Scripts\python.exe -m pip install -r requirements.txt
   .\.venv\Scripts\python.exe main.py
   ```
3. The application will launch in your default web browser at:
   ```
   http://127.0.0.1:8765/embedded-c-lab/
   ```

To run offline with local Monaco Editor assets:
```powershell
.\.venv\Scripts\python.exe scripts\download_monaco.py
```

---

## Running Automated Tests

To run the backend integration and verification test suite:

```powershell
.\.venv\Scripts\python.exe -m pytest -v
```

---

## Building Standalone Windows Executable

To compile the self-contained single-file `.exe`:

1. Run the build script:
   ```powershell
   .\build.bat
   ```
2. The standalone distributable will be generated in `dist/`:
   - `dist\EmbeddedCLab.exe` (Single-file executable)
   - `dist\lessons\` (Editable curriculum problems)
