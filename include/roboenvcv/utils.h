#ifndef _ROBOENVCV_UTILS_
#define _ROBOENVCV_UTILS_

#include <cmath>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <chrono>

#include "roboenvcv/types.h"

namespace roboenvcv
{
  /// @brief Creates log directory for roboenvcv functions.
  /// @return Name of created directory.
  std::string createLogDir()
  {
    auto date =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto localdate = std::localtime(&date);
    std::string dbgfolder =
      "Roboenvcv_" + std::to_string(++localdate->tm_mon) + "mon"
      + std::to_string(localdate->tm_mday) + "d"
      + std::to_string(localdate->tm_hour) + "h"
      + std::to_string(localdate->tm_min) + "m"
      + std::to_string(localdate->tm_sec) + "s";
    const int err = mkdir(dbgfolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (err == -1) {
      printf("failed to create log directory aborting!");
      std::abort();
    }
    dbgfolder += "/"; // requires external slash
    return dbgfolder;
  }

  /// @brief Filters list of objects with given indices.
  /// @param[in] _objects List of objects to filter.
  /// @param[in] _indices The indices of objects to retrieve.
  /// @return List of objects from applied indices.
  std::vector<objectarea> filter
    (std::vector<objectarea> _objects, std::vector<int> _indices)
  {
    std::vector<objectarea> result;
    for (auto it = _indices.begin(); it != _indices.end(); ++it)
      if (*it >= 0 && *it < _objects.size())
        result.push_back(_objects.at(*it));
    return result;
  }

  /// @brief Calculates distance of object to environment plane.
  /// @param[in] _obj Target object.
  /// @param[in] _env Target environment (should be plane).
  ///    Make sure _env and _obj has values in the same coordinate.
  /// @return Distance of object to environment plane.
  float getDistance
    (objectarea _obj, objectarea _env)
  {
    Eigen::Vector3f w = _obj.center3d - _env.center3d;
    return fabs(_env.normal3d.dot(w));
  }
}

#endif