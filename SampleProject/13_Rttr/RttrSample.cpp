#include "RttrSample.h"
#include "TestClassB.h"

#include <iostream>

AUTO_APPLICATION_MAIN(RttrSample)

void PrintClassA()
{
	Type t = Type::get_by_name("ns_3d::node");
	
	Variant var = t.create({ std::string("MyNode") }); // will create an instance of ns_3d::node as std::shared_ptr<ns_3d::node>

	LogString(RtToStr(var.get_type().get_name()));
	
	Property prop = t.get_property("name"); // sets/gets a property
	
	prop.set_value(var, std::string("A New Name"));// remark: you can also set a member, although the instance is of type: 'std::shared_ptr<T>'

	LogString(RtToStr(prop.get_value(var).to_string()));
	LogString("MetaData TOOL_TIP: " + RtToStr(prop.get_metadata("TOOL_TIP").to_string())); // retrieve the stored meta data of the property

	Method meth = t.get_method("set_visible");	// invoke a method

	Variant ret = meth.invoke(var, true); //remark: the 2nd argument will be provided automatically, because it has a default argument
	LogString("invoke of method 'set_visible' was successfully : " + FString(ret.is_valid())); // a valid return value indicates a successful invoke
	LogString("'node' properties:");// retrieve all properties
	for (auto& prop : t.get_properties())
	{
		LogString("  name: " + RtToStr(prop.get_name()));
		LogString("    type: " + RtToStr(prop.get_type().get_name()));
	}
	LogString("");
	LogString("'node' methods:");// retrieve all methods
	for (auto& meth : t.get_methods())
	{
		LogString("  name: " + RtToStr(meth.get_name()) + "  signature: " + RtToStr(meth.get_signature()));
		for (auto& info : meth.get_parameter_infos())
		{
			LogString("    param " + FString(info.get_index()) + ": name: " + RtToStr(info.get_name()));
		}
	}

}
void PrintClassB()
{
	std::shared_ptr<ns_3d::node> obj = ns_3d::mesh::create_mesh("House.obj");
	LogString(RtToStr(Type::get(obj).get_name()));						// prints 'std::shared_ptr<ns_3d::node>'         
	LogString(RtToStr(Type::get(obj).get_wrapped_type().get_name()));// prints 'ns_3d::node*'
	LogString(RtToStr(Type::get(*obj.get()).get_name())); // prints 'ns_3d::mesh'

	// for glvalue expressions the most derived type is returned, in this case: 'ns_3d::mesh'; like typeid()
	Type t = Type::get(*obj.get());
	LogString("");
	LogString("'mesh' properties:");
	for (auto& prop : t.get_properties())
	{
		LogString("  name: " + RtToStr(prop.get_name()));
		LogString("    type: " + RtToStr(prop.get_type().get_name()));
	}
	Property prop = t.get_property("render_mode");
	// set the property of the derived type, although we hold only a shared_ptr of the base class
	bool ret = prop.set_value(obj, ns_3d::mesh::render_mode::SOLID); // yields to 'true'; when set was possible
	LogString("");
	Method meth = t.get_method("render");
	LogString(RtToStr(meth.get_declaring_type().get_name())); // prints 'ns_3d::node'
	std::shared_ptr<ns_3d::mesh> obj_derived = std::dynamic_pointer_cast<ns_3d::mesh>(obj);
	// invoke the method, although we have the most derived type now
	Variant var = meth.invoke(obj_derived);
	LogString("invoke of method 'render' was successfully: " + FString(var.is_valid()));
}
void RttrSample::Init()
{
	PrintClassA();
	PrintClassB();

	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Rttr Sample");
}
void RttrSample::Start()
{
	
}
void RttrSample::Update()
{
	Super::Update();
}

void RttrSample::Stop()
{

}

