#include "elf32.h"


Elf32::Elf32(std::string path)
{
    this->fd.open(path,std::ios::in|std::ios::binary);
}

Elf32SH::~Elf32SH()
{
}

Elf32SH::Elf32SH()
{
    this->name = "";
}


std::string Elf32::getHeaderInfo()
{
    std::stringstream ss;
    std::string temp_string = "";

    ss << "Magic bytes: "<< std::hex << std::showbase << id.ei_magic[0] << id.ei_magic[1] << id.ei_magic[2] << id.ei_magic[3] << "\n";
    /*Find class of file*/
    switch(id.ei_class)
    {
    case 0:
        temp_string = "Invalid object";
        break;
    case 1:
        temp_string = "32-bit object";
        break;
    case 2:
        temp_string = "64-bit object";
        break;
    default:
        temp_string = "Invalid object";
        break;
    }
    ss << "File Class: " << temp_string << "\n";

    /*find endianness of file*/
    switch(id.ei_data)
    {
    case 0:
        temp_string = "Invalid data encoding";
        break;
    case 1:
        temp_string = "Little endian";
        break;
    case 2:
        temp_string = "Big endian";
        break;
    default:
        temp_string = "Invalid data encoding";
        break;
    }
    ss << "File Endianness: " << temp_string << "\n";

    /*header version*/
    ss << "ELF Header Version: " << std::hex << std::showbase << (uint32_t) id.ei_version << "\n";

    switch(id.ei_osabi)
    {
    case 0:
        temp_string = "System V";
        break;
    case 1:
        temp_string = "HP-UX";
        break;
    case 2:
        temp_string = "NetBSD";
        break;
    case 3:
        temp_string = "Linux";
        break;
    case 6:
        temp_string = "Solaris";
        break;
    case 7:
        temp_string = "AIX";
        break;
    case 8:
        temp_string = "IRIX";
        break;
    case 9:
        temp_string = "FreeBSD";
        break;
    case 12:
        temp_string = "OpenBSD";
        break;
    case 13:
        temp_string = "OpenVMS";
        break;
    case 14:
        temp_string = "NonStop Kernel";
        break;
    case 15:
        temp_string = "AROS";
        break;
    case 16:
        temp_string = "Fenix OS";
        break;
    case 17:
        temp_string = "CloudABI";
        break;
    case 0x53:
        temp_string = "Sortix";
        break;
    default:
        temp_string = "Invalid operating system abi";
        break;
    }
    ss << "Operating system ABI: " << temp_string << "\n";

    ss << "ABI version: " << std::hex << std::showbase << id.ei_abiversion << "\n";

    /*file type*/
    if(e_type == 0)
        temp_string = "No file type";
    else if(e_type == 1)
        temp_string = "Relocatable file";
    else if(e_type == 2)
        temp_string = "Executable file";
    else if(e_type == 3)
        temp_string = "Shared object file";
    else if(e_type == 4)
        temp_string = "Core file";
    else if(e_type > 0xff00)
        temp_string = "Processor Specific information";
    else
        temp_string = "Invalid file type";
    ss << "File type: " << temp_string << "\n";

    /*Target Machine*/
    switch(e_machine)
    {
    case 0x0:
        temp_string = "No machine";
        break;
    case 0x1:
        temp_string = "AT&T WE 32100";
        break;
    case 0x2:
        temp_string = "SPARC";
        break;
    case 0x3:
        temp_string = "Intel 80386(x86)";
        break;
    case 0x4:
        temp_string = "Motorola 68000";
        break;
    case 0x5:
        temp_string = "Motorola 88000";
        break;
    case 0x6:
        temp_string = "Intel 80860";
        break;
    case 0x7:
        temp_string = "Intel 80860";
        break;
    case 0x8:
        temp_string = "MIPS RS3000";
        break;
    case 0x14:
        temp_string = "PowerPC";
        break;
    case 0x28:
        temp_string = "ARM";
        break;
    case 0x2A:
        temp_string = "Super H";
        break;
    case 0x32:
        temp_string = "IA-64 (Intel Itanium)";
        break;
    case 0x3E:
        temp_string = "AMD64";
        break;
    case 0xB7:
        temp_string = "AArch64";
        break;
    default:
        temp_string = "Invalid architecture";
        break;
    }
    ss << "Target architecture: " << temp_string << "\n";

    ss << "Program start address: " << std::hex << std::showbase <<e_entry << "\n";/*convert to hex ?*/
    ss << "Start of program headers: " << std::hex << std::showbase <<e_phoff << "\n";
    ss << "Start of section headers: " << std::hex << std::showbase <<e_shoff << "\n";
    ss << "Flag: " << std::hex << std::showbase <<e_flags << "\n";
    ss << "Elf header size: " << e_ehsize << "\n";
    ss << "Entry size program headers: " << e_phentsize << "\n";
    ss << "Number of program headers: " << e_phnum << "\n";
    ss << "Entry size section headers: " << e_shentsize << "\n";
    ss << "Number of section headers: " << e_shnum << "\n";
    ss << "Index of section header names section:" << e_shstrndx <<"\n";

    return ss.str();
}

