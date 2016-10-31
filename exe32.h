//
//  exe32.h
//  Exe
//
//  Created by İsmail Fırat Sezel on 28/09/16.
//  Copyright © 2016 İsmail Fırat Sezel. All rights reserved.
//

#include <cstdint>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#ifndef READER
    #include "reader.h"
#endif
#include "EInterface.h"

#ifndef EXE_STRUCT
#define EXE_STRUCT
	struct exe_ident {
		uint16_t exe_magic;
		uint8_t exe_bytes_in_last_block;
		uint8_t exe_blocks_in_file;
		uint8_t exe_num_relocs;
		uint8_t exe_header_paragraphs;
		uint8_t exe_min_extra_paragraphs;
		uint8_t exe_max_extra_paragraphs;
		uint8_t exe_ss;
		uint8_t exe_sp;
		uint8_t exe_checksum;
		uint8_t exe_ip;
		uint8_t exe_cs;
		uint8_t exe_relocpos;
		uint8_t exe_noverlay;
		uint8_t exe_reserved1[4];
		uint8_t exe_oem_id;
		uint8_t exe_oem_info;
		uint8_t exe_reserved2[10];
		uint16_t  exe_e_lfanew;
		uint32_t exe_pe_address;
	};

	/*Coff Header*/
	struct exe_coff {
		uint32_t Signature;
		uint16_t Machine;
		uint16_t NumberOfSections;
		uint32_t TimeDateStamp;
		uint32_t PointerToSymbolTable;
		uint32_t NumberOfSymbols;
		uint16_t SizeOfOptionalHeader;
		uint16_t Characteristics;
	};

	/*section Table Header*/
	struct exe_section_table {
		uint64_t Name;
		std::string nameStr;
		std::vector<uint8_t> contents;
		uint32_t VirtualSize;
		uint32_t VirtualAddress;
		uint32_t SizeofRawData;
		uint32_t PointerToRawData;
		uint32_t PointertoRelocations;
		uint32_t PointertoLineNumbers;
		uint16_t NumberofRelocations;
		uint16_t NumberofLineNumbers;
		uint32_t Characteristics;
	};
#endif
#ifndef EXE32
#define EXE32

class Exe32 : public ExecutableFile
{
	public:
		struct exe_ident id;
		struct exe_coff coff;
		struct exe_section_table st;
		std::vector<exe_section_table> buffer;
		
		std::ifstream fd;

		Exe32(std::ifstream&);
		Exe32(std::string);
		
		void readDosHeader();
		void readPESignature();
		void readCoffHeader();
		void readSectionTable();
		
		std::vector<uint8_t> getSectionContent(std::string);
		std::vector<std::string> getSectionNames();
};
#endif
