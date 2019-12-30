#pragma once
#include "GameFramework/Actor.h"

namespace Auto3D
{
class AController;

class AUTO_API APawn : public AActor
{
	DECLARE_CLASS(APawn,AActor)
public:
	APawn() {}
	~APawn() {}

	/// Called when this Pawn is possessed. Only called on the server (or in standalone).
	virtual void PossessedBy(AController* newController);
	/// Called when our Controller no longer possesses us. 
	virtual void UnPossessed();

	/// Returns controller for this actor.
	AController* GetController() const;
	/// Is possess by controller;
	bool IsPossess() { return _isPossess; }
private:
	AController* _controller;

	bool _isPossess;
};

}