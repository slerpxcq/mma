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

		pmx = PMXFile::Load("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
		auto model = ModelNode::LoadFromPMX(*pmx);
		modelNode = model.get();
		GetRootNode()->AttachChild(std::move(model));
	}

	void Shutdown() override {
		// Leak the resources
	}

	void NewFrame(f32) override {}

public:
	Ref<PMXFile> pmx{};
	ModelNode* modelNode{};
};

TEST(ModelNodeLoadTest, LoadPMX)
{
	auto app = MakeScoped<TestApplication>();
	app->Run();

	const auto& meshes = app->modelNode->GetMeshes();
	const auto& pmx = app->pmx;

	/* Meshes */
	ASSERT_EQ(meshes.size(), pmx->materials.size());
	ASSERT_EQ(meshes[0]->GetIndexCount(), 2034 * 3);
	ASSERT_EQ(meshes[1]->GetIndexCount(), 1980 * 3);
	ASSERT_EQ(meshes[2]->GetIndexCount(), 18 * 3);
	ASSERT_EQ(meshes[3]->GetIndexCount(), 228 * 3);
	ASSERT_EQ(meshes[4]->GetIndexCount(), 256 * 3);

	/* Textures */
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}