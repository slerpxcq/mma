#include <gtest/gtest.h>

#include "Core/CorePch.hpp"
#include "Core/Application.hpp"
#include "Core/Node.hpp"
#include "Core/ModelNode.hpp"
#include "Core/File/PMXFile.hpp"

using namespace mm;

class TestApplication : public Application
{
public:
	void Startup() override {
		Application::Startup();
		m_running = false;
		auto pmx = PMXFile::Load("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
		auto model = ModelNode::LoadFromPMX(*pmx);
		modelNode = model.get();
		Node::Root().AttachChild(std::move(model));
	}

	void NewFrame(f32) override {}

public:
	ModelNode* modelNode{};
};

TEST(ModelNodeLoadTest, LoadPMX)
{
	auto app = MakeScoped<TestApplication>();
	app->Run();
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}