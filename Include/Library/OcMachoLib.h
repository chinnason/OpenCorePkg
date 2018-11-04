/** @file
  Provides Mach-O parsing helper functions.

Copyright (c) 2016 - 2018, Download-Fritz.  All rights reserved.<BR>
This program and the accompanying materials are licensed and made available
under the terms and conditions of the BSD License which accompanies this
distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef OC_MACHO_LIB_H_
#define OC_MACHO_LIB_H_

#include <IndustryStandard/AppleMachoImage.h>

///
/// Context used to refer to a Mach-O.  This struct is exposed for reference
/// only.  Members are not guaranteed to be sane.
///
typedef struct {
  CONST MACH_HEADER_64        *MachHeader;
  UINTN                       FileSize;
  CONST MACH_SYMTAB_COMMAND   *Symtab;
  CONST MACH_NLIST_64         *SymbolTable;
  CONST CHAR8                 *StringTable;
  CONST MACH_DYSYMTAB_COMMAND *DySymtab;
  CONST MACH_NLIST_64         *IndirectSymbolTable;
  CONST MACH_RELOCATION_INFO  *LocalRelocations;
  CONST MACH_RELOCATION_INFO  *ExternRelocations;
} OC_MACHO_CONTEXT;

/**
  Returns the size of a Mach-O Context.

**/
UINTN
MachoGetContextSize (
  VOID
  );

/**
  Initializes a Mach-O Context.

  @param[out] Context   Mach-O Context to initialize.
  @param[in]  FileData  Pointer to the file's data.
  @param[in]  FileSize  File size of FIleData.

  @return  Whether Context has been initialized successfully.

**/
BOOLEAN
MachoInitializeContext (
  OUT OC_MACHO_CONTEXT  *Context,
  IN  CONST VOID        *FileData,
  IN  UINTN             FileSize
  );

/**
  Returns the Mach-O Header structure.

  @param[in,out] Context  Context of the Mach-O.

**/
CONST MACH_HEADER_64 *
MachoGetMachHeader64 (
  IN OUT OC_MACHO_CONTEXT  *Context
  );

/**
  Returns the Mach-O's file size.

  @param[in,out] Context  Context of the Mach-O.

**/
UINTN
MachoGetFileSize (
  IN OUT OC_MACHO_CONTEXT  *Context
  );

/**
  Returns the last virtual address of a Mach-O.

  @param[in] Context  Context of the Mach-O.

**/
UINT64
MachoGetLastAddress64 (
  IN OUT OC_MACHO_CONTEXT  *Context
  );

/**
  Retrieves the first UUID Load Command.

  @param[in,out] Context  Context of the Mach-O.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_UUID_COMMAND *
MachoGetUuid64 (
  IN OUT OC_MACHO_CONTEXT  *Context
  );

/**
  Retrieves the first segment by the name of SegmentName.

  @param[in,out] Context      Context of the Mach-O.
  @param[in]     SegmentName  Segment name to search for.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SEGMENT_COMMAND_64 *
MachoGetSegmentByName64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     CONST CHAR8       *SegmentName
  );

/**
  Retrieves the first section by the name of SectionName.

  @param[in,out] Context      Context of the Mach-O.
  @param[in]     Segment      Segment to search in.
  @param[in]     SectionName  Section name to search for.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SECTION_64 *
MachoGetSectionByName64 (
  IN OUT OC_MACHO_CONTEXT               *Context,
  IN     CONST MACH_SEGMENT_COMMAND_64  *Segment,
  IN     CONST CHAR8                    *SectionName
  );

/**
  Retrieves a section within a segment by the name of SegmentName.

  @param[in,out] Context      Context of the Mach-O.
  @param[in]     SegmentName  The name of the segment to search in.
  @param[in]     SectionName  The name of the section to search for.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SECTION_64 *
MachoGetSegmentSectionByName64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     CONST CHAR8       *SegmentName,
  IN     CONST CHAR8       *SectionName
  );

/**
  Retrieves the next segment.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Segment  Segment to retrieve the successor of.
                          if NULL, the first segment is returned.

  @retal NULL  NULL is returned on failure.

**/
CONST MACH_SEGMENT_COMMAND_64 *
MachoGetNextSegment64 (
  IN OUT OC_MACHO_CONTEXT               *Context,
  IN     CONST MACH_SEGMENT_COMMAND_64  *Segment  OPTIONAL
  );

