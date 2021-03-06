cmake_minimum_required(VERSION 2.6)

# Use Clang
set(CMAKE_C_COMPILER /usr/bin/clang-3.9)
# set(CMAKE_C_COMPILER /usr/bin/clang++-3.9)
add_definitions("-fPIC")


project(Atomical)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Locate LLVM
message(STATUS "LLVM LIB PATH: ${LLVM_LIBRARIES_PATH}")
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS})

set(LLVM_LIBS
    LLVMLTO
    LLVMObjCARCOpts
    LLVMSymbolize
    LLVMDebugInfoPDB
    LLVMDebugInfoDWARF
    LLVMXCoreDisassembler
    LLVMXCoreCodeGen
    LLVMXCoreDesc
    LLVMXCoreInfo
    LLVMXCoreAsmPrinter
    LLVMSystemZDisassembler
    LLVMSystemZCodeGen
    LLVMSystemZAsmParser
    LLVMSystemZDesc
    LLVMSystemZInfo
    LLVMSystemZAsmPrinter
    LLVMSparcDisassembler
    LLVMSparcCodeGen
    LLVMSparcAsmParser
    LLVMSparcDesc
    LLVMSparcInfo
    LLVMSparcAsmPrinter
    LLVMPowerPCDisassembler
    LLVMPowerPCCodeGen
    LLVMPowerPCAsmParser
    LLVMPowerPCDesc
    LLVMPowerPCInfo
    LLVMPowerPCAsmPrinter
    LLVMNVPTXCodeGen
    LLVMNVPTXDesc
    LLVMNVPTXInfo
    LLVMNVPTXAsmPrinter
    LLVMMSP430CodeGen
    LLVMMSP430Desc
    LLVMMSP430Info
    LLVMMSP430AsmPrinter
    LLVMMipsDisassembler
    LLVMMipsCodeGen
    LLVMMipsAsmParser
    LLVMMipsDesc
    LLVMMipsInfo
    LLVMMipsAsmPrinter
    LLVMHexagonDisassembler
    LLVMHexagonCodeGen
    LLVMHexagonAsmParser
    LLVMHexagonDesc
    LLVMHexagonInfo
    LLVMCppBackendCodeGen
    LLVMCppBackendInfo
    LLVMBPFCodeGen
    LLVMBPFDesc
    LLVMBPFInfo
    LLVMBPFAsmPrinter
    LLVMARMDisassembler
    LLVMARMCodeGen
    LLVMARMAsmParser
    LLVMARMDesc
    LLVMARMInfo
    LLVMARMAsmPrinter
    LLVMAMDGPUCodeGen
    LLVMAMDGPUAsmParser
    LLVMAMDGPUDesc
    LLVMAMDGPUUtils
    LLVMAMDGPUInfo
    LLVMAMDGPUAsmPrinter
    LLVMAArch64Disassembler
    LLVMAArch64CodeGen
    LLVMAArch64AsmParser
    LLVMAArch64Desc
    LLVMAArch64Info
    LLVMAArch64AsmPrinter
    LLVMAArch64Utils
    LLVMMIRParser
    LLVMLibDriver
    LLVMOption
    LLVMTableGen
    LLVMLineEditor
    LLVMX86Disassembler
    LLVMX86AsmParser
    LLVMX86CodeGen
    LLVMSelectionDAG
    LLVMAsmPrinter
    LLVMX86Desc
    LLVMMCDisassembler
    LLVMX86Info
    LLVMX86AsmPrinter
    LLVMX86Utils
    LLVMMCJIT
    LLVMPasses
    LLVMipo
    LLVMVectorize
    LLVMLinker
    LLVMIRReader
    LLVMAsmParser
    LLVMDebugInfoCodeView
    LLVMInterpreter
    LLVMCodeGen
    LLVMScalarOpts
    LLVMInstCombine
    LLVMInstrumentation
    LLVMProfileData
    LLVMBitWriter
    LLVMOrcJIT
    LLVMTransformUtils
    LLVMExecutionEngine
    LLVMTarget
    LLVMAnalysis
    LLVMRuntimeDyld
    LLVMObject
    LLVMMCParser
    LLVMBitReader
    LLVMMC
    LLVMCore
    LLVMSupport
)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Compiler library
add_library(atomical ../src/lib.c)
target_link_libraries(atomical ${LLVM_LIBS})
target_compile_options(atomical PRIVATE "-Werror")
target_compile_options(atomical PRIVATE "-std=c11")

# Compiler executable
add_executable(atomical-bin ../src/main.c)
target_link_libraries(atomical-bin ${LLVM_LIBS} atomical)
target_compile_options(atomical-bin PRIVATE "-Werror")
target_compile_options(atomical-bin PRIVATE "-std=c11")
set_target_properties(atomical-bin PROPERTIES OUTPUT_NAME atomical)

# Test executable 
add_executable(atomical-test ../tests/test.cpp)
target_link_libraries(atomical-test ${GTEST_LIBRARIES} pthread ${LLVM_LIBS} atomical)
target_compile_options(atomical-test PRIVATE "-fpermissive") # required by GTEST
target_compile_options(atomical-test PRIVATE "-Werror")