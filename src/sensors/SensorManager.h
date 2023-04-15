/*
    SlimeVR Code is placed under the MIT license
    Copyright (c) 2022 TheDevMinerTV

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef SLIMEVR_SENSORMANAGER
#define SLIMEVR_SENSORMANAGER

#include "globals.h"
#include "sensor.h"
#include "EmptySensor.h"
#include "logging/Logger.h"

#define MAX_SENSOR_COUNT 10

namespace SlimeVR
{
    namespace Sensors
    {
        class SensorManager
        {

        public:

            SensorManager() : m_Logger(SlimeVR::Logging::Logger("SensorManager")), m_Sensors{NULL}
            {
                // Do nothing.
            }

            ~SensorManager()
            {
                for (int loop = 0; loop < MAX_SENSOR_COUNT; loop++)
                {
                    if (m_Sensors[loop])
                    {
                        delete m_Sensors[loop];
                        m_Sensors[loop] = NULL;
                    }
                }
            }

            void setup();
            void postSetup();

            void update();

            Sensor **getSensors()
            {
                return m_Sensors;
            }

            Sensor *getSensor(int index)
            {
                if (index >= MAX_SENSOR_COUNT) { return NULL; }
                return m_Sensors[index];
            }

        private:

            SlimeVR::Logging::Logger m_Logger = NULL;
            Sensor *m_Sensors[MAX_SENSOR_COUNT] = {NULL};

            struct {
                uint8_t type;
                uint8_t address;
                float rotation;
                uint8_t intPin;
            }
            m_SensorsConfig[MAX_SENSOR_COUNT] =
            {
                { IMU,        0, IMU_ROTATION,        PIN_IMU_INT   },
                { SECOND_IMU, 0, SECOND_IMU_ROTATION, PIN_IMU_INT_2 },
            };

        };
    }
}

#endif // SLIMEVR_SENSORFACTORY_H_
