// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_HEAP_COLLECTION_SUPPORT_HEAP_LINKED_HASH_SET_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_HEAP_COLLECTION_SUPPORT_HEAP_LINKED_HASH_SET_H_

#include "third_party/blink/renderer/platform/heap/forward.h"
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/heap/heap_allocator_impl.h"
#include "third_party/blink/renderer/platform/wtf/linked_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/type_traits.h"

namespace blink {

template <typename ValueArg,
          typename TraitsArg = HashTraits<ValueArg>,
          typename HashArg = typename DefaultHash<ValueArg>::Hash>
class HeapLinkedHashSet final
    : public GarbageCollected<HeapLinkedHashSet<ValueArg, TraitsArg, HashArg>>,
      public LinkedHashSet<ValueArg, TraitsArg, HashArg, HeapAllocator> {
  static void CheckType() {
    static_assert(WTF::IsMemberOrWeakMemberType<ValueArg>::value,
                  "HeapLinkedHashSet supports only Member and WeakMember.");
    // If not trivially destructible, we have to add a destructor which will
    // hinder performance.
    static_assert(std::is_trivially_destructible<HeapLinkedHashSet>::value,
                  "HeapLinkedHashSet must be trivially destructible.");
    static_assert(WTF::IsTraceable<ValueArg>::value,
                  "For sets without traceable elements, use LinkedHashSet<> "
                  "instead of HeapLinkedHashSet<>.");
  }

 public:
  HeapLinkedHashSet() { CheckType(); }

  void Trace(Visitor* v) const {
    LinkedHashSet<ValueArg, TraitsArg, HashArg, HeapAllocator>::Trace(v);
  }
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_HEAP_COLLECTION_SUPPORT_HEAP_LINKED_HASH_SET_H_