std::vector<uint8_t> Elf32::getHexHeader()
{
    std::vector<uint8_t> hex_buffer(e_ehsize);
    this->fd.seekg(0,std::ios::beg);
    this->fd.read((char *)&hex_buffer[0], e_ehsize);
    return hex_buffer;
}

void Elf32::readIdent()
{
    this->fd.read((char *)this->id.ei_magic,4);
    this->fd.read((char *)&this->id.ei_class,1);
    this->fd.read((char *)&this->id.ei_data,1);
    this->fd.read((char *)&this->id.ei_version,1);
    this->fd.read((char *)&this->id.ei_osabi,1);
    this->fd.read((char *)&this->id.ei_abiversion,1);
    this->fd.read((char *)this->id.ei_pad,7);
    return; 
}

void Elf32::read(uint8_t* to, uint8_t* from, uint8_t offset)
{
    if(this->id.ei_data == 1)//lsb
        readLittleEndian(to,from,offset);
    else//msb
        readBigEndian(to,from,offset);
}

void Elf32::read(uint16_t* to, uint8_t* from, uint8_t offset)
{
    if(this->id.ei_data == 1)//lsb
        readLittleEndian(to,from,offset);
    else//msb
        readBigEndian(to,from,offset);
}

void Elf32::read(uint32_t* to, uint8_t* from, uint8_t offset)
{
    if(this->id.ei_data == 1)//lsb
        readLittleEndian(to,from,offset);
    else//msb
        readBigEndian(to,from,offset);
}

void Elf32::read(uint64_t* to, uint8_t* from, uint8_t offset)
{
    if(this->id.ei_data == 1)//lsb
        readLittleEndian(to,from,offset);
    else//msb
        readBigEndian(to,from,offset);
}

std::string Elf32::getRelInfo()
{
    std::stringstream ss;
    std::string temp_string;
    uint64_t counter = 0;
    /*this is an range based for loop*/
    ss << "Rel section\n";
    for(auto rel : this->relList)/*First use of auto*/
    {
        ss << "Index: " << counter++ << "\n";
        ss << "\tOffset: " << std::hex << rel.r_offset << "\n";
        ss << "\tRelated symbol table index: " << (rel.r_info >> 8) << "\n";
        switch(rel.r_info & 0x0f)
        {
        case 0:
            temp_string = "R_386_NONE";
            break;
        case 1:
            temp_string = "R_386_32";
            break;
        case 2:
            temp_string = "R_386_PC32";
            break;
        case 3:
            temp_string = "R_386_GOT32";
            break;
        case 4:
            temp_string = "R_386_PLT32";
            break;
        case 5:
            temp_string = "R_386_COPY";
            break;
        case 6:
            temp_string = "R_386_GLOB_DAT";
            break;
        case 7:
            temp_string = "R_386_JMP_SLOT";
            break;
        case 8:
            temp_string = "R_386_RELATIVE";
            break;
        case 9:
            temp_string = "R_386_GOTOFF";
            break;
        case 10:
            temp_string = "R_386_GOTPC";
            break;
        default:
            temp_string = "Invalid relocation type";
            break;
        }
        ss << "\tRelocation type: " <<temp_string << "\n";
    }
    return ss.str();
}

