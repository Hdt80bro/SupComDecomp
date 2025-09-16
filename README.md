This is a decompilation of the game *Supreme Commander: Forged Alliance*.
**It is known to not compile**, and will not for many years. While the ideal is for
only linking errors to exist, the quick speed of the project necessitated by the large
volume of binary to comb through precludes any detailed analysis or architecture that
might be expected of a project this size.
In other words, syntax errors and other semantic errors likely exist - use for reference only.

Only the base executable of *Forged Alliance* is presented here - no gamedata is present.

Names come from both class `type_info` data and external vestigial dll symbols from when
the game was in a dynamically-linked phase of development that were blessedly included
on release.
Basic file structure comes from scattered assertations left in release containing file paths.

It was compiled using MSVC 8.0 and developed with Visual Studio 2005. This repository uses
some modern-style CXX that would not have been available at the time and cannot at present
time be targeted at that distribution.

The following libraries are known to be used (outside of the in-house GPG ones).
Though not included in this repository, the important ones are easy to find:
  * Boost 1.34.X (could be either 1.34.0 or 1.34.1)
  * LuaPlus 5.0 build 1081 (with slight modifications, to Lua threads in particular)
  * WX Widgets 2.4.2 (MSW version, though using the portable build)
  * Wild Magic 3.8 (a physics library now called Geometric Tools)
  * zlib 1.2.3 (also included in WX but seems to be included separately as well)
  * BugSplat
  * CRI Middleware (Sofdec and ADX) - proprietary
  * DirectX 9 & 10 (including XACT audio engine) - DX10 is not fully enabled and does not work
