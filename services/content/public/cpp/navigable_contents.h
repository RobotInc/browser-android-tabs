// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_CONTENT_PUBLIC_CPP_NAVIGABLE_CONTENTS_H_
#define SERVICES_CONTENT_PUBLIC_CPP_NAVIGABLE_CONTENTS_H_

#include <memory>

#include "base/callback.h"
#include "base/component_export.h"
#include "base/macros.h"
#include "mojo/public/cpp/bindings/binding.h"
#include "services/content/public/mojom/navigable_contents.mojom.h"
#include "services/content/public/mojom/navigable_contents_factory.mojom.h"

namespace content {

class NavigableContentsView;

// A NavigableContents controls a single dedicated instance of a top-level,
// navigable content frame hosted by the Content Service. In addition to
// maintaining its own navigation state, a NavigableContents may be used to
// acquire an embeddable native UI object to display renderered content within a
// client application's own UI.
class COMPONENT_EXPORT(CONTENT_SERVICE_CPP) NavigableContents
    : public mojom::NavigableContentsClient {
 public:
  // Constructs a new NavigableContents using |factory|.
  explicit NavigableContents(mojom::NavigableContentsFactory* factory);
  ~NavigableContents() override;

  // Returns a NavigableContentsView which renders this NavigableContents's
  // currently navigated contents. This widget can be parented and displayed
  // anywhere within the application's own window tree.
  //
  // Note that this NavigableContentsView is created lazily on first call, and
  // by default NavigableContents does not otherwise create or manipulate UI
  // objects.
  NavigableContentsView* GetView();

  // Begins an attempt to asynchronously navigate this NavigableContents to
  // |url|.
  void Navigate(const GURL& url);

  void set_did_stop_loading_callback_for_testing(
      base::RepeatingClosure callback) {
    did_stop_loading_callback_ = std::move(callback);
  }

 private:
  // mojom::NavigableContentsClient:
  void DidStopLoading() override;

  void OnEmbedTokenReceived(const base::UnguessableToken& token);

  mojom::NavigableContentsPtr contents_;
  mojo::Binding<mojom::NavigableContentsClient> client_binding_;
  std::unique_ptr<NavigableContentsView> view_;

  base::RepeatingClosure did_stop_loading_callback_;

  DISALLOW_COPY_AND_ASSIGN(NavigableContents);
};

}  // namespace content

#endif  // SERVICES_CONTENT_PUBLIC_CPP_NAVIGABLE_CONTENTS_H_