/**
  Retrieves the next section of a segment.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Segment  The segment to get the section of.
  @param[in]     Section  The section to get the successor of.
                          If NULL, the first section is returned.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SECTION_64 *
MachoGetNextSection64 (
  IN OUT OC_MACHO_CONTEXT               *Context,
  IN     CONST MACH_SEGMENT_COMMAND_64  *Segment,
  IN     CONST MACH_SECTION_64          *Section  OPTIONAL
  );

/**
  Retrieves a section by its index.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Index    Index of the section to retrieve.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SECTION_64 *
MachoGetSectionByIndex64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     UINT32            Index
  );

/**
  Retrieves a section by its address.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Address  Address of the section to retrieve.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_SECTION_64 *
MachoGetSectionByAddress64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     UINT64            Address
  );

/**
  Returns whether Symbol describes a section.

  @param[in] Symbol  Symbol to evaluate.

**/
BOOLEAN
MachoSymbolIsSection (
  IN CONST MACH_NLIST_64  *Symbol
  );

/**
  Returns whether Symbol is defined.

  @param[in] Symbol  Symbol to evaluate.

**/
BOOLEAN
MachoSymbolIsDefined (
  IN CONST MACH_NLIST_64  *Symbol
  );

/**
  Returns whether Symbol is defined locally.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   Symbol to evaluate.

**/
BOOLEAN
MachoSymbolIsLocalDefined (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Retrieves a locally defined symbol by its name.

  @param[in] Context  Context of the Mach-O.
  @param[in] Name     Name of the symbol to locate.

**/
CONST MACH_NLIST_64 *
MachoGetLocalDefinedSymbolByName (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     CONST CHAR8  *Name
  );

/**
  Retrieves a symbol by its index.

  @param[in] Context  Context of the Mach-O.
  @param[in] Index    Index of the symbol to locate.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_NLIST_64 *
MachoGetSymbolByIndex64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     UINT32            Index
  );

/**
  Retrieves Symbol's name.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   Symbol to retrieve the name of.

**/
CONST CHAR8 *
MachoGetSymbolName64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Retrieves Symbol's name.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   Indirect symbol to retrieve the name of.

**/
CONST CHAR8 *
MachoGetIndirectSymbolName64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Returns whether the symbol's value is a valid address within the Mach-O
  referenced to by Context.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   Symbol to verify the value of.

**/
BOOLEAN
MachoIsSymbolValueSane64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Retrieves the symbol referenced by the Relocation targeting Address.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Address  Address to search for.
  @param[out]    Symbol   Buffer the pointer to the symbol is returned in.
                          May be NULL when the symbol data is invalid.
                          The output data is undefined when FALSE is returned.

  @returns  Whether Relocation exists.

**/
BOOLEAN
MachoGetSymbolByExternRelocationOffset64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     UINT64               Address,
  OUT    CONST MACH_NLIST_64  **Symbol
  );

/**
  Relocate Symbol to be against LinkAddress.

  @param[in,out] Context      Context of the Mach-O.
  @param[in]     LinkAddress  The address to be linked against.
  @param[in,out] Symbol       The symbol to be relocated.

  @returns  Whether the operation has been completed successfully.

**/
BOOLEAN
MachoRelocateSymbol64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     UINT64            LinkAddress,
  IN OUT MACH_NLIST_64     *Symbol
  );

/**
  Returns whether Name is pure virtual.

  @param[in] Name  The name to evaluate.

**/
BOOLEAN
MachoIsSymbolNamePureVirtual (
  IN CONST CHAR8  *Name
  );

/**
  Returns whether Name is a Padslot.

  @param[in] Name  The name to evaluate.

**/
BOOLEAN
MachoIsSymbolNamePadslot (
  IN CONST CHAR8  *Name
  );

/**
  Returns whether Symbol defines a Super Metaclass Pointer.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   The symbol to check.

**/
BOOLEAN
MachoSymbolIsSmcp64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Returns whether Symbol defines a Super Metaclass Pointer.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   The symbol to check.

**/
BOOLEAN
MachoSymbolIsMetaclassPointer64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Retrieves the class name of a Super Meta Class Pointer.

  @param[in,out] Context        Context of the Mach-O.
  @param[in]     SmcpSymbol     SMCP Symbol to get the class name of.
  @param[in]     ClassNameSize  The size of ClassName.
  @param[out]    ClassName      The output buffer for the class name.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetClassNameFromSuperMetaClassPointer (
  IN OUT  OC_MACHO_CONTEXT     *Context,
  IN      CONST MACH_NLIST_64  *SmcpSymbol,
  IN      UINTN                ClassNameSize,
  OUT     CHAR8                *ClassName
  );

/**
  Retrieves the class name of a VTable.

  @param[out] VtableName  The VTable's name.

**/
CONST CHAR8 *
MachoGetClassNameFromVtableName (
  IN  CONST CHAR8  *VtableName
  );

