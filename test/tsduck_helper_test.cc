// SPDX-License-Identifier: GPL-2.0-or-later

// mirakc-arib
// Copyright (C) 2019 masnagam
//
// This program is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
// the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program; if
// not, write to the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tsduck_helper.hh"

TEST(TsduckHelperTest, ConvertUnixTimeToJstTime) {
  ts::Time unix_epoch_jst = ts::Time::UnixEpoch + kJstTzOffset;
  EXPECT_EQ(unix_epoch_jst, ConvertUnixTimeToJstTime(0));
}

TEST(TsduckHelperTest, ConvertJstTimeToUnixTime) {
  ts::Time unix_epoch_jst = ts::Time::UnixEpoch + kJstTzOffset;
  EXPECT_EQ(0, ConvertJstTimeToUnixTime(unix_epoch_jst));
}

TEST(TsduckHelperTest, MakeEventsJsonValue) {
  // clang-format off
  static const uint8_t kData[] = {
    // event_id
    0x00, 0x01,
    // start_time, duration
    0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56,
    // running_status, free_CA_mode, descriptors_loop_length
    0x00, 0x00,
  };
  // clang-format on
  EitSection eit;
  eit.events_data = kData;
  eit.events_size = sizeof(kData);
  rapidjson::Document doc(rapidjson::kObjectType);
  auto& allocator = doc.GetAllocator();
  auto events = MakeEventsJsonValue(eit, allocator);
  EXPECT_EQ(1, events.Size());
  EXPECT_EQ(1, events[0]["eventId"]);
#if defined(_WIN32)
  // The epoch of ts::Time is 1601-01-01 on Windows.  So, MJD 0 (1858-11-17) can be represented.
  EXPECT_EQ(-40587LL * 86400000 - 9 * 3600000, events[0]["startTime"].GetInt64());
#else
  // MJD 0 (1858-11-17) is before the epoch of ts::Time (1970-01-01) and it's clamped to the epoch.
  EXPECT_EQ(-9 * 3600000, events[0]["startTime"]);
#endif
  EXPECT_EQ(12 * 3600000 + 34 * 60000 + 56000, events[0]["duration"]);
  EXPECT_TRUE(events[0]["scrambled"].IsFalse());
  EXPECT_EQ(0, events[0]["descriptors"].Size());
}

TEST(TsduckHelperTest, MakeEventsJsonValue_UndefinedStartTimeAndDuration) {
  // clang-format off
  static const uint8_t kData[] = {
    // event_id
    0x00, 0x01,
    // start_time(undefined), duration(undefined)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    // running_status, free_CA_mode, descriptors_loop_length
    0x00, 0x00,
  };
  // clang-format on
  EitSection eit;
  eit.events_data = kData;
  eit.events_size = sizeof(kData);
  rapidjson::Document doc(rapidjson::kObjectType);
  auto& allocator = doc.GetAllocator();
  auto events = MakeEventsJsonValue(eit, allocator);
  EXPECT_EQ(1, events.Size());
  EXPECT_EQ(1, events[0]["eventId"]);
  EXPECT_TRUE(events[0]["startTime"].IsNull());
  EXPECT_TRUE(events[0]["duration"].IsNull());
  EXPECT_TRUE(events[0]["scrambled"].IsFalse());
  EXPECT_EQ(0, events[0]["descriptors"].Size());
}
