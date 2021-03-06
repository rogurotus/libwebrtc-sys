// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_CSSPAINT_PAINT_SIZE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_CSSPAINT_PAINT_SIZE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/geometry/float_size.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class PaintSize : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  explicit PaintSize(FloatSize size) : size_(size) {}

  PaintSize(const PaintSize&) = delete;
  PaintSize& operator=(const PaintSize&) = delete;

  ~PaintSize() override = default;

  float width() const { return size_.width(); }
  float height() const { return size_.height(); }

 private:
  FloatSize size_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_CSSPAINT_PAINT_SIZE_H_
