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

#include <Live2DCubismCore.h>
#include <Live2DCubismFramework.h>
#include <Live2DCubismGlRendering.h>


// -------------- //
// IMPLEMENTATION //
// -------------- //

static struct
{
  csmMoc* Moc;

  csmModel* Model;

  csmModelHashTable* Table;

  csmAnimation* Animation;

  csmAnimationState AnimationState;

  csmGlRenderer* Renderer;

  GLuint Texture;
}
Sample;


const char* GetSampleTitle()
{
  return "Hello Cubism";
}

float GetSampleSize()
{
  return 2.0f;
}


void OnDidStart()
{
  void* mocMemory, * modelMemory, * tableMemory, * motionJsonMemory, * animationMemory, * rendererMemory;
  unsigned int mocSize, modelSize, tableSize, animationSize, rendererSize;


  // Load and revive moc.
  mocMemory = ReadBlobAligned("Koharu/Koharu.moc3", csmAlignofMoc, &mocSize);


  Sample.Moc = csmReviveMocInPlace(mocMemory, mocSize);


  // Instantiate moc.
  modelSize = csmGetSizeofModel(Sample.Moc);
  modelMemory = AllocateAligned(modelSize, csmAlignofModel);


  Sample.Model = csmInitializeModelInPlace(Sample.Moc, modelMemory, modelSize);


  // Create hash table.
  tableSize = csmGetSizeofModelHashTable(Sample.Model);
  tableMemory = Allocate(tableSize);


  Sample.Table = csmInitializeModelHashTableInPlace(Sample.Model, tableMemory, tableSize);


  // Load animation.
  motionJsonMemory = ReadBlob("Koharu/Koharu.motion3.json", 0);


  animationSize = csmGetDeserializedSizeofAnimation(motionJsonMemory);
  animationMemory = Allocate(animationSize);


  Sample.Animation = csmDeserializeAnimationInPlace(motionJsonMemory, animationMemory, animationSize);


  Deallocate(motionJsonMemory);


  // Initialize animation state by resetting it.
  csmResetAnimationState(&Sample.AnimationState);


  // Create renderer.
  rendererSize = csmGetSizeofGlRenderer(Sample.Model);
  rendererMemory = Allocate(rendererSize);


  Sample.Renderer = csmMakeGlRendererInPlace(Sample.Model, rendererMemory, rendererSize);


  // Load texture.
  Sample.Texture = LoadTextureFromPng("Koharu/Koharu.png");
}


void OnTick(float deltaTime)
{
  // Update animation.
  csmUpdateAnimationState(&Sample.AnimationState, deltaTime);
  csmEvaluateAnimationFAST(Sample.Animation,
                           &Sample.AnimationState,
                           csmOverrideFloatBlendFunction,
                           1.0f,
                           Sample.Model,
                           Sample.Table,
                           0,
                           0);


  // Update model.
  csmUpdateModel(Sample.Model);


  // Update renderer.
  csmUpdateGlRenderer(Sample.Renderer);


  // Reset dynamic drawable flags.
  csmResetDrawableDynamicFlags(Sample.Model);


  // Draw.
  csmGlDraw(Sample.Renderer, GetViewProjectionMatrix(), &Sample.Texture);
}


void OnWillQuit()
{
  // Release resources.
  ReleaseTexture(Sample.Texture);


  csmReleaseGlRenderer(Sample.Renderer);


  // Free memory.
  Deallocate(Sample.Renderer);
  Deallocate(Sample.Animation);
  Deallocate(Sample.Table);
  DeallocateAligned(Sample.Model);
  DeallocateAligned(Sample.Moc);
}