std::string Elf32::getRelaInfo()
{
    std::stringstream ss;
    std::string temp_string;
    uint64_t counter = 0;
    /*this is an range based for loop*/
    ss << "Rela section\n";
    for(auto rela : this->relaList)/*First use of auto*/
    {
        ss << "Index: " << counter++ << "\n";
        ss << "\tOffset: " << std::hex << rela.r_offset << "\n";
        ss << "\tRelated symbol table index: " << (rela.r_info >> 8) << "\n";
        switch(rela.r_info & 0x0f)
        {
        case 0:
            temp_string = "R_386_NONE";
            break;
        case 1:
            temp_string = "R_386_32";
            break;
        case 2:
            temp_string = "R_386_PC32";
            break;
        case 3:
            temp_string = "R_386_GOT32";
            break;
        case 4:
            temp_string = "R_386_PLT32";
            break;
        case 5:
            temp_string = "R_386_COPY";
            break;
        case 6:
            temp_string = "R_386_GLOB_DAT";
            break;
        case 7:
            temp_string = "R_386_JMP_SLOT";
            break;
        case 8:
            temp_string = "R_386_RELATIVE";
            break;
        case 9:
            temp_string = "R_386_GOTOFF";
            break;
        case 10:
            temp_string = "R_386_GOTPC";
            break;
        default:
            temp_string = "Invalid relocation type";
            break;
        }
        ss << "\tRelocation type: " <<temp_string << "\n";
        ss << "\tAddend: " << rela.r_addend << "\n";
    }
    return ss.str();
}

void Elf32::readHeader()
{
    uint8_t buffer[36];
    this->fd.read((char *)&buffer,36);
    readLittleEndian(&(this->e_type),buffer,0);
    readLittleEndian(&(this->e_machine), buffer,2);
    readLittleEndian(&(this->e_version), buffer,4);
    readLittleEndian(&(this->e_entry), buffer,8);
    readLittleEndian(&(this->e_phoff), buffer, 12);
    readLittleEndian(&(this->e_shoff), buffer, 16);
    readLittleEndian(&(this->e_flags), buffer, 20);
    readLittleEndian(&(this->e_ehsize), buffer, 24);
    readLittleEndian(&(this->e_phentsize), buffer, 26);
    readLittleEndian(&(this->e_phnum), buffer, 28);
    readLittleEndian(&(this->e_shentsize), buffer, 30);
    readLittleEndian(&(this->e_shnum), buffer, 32);
    readLittleEndian(&(this->e_shstrndx), buffer, 34);

    return;
}

void Elf32::readSectionHeader(uint32_t offset, uint32_t strtab)
{
    uint8_t buffer[40];
    char c;
    Elf32SH header;

    this->fd.seekg(offset,std::ios::beg);
    this->fd.read((char *)buffer, 40);
    readLittleEndian(&(header.sh_name), buffer, 0);
    readLittleEndian(&(header.sh_type), buffer, 4);
    readLittleEndian(&(header.sh_flags), buffer, 8);
    readLittleEndian(&(header.sh_addr), buffer, 12);
    readLittleEndian(&(header.sh_offset), buffer, 16);
    readLittleEndian(&(header.sh_size), buffer, 20);
    readLittleEndian(&(header.sh_link), buffer, 24);
    readLittleEndian(&(header.sh_info), buffer, 28);
    readLittleEndian(&(header.sh_addralign), buffer, 32);
    readLittleEndian(&(header.sh_entsize), buffer, 36);
    
    header.name = "";
    //reading name
    this->fd.seekg(strtab+header.sh_name,std::ios::beg);
    do
    {
        this->fd.read(&c,1);
        if(c != '\0')
			header.name += c;
    }while(c != '\0');

    if(header.sh_type == 2 || header.sh_type == 11)
	{
		readSymbolTable(header);
	}
    else if(header.sh_type == 9)/*rel*/
    {
        /*may be a function*/
        uint8_t rel_buffer[8];
        uint64_t counter;

        /*set file descriptor to beginning of rel section content*/
        this->fd.seekg(header.sh_offset, std::ios::beg);
        for(counter = 0; counter < header.sh_size; counter += 8)
        {
            Elf32_Rel relContent;

            this->fd.read((char *)&rel_buffer, 8);/*& neden?*/
            readLittleEndian(&(relContent.r_offset), rel_buffer, 0);
            readLittleEndian(&(relContent.r_info), rel_buffer, 4);

            this->relList.push_back(relContent);
        }
    }
    else if(header.sh_type == 4)/*rela*/
    {
        /*may be a function*/
        uint8_t rela_buffer[12];
        uint64_t counter;

        /*set file descriptor to beginning of rel section content*/
        this->fd.seekg(header.sh_offset, std::ios::beg);
        for(counter = 0; counter < header.sh_size; counter += 12)
        {
            Elf32_Rela relaContent;

            this->fd.read((char *)&rela_buffer, 12);/*& neden?*/
            readLittleEndian(&(relaContent.r_offset), rela_buffer, 0);
            readLittleEndian(&(relaContent.r_info), rela_buffer, 4);
            readLittleEndian((uint32_t *)&relaContent.r_addend, rela_buffer, 8);

            this->relaList.push_back(relaContent);
        }
    }

    if(header.sh_type != 8)
	{
        /*If type of section is SHT_NOBITS then it has no data information on file*/
		this->fd.seekg(header.sh_offset,fd.beg);
		header.content.resize(header.sh_size);
		this->fd.read((char *)&header.content[0], header.sh_size);
	}


    this->sHeaders.push_back(header);
}

