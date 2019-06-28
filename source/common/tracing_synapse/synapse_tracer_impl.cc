#include "common/tracing_synapse/synapse_tracer_impl.h"

#include <string>

#include "common/access_log/access_log_formatter.h"
#include "common/common/assert.h"
#include "common/common/fmt.h"
#include "common/common/macros.h"
#include "common/common/utility.h"
#include "common/http/codes.h"
#include "common/http/header_map_impl.h"
#include "common/http/headers.h"
#include "common/http/utility.h"
#include "common/runtime/uuid_util.h"
#include "common/stream_info/utility.h"

namespace Envoy {
namespace Tracing {


void SynapseTracerUtility::fillSpan(Span& span, const Http::HeaderMap* request_headers,
                                    const Http::HeaderMap* response_headers,
                                    const Http::HeaderMap* response_trailers,
                                    const StreamInfo::StreamInfo& stream_info,
                                    const Config& tracing_config) {
  if (request_headers) {
    // TODO
  }

  if (response_headers) {
    // TODO
  }

  if (response_trailers) {
    // TODO
  }
}

} // namespace Tracing
} // namespace Envoy
