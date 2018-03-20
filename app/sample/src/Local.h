/*
 * Copyright(c) Live2D Inc. All rights reserved.
 * 
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */


#pragma once


// -------- //
// REQUIRES //
// -------- //

#if _USE_GLAD
  #include <glad/glad.h>
#elif _USE_GLES20
  #if _IOS
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
  #else
    #include <GLES2/gl2.h>
  #endif
#endif


// ---------------- //
// SAMPLE INTERFACE //
// ---------------- //

/// Gets sample title.
///
/// @return  Sample title.
const char* GetSampleTitle();

/// Gets sample screen size in units.
///
/// @return  Sample size.
float GetSampleSize();


/// Called once when app did start.
void OnDidStart();

// Called once each frame.
//
// @param  deltaTime  Delta since last call to tick in seconds.
void OnTick(float deltaTime);


/// Called once when app will shut down.
void OnWillQuit();


// ---------- //
// ALLOCATION //
// ---------- //

/// Allocates heap memory without user align constraint.
///
/// @param  size   Number of bytes to allocate.
///
/// @return  Valid address to allocated memory on success; '0' otherwise.
void* Allocate(const unsigned int size);

/// Frees non-user alignment constraint heap memory.
///
/// @param  memory  Address of memory block to free.
void Deallocate(void* memory);


/// Allocates aligned heap memory.
///
/// @param  size       Number of bytes to allocate.
/// @param  alignment  Alignment for memory block.
///
/// @return  Valid address to allocated memory on success; '0' otherwise.
void* AllocateAligned(const unsigned int size, const unsigned int alignment);

/// Frees aligned heap memory.
///
/// @param  memory  Address of memory block to free.
void DeallocateAligned(void* alignedMemory);


// --- //
// APP //
// --- //

/// Gets the app view projection matrix.
///
/// @param  4x4 OpenGL matrix.
const GLfloat* GetViewProjectionMatrix();


// ------ //
// ASSETS //
// ------ //

void* ReadBlob(const char* path, unsigned int* outSize);


void* ReadBlobAligned(const char* path, const unsigned int alignment, unsigned int* outSize);

GLuint LoadTextureFromPng(const char* path);

void ReleaseTexture(GLuint texture);