void Elf32::readSectionHeaders()
{
	char c;
    uint8_t buf[4];
    uint16_t i;
    uint32_t index;
    uint64_t offset = this->e_shoff;
    uint64_t shstrtab = (this->e_shstrndx*this->e_shentsize) + this->e_shoff + 16;
    this->fd.seekg(shstrtab,std::ios::beg);
    this->fd.read((char *)buf,4);
    readLittleEndian(&shstrtab,buf,0);
    for(i = 0; i < this->e_shnum; i++)
    {
        readSectionHeader(offset,shstrtab);
        offset += this->e_shentsize;
    }

    std::vector<uint8_t> &dynstr = this->sHeaders[getSection(".dynstr")].content;
	std::vector<uint8_t> &strtab = this->sHeaders[getSection(".strtab")].content;
	/*read the names of symbols*/
	for(i = 0; i < this->dynamicSymbolTable.size(); i++)
	{
		std::string &name = this->dynamicSymbolTable[i].name;
		index = this->dynamicSymbolTable[i].st_name;
		do
		{
			c = dynstr[index++];
			if(c != '\0')
				name += c;
		}while(c != '\0');
	}

	for(i = 0; i < this->staticSymbolTable.size(); i++)
	{
		std::string &name = this->staticSymbolTable[i].name;
		index = this->staticSymbolTable[i].st_name;
		do
		{
			c = strtab[index++];
			if(c != '\0')
				name += c;
		}while(c != '\0');
	}
	return;

}

std::vector<std::string> Elf32::getSectionNames()
{
    uint32_t i;
    std::vector<std::string> nameList;
    for(i = 0; i < this->sHeaders.size(); i++)
    {
        nameList.push_back(this->sHeaders[i].name);
    }
    return nameList;
}


