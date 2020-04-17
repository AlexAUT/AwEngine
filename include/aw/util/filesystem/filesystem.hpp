#pragma once

#if defined(__cplusplus) && __cplusplus >= 201703L && defined(__has_include)
#if __has_include(<filesystem>)
#define GHC_USE_STD_FS
#include <filesystem>
namespace aw {
namespace fs = std::filesystem;
}
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <ghc/filesystem.hpp>
namespace aw {
namespace aw::fs = ghc::filesystem;
}
#endif
