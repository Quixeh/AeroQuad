#ifndef _AQ_DATA_LOGGER_H_
#define _AQ_DATA_LOGGER_H_

class DataLogger {
  public:
    enum EventType {
      absoluteTimestamp, //  For internal logger use.
      relativeTimestamp, //  For internal logger use.
      typeError,         //  For internal logger use.
      string,
      rawPressure,
      rawTemperature,
      rawPressureSumCount,
      baroRawAltitude,
      baroAltitude,
      baroAltitudeRate,
      kinematicsAngleX,
      kinematicsAngleY,
      kinematicsAngleZ,
      q0,
      q1,
      q2,
      q3,
      meterPerSecSecX,
      meterPerSecSecY,
      meterPerSecSecZ,
      altitude,
      altitudeCorrection,
      zVelocity,
      zAcceleration,
      estimatedZVelocity,
      rangeFinderRange,
      rangeFinderRate,
      altitudeHoldThrottleCorrection,
      altitudeHoldThrottle,
      targetVerticalSpeed,
      altitudeToHoldTarget,
      altitudeHoldMode,
      receiverCommandThrottle,
      receiverCommandAUX3,
      baroSmoothFactor,
      altitudeSpeedPID_P,
      altitudeSpeedPID_I,
      altitudeSpeedPID_D,
      altitudeThrottlePID_P,
      altitudeThrottlePID_I,
      altitudeThrottlePID_D,
      altitudeHoldThrottleSmoothingFactor,
      altitudeSpeedPID_Pr,
      altitudeSpeedPID_Ir,
      altitudeSpeedPID_Dr,
      altitudeThrottlePID_Pr,
      altitudeThrottlePID_Ir,
      altitudeThrottlePID_Dr,
      accelOneG,
    };

    DataLogger();
    void write_enable() { write_enabled = true; }
    bool is_write_enabled() const { return write_enabled; }
    static const char *const nameOf(EventType type);

    void clear(void);
    bool log(unsigned long timestamp, EventType type, unsigned long value);
    bool log(unsigned long timestamp, EventType type, int value);
    bool log(unsigned long timestamp, EventType type, short value);
    bool log(unsigned long timestamp, EventType type, unsigned short value);
    bool log(unsigned long timestamp, EventType type, unsigned char value);
    bool log(unsigned long timestamp, EventType type, float value);
    bool log(unsigned long timestamp, EventType type, const char *s);

    bool read_next(char *buffer, int size);

  private:
    //  Data store
    unsigned char data[1024 * 64];
    unsigned int writepos;

    //  Writer state
    bool write_enabled;
    bool full;
    unsigned long last_write_timestamp;

    //  Reader state
    unsigned int readpos;
    unsigned long last_read_timestamp;

    bool _mark_time(unsigned long timestamp);
    bool _put(unsigned long timestamp, EventType type, char dtype, void *ptr, int nbytes);
    bool _log_error(int i);
    bool _put(EventType type, const void *buffer, int size);
};

#endif // _AQ_DATA_LOGGER_H_
