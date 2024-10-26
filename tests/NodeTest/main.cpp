#include <gtest/gtest.h>

#include "Core/CorePch.hpp"
#include "Core/Application.hpp"
#include "Core/Node.hpp"

using namespace mm;

// class TestApplication : public Application
// {
// public:
// 	void Startup() override {
// 		m_running = false;
// 	}
// 
// 	void NewFrame(f32) override {}
// 
// public:
// };

/* 
 *              ROOT
 *             /    \
 *            A      B      
 *           /          
 *          C----          
 *         / \   \ 
 *        D   E   F
 */

TEST(NodeTest, Transform)
{
	auto& root = Node::Root();
	auto& A = root.AddChild<Node>("A");
	auto& B = root.AddChild<Node>("B");
	auto& C = A.AddChild<Node>("C");
	auto& D = C.AddChild<Node>("D");
	auto& E = C.AddChild<Node>("E");
	auto& F = C.AddChild<Node>("F");

	A.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });
	B.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });
	C.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });
	D.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });
	E.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });
	F.SetLocalTransform(Transform{ glm::vec3(2, 3, 5) });

	root.UpdateSubtreeWorldTransform();

	ASSERT_EQ(A.GetWorldTransform().translation, glm::vec3(2, 3, 5));
	ASSERT_EQ(B.GetWorldTransform().translation, glm::vec3(2, 3, 5));
	ASSERT_EQ(C.GetWorldTransform().translation, glm::vec3(4, 6, 10));
	ASSERT_EQ(D.GetWorldTransform().translation, glm::vec3(6, 9, 15));
	ASSERT_EQ(E.GetWorldTransform().translation, glm::vec3(6, 9, 15));
	ASSERT_EQ(F.GetWorldTransform().translation, glm::vec3(6, 9, 15));
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}