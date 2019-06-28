#pragma once

#include <string>

#include "envoy/local_info/local_info.h"
#include "envoy/runtime/runtime.h"
#include "envoy/thread_local/thread_local.h"
#include "envoy/tracing/http_tracer.h"
#include "envoy/upstream/cluster_manager.h"

#include "common/http/header_map_impl.h"
#include "common/json/json_loader.h"

namespace Envoy {
namespace Tracing {

/**
 * Synapse Tracing tag names.
 */
class SynapseTracingTagNames {
public:
  // Synapse Tracing Header prefix
  const std::string Prefix = "x-synapse-";

  // Synapse additional tags
  const std::string GrpcStatus = "response.grpc_status";
  const std::string GrpcMessage = "response.grpc_message";
};

using SynapseTags = ConstSingleton<SynapseTracingTagNames>;

class SynapseTracerUtility {
public:
  /**
   * Fill in span tags based on the synapse headers.
   */
  static void fillSpan(Span& span, const Http::HeaderMap* request_headers,
                           const Http::HeaderMap* response_headers,
                           const Http::HeaderMap* response_trailers,
                           const StreamInfo::StreamInfo& stream_info,
                           const Config& tracing_config);
};

} // namespace Tracing
} // namespace Envoy
