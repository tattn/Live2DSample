/*
 * Copyright(c) Live2D Inc. All rights reserved.
 * 
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */


#include "Local.h"


// -------- //
// REQUIRES //
// -------- //

#include <SDL_rwops.h>

#define STBI_NO_STDIO
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



// -------------- //
// IMPLEMENTATION //
// -------------- //

void* ReadBlob(const char* path, unsigned int* outSize)
{
  unsigned int size;
  SDL_RWops* file;
  void* blob;


  // Open file.
  file = SDL_RWFromFile(path, "rb");


  // Get size in bytes.
  size = (unsigned int)SDL_RWsize(file);


  // Allocate aligned memory for moc.
  blob = Allocate(size);


  // Read file into memory and close it afterwards.
  SDL_RWread(file, blob, sizeof(char), size);
  SDL_RWclose(file);


  // Store size.
  if (outSize)
  {
      (*outSize) = size;
  }


  return blob;
}

void* ReadBlobAligned(const char* path, const unsigned int alignment, unsigned int* outSize)
{
  unsigned int size;
  SDL_RWops* file;
  void* blob;


  // Open file.
  file = SDL_RWFromFile(path, "rb");


  // Get size in bytes.
  size = (unsigned int)SDL_RWsize(file);


  // Allocate aligned memory for moc.
  blob = AllocateAligned(size, alignment);


  // Read file into memory and close it afterwards.
  SDL_RWread(file, blob, sizeof(char), size);
  SDL_RWclose(file);


  // Store size.
  if (outSize)
  {
      (*outSize) = size;
  }


  return blob;
}


GLuint LoadTextureFromPng(const char* path)
{
  int width, height, channels;
  stbi_uc* png, * address;
  unsigned int size;
  SDL_RWops* file;
  GLuint texture;


  // Open file.
  file = SDL_RWFromFile(path, "rb");


  // Get moc size in bytes.
  size = (unsigned int)SDL_RWsize(file);


  // Allocate memory for PNG and read it into memory.
  address = Allocate(size);


  SDL_RWread(file, address, sizeof(char), size);


  // Flip for making OpenGL happy.
  stbi_set_flip_vertically_on_load(1);


	// Load png.
	png = stbi_load_from_memory(address,
		(int)size,
		&width,
		&height,
		&channels,
		STBI_rgb_alpha);


  // Create OpenGL texture.
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);


  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, png);
	
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  // Unbind texture and release PNG..
  glBindTexture(GL_TEXTURE_2D, 0);


  // Clean up.
  stbi_image_free(png);
  Deallocate(address);
  SDL_RWclose(file);


  return texture;
}

void ReleaseTexture(GLuint texture)
{
  glDeleteTextures(1, &texture);
}
