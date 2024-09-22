//
// Created by jadjer on 8/9/24.
//

#include "Controller.hpp"
#include "configuration/Configuration.hpp"

extern "C" void app_main() {

  auto configuration = std::make_shared<Configuration>();

  Controller controller(configuration);
  controller.spin();
}
