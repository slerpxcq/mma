#include <gtest/gtest.h>

#include "Core/CorePch.hpp"
#include "Core/Application/Application.hpp"
#include "Core/File/VPDFile.hpp"

using namespace mm;

class TestApplication : public Application
{
public:
	void Startup() override {
		Application::Startup();
		vpd = VPDFile::Load("../../resources/motion/Standing 1 -Stellas Shoujo Walk.vpd");

		m_running = false;
	}

	void OnUpdate(f32) override {}
	void OnNewFrame(f32) override {}

public:
	Ref<VPDFile> vpd;
};

TEST(VPDTest, Loading)
{
	auto app = MakeScoped<TestApplication>();
	app->Run();
	auto& vpd = app->vpd;

	ASSERT_EQ(vpd->GetTransform("センター"),
			  Transform(Vec3{ -0.598952,-0.428149,-1.242271 },
						Quat{ 1,0,0,0 } ));

	ASSERT_EQ(vpd->GetTransform("下半身"),
			  Transform(Vec3{0.000000,0.000000,0.000000},
						Quat{0.997888,0.000000,0.000000,0.064954} ));

	ASSERT_EQ(vpd->GetTransform("左薬指１"),
			  Transform(Vec3{0.000000,0.000000,0.000000},
						Quat{0.967891,0.000000,0.000000,-0.251370} ));

	ASSERT_EQ(vpd->GetTransform("左親指２"),
			  Transform(Vec3{0.000000,0.000000,0.000000},
						Quat{0.982871,-0.048807,-0.117068,0.133707} ));
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}