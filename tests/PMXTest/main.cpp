#include <gtest/gtest.h>

#include "Core/CorePch.hpp"
#include "Core/Application.hpp"
#include "Core/File/PMXFile.hpp"

using namespace mm;

class TestApplication : public Application
{
public:
	void Startup() override {
		Application::Startup();
		pmx = PMXParser{}.Parse("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");

		/* Exit immediately */
		m_running = false;
	}

	void NewFrame(f32) override {}

public:
	Ref<PMXFile> pmx{};
};

TEST(PMXTest, Loading)
{
	auto app = MakeScoped<TestApplication>();
	app->Run();

	const auto& pmx = app->pmx;
	EXPECT_STREQ(pmx->info.nameJP.c_str(), "つみ式ミクさん");
	EXPECT_EQ(pmx->vertices.size(), 28046);
	EXPECT_EQ(pmx->faces.size(), 45011);
	EXPECT_EQ(pmx->materials.size(), 38);
	EXPECT_EQ(pmx->bones.size(), 284);
	EXPECT_EQ(pmx->morphs.size(), 56);
	EXPECT_EQ(pmx->clusters.size(), 12);
	EXPECT_EQ(pmx->rigidbodies.size(), 129);
	EXPECT_EQ(pmx->joints.size(), 134);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}