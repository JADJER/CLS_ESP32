// Copyright 2023 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 14.02.23.
//


#pragma once

#include <string>

/**
 * @class Bluetooth
 */
class Bluetooth {
public:
    /**
     * Default constructor
     */
    Bluetooth();
    /**
     * Default destructor
     */
    ~Bluetooth();

public:
    void spinOnce();

private:
    /**
     *
     * @param addr
     */
    static void printAddr(const uint8_t *addr);
    /**
     *
     */
    static void blehrOnSync();
    /**
     *
     * @param reason
     */
    static void blehrOnReset(int reason);
    /**
     * Enables advertising with parameters:
     *     o General discoverable mode
     *     o Undirected connectable mode
     */
    static void blehrAdvertise();
    /**
     *
     */
    static void blehrTxHrateStop();
    /**
     * Reset heart rate measurement
     */
    static void blehrTxHrateReset();
    /**
     * This function simulates heart beat and notifies it to the client
     * @param ev
     */
    static void blehrTxHrate();
    /**
     *
     * @param event
     * @param arg
     */
    static int blehrGapEvent(struct ble_gap_event *event, void *arg);
    static void blehrHostTask(void *param);
};
