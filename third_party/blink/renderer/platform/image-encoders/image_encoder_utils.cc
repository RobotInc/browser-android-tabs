// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/image-encoders/image_encoder_utils.h"

#include "third_party/blink/renderer/platform/histogram.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#include "third_party/blink/renderer/platform/wtf/threading.h"

namespace blink {

const ImageEncodingMimeType ImageEncoderUtils::kDefaultEncodingMimeType =
    kMimeTypePng;
const char ImageEncoderUtils::kDefaultRequestedMimeType[] = "image/png";

// This enum is used in a UMA histogram; the values should not be changed.
enum RequestedImageMimeType {
  kRequestedImageMimeTypePng = 0,
  kRequestedImageMimeTypeJpeg = 1,
  kRequestedImageMimeTypeWebp = 2,
  kRequestedImageMimeTypeGif = 3,
  kRequestedImageMimeTypeBmp = 4,
  kRequestedImageMimeTypeIco = 5,
  kRequestedImageMimeTypeTiff = 6,
  kRequestedImageMimeTypeUnknown = 7,
  kNumberOfRequestedImageMimeTypes
};

ImageEncodingMimeType ImageEncoderUtils::ToEncodingMimeType(
    const String& mime_type_name,
    const EncodeReason encode_reason) {
  String lowercase_mime_type = mime_type_name.DeprecatedLower();

  RequestedImageMimeType requested_mime_type;
  if (mime_type_name.IsNull())
    lowercase_mime_type = kDefaultRequestedMimeType;

  if (lowercase_mime_type == "image/png") {
    requested_mime_type = kRequestedImageMimeTypePng;
  } else if (lowercase_mime_type == "image/jpeg") {
    requested_mime_type = kRequestedImageMimeTypeJpeg;
  } else if (lowercase_mime_type == "image/webp") {
    requested_mime_type = kRequestedImageMimeTypeWebp;
  } else if (lowercase_mime_type == "image/gif") {
    requested_mime_type = kRequestedImageMimeTypeGif;
  } else if (lowercase_mime_type == "image/bmp" ||
             lowercase_mime_type == "image/x-windows-bmp") {
    requested_mime_type = kRequestedImageMimeTypeBmp;
  } else if (lowercase_mime_type == "image/x-icon") {
    requested_mime_type = kRequestedImageMimeTypeIco;
  } else if (lowercase_mime_type == "image/tiff" ||
             lowercase_mime_type == "image/x-tiff") {
    requested_mime_type = kRequestedImageMimeTypeTiff;
  } else {
    requested_mime_type = kRequestedImageMimeTypeUnknown;
  }

  if (encode_reason == kEncodeReasonToDataURL) {
    DEFINE_THREAD_SAFE_STATIC_LOCAL(EnumerationHistogram,
                                    to_data_url_requested_mime_type_histogram,
                                    ("Canvas.RequestedImageMimeTypes_toDataURL",
                                     kNumberOfRequestedImageMimeTypes));
    to_data_url_requested_mime_type_histogram.Count(requested_mime_type);
  } else if (encode_reason == kEncodeReasonToBlobCallback) {
    DEFINE_THREAD_SAFE_STATIC_LOCAL(
        EnumerationHistogram, to_blob_callback_requested_mime_type_histogram,
        ("Canvas.RequestedImageMimeTypes_toBlobCallback",
         kNumberOfRequestedImageMimeTypes));
    to_blob_callback_requested_mime_type_histogram.Count(requested_mime_type);
  } else if (encode_reason == kEncodeReasonConvertToBlobPromise) {
    DEFINE_THREAD_SAFE_STATIC_LOCAL(
        EnumerationHistogram,
        convert_to_blob_promise_requested_mime_type_histogram,
        ("Canvas.RequestedImageMimeTypes_convertToBlobPromise",
         kNumberOfRequestedImageMimeTypes));
    convert_to_blob_promise_requested_mime_type_histogram.Count(
        requested_mime_type);
  }

  ImageEncodingMimeType encoding_mime_type = kDefaultEncodingMimeType;
  // FIXME: Make isSupportedImageMIMETypeForEncoding threadsafe (to allow this
  // method to be used on a worker thread).
  if (MIMETypeRegistry::IsSupportedImageMIMETypeForEncoding(
          lowercase_mime_type))
    ParseImageEncodingMimeType(lowercase_mime_type, encoding_mime_type);
  return encoding_mime_type;
}

}  // namespace blink
