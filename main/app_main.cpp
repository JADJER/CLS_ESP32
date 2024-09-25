//
// Created by jadjer on 8/9/24.
//

#include "bluetooth/Bluetooth.hpp"
#include "configuration/Configuration.hpp"
#include "controller/Controller.hpp"

extern "C" void app_main() {
  auto configuration = std::make_shared<Configuration>();

  Bluetooth bluetooth(configuration);
  bluetooth.advertise();

  Controller controller(configuration);
  controller.spin();
}
