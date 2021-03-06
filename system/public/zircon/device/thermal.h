// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <zircon/device/ioctl.h>
#include <zircon/device/ioctl-wrapper.h>

// temperature units are in 10th of a degree kelvin

typedef struct {
    // state is a bitmask
    uint32_t state;
    // trip points for below states are defined below
#define THERMAL_STATE_NORMAL         0
#define THERMAL_STATE_TRIP_VIOLATION 1

    // the sensor temperature at which the system should activate
    // passive cooling policy
    uint32_t passive_temp;

    // the sensor temperature at which the system should perform
    // critical shutdown
    uint32_t critical_temp;

    // number of trip points supported
    uint32_t max_trip_count;

    // the currently active trip point
    uint32_t active_trip[9];
} thermal_info_t;

typedef struct {
    uint32_t id;
    uint32_t up_temp;
    uint32_t down_temp;
} thermal_temperature_info_t;

typedef struct {
    // active cooling support
    bool active_cooling;

    // passive cooling support
    bool passive_cooling;

    // number of levels supported by active cooling device
    uint32_t num_fan_level;

    // number of trip points
    uint32_t num_trip_points;

#define MAX_TRIP_POINTS             9
    thermal_temperature_info_t trip_point_info[MAX_TRIP_POINTS];
} thermal_device_info_t;

typedef struct {
    uint32_t id;
    uint32_t temp;
} trip_point_t;

// Get thermal info
#define IOCTL_THERMAL_GET_INFO \
    IOCTL(IOCTL_KIND_DEFAULT, IOCTL_FAMILY_THERMAL, 1)

// Sets a trip point. When the sensor reaches the trip point temperature
// the device will notify on an event.
#define IOCTL_THERMAL_SET_TRIP \
    IOCTL(IOCTL_KIND_DEFAULT, IOCTL_FAMILY_THERMAL, 2)

// Get an event to get trip point notifications on. ZX_USER_SIGNAL_0 is changed
// when either trip point is reached. It is deasserted when the state is read
// via IOCTL_THERMAL_GET_INFO.
#define IOCTL_THERMAL_GET_STATE_CHANGE_EVENT \
    IOCTL(IOCTL_KIND_GET_HANDLE, IOCTL_FAMILY_THERMAL, 3)

// Get a port to get trip point notification packets.
#define IOCTL_THERMAL_GET_STATE_CHANGE_PORT \
    IOCTL(IOCTL_KIND_GET_HANDLE, IOCTL_FAMILY_THERMAL, 4)

#define IOCTL_THERMAL_GET_DEVICE_INFO \
    IOCTL(IOCTL_KIND_DEFAULT, IOCTL_FAMILY_THERMAL, 5)

#define IOCTL_THERMAL_SET_FAN_LEVEL \
    IOCTL(IOCTL_KIND_DEFAULT, IOCTL_FAMILY_THERMAL, 6)


// ssize_t ioctl_thermal_get_info(int fd, thermal_info_t* out)
IOCTL_WRAPPER_OUT(ioctl_thermal_get_info, IOCTL_THERMAL_GET_INFO, thermal_info_t);

// ssize_t ioctl_thermal_set_trip(int fd, uint32_t temp)
IOCTL_WRAPPER_IN(ioctl_thermal_set_trip, IOCTL_THERMAL_SET_TRIP, trip_point_t);

// ssize_t ioctl_thermal_get_state_change_event(int fd, zx_handle_t* out)
IOCTL_WRAPPER_OUT(ioctl_thermal_get_state_change_event,
                  IOCTL_THERMAL_GET_STATE_CHANGE_EVENT,
                  zx_handle_t);

// ssize_t ioctl_thermal_get_state_change_port(int fd, zx_handle_t* out)
IOCTL_WRAPPER_OUT(ioctl_thermal_get_state_change_port,
                  IOCTL_THERMAL_GET_STATE_CHANGE_PORT,
                  zx_handle_t);

// ssize_t ioctl_thermal_get_device_info(int fd, thermal_info_t* out)
IOCTL_WRAPPER_OUT(ioctl_thermal_get_device_info,
                 IOCTL_THERMAL_GET_DEVICE_INFO, thermal_device_info_t);

// ssize_t ioctl_thermal_set_fan_level(int fd, uint32_t temp)
IOCTL_WRAPPER_IN(ioctl_thermal_set_fan_level, IOCTL_THERMAL_SET_FAN_LEVEL, uint32_t);
