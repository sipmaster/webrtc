/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/video_coding/codecs/test/videoprocessor_integrationtest.h"

#include <vector>

#include "common_types.h"  // NOLINT(build/include)
#include "test/field_trial.h"
#include "test/testsupport/fileutils.h"

namespace webrtc {
namespace test {

namespace {
const int kForemanNumFrames = 300;
}  // namespace

class VideoProcessorIntegrationTestMediaCodec
    : public VideoProcessorIntegrationTest {
 protected:
  VideoProcessorIntegrationTestMediaCodec() {
    config_.filename = "foreman_cif";
    config_.filepath = ResourcePath(config_.filename, "yuv");
    config_.num_frames = kForemanNumFrames;
    config_.hw_encoder = true;
    config_.hw_decoder = true;
  }
};

TEST_F(VideoProcessorIntegrationTestMediaCodec, ForemanCif500kbpsVp8) {
  config_.SetCodecSettings(kVideoCodecVP8, 1, 1, 1, false, false, false, false,
                           352, 288);

  std::vector<RateProfile> rate_profiles = {{500, 30, kForemanNumFrames}};

  // The thresholds below may have to be tweaked to let even poor MediaCodec
  // implementations pass. If this test fails on the bots, disable it and
  // ping brandtr@.
  std::vector<RateControlThresholds> rc_thresholds = {
      {10, 1, 1, 0.1, 0.2, 0.1, 0, 1}};

  std::vector<QualityThresholds> quality_thresholds = {{36, 31, 0.92, 0.86}};

  ProcessFramesAndMaybeVerify(rate_profiles, &rc_thresholds,
                              &quality_thresholds, nullptr, nullptr);
}

TEST_F(VideoProcessorIntegrationTestMediaCodec, ForemanCif500kbpsH264CBP) {
  config_.encoded_frame_checker = &h264_keyframe_checker_;
  config_.SetCodecSettings(kVideoCodecH264, 1, 1, 1, false, false, false, false,
                           352, 288);

  std::vector<RateProfile> rate_profiles = {{500, 30, kForemanNumFrames}};

  // The thresholds below may have to be tweaked to let even poor MediaCodec
  // implementations pass. If this test fails on the bots, disable it and
  // ping brandtr@.
  std::vector<RateControlThresholds> rc_thresholds = {
      {10, 1, 1, 0.1, 0.2, 0.1, 0, 1}};

  std::vector<QualityThresholds> quality_thresholds = {{36, 31, 0.92, 0.86}};

  ProcessFramesAndMaybeVerify(rate_profiles, &rc_thresholds,
                              &quality_thresholds, nullptr, nullptr);
}

// TODO(brandtr): Enable this test when we have trybots/buildbots with
// HW encoders that support CHP.
TEST_F(VideoProcessorIntegrationTestMediaCodec,
       DISABLED_ForemanCif500kbpsH264CHP) {
  ScopedFieldTrials override_field_trials("WebRTC-H264HighProfile/Enabled/");

  config_.h264_codec_settings.profile = H264::kProfileConstrainedHigh;
  config_.encoded_frame_checker = &h264_keyframe_checker_;
  config_.SetCodecSettings(kVideoCodecH264, 1, 1, 1, false, false, false, false,
                           352, 288);

  std::vector<RateProfile> rate_profiles = {{500, 30, kForemanNumFrames}};

  // The thresholds below may have to be tweaked to let even poor MediaCodec
  // implementations pass. If this test fails on the bots, disable it and
  // ping brandtr@.
  std::vector<RateControlThresholds> rc_thresholds = {
      {5, 1, 0, 0.1, 0.2, 0.1, 0, 1}};

  std::vector<QualityThresholds> quality_thresholds = {{37, 35, 0.93, 0.91}};

  ProcessFramesAndMaybeVerify(rate_profiles, &rc_thresholds,
                              &quality_thresholds, nullptr, nullptr);
}

}  // namespace test
}  // namespace webrtc
