/*
  AeroQuad v3.0 - Nov 2011
 www.AeroQuad.com
 Copyright (c) 2011 Ted Carancho.  All rights reserved.
 An Open Source Arduino based multicopter.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _AQ_OSD_MAX7456_ALTITUDE_H_
#define _AQ_OSD_MAX7456_ALTITUDE_H_

//////////////////////////////////////////////////////////////////////////////
/////////////////////////// AltitudeHold Display /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int lastAltitude     = 12345;     // bogus initial values to force update
int lastHoldAltitude = 12345;
AltitudeHoldMode_t lastHoldState   = ALT_OFF;

void displayAltitude(float readedAltitude, float desiredAltitudeToKeep, AltitudeHoldMode_t altHoldState) {
  #ifdef USUnits
    int currentAltitude = readedAltitude*3.281;
    int currentHoldAltitude = desiredAltitudeToKeep*3.281;
  #else // metric
    int currentAltitude = readedAltitude*10.0; // 0.1m accuracy!!
    int currentHoldAltitude = desiredAltitudeToKeep*10.0;
  #endif
  char buf[7];

  if ( lastAltitude != currentAltitude ) {
    #ifdef USUnits
      snprintf(buf, sizeof(buf), "\011%4df", currentAltitude);
    #else
      if (abs(currentAltitude)<100) {
        // snprintf(buf, sizeof(buf), "\011%c%1d.%1dm", currentAltitude < 0 ? '-' : ' ', abs(currentAltitude/10),abs(currentAltitude%10));
        snprintf(buf, sizeof(buf), "\011% 5.1fm", readedAltitude);
      }
      else {
        snprintf(buf, sizeof(buf), "\011%4.0fm", readedAltitude);
      }
    #endif
    writeChars(buf, 6, 0, ALTITUDE_ROW, ALTITUDE_COL);
    lastAltitude = currentAltitude;
  }

  // AltitudeHold handling:
  // - show hold altitude when it is active
  // - show "panic" if 'paniced' out
  boolean isWriteNeeded = false;
  switch (altHoldState) {
  case ALT_OFF:
  case ALT_SONAR:
    if (lastHoldState != altHoldState) {
      memset(buf, 0, sizeof(buf));
      isWriteNeeded = true;
    }
    break;
  case ALT_BARO:
    if ((lastHoldState != altHoldState) || (lastHoldAltitude != currentHoldAltitude)) {
      lastHoldState = altHoldState;
      #ifdef USUnits
        snprintf(buf, sizeof(buf), "\012%4df", currentHoldAltitude);
      #else
        if (abs(currentHoldAltitude)<100) {
          snprintf(buf, sizeof(buf), "\012% 5.1fm", readedAltitude);
        }
        else {
          snprintf(buf, sizeof(buf), "\012%4.0fm", readedAltitude);
        }
      #endif
      isWriteNeeded = true;
    }
    break;
  case ALT_PANIC:
    if (lastHoldState != altHoldState) {
      snprintf(buf, sizeof(buf), "\012panic");
      isWriteNeeded = true;
    }
    break;
  }
  lastHoldState = altHoldState;

  if (isWriteNeeded) {
    writeChars(buf, 6, 0, ALTITUDE_ROW, ALTITUDE_COL+6 );
  }
}

#endif  // #define _AQ_OSD_MAX7456_ALTITUDE_H_
