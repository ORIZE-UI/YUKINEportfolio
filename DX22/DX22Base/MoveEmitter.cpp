#include "MoveEmitter.h"

MoveEmitter::MoveEmitter(void)
	: Emitter("Assets/Texture/Smoke.png", { 10, 0.05f, 0.2f })
{

}

MoveEmitter::~MoveEmitter(void)
{

}

void MoveEmitter::Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos)
{
	// “y‰Œ‚ðÄŒ»
	particle->pos.value = rootPos;
	particle->life = 0.3f;
	particle->alpha.value = { 0.8f, 0.8f, 0.8f };
	particle->alpha.add = { -0.016f, -0.016f, -0.016f };
	particle->size.value = { 2.0f, 2.0f, 2.0f };
	particle->size.add = { -0.01f, -0.01f, -0.01f };
	//particle->color.value = {0.72f, 0.53f, 0.23f};
}