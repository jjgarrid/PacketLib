/***************************************************************************
                          PacketHeader.cpp  -  description
                             -------------------
    begin                : Thu Nov 29 2001
    copyright            : (C) 2001, 2013 by Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/
 
#include "PacketHeader.h"
#include "ConfigurationFile.h"

using namespace PacketLib;



PacketHeader::PacketHeader() : PartOfPacket("Packet Header")
{
    name = 0;
    dimensionOfPacketLenght = 16;
}


PacketHeader::~PacketHeader()
{
    delete[] name;
}



bool PacketHeader::loadHeader(char* fileName) throw(PacketException*)
{
    ConfigurationFile header;
    char **argv = new char* [1];
    argv[0] = (char*)fileName;
    if(header.open(argv))
    {
        delete[] argv;
        char* line = header.getLine();
        /// retrieve name of packet header
        name = line;

        line = header.getLine();
        numberOfFieldWithPacketDimension = atoi(line);
        /// delete[] line;

        line = header.getLine();
        if(strcmp(line, "[Field]") == 0)
            dimensionOfPacketLenght = 16;
        else
            dimensionOfPacketLenght = atoi(line);

        if(!(dimensionOfPacketLenght == 16 || dimensionOfPacketLenght == 32))
            throw new PacketExceptionIO("Dimension of packet length, 16 or 32 bit");;

        header.setpos(0);

        /// find the start position of the fields
        line = header.getLine("[Field]");
        /// delete[] line;

        if(loadFields(header))
        {
            header.close();
            return true;
        }
        else
            throw new PacketExceptionIO("Can't create the header.");

    }
    else
    {
        delete[] argv;
        throw new PacketExceptionIO("Can't open the header file configuration.");
    }
}



dword PacketHeader::getPacketLength()
{
    if(dimensionOfPacketLenght == 16)
    {
        Field* f = getFields(numberOfFieldWithPacketDimension);
        /// ESA standard: in the packet length the value 1 must be added
        return f->value + 1;
    }
    else
    {
        return getFieldValue_4_14(numberOfFieldWithPacketDimension) + 1;
    }
}

void PacketHeader::setPacketLength(dword dim)
{
    if(dimensionOfPacketLenght == 16)
    {
        Field* f = getFields(numberOfFieldWithPacketDimension);
        /// ESA standard: in the packet length the value 1 must be added
        f->value = (word) dim-1;
    }
    else
    {
        setFieldValue_4_14(numberOfFieldWithPacketDimension, dim-1);
    }
}


Field * PacketHeader::getFieldWithPacketDimension()
{
    return getFields(numberOfFieldWithPacketDimension);
}