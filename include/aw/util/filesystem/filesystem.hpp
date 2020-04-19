#pragma once

#if defined(__cplusplus) && __cplusplus >= 201703L
#define GHC_USE_STD_FS
#elif defined(__has_include)
#if __has_include(<filesystem>)
#define GHC_USE_STD_FS
#endif
#endif

#ifdef GHC_USE_STD_FS
#include <filesystem>
namespace aw {
namespace fs = std::filesystem;
}
#else
#include <ghc/filesystem.hpp>
namespace aw {
namespace aw::fs = ghc::filesystem;
}
#endif
