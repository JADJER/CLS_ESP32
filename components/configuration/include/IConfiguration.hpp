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
// Created by jadjer on 10.02.23.
//


#pragma once

#include <memory>

class IConfiguration {
public:
    virtual ~IConfiguration() = default;

public:
    [[nodiscard]] virtual bool isLubricateFromTimer() const = 0;
    [[nodiscard]] virtual bool isLubricateFromDistance() const = 0;

public:
    [[nodiscard]] virtual uint64_t getLimitDistance() const = 0;
};

using IConfigurationPtr = std::shared_ptr<IConfiguration>;
