/**
 * regtable
 *
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Giacomo Genovese
 * Creation date: 25/03/2015
 */

#include "product.h"
#include "regtable.h"
#include "EEPROM.h"
#include "HardwareSerial.h"
/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()


/*
 * Definition of custom registers
 */
// Register updated depending on Maxsonar value
byte dtBinInputs[1];    // 8 Binary input states
REGISTER regBinInputs(dtBinInputs, sizeof(dtBinInputs), &updtBinInputs, NULL);

//Output Register (NOT USED)
byte dtBinOutputs[1];       // 8 Binary output states
REGISTER regBinOutputs(dtBinOutputs, sizeof(dtBinOutputs), NULL, &setBinOutputs);
//Register related to Mote Coordinates
byte dtCoordinates[8];
REGISTER regCoordinates(dtCoordinates, sizeof(dtCoordinates), &setCoordinates, NULL);


// Initialize table of registers
 
DECLARE_REGISTERS_START()
  &regBinInputs,
  &regBinOutputs,
  &regCoordinates
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */
 
 // updtBinInputs
  //Read binary inputs
 //'rId'  Register ID
const void updtBinInputs(byte rId)
{
  // Update register
  dtBinInputs[0] = stateLowByte;
  //Serial.println(dtBinInputs[0]);
}

//Not used
const void setBinOutputs(byte rId, byte *states)
{
  byte i;
  //Serial.println("Update RegisterOUT");
  // Update register
  // memcpy(dest,source,sizeToCopy), copy "states" on "dtBinOutputs"
  memcpy(dtBinOutputs, states, sizeof(dtBinOutputs));

  // Control pins
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    digitalWrite(binaryPin[i], (dtBinOutputs[0] >> i) & 0x01);
}
//Write Coordinates on Register in byte
const void setCoordinates(byte rId){
  dtCoordinates[0]=coordinates0;
  dtCoordinates[1]=coordinates1;
  dtCoordinates[2]=coordinates2;
  dtCoordinates[3]=coordinates3;
  dtCoordinates[4]=coordinates4;
  dtCoordinates[5]=coordinates5;
  dtCoordinates[6]=coordinates6;
  dtCoordinates[7]=coordinates7;
}
