// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/metrics/android_metrics_provider.h"

#include "base/test/metrics/histogram_tester.h"
#include "testing/gtest/include/gtest/gtest.h"

class AndroidMetricsProviderTest : public testing::Test {
 public:
  AndroidMetricsProviderTest() = default;
  ~AndroidMetricsProviderTest() override = default;

 protected:
  base::HistogramTester histogram_tester_;
  AndroidMetricsProvider metrics_provider_;
};

TEST_F(AndroidMetricsProviderTest, ProvidePreviousSessionData_IsLowRamDevice) {
  metrics_provider_.ProvidePreviousSessionData(nullptr /* uma_proto */);
  histogram_tester_.ExpectTotalCount("MemoryAndroid.LowRamDevice", 1);
}

TEST_F(AndroidMetricsProviderTest, ProvideCurrentSessionData_IsLowRamDevice) {
  metrics_provider_.ProvideCurrentSessionData(nullptr /* uma_proto */);
  histogram_tester_.ExpectTotalCount("MemoryAndroid.LowRamDevice", 1);
}

TEST_F(AndroidMetricsProviderTest, ProvideCurrentSessionData_CustomTabs) {
  metrics_provider_.ProvideCurrentSessionData(nullptr /* uma_proto */);
  histogram_tester_.ExpectTotalCount("CustomTabs.Visible", 1);
}

TEST_F(AndroidMetricsProviderTest, ProvideCurrentSessionData_MultiWindowMode) {
  metrics_provider_.ProvideCurrentSessionData(nullptr /* uma_proto */);
  histogram_tester_.ExpectTotalCount("Android.MultiWindowMode.Active", 1);
}

TEST_F(AndroidMetricsProviderTest, ProvideCurrentSessionData_AppNotifications) {
  metrics_provider_.ProvideCurrentSessionData(nullptr /* uma_proto */);
  histogram_tester_.ExpectTotalCount("Android.AppNotificationStatus", 1);
}
