#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <alloca.h>

#include "DataLogger.h"

#define ARRAY_LEN(A) (sizeof(A) / sizeof((A)[0]))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

enum Encodings {
  E_ulong,  //  unsigned long
  E_int,    //  int
  E_short,  //  short
  E_ushort, //  unsigned short
  E_uchar,  //  unsigned char
  E_float,  //  float
  E_string, //  string
};

#define TABENT(type, encoding) { #type, encoding }
static const struct {
  const char *name;
  Encodings encoding;
} eventInfo[] = {
  TABENT(absoluteTimestamp,              E_ulong ),
  TABENT(relativeTimestamp,              E_ushort),
  TABENT(typeError,                      E_int   ),
  TABENT(string,                         E_string),
  TABENT(rawPressure,                    E_float ),
  TABENT(rawTemperature,                 E_ulong ),
  TABENT(rawPressureSumCount,            E_uchar ),
  TABENT(baroRawAltitude,                E_float ),
  TABENT(baroAltitude,                   E_float ),
  TABENT(baroAltitudeRate,               E_float ),
  TABENT(kinematicsAngleX,               E_float ),
  TABENT(kinematicsAngleY,               E_float ),
  TABENT(kinematicsAngleZ,               E_float ),
  TABENT(q0,                             E_float ),
  TABENT(q1,                             E_float ),
  TABENT(q2,                             E_float ),
  TABENT(q3,                             E_float ),
  TABENT(meterPerSecSecX,                E_float ),
  TABENT(meterPerSecSecY,                E_float ),
  TABENT(meterPerSecSecZ,                E_float ),
  TABENT(meterPerSecSec,                 E_float ),
  TABENT(altitude,                       E_float ),
  TABENT(altitudeCorrection,             E_float ),
  TABENT(verticalSpeedCorrection,        E_float ),
  TABENT(zVelocity,                      E_float ),
  TABENT(zAcceleration,                  E_float ),
  TABENT(estimatedZVelocity,             E_float ),
  TABENT(rangeFinderRange,               E_float ),
  TABENT(rangeFinderRate,                E_float ),
  TABENT(altitudeHoldThrottleCorrection, E_short ),
  TABENT(altitudeHoldThrottle,           E_short ),
  TABENT(targetVerticalSpeed,            E_float ),
  TABENT(altitudeToHoldTarget,           E_float ),
  TABENT(altitudeHoldMode,               E_uchar ),
  TABENT(receiverCommandThrottle,        E_ushort),
  TABENT(receiverCommandAUX3,            E_ushort),
  TABENT(baroSmoothFactor,               E_float ),
  TABENT(altitudeSpeedPID_P,             E_float ),
  TABENT(altitudeSpeedPID_I,             E_float ),
  TABENT(altitudeSpeedPID_D,             E_float ),
  TABENT(altitudeThrottlePID_P,          E_float ),
  TABENT(altitudeThrottlePID_I,          E_float ),
  TABENT(altitudeThrottlePID_D,          E_float ),
  TABENT(altitudeHoldThrottleSmoothingFactor, E_float ),
  TABENT(altitudeSpeedPID_Pr,            E_float ),
  TABENT(altitudeSpeedPID_Ir,            E_float ),
  TABENT(altitudeSpeedPID_Dr,            E_float ),
  TABENT(altitudeThrottlePID_Pr,         E_float ),
  TABENT(altitudeThrottlePID_Ir,         E_float ),
  TABENT(altitudeThrottlePID_Dr,         E_float ),
  TABENT(accelOneG,                      E_float ),
  TABENT(throttle,                       E_float ),
};
#undef TABENT


DataLogger::DataLogger()
{
  write_enabled = false;
  writepos = 0;
  last_write_timestamp = 0;
  readpos = 0;
  last_read_timestamp = 0;
  full = false;
}


const char *const DataLogger::nameOf(EventType type)
{
  if (type >= ARRAY_LEN(eventInfo))
    return "(invalid)";
  else
    return eventInfo[type].name;
}


void DataLogger::clear(void)
{
  writepos = 0;
  last_write_timestamp = 0;
  readpos = 0;
  last_read_timestamp = 0;
  full = false;
}


bool DataLogger::log(unsigned long timestamp, EventType type, unsigned long value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_ulong != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, int value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  // If not, can we convert it?
  if (E_uchar == eventInfo[type].encoding && value >= 0 && value <= UCHAR_MAX)
    return log(timestamp, type, (unsigned char) value);
  if (E_short == eventInfo[type].encoding && value >= SHRT_MIN && value <= SHRT_MAX)
    return log(timestamp, type, (short) value);
  if (E_ushort == eventInfo[type].encoding && value >= 0 && value <= USHRT_MAX)
    return log(timestamp, type, (unsigned short) value);
  if (E_int != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, short value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_short != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, unsigned short value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_ushort != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, unsigned char value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_uchar != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, float value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_float != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, double value)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_float == eventInfo[type].encoding)
    return log(timestamp, type, (float) value);
  else if (E_float != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  return _mark_time(timestamp) && _put(type, &value, sizeof(value));
}