/**
  Retrieves the function prefix of a class name.

  @param[in]  ClassName           The class name to evaluate.
  @param[in]  FunctionPrefixSize  The size of FunctionPrefix.
  @param[out] FunctionPrefix      The output buffer for the function prefix.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetFunctionPrefixFromClassName (
  IN  CONST CHAR8  *ClassName,
  IN  UINTN        FunctionPrefixSize,
  OUT CHAR8        *FunctionPrefix
  );

/**
  Retrieves the class name of a Meta Class Pointer.

  @param[in,out] Context             Context of the Mach-O.
  @param[in]     MetaClassPtrSymbol  MCP Symbol to get the class name of.
  @param[in]     ClassNameSize       The size of ClassName.
  @param[out]    ClassName           The output buffer for the class name.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetClassNameFromMetaClassPointer (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *MetaClassPtrSymbol,
  IN     UINTN                ClassNameSize,
  OUT    CHAR8                *ClassName
  );

/**
  Retrieves the VTable name of a class name.

  @param[in]  ClassName       Class name to get the VTable name of.
  @param[in]  VtableNameSize  The size of VtableName.
  @param[out] VtableName      The output buffer for the VTable name.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetVtableNameFromClassName (
  IN  CONST CHAR8  *ClassName,
  IN  UINTN        VtableNameSize,
  OUT CHAR8        *VtableName
  );

/**
  Retrieves the Meta VTable name of a class name.

  @param[in]  ClassName       Class name to get the Meta VTable name of.
  @param[in]  VtableNameSize  The size of VtableName.
  @param[out] VtableName      The output buffer for the VTable name.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetMetaVtableNameFromClassName (
  IN  CONST CHAR8  *ClassName,
  IN  UINTN        VtableNameSize,
  OUT CHAR8        *VtableName
  );

/**
  Retrieves the final symbol name of a class name.

  @param[in]  ClassName            Class name to get the final symbol name of.
  @param[in]  FinalSymbolNameSize  The size of FinalSymbolName.
  @param[out] FinalSymbolName      The output buffer for the final symbol name.

  @returns  Whether the name has been retrieved successfully.

**/
BOOLEAN
MachoGetFinalSymbolNameFromClassName (
  IN  CONST CHAR8  *ClassName,
  IN  UINTN        FinalSymbolNameSize,
  OUT CHAR8        *FinalSymbolName
  );

/**
  Returns whether Symbol defines a VTable.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Symbol   The symbol to check.

**/
BOOLEAN
MachoSymbolIsVtable64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Symbol
  );

/**
  Returns whether the symbol name describes a C++ symbol.

  @param[in] Name  The name to evaluate.

**/
BOOLEAN
MachoIsSymbolNameCxx (
  IN CONST CHAR8  *Name
  );

/**
  Returns the number of VTable entires in VtableData.

  @param[in,out] Context     Context of the Mach-O.
  @param[in]     VtableData  The VTable's data.

**/
UINTN
MachoVtableGetNumberOfEntries64 (
  IN OUT OC_MACHO_CONTEXT  *Context,
  IN     CONST UINT64      *VtableData
  );

/**
  Retrieves Metaclass symbol of a SMCP.

  @param[in,out] Context  Context of the Mach-O.
  @param[in]     Smcp     The SMCP to evaluate.

  @retval NULL  NULL is returned on failure.

**/
CONST MACH_NLIST_64 *
MachoGetMetaclassSymbolFromSmcpSymbol64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *Smcp
  );

/**
  Retrieves VTable and Meta VTable of a SMCP.
  Logically matches XNU's get_vtable_syms_from_smcp.

  @param[in,out] Context     Context of the Mach-O.
  @param[in]     SmcpSymbol  SMCP Symbol to retrieve the VTables from.
  @param[out]    Vtable      Output buffer for the VTable symbol pointer.
  @param[out]    MetaVtable  Output buffer for the Meta VTable symbol pointer.

**/
BOOLEAN
MachoGetVtableSymbolsFromSmcp64 (
  IN OUT OC_MACHO_CONTEXT     *Context,
  IN     CONST MACH_NLIST_64  *SmcpSymbol,
  OUT    CONST MACH_NLIST_64  **Vtable,
  OUT    CONST MACH_NLIST_64  **MetaVtable
  );

/**
  Returns whether the Relocation's type indicates a Pair for the Intel 64
  platform.
  
  @param[in] Type  The Relocation's type to verify.

**/
BOOLEAN
MachoRelocationIsPairIntel64 (
  IN UINT8  Type
  );

/**
  Returns whether the Relocation's type matches a Pair's for the Intel 64
  platform.
  
  @param[in] Type  The Relocation's type to verify.

**/
BOOLEAN
MachoIsRelocationPairTypeIntel64 (
  IN UINT8  Type
  );

/**
  Returns whether the Relocation shall be preserved for the Intel 64 platform.
  
  @param[in] Type  The Relocation's type to verify.

**/
BOOLEAN
MachoPreserveRelocationIntel64 (
  IN UINT8  Type
  );

#endif // OC_MACHO_LIB_H_
