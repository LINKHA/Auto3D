#include "RttrSample.h"

struct MyStruct { MyStruct() {}; void func(double s) { LogString(s); }; int data; };

static void rttr_auto_register_reflection_function_();
namespace
{
	struct rttr__auto__register__
	{
		rttr__auto__register__()
		{
			rttr_auto_register_reflection_function_();
		}
	};
}
static const rttr__auto__register__ RTTR_CAT(auto_register__, __LINE__);
static void rttr_auto_register_reflection_function_()
{
	Registration::class_<MyStruct>("MyStruct")
		.constructor<>()
		.property("data", &MyStruct::data)
		.method("func", &MyStruct::func);
}


void RttrSample::Init()
{

	{
		Type t = Type::get<MyStruct>();
		for (auto& prop : t.get_properties())
			LogString("name: " + RtToStr(prop.get_name()));

		for (auto& meth : t.get_methods())
			LogString("name: " + RtToStr(meth.get_name()));
	}

	{
		Type t = Type::get_by_name("MyStruct");
		Variant var = t.create();    // will invoke the previously registered ctor

		Constructor ctor = t.get_constructor();  // 2nd way with the constructor class
		var = ctor.invoke();
		LogString(RtToStr(var.get_type().get_name()));
	}

	{
		MyStruct obj;

		Property prop = Type::get(obj).get_property("data");
		prop.set_value(obj, 23);

		Variant var_prop = prop.get_value(obj);
		LogString(var_prop.to_int());// prints '23'
	}

	{
		MyStruct obj;

		Method meth = Type::get(obj).get_method("func");
		meth.invoke(obj, 42.0);

		Variant var = Type::get(obj).create();
		meth.invoke(var, 48.0);

	}

	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Rttr Sample");
}
void RttrSample::Start()
{

}
void RttrSample::Update()
{
}

void RttrSample::Stop()
{

}

AUTO_APPLICATION_MAIN(RttrSample)