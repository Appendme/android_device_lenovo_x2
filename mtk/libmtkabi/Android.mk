# Copyright (C) 2015 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtk_parcel.cpp \
    mtk_string.cpp \
    mtkcam_cpuctrl.cpp \
    mtk_atci.cpp \
    mtk_gui.cpp

# mtk_ui.cpp \
#mtk_omx.cpp \
#mtk_audio.cpp
    
LOCAL_SHARED_LIBRARIES := libbinder libutils libgui
LOCAL_MODULE := libmtkabi
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
