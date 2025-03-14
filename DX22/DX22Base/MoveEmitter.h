#ifndef __MOVE_EMITTER_H__
#define __MOVE_EMITTER_H__

#include "Effect.h"

class MoveEmitter : public Emitter
{
public:
	MoveEmitter(void);
	~MoveEmitter(void);
protected:
	void Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos);
};

#endif