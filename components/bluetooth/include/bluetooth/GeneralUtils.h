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
// Created by jadjer on 16.02.23.
//

#pragma once

#include <algorithm>
#include <esp_err.h>
#include <string>
#include <vector>

/**
 * @brief General utilities.
 */
class GeneralUtils {
   public:
    static bool base64Decode(const std::string& in, std::string* out);
    static bool base64Encode(const std::string& in, std::string* out);
    static void dumpInfo();
    static bool endsWith(std::string str, char c);
    static const char* errorToString(esp_err_t errCode);
    static const char* wifiErrorToString(uint8_t value);
    static void hexDump(const uint8_t* pData, uint32_t length);
    static std::string ipToString(uint8_t* ip);
    static std::vector<std::string> split(std::string source, char delimiter);
    static std::string toLower(std::string& value);
    static std::string trim(const std::string& str);
};
