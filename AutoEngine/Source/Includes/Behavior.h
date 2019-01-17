#pragma once
#include "GameManager.h"

namespace Auto3D {

class Behavior : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Behavior, GlobalGameManager)
public:
	/**
	* @brief : Construct
	*/
	explicit Behavior(SharedPtr<Ambient> ambient);
	/**
	* @brief : Call all Awake funcation
	*/
	void Awake();
	/**
	* @brief : Call all Start funcation
	*/
	void Start();
	/**
	* @brief : Call all Update funcation
	*/
	void Update();
	/**
	* @brief : Call all FixUpdate funcation
	*/
	void FixUpdate();
	/**
	* @brief : Call all Finish funcation
	*/
	void Finish();
	/**
	* @brief : Call all Draw funcation
	*/
	void Draw();
};

}