std::string Elf32::getSectionContent(std::string needle, bool linearSweep)
{
    std::string temp_string;
    uint32_t index;
    std::stringstream ss;
    std::vector<std::pair<uint64_t,std::string>> container = std::vector<std::pair<uint64_t,std::string>>();
    for(index = 0; index < this->sHeaders.size(); index++)
    {
        if(!this->sHeaders[index].name.compare(needle))
        {
            break;
        }
    }
    ss << "--------------Section Header Info--------------\n";
    /*Section header info*/
    ss << "Section index: " << index << "\n";
    ss << "Section name: " << sHeaders[index].name << "\n";
    ss << "Section name index: " << sHeaders[index].sh_name << "\n";
    switch (sHeaders[index].sh_type)
    {
    case 0:
        temp_string = "SHT_NULL";
        break;
    case 1:
        temp_string = "SHT_PROGBITS";
        break;
    case 2:
        temp_string = "SHT_SYMTAB";
        break;
    case 3:
        temp_string = "SHT_STRTAB";
        break;
    case 4:
        temp_string = "SHT_RELA";
        break;
    case 5:
        temp_string = "SHT_HASH";
        break;
    case 6:
        temp_string = "SHT_DYNAMIC";
        break;
    case 7:
        temp_string = "SHT_NOTE";
        break;
    case 8:
        temp_string = "SHT_NOBITS";
        break;
    case 9:
        temp_string = "SHT_REL";
        break;
    case 10:
        temp_string = "SHT_SHLIB";
        break;
    case 11:
        temp_string = "SHT_DYNSYM";
        break;
    default:
        if(sHeaders[index].sh_type >= 0x70000000 && sHeaders[index].sh_type <= 0x7fffffff )
        {
            temp_string = "Processor specific type";
        }
        else if(sHeaders[index].sh_type >= 0x80000000 && sHeaders[index].sh_type <= 0xffffffff )
        {
            temp_string = "User defined section type";
        }
        else
        {
            temp_string = "Invalid section header type";
        }
        break;
    }
    ss << "Section type: " << temp_string << "\n";

    /*flags*/
    temp_string = "";
    if((sHeaders[index].sh_flags & 0x00000001) != 0)
        temp_string += "SHF_WRITE ";
    if((sHeaders[index].sh_flags & 0x00000002) != 0)
        temp_string += "SHF_ALLOC ";
    if((sHeaders[index].sh_flags & 0x00000004) != 0)
        temp_string += "SHF_EXECINSTR";

    ss << "Section flags: " << temp_string << "\n";


    ss << "Virtual address of section: " << std::hex << std:: showbase << sHeaders[index].sh_addr << "\n";
    ss << "File offset of section: " << std::hex << std:: showbase << sHeaders[index].sh_offset << "\n";
    ss << "Size of section: " << std::hex << std:: showbase << sHeaders[index].sh_size << "\n";
    ss << "Section link information: " << sHeaders[index].sh_link << "\n";
    ss << "Section extra information: " << sHeaders[index].sh_info << "\n";
    ss << "Size of this header: " << sHeaders[index].sh_entsize << "\n";

    ss << "-----------------------------------------------\n";
    /*Section related info*/
    if((sHeaders[index].sh_flags & 0x4) != 0)//if section is executable
    {
        if(e_machine == 3 || e_machine == 0x3e)
        {
            //machine_to_opcode(container, this->sHeaders[index].content,this->sHeaders[index].sh_offset, id.ei_class == 1 ? 1 : 0);
            disassemble_content(container, this->sHeaders[index].content,this->sHeaders[index].sh_offset, id.ei_class == 1 ? 1 : 0,0,linearSweep);
            for(std::pair<uint64_t,std::string> item : container)
            {
                ss << std::hex << std:: showbase << item.first << "\t" << item.second << "\n";
            }
        }
        else
        {
            ss << "Architecture not supported\n";
        }
    }
    else if(sHeaders[index].sh_type == 2 || sHeaders[index].sh_type == 11)/*Symbol table*/
    {
        uint32_t counter = 0;
        if(sHeaders[index].sh_type == 2)
        {
            ss << "Static Symbol Table" << "\n";
        }
        else
        {
            ss << "Dynamic Symbol Table" << "\n";
        }
        for(auto symbol: sHeaders[index].sh_type == 2 ? staticSymbolTable : dynamicSymbolTable)
        {
            ss << "Symbol no: " << counter++ << "\n";
            ss << "\tName: " << symbol.name << "\n";
            ss << "\tIndex of name: " << symbol.st_name << "\n";
            ss << "\tValue: " << symbol.st_value << "\n";
            /*bind*/
            switch (symbol.st_info >> 4)
            {
            case 0:
                temp_string = "STB_LOCAL";
                break;
            case 1:
                temp_string = "STB_GLOBAL";
                break;
            case 2:
                temp_string = "STB_WEAK";
                break;
            case 10:
            case 11:
            case 12:
                temp_string = "Environment specific information";
            case 13:
            case 14:
            case 15:
                temp_string = "Processor specific information";
                break;
            default:
                temp_string = "Invalid bind value";
                break;
            }
            ss << "\tBind: " << temp_string<< "\n";

            /*type*/
            switch (symbol.st_info & 0x0f)
            {
            case 0:
                temp_string = "STT_NOTYPE";
                break;
            case 1:
                temp_string = "STT_OBJECT";
                break;
            case 2:
                temp_string = "STT_FUNC";
                break;
            case 3:
                temp_string = "STT_SECTION";
                break;
            case 4:
                temp_string = "STT_FILE";
                break;
            case 10:
            case 11:
            case 12:
                temp_string = "Environment specific information";
            case 13:
            case 14:
            case 15:
                temp_string = "Processor specific information";
                break;
            default:
                temp_string = "Invalid bind value";
                break;
            }
            ss << "\tType: " << temp_string << "\n";
            ss << "\tOther: " << symbol.st_other << "\n";
            ss << "\tRelated section index: "<< symbol.st_shndx << "\n";
        }
    }
    else if(sHeaders[index].sh_type == 4)/*rela*/
    {
        ss << getRelaInfo();
    }
    else if(sHeaders[index].sh_type == 9)/*rel*/
    {
        ss << getRelInfo();
    }
    else if(sHeaders[index].sh_type == 3)/*strtab*/
    {
        ss << getStringTableInfo(sHeaders[index]);
    }

    return ss.str();
}

