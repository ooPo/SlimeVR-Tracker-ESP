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

#include "SensorManager.h"
#include <i2cscan.h>
#include "network/network.h"
#include "bno055sensor.h"
#include "bno080sensor.h"
#include "mpu9250sensor.h"
#include "mpu6050sensor.h"
#include "bmi160sensor.h"
#include "icm20948sensor.h"
#include "ErroneousSensor.h"
#include "sensoraddresses.h"

namespace SlimeVR
{
    namespace Sensors
    {
        void SensorManager::setup()
        {
            for (int loop = 0; loop < MAX_SENSOR_COUNT; loop++)
            {
                if (m_Sensors[loop]) {
                    delete m_Sensors[loop];
                    m_Sensors[loop] = NULL;
                }

                if (I2CSCAN::isI2CExist(m_SensorsConfig[loop].address) || m_SensorsConfig[loop].type == IMU_EMPTY)
                {
                    switch(m_SensorsConfig[loop].type)
                    {
                        case IMU_BNO080:
                        case IMU_BNO085:
                        case IMU_BNO086:
                            m_Sensors[loop] = new BNO080Sensor(loop,
                                m_SensorsConfig[loop].type,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation,
                                m_SensorsConfig[loop].intPin);
                            break;

                        case IMU_BNO055:
                            m_Sensors[loop] = new BNO055Sensor(loop,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation);
                            break;

                        case IMU_MPU9250:
                            m_Sensors[loop] = new MPU9250Sensor(loop,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation);
                            break;

                        case IMU_BMI160:
                            m_Sensors[loop] = new BMI160Sensor(loop,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation);
                            break;

                        case IMU_MPU6500:
                        case IMU_MPU6050:
                            m_Sensors[loop] = new MPU6050Sensor(loop,
                                m_SensorsConfig[loop].type,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation);
                            break;

                        case IMU_ICM20948:
                            m_Sensors[loop] = new ICM20948Sensor(loop,
                                m_SensorsConfig[loop].address,
                                m_SensorsConfig[loop].rotation);
                            break;

                        case IMU_EMPTY:
                            m_Sensors[loop] = new EmptySensor(loop);
                            break;
                    }

                    if (m_Sensors[loop]) {
                        m_Sensors[loop]->motionSetup();
                    }
                }
            }
        }

        void SensorManager::postSetup()
        {
            for (int loop = 0; loop < MAX_SENSOR_COUNT; loop++)
            {
                if (m_Sensors[loop])
                {
                    m_Sensors[loop]->postSetup();
                }
            }
        }

        void SensorManager::update()
        {
            // Gather IMU data
            for (int loop = 0; loop < MAX_SENSOR_COUNT; loop++)
            {
                if (m_Sensors[loop])
                {
                    m_Sensors[loop]->motionLoop();
                }
            }

            if (!ServerConnection::isConnected())
            {
                return;
            }

            // Send updates
            for (int loop = 0; loop < MAX_SENSOR_COUNT; loop++)
            {
                if (m_Sensors[loop])
                {
                    m_Sensors[loop]->sendData();
                }
            }
        }
    }
}
