#ifndef __Custom_Config_H_
#define __Custom_Config_H_

// CMake auto-generated configuration options

/* #undef OGRE_STATIC_LIB */

/* #undef OGRE_BUILD_RENDERSYSTEM_D3D9 */
/* #undef OGRE_BUILD_RENDERSYSTEM_D3D11 */
#define OGRE_BUILD_RENDERSYSTEM_GL
/* #undef OGRE_BUILD_RENDERSYSTEM_GL3PLUS */
/* #undef OGRE_BUILD_RENDERSYSTEM_GLES */
#define OGRE_BUILD_RENDERSYSTEM_GLES2
#define OGRE_BUILD_PLUGIN_BSP
#define OGRE_BUILD_PLUGIN_OCTREE
#define OGRE_BUILD_PLUGIN_PCZ
#define OGRE_BUILD_PLUGIN_PFX
/* #undef OGRE_BUILD_PLUGIN_CG */
#define OGRE_BUILD_COMPONENT_PAGING
#define OGRE_BUILD_COMPONENT_TERRAIN
#define OGRE_BUILD_COMPONENT_VOLUME
#define OGRE_BUILD_COMPONENT_PROPERTY
#define OGRE_BUILD_COMPONENT_OVERLAY
#define OGRE_BUILD_COMPONENT_RTSHADERSYSTEM

#define OGRE_CONFIG_LITTLE_ENDIAN
/* #undef OGRE_CONFIG_BIG_ENDIAN */

#define OGRE_DOUBLE_PRECISION 0

#define OGRE_MEMORY_ALLOCATOR 4

#define OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR 1

#define OGRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 0

#define OGRE_MEMORY_TRACKER_DEBUG_MODE 0

#define OGRE_MEMORY_TRACKER_RELEASE_MODE 0

#define OGRE_ASSERT_MODE 0

#define OGRE_THREAD_SUPPORT 2

#define OGRE_THREAD_PROVIDER 1

#define OGRE_NO_FREEIMAGE 0

#define OGRE_NO_DDS_CODEC 0

#define OGRE_NO_PVRTC_CODEC 0

#define OGRE_NO_ETC_CODEC 0

#define OGRE_NO_ZIP_ARCHIVE 0

#define OGRE_NO_VIEWPORT_ORIENTATIONMODE 1

#define OGRE_NO_GLES2_CG_SUPPORT 1

#define OGRE_NO_GLES2_GLSL_OPTIMISER 1

#define OGRE_NO_GLES2_VAO_SUPPORT 1

#define OGRE_NO_GL_STATE_CACHE_SUPPORT 1

#define OGRE_NO_GLES3_SUPPORT 1

#define OGRE_NO_LIBCPP_SUPPORT 1

#define OGRE_USE_BOOST 1

#define OGRE_PROFILING 0

#define RTSHADER_SYSTEM_BUILD_CORE_SHADERS

#define RTSHADER_SYSTEM_BUILD_EXT_SHADERS

/* #undef __FLASHCC__ */

#endif