std::string Elf32::getStringTableInfo(Elf32SH &section)
{
    std::stringstream ss;
    uint64_t counter;
    ss << "String table\n";
    for(counter = 0L; counter < section.sh_size; counter++)
    {
        if(section.content[counter] == '\0')
        {
            ss << "\n" << std::hex << std::showbase << counter << ": ";
        }
        else
        {
            ss << (char)section.content[counter];
        }
    }

    return ss.str();
}

//returns null if cant find desired name, but that statement should never be executed
std::vector<uint8_t> Elf32::getHexSectionContent(std::string needle)
{
    uint32_t i;
    for(i = 0; i < this->sHeaders.size(); i++)
    {
        if(!this->sHeaders[i].name.compare(needle))
        {
            return this->sHeaders[i].content;
        }
    }
    return std::vector<uint8_t>();
}

uint32_t Elf32::getSection(std::string needle)
{
    uint32_t i;
    for(i = 0; i < this->sHeaders.size(); i++)
    {
        if(!this->sHeaders[i].name.compare(needle))
        {
            return i;
        }
    }
    return 0;
}

uint32_t Elf32::getSectionAddress(std::string needle)
{
	uint32_t i;
    for(i = 0; i < this->sHeaders.size(); i++)
    {
        if(!this->sHeaders[i].name.compare(needle))
        {
            return this->sHeaders[i].sh_addr;
        }
    }
    return 0;
}

void Elf32::readSymbolTable(Elf32SH &section)
{
	/*TODO: test here*/;
	uint8_t buffer[16];
	uint64_t counter;

	if(section.sh_entsize != 16)
	{
		printf("Something seems wrong, trying to read wrong sized symbol table.");
        return;
		/*ERROR*/
	}

	this->fd.seekg(section.sh_offset,std::ios::beg);
	for(counter = 0; counter < section.sh_size; counter += 16)
	{
		struct Elf32_sym symHeader;

		this->fd.read((char *)&buffer,16);
		readLittleEndian(&(symHeader.st_name), buffer, 0);
		readLittleEndian(&(symHeader.st_value), buffer, 4);
		readLittleEndian(&(symHeader.st_size), buffer, 8);
		readLittleEndian(&(symHeader.st_info), buffer, 12);
		readLittleEndian(&(symHeader.st_other), buffer, 13);
		readLittleEndian(&(symHeader.st_shndx), buffer, 14);

		if(section.sh_type == 2)
			this->staticSymbolTable.push_back(symHeader);
		else if(section.sh_type == 11)
			this->dynamicSymbolTable.push_back(symHeader);
	}
}

void Elf32::create_svg(std::string section_name)
{
    Elf32SH &section = this->sHeaders[getSection(section_name)];
    std::map<uint64_t, Block> block_table = get_blocks(section.content, section.sh_addr, 1, 0);

    svg_create(block_table);
}
