//===- SPIRV.h ? Read and write SPIR-V binary -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
// Copyright (c) 2014 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal with the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimers.
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimers in the documentation
// and/or other materials provided with the distribution.
// Neither the names of Advanced Micro Devices, Inc., nor the names of its
// contributors may be used to endorse or promote products derived from this
// Software without specific prior written permission.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH
// THE SOFTWARE.
//
//===----------------------------------------------------------------------===//
/// \file SPIRV.h
///
/// This files declares functions and passes for translating between LLVM and
/// SPIR-V.
///
///
//===----------------------------------------------------------------------===//
#ifndef LLVM_SUPPORT_SPIRV_H
#define LLVM_SUPPORT_SPIRV_H

#include <string>
#include <iostream>

namespace llvm {
// Pass initialization functions need to be declared before inclusion of
// PassSupport.h.
class PassRegistry;
void initializeLLVMToSPIRVPass(PassRegistry&);
void initializeOCL20To12Pass(PassRegistry&);
void initializeOCL20ToSPIRVPass(PassRegistry&);
void initializeOCL21ToSPIRVPass(PassRegistry&);
void initializeOCLTypeToSPIRVPass(PassRegistry&);
void initializeSPIRVLowerBoolPass(PassRegistry&);
void initializeSPIRVLowerConstExprPass(PassRegistry&);
void initializeSPIRVLowerOCLBlocksPass(PassRegistry&);
void initializeSPIRVRegularizeLLVMPass(PassRegistry&);
void initializeSPIRVToOCL20Pass(PassRegistry&);
void initializeTransOCLMDPass(PassRegistry&);
}

#include "llvm/IR/Module.h"

namespace SPIRV {
class SPIRVModule;

/// \brief Check if a string contains SPIR-V binary.
bool IsSPIRVBinary(std::string &Img);

#ifdef _SPIRV_SUPPORT_TEXT_FMT
/// \brief Convert SPIR-V between binary and internal textual formats.
/// This function is not thread safe and should not be used in multi-thread
/// applications unless guarded by a critical section.
/// \returns true if succeeds.
bool ConvertSPIRV(std::istream &IS, llvm::raw_ostream &OS,
    std::string &ErrMsg, bool FromText, bool ToText);

/// \brief Convert SPIR-V between binary and internel text formats.
/// This function is not thread safe and should not be used in multi-thread
/// applications unless guarded by a critical section.
bool ConvertSPIRV(std::string &Input, std::string &Out,
    std::string &ErrMsg, bool ToText);

/// \brief Check if a string contains SPIR-V in internal text format.
bool IsSPIRVText(std::string &Img);
#endif

} // End namespace SPIRV

namespace llvm {

/// \brief Translate LLVM module to SPIRV and write to ostream.
/// \returns true if succeeds.
bool WriteSPIRV(llvm::Module *M, llvm::raw_ostream &OS, std::string &ErrMsg);

/// \brief Load SPIRV from istream and translate to LLVM module.
/// \returns true if succeeds.
bool ReadSPIRV(llvm::LLVMContext &C, std::istream &IS, llvm::Module *&M,
    std::string &ErrMsg);

/// \brief Regularize LLVM module by removing entities not representable by
/// SPIRV.
bool RegularizeLLVMForSPIRV(llvm::Module *M, std::string &ErrMsg);

/// \brief Mangle OpenCL builtin function function name.
void MangleOpenCLBuiltin(const std::string &UnmangledName,
    ArrayRef<Type*> ArgTypes, std::string &MangledName);

/// Create a pass for translating LLVM to SPIR-V.
ModulePass *createLLVMToSPIRV(SPIRV::SPIRVModule *);

/// Create a pass for translating OCL 2.0 builtin functions to equivalent
/// OCL 1.2 builtin functions.
ModulePass *createOCL20To12();

/// Create a pass for translating OCL 2.0 builtin functions to SPIR-V builtin
/// functions.
ModulePass *createOCL20ToSPIRV();

/// Create a pass for translating OCL 2.1 builtin functions to SPIR-V builtin
/// functions.
ModulePass *createOCL21ToSPIRV();

/// Create a pass for adapting OCL types for SPIRV.
ModulePass *createOCLTypeToSPIRV();

/// Create a pass for lowering cast instructions of i1 type.
ModulePass *createSPIRVLowerBool();

/// Create a pass for lowering constant expressions to instructions.
ModulePass *createSPIRVLowerConstExpr();

/// Create a pass for lowering OCL 2.0 blocks to functions calls.
ModulePass *createSPIRVLowerOCLBlocks();

/// Create a pass for regularize LLVM module to be translated to SPIR-V.
ModulePass *createSPIRVRegularizeLLVM();

/// Create a pass for translating SPIR-V builtin functions to OCL 2.0 builtin
/// functions.
ModulePass *createSPIRVToOCL20();

/// Create a pass for translating SPIR 1.2/2.0 metadata to SPIR-V friendly
/// metadata.
ModulePass *createTransOCLMD();

/// Create and return a pass that writes the module to the specified
/// ostream.
ModulePass *createSPIRVWriterPass(llvm::raw_ostream &Str);

} // namespace llvm



#endif
