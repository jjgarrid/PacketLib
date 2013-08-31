/***************************************************************************
                          Field.cpp  -  description
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


#include "Field.h"
#include "Utility.h"

using namespace PacketLib;

static FieldType** filedTypeList = 0;


Field::Field(char* n, char* dim, char* prVal, int count) : progressiv(count)
{
    value = 0;
    /// create FieldType list
    if(filedTypeList == 0)
    {
        //cout << "create list" << endl;
        dword nb = CONFIG_MAXNUMBEROFLINES_OFCONFIGILES/3;
        filedTypeList = (FieldType**) new FieldType* [nb];
        for(dword i = 0; i<nb; i++)
            filedTypeList[i] = 0;
    }
    //cout << "------" << endl;
    int i = 0;
    type = 0;
    while(filedTypeList[i] != 0)
    {
        //cout << filedTypeList[i]->name << " " << n << endl;
        if(filedTypeList[i]->name == n)
        {
            type = filedTypeList[i];
            break;
        }
        else
            i++;
    }
    //cout << type << endl;
    if(type == 0)
    {
        type = new FieldType;
        filedTypeList[i] = type;

        type->name = n;

        type->dimension = atoi(dim);
        if(strcmp(prVal, "none") != 0)
        {
            type->thereIsPredefinedValue = true;
            type->predefinedValue = Utility::convertToInteger(prVal);
        }
        else
        {
            type->predefinedValue = 0;
            type->thereIsPredefinedValue = false;
        }
    }
}


Field::~Field()
{

}