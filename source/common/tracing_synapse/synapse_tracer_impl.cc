#include "common/tracing_synapse/synapse_tracer_impl.h"

#include <regex>
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

static bool startsWith(std::string const &str, std::string const &prefix) {
   return str.compare(0, prefix.length(), prefix) == 0;
}

static bool endsWith(std::string const &str, std::string const &suffix) {
  if (str.length() >= suffix.length()) {
    return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
  } else {
    return false;
  }
}

const std::string SynapseTracerUtility::SynapsePrefix = "x-synapse-";
const std::string SynapseTracerUtility::BinarySuffix = "-bin";
const std::regex SynapseTracerUtility::SynapseHeaderRegex("^x-synapse-(.+?)([-_]bin)?$");
const std::regex SynapseTracerUtility::HeaderNameFilterRegex("-");


void SynapseTracerUtility::fillSpan(Span& span, const Http::HeaderMap* request_headers,
                                    const Http::HeaderMap* response_headers,
                                    const Http::HeaderMap* response_trailers,
                                    const StreamInfo::StreamInfo& stream_info,
                                    const Config& tracing_config) {
  if (request_headers) {
    SynapseTracerUtility::addSynapseTagsFromHeaders(span, request_headers);
  }

  if (response_headers) {
    SynapseTracerUtility::addSynapseTagsFromHeaders(span, response_headers);
  }

  if (response_trailers) {
    SynapseTracerUtility::addSynapseTagsFromHeaders(span, response_trailers);
  }
}

void SynapseTracerUtility::addSynapseTagsFromHeaders(Span& span, const Http::HeaderMap* headers) {
  headers->iterate(
    [](const Http::HeaderEntry& header, void* context) -> {
      std::string key = std::string(header.key().getStringView());
      std::match match;
      if (std::regex_match(key, match, SynapseTracerUtility::SynapseHeaderRegex)) {
        std::string tagName = "synapse." + std::regex_replace(match[1], SynapseTracerUtility::HeaderNameFilterRegex, "_");
        if (match.size() > 2) {
          tagName += "_bin";
        }
        span.setTag(headerNameToTagName(tagName),
                    std::string(header.value().getStringView()));
      }   
      return Http::HeaderMap::Iterate::Continue;
    },
    &span
  )
}

std::string& SynapseTracerUtility::headerNameToTagName(const std::string& headerName) {
  std::match match;
  if (std::regex_match(headerName, match, SynapseTracerUtility::SynapseHeaderRegex)) {
    std::string tagName = "synapse." + std::regex_replace(match[1], SynapseTracerUtility::HeaderNameFilterRegex, "_");
    if (match.size() > 2) {
      tagName += "_bin";
    }
  }
}

} // namespace Tracing
} // namespace Envoy