bool DataLogger::log(unsigned long timestamp, EventType type, const char *s)
{
  //  Is event type sane?
  if (ARRAY_LEN(eventInfo) <= type) {
    _log_error(type);
    return false;
  }

  // Does the type of value match what we're expecting to store?
  if (E_string != eventInfo[type].encoding) {
    _log_error(type);
    return false;
  }

  //  Store it.
  unsigned short len = MIN(strlen(s), USHRT_MAX);
  char *buf = (char *) alloca(sizeof(unsigned short) + len);
  memcpy(buf, &len, sizeof(len));
  memcpy(buf + sizeof(len), s, len);
  return _mark_time(timestamp) && _put(type, buf, sizeof(unsigned short) + len);
}


bool DataLogger::_mark_time(unsigned long timestamp)
{
  bool success = true;
  if (timestamp != last_write_timestamp) {
    unsigned long delta = timestamp - last_write_timestamp;
    if (timestamp > last_write_timestamp && delta <= 0xffffUL) {
      unsigned short short_delta = delta;
      success = _put(::DataLogger::relativeTimestamp, &short_delta, sizeof(short_delta));
    }
    else {
      success = _put(::DataLogger::absoluteTimestamp, &timestamp, sizeof(timestamp));
    }
    if (success)
      last_write_timestamp = timestamp;
  }
  return success;
}


bool DataLogger::_put(EventType type, const void *buffer, int size)
{
  if (!write_enabled)
    return false;
  if (full)
    return false;
  if (writepos + size + 1 >= sizeof(data)) {
    full = true;
    return false;
  }
  data[writepos++] = type;
  memcpy(&(data[writepos]), buffer, size);
  writepos += size;
  return true;
}


bool DataLogger::_log_error(int i)
{
    return _put(::DataLogger::typeError, &i, sizeof(i));
}


bool DataLogger::read_next(char *buffer, int size)
{
  if (readpos < writepos) {

    int len = -1;
    if (DataLogger::absoluteTimestamp == data[readpos]) {
      ++readpos;
      memcpy(&last_read_timestamp, &data[readpos], sizeof(last_read_timestamp));
      readpos += sizeof(last_read_timestamp);
      len = snprintf(buffer, size, "%s, %.6f", "time", last_read_timestamp / 1000000.0);
      return len > -1 && len < size;
    }
    else if (DataLogger::relativeTimestamp == data[readpos]) {
      ++readpos;
      unsigned short short_delta;
      memcpy(&short_delta, &data[readpos], sizeof(short_delta));
      readpos += sizeof(short_delta);
      last_read_timestamp += short_delta;
      len = snprintf(buffer, size, "%s, %.6f", "time", last_read_timestamp / 1000000.0);
      return len > -1 && len < size;
    }
    else {
      EventType type = (EventType) data[readpos++];
      void *ptr = &(data[readpos]);

      switch (eventInfo[type].encoding) {
        case E_ulong:
          len = snprintf(buffer, size, "%s, %lu", nameOf(type), *(unsigned long *)ptr);
          readpos += sizeof(unsigned long);
          break;
        case E_int:
          len = snprintf(buffer, size, "%s, %d", nameOf(type), *(int *)ptr);
          readpos += sizeof(unsigned long);
          break;
        case E_short:
          len = snprintf(buffer, size, "%s, %d", nameOf(type), *(short *)ptr);
          readpos += sizeof(short);
          break;
        case E_ushort:
          len = snprintf(buffer, size, "%s, %u", nameOf(type), *(unsigned short *)ptr);
          readpos += sizeof(short);
          break;
        case E_uchar:
          len = snprintf(buffer, size, "%s, %d", nameOf(type), *(unsigned char*)ptr);
          readpos += sizeof(unsigned char);
          break;
        case E_float:
          len = snprintf(buffer, size, "%s, %f", nameOf(type), *(float *)ptr);
          readpos += sizeof(float);
          break;
        case E_string:
          int slen = *(unsigned short *) ptr;
          const char *s =  ((const char *)ptr + sizeof(unsigned short));
          len = snprintf(buffer, size, "%s, %*.*s", nameOf(type), slen, slen, s);
          readpos += sizeof(unsigned short) + slen;
          break;
      }
      return len > -1 && len < size;
    }
  }
  return false;
}

#if 0
int main(void)
{
  DataLogger logger;
  logger.write_enable();
  logger.log(5000000, DataLogger::string, "Hi, there!");
  logger.log(6000000, DataLogger::rangeFinderRange, (float) 13.0);

  char buffer[512];
  while (logger.read_next(buffer, sizeof(buffer)))
    fprintf(stdout, "%s\n", buffer);
  return 0;
}
#endif
