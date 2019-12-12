#include "RttrSample.h"
#include "TestClassB.h"

#include <iostream>

AUTO_APPLICATION_MAIN(RttrSample)

void PrintClassA()
{
	LogString("-----------------------------------------------------------------------");

	FType t = FType::get_by_name("Ns3D::Node");
	
	FVariant var = t.create({ FString("MyNode") }); // Will create an instance of ns_3d::node as std::shared_ptr<ns_3d::node>

	LogString(RtToStr(var.get_type().get_name()));
	
	FProperty prop = t.get_property("name"); // Sets/Gets a property

	{
		FProperty parentProps = t.get_property("parent"); // Sets/Gets a property
		LogString("  name: " + RtToStr(parentProps.get_name()));
		LogString("    type: " + RtToStr(parentProps.get_type().get_name()));

		FProperty boolVProps = t.get_property("boolV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(boolVProps.get_name()));
		LogString("    type: " + RtToStr(boolVProps.get_type().get_name()));

		FProperty charVProps = t.get_property("charV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(charVProps.get_name()));
		LogString("    type: " + RtToStr(charVProps.get_type().get_name()));

		FProperty unsignedVProps = t.get_property("unsignedV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(unsignedVProps.get_name()));
		LogString("    type: " + RtToStr(unsignedVProps.get_type().get_name()));

		FProperty intVProps = t.get_property("intV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(intVProps.get_name()));
		LogString("    type: " + RtToStr(intVProps.get_type().get_name()));

		FProperty tVector2IVProps = t.get_property("tVector2IV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(tVector2IVProps.get_name()));
		LogString("    type: " + RtToStr(tVector2IVProps.get_type().get_name()));

		FProperty tRectIVProps = t.get_property("tRectIV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(tRectIVProps.get_name()));
		LogString("    type: " + RtToStr(tRectIVProps.get_type().get_name()));

		FProperty floatVProps = t.get_property("floatV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(floatVProps.get_name()));
		LogString("    type: " + RtToStr(floatVProps.get_type().get_name()));

		FProperty tVector2FVProps = t.get_property("tVector2FV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(tVector2FVProps.get_name()));
		LogString("    type: " + RtToStr(tVector2FVProps.get_type().get_name()));

		FProperty fQuaternionVProps = t.get_property("fQuaternionV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(fQuaternionVProps.get_name()));
		LogString("    type: " + RtToStr(fQuaternionVProps.get_type().get_name()));

		FProperty fColorVProps = t.get_property("fColorV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(fColorVProps.get_name()));
		LogString("    type: " + RtToStr(fColorVProps.get_type().get_name()));

		FProperty tRectFVProps = t.get_property("tRectFV"); // Sets/Gets a property
		LogString("  name: " + RtToStr(tRectFVProps.get_name()));
		LogString("    type: " + RtToStr(tRectFVProps.get_type().get_name()));
	}
	





	prop.set_value(var, FString("A New Name"));// Remark: you can also set a member, although the instance is of type: 'std::shared_ptr<T>'

	LogString(prop.get_value(var).get_value<FString>());
	LogString("MetaData TOOL_TIP: " + RtToStr(prop.get_metadata("TOOL_TIP").to_string())); // Retrieve the stored meta data of the property

	FMethod meth = t.get_method("SetVisible");	// Invoke a method

	FVariant ret = meth.invoke(var, true); //Remark: the 2nd argument will be provided automatically, because it has a default argument
	LogString("invoke of method 'SetVisible' was successfully : " + FString(ret.is_valid())); // A valid return value indicates a successful invoke
	LogString("'Node' properties:");// Retrieve all properties
	for (auto& prop : t.get_properties())
	{
		LogString("  name: " + RtToStr(prop.get_name()));
		LogString("    type: " + RtToStr(prop.get_type().get_name()));
	}
	LogString("");
	LogString("'Node' methods:");// Retrieve all methods
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
	LogString("-----------------------------------------------------------------------");
	TSharedPtr<Ns3D::Node> obj = Ns3D::Mesh::CreateMesh("House.obj");
	LogString(RtToStr(FType::get(obj).get_name()));						// Prints 'Auto::TSharedPtr<Ns3D::Node>'         
	LogString(RtToStr(FType::get(*obj.Get()).get_name())); // prints 'Ns3D::Mesh'

	// For glvalue expressions the most derived type is returned, in this case: 'ns_3d::mesh'; like typeid()
	FType t = FType::get(*obj.Get());
	LogString("");
	LogString("'Mesh' properties:");
	for (auto& prop : t.get_properties())
	{
		LogString("  name: " + RtToStr(prop.get_name()));
		LogString("    type: " + RtToStr(prop.get_type().get_name()));
	}
	FProperty prop = t.get_property("renderMode");
	// Set the property of the derived type, although we hold only a shared_ptr of the base class
	bool ret = prop.set_value(obj, Ns3D::Mesh::RenderMode::SOLID); // Yields to 'true'; when set was possible
	LogString("");
	FMethod meth = t.get_method("Render");
	LogString(RtToStr(meth.get_declaring_type().get_name())); // prints 'ns_3d::node'
	TSharedPtr<Ns3D::Mesh> obj_derived = DynamicCast<Ns3D::Mesh>(obj);
	// Invoke the method, although we have the most derived type now
	FVariant var = meth.invoke(*obj_derived.Get());
	LogString("Invoke of method 'Render' was successfully: " + FString(var.is_valid()));
}
void RttrSample::Init()
{
	PrintClassA();
	PrintClassB();
	LogString("-----------------------------------------------------------------------");
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

