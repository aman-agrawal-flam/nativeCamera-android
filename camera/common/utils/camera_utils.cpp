/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "camera_utils.h"

#include <camera/NdkCameraManager.h>
#include <media/NdkImage.h>

#include <cinttypes>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "utils/native_debug.h"

#define UKNOWN_TAG "UNKNOW_TAG"
#define MAKE_PAIR(val) std::make_pair(val, #val)
template <typename T>
const char* GetPairStr(T key, std::vector<std::pair<T, const char*>>& store) {
  typedef typename std::vector<std::pair<T, const char*>>::iterator iterator;
  for (iterator it = store.begin(); it != store.end(); ++it) {
    if (it->first == key) {
      return it->second;
    }
  }
  LOGW("(%#08x) : UNKNOWN_TAG for %s", key, typeid(store[0].first).name());
  return UKNOWN_TAG;
}

/*
 * camera_status_t error translation
 */
using ERROR_PAIR = std::pair<camera_status_t, const char*>;
static std::vector<ERROR_PAIR> errorInfo{
    MAKE_PAIR(ACAMERA_OK),
    MAKE_PAIR(ACAMERA_ERROR_UNKNOWN),
    MAKE_PAIR(ACAMERA_ERROR_INVALID_PARAMETER),
    MAKE_PAIR(ACAMERA_ERROR_CAMERA_DISCONNECTED),
    MAKE_PAIR(ACAMERA_ERROR_NOT_ENOUGH_MEMORY),
    MAKE_PAIR(ACAMERA_ERROR_METADATA_NOT_FOUND),
    MAKE_PAIR(ACAMERA_ERROR_CAMERA_DEVICE),
    MAKE_PAIR(ACAMERA_ERROR_CAMERA_SERVICE),
    MAKE_PAIR(ACAMERA_ERROR_SESSION_CLOSED),
    MAKE_PAIR(ACAMERA_ERROR_INVALID_OPERATION),
    MAKE_PAIR(ACAMERA_ERROR_STREAM_CONFIGURE_FAIL),
    MAKE_PAIR(ACAMERA_ERROR_CAMERA_IN_USE),
    MAKE_PAIR(ACAMERA_ERROR_MAX_CAMERA_IN_USE),
    MAKE_PAIR(ACAMERA_ERROR_CAMERA_DISABLED),
    MAKE_PAIR(ACAMERA_ERROR_PERMISSION_DENIED),
};
const char* GetErrorStr(camera_status_t err) {
  return GetPairStr<camera_status_t>(err, errorInfo);
}


/*
 * CameraDevice error state translation, used in
 *     ACameraDevice_ErrorStateCallback
 */
using DEV_ERROR_PAIR = std::pair<int, const char*>;
static std::vector<DEV_ERROR_PAIR> devErrors{
    MAKE_PAIR(ERROR_CAMERA_IN_USE),   MAKE_PAIR(ERROR_MAX_CAMERAS_IN_USE),
    MAKE_PAIR(ERROR_CAMERA_DISABLED), MAKE_PAIR(ERROR_CAMERA_DEVICE),
    MAKE_PAIR(ERROR_CAMERA_SERVICE),
};

const char* GetCameraDeviceErrorStr(int err) {
  return GetPairStr<int>(err, devErrors);
}
void PrintCameraDeviceError(int err) {
  LOGI("CameraDeviceError(%#x): %s", err, GetCameraDeviceErrorStr(err));
}